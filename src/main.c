#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <process.h>
#define GET_PID() _getpid()
#else
#include <unistd.h>
#define GET_PID() getpid()
#endif
#include "quiz.h"
#include "score.h"
#include "storage.h"
#include "ui.h"

#define MENU_MIN 1
#define MENU_MAX 4
#define MENU_EXIT 4
#define MENU_EOF (-2)      /* returned when input ends (Ctrl+D / Ctrl+Z) */

#define QUESTIONS_FILE  "data/quiz.db"
#define MAX_QUESTIONS   100
#define QUIZ_LENGTH     10      /* questions asked in one quiz */
#define PLAYER_NAME_LEN 64

/* Print the numbered main menu (REQ-1). */
static void show_menu(void)
{
    printf("\n%s%s===== Online Quiz System =====%s\n", ui(UI_BOLD), ui(UI_CYAN), ui(UI_RESET));
    printf("%s1.%s Start Quiz\n", ui(UI_YELLOW), ui(UI_RESET));
    printf("%s2.%s View Leaderboard\n", ui(UI_YELLOW), ui(UI_RESET));
    printf("%s3.%s Instructions\n", ui(UI_YELLOW), ui(UI_RESET));
    printf("%s4.%s Exit\n", ui(UI_YELLOW), ui(UI_RESET));
}

/* Read one line and accept it only if it is a single integer that is a
 * listed menu option (REQ-2). Returns the option, -1 if invalid, or
 * MENU_EOF if there is no more input. */
static int read_menu_choice(void)
{
    char line[64];
    char *end;
    long value;

    printf("%sEnter your choice (%d-%d): %s", ui(UI_BOLD), MENU_MIN, MENU_MAX, ui(UI_RESET));
    if (fgets(line, sizeof line, stdin) == NULL)
        return MENU_EOF;

    if (strchr(line, '\n') == NULL && !feof(stdin)) {   /* line too long */
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;                                           /* discard the rest */
        return -1;
    }

    value = strtol(line, &end, 10);
    if (end == line)                        /* no digits at all */
        return -1;
    while (*end == ' ' || *end == '\t' || *end == '\r')  /* trailing blanks */
        end++;
    if (*end != '\n' && *end != '\0')       /* extra characters */
        return -1;
    if (value < MENU_MIN || value > MENU_MAX)
        return -1;
    return (int)value;
}

/* Read a line into buf without the newline; discard any overflow.
 * Returns 0 on success, -1 if there is no more input. */
static int read_line(char *buf, size_t size)
{
    size_t len;

    if (fgets(buf, (int)size, stdin) == NULL)
        return -1;
    len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[--len] = '\0';
    } else if (!feof(stdin)) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;
    }
    if (len > 0 && buf[len - 1] == '\r')
        buf[--len] = '\0';
    return 0;
}

/* Option 1: ask for a name, run the quiz, show and save the result (REQ-4). */
static void start_quiz(void)
{
    static Question qs[MAX_QUESTIONS];
    char answers[MAX_QUESTIONS];
    char name[PLAYER_NAME_LEN];
    Result result;
    int n;

    printf("%sEnter your name: %s", ui(UI_BOLD), ui(UI_RESET));
    if (read_line(name, sizeof name) != 0)
        return;
    if (name[0] == '\0')
        strcpy(name, "Player");

    memset(answers, 0, sizeof answers);   /* unanswered stays 0 if the quiz ends early */
    n = load_questions(QUESTIONS_FILE, qs, MAX_QUESTIONS);
    if (n <= 0) {
        printf("%sError: could not load any questions.%s\n", ui(UI_RED), ui(UI_RESET));
        return;
    }

    /* A random selection each time, so quizzes differ. The tests set
     * QUIZ_ORDERED=1 (and QUIZ_COUNT) to get the same questions every run. */
    {
        const char *count_env = getenv("QUIZ_COUNT");
        int count = (count_env != NULL) ? atoi(count_env) : QUIZ_LENGTH;

        n = pick_questions(qs, n, count, getenv("QUIZ_ORDERED") == NULL);
    }

    run_quiz(qs, n, answers);
    result = calculate_result(qs, answers, n);
    show_result(&result);
    if (save_result(name, &result) != 0)
        printf("%sError: could not save your result.%s\n", ui(UI_RED), ui(UI_RESET));
}

/* Option 3: short instructions (REQ-4). */
static void show_instructions(void)
{
    printf("\n%s--- Instructions ---%s\n", ui(UI_BOLD), ui(UI_RESET));
    printf("1. Choose 'Start Quiz' and enter your name.\n");
    printf("2. Each question has four options: A, B, C and D.\n");
    printf("3. Type the letter of your answer and press Enter.\n");
    printf("4. Your score is shown at the end and saved to the leaderboard.\n");
}

int main(void)
{
    int choice;

    /* time alone repeats if the program is started twice in one second */
    srand((unsigned)time(NULL) ^ ((unsigned)GET_PID() << 16));

    /* Show the menu again after every feature until Exit (REQ-5). */
    do {
        show_menu();
        choice = read_menu_choice();

        /* Invalid input only prints an error and loops (REQ-3). */
        if (choice == -1) {
            printf("%sError: please enter a number from %d to %d.%s\n", ui(UI_RED),
                   MENU_MIN, MENU_MAX, ui(UI_RESET));
            continue;
        }

        /* Route to the selected feature (REQ-4). */
        switch (choice) {
        case 1: start_quiz();        break;
        case 2: show_leaderboard();  break;
        case 3: show_instructions(); break;
        default:                     break;     /* 4 = exit */
        }
    } while (choice != MENU_EXIT && choice != MENU_EOF);

    /* Clean exit (REQ-6): main.c keeps no files open (each feature opens and
     * closes its own), so just say goodbye and return from main. */
    printf("\n%sThank you for playing. Goodbye!%s\n", ui(UI_GREEN), ui(UI_RESET));
    return 0;
}
