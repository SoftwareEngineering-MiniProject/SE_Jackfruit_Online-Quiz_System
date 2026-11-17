/**
 * @file main.c
 * @brief Entry point and main menu (owner: Tejas).
 *
 * Shows the menu, checks the menu input, and sends the user to the quiz, the
 * leaderboard or the instructions until they choose Exit.
 */

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
static void showMenu(void)
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
static int readMenuChoice(void)
{
    char inputLine[64];
    char *numberEnd;
    long choiceValue;

    printf("%sEnter your choice (%d-%d): %s", ui(UI_BOLD), MENU_MIN, MENU_MAX, ui(UI_RESET));
    if (fgets(inputLine, sizeof inputLine, stdin) == NULL)
        return MENU_EOF;

    if (strchr(inputLine, '\n') == NULL && !feof(stdin)) {   /* line too long */
        int discarded;
        while ((discarded = getchar()) != '\n' && discarded != EOF)
            ;                                                /* discard the rest */
        return -1;
    }

    choiceValue = strtol(inputLine, &numberEnd, 10);
    if (numberEnd == inputLine)                  /* no digits at all */
        return -1;
    while (*numberEnd == ' ' || *numberEnd == '\t' || *numberEnd == '\r')  /* trailing blanks */
        numberEnd++;
    if (*numberEnd != '\n' && *numberEnd != '\0')   /* extra characters */
        return -1;
    if (choiceValue < MENU_MIN || choiceValue > MENU_MAX)
        return -1;
    return (int)choiceValue;
}

/* Read a line into buffer without the newline; discard any overflow.
 * Returns 0 on success, -1 if there is no more input. */
static int readLine(char *buffer, size_t size)
{
    size_t length;

    if (fgets(buffer, (int)size, stdin) == NULL)
        return -1;
    length = strlen(buffer);
    if (length > 0 && buffer[length - 1] == '\n') {
        buffer[--length] = '\0';
    } else if (!feof(stdin)) {
        int discarded;
        while ((discarded = getchar()) != '\n' && discarded != EOF)
            ;
    }
    if (length > 0 && buffer[length - 1] == '\r')
        buffer[--length] = '\0';
    return 0;
}

/* Option 1: ask for a name, run the quiz, show and save the result (REQ-4). */
static void startQuiz(void)
{
    static Question questions[MAX_QUESTIONS];
    char answers[MAX_QUESTIONS];
    char playerName[PLAYER_NAME_LEN];
    Result result;
    int questionCount;

    printf("%sEnter your name: %s", ui(UI_BOLD), ui(UI_RESET));
    if (readLine(playerName, sizeof playerName) != 0)
        return;
    if (playerName[0] == '\0')
        strcpy(playerName, "Player");

    memset(answers, 0, sizeof answers);   /* unanswered stays 0 if the quiz ends early */
    questionCount = loadQuestions(QUESTIONS_FILE, questions, MAX_QUESTIONS);
    if (questionCount <= 0) {
        printf("%sError: could not load any questions.%s\n", ui(UI_RED), ui(UI_RESET));
        return;
    }

    /* A random selection each time, so quizzes differ. The tests set
     * QUIZ_ORDERED=1 (and QUIZ_COUNT) to get the same questions every run. */
    {
        const char *countSetting = getenv("QUIZ_COUNT");
        int wantedCount = (countSetting != NULL) ? atoi(countSetting) : QUIZ_LENGTH;

        questionCount = pickQuestions(questions, questionCount, wantedCount,
                                      getenv("QUIZ_ORDERED") == NULL);
    }

    runQuiz(questions, questionCount, answers);
    result = calculateResult(questions, answers, questionCount);
    showResult(&result);
    if (saveResult(playerName, &result) != 0)
        printf("%sError: could not save your result.%s\n", ui(UI_RED), ui(UI_RESET));
}

/* Option 3: short instructions (REQ-4). */
static void showInstructions(void)
{
    printf("\n%s--- Instructions ---%s\n", ui(UI_BOLD), ui(UI_RESET));
    printf("1. Choose 'Start Quiz' and enter your name.\n");
    printf("2. Each question has four options: A, B, C and D.\n");
    printf("3. Type the letter of your answer and press Enter.\n");
    printf("4. Your score is shown at the end and saved to the leaderboard.\n");
}

/* Runs the menu loop until the user chooses Exit or the input ends. */
int main(void)
{
    int menuChoice;

    /* time alone repeats if the program is started twice in one second */
    srand((unsigned)time(NULL) ^ ((unsigned)GET_PID() << 16));

    /* Show the menu again after every feature until Exit (REQ-5). */
    do {
        showMenu();
        menuChoice = readMenuChoice();

        /* Invalid input only prints an error and loops (REQ-3). */
        if (menuChoice == -1) {
            printf("%sError: please enter a number from %d to %d.%s\n", ui(UI_RED),
                   MENU_MIN, MENU_MAX, ui(UI_RESET));
            continue;
        }

        /* Route to the selected feature (REQ-4). */
        switch (menuChoice) {
        case 1: startQuiz();         break;
        case 2: showLeaderboard();   break;
        case 3: showInstructions();  break;
        default:                     break;     /* 4 = exit */
        }
    } while (menuChoice != MENU_EXIT && menuChoice != MENU_EOF);

    /* Clean exit (REQ-6): main.c keeps no files open (each feature opens and
     * closes its own), so just say goodbye and return from main. */
    printf("\n%sThank you for playing. Goodbye!%s\n", ui(UI_GREEN), ui(UI_RESET));
    return 0;
}
