#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sqlite3.h>
#include "quiz.h"
#include "ui.h"

#define ANSWER_LINE_LEN 64

/* Returns 'A'..'D' if the line is exactly one of those letters (either case,
   spaces around it allowed), otherwise 0. */
static char parse_answer(const char *s)
{
    char c;

    while (*s == ' ' || *s == '\t')
        s++;
    c = (char)toupper((unsigned char)*s);
    if (c < 'A' || c > 'D')
        return 0;
    s++;
    while (*s == ' ' || *s == '\t')
        s++;
    return (*s == '\0') ? c : 0;
}

/* Chooses which questions a quiz uses: a random selection of count questions
   without repeats (partial Fisher-Yates shuffle), or the first count in order. */
int pick_questions(Question qs[], int n, int count, int shuffle)
{
    int i;

    if (qs == NULL || n <= 0)
        return 0;
    if (count <= 0 || count > n)
        count = n;
    if (shuffle) {
        for (i = 0; i < count; i++) {
            int j = i + rand() % (n - i);
            Question tmp = qs[i];

            qs[i] = qs[j];
            qs[j] = tmp;
        }
    }
    return count;
}

/* Copies a text column into dest (size bytes). A NULL column becomes "" and
   anything too long is cut, so the fields can never overflow (NFR-4). */
static void copy_column(char *dest, size_t size, const unsigned char *src)
{
    if (src == NULL)
        src = (const unsigned char *)"";
    snprintf(dest, size, "%s", (const char *)src);
}

/* Loads up to max questions from the SQLite database at path, table
   "questions" (see data/questions.sql). A row with a bad answer letter is
   skipped. Returns the number of questions loaded, or -1 if the database
   cannot be opened or read, or the arguments are invalid (NFR-4). */
int load_questions(const char *path, Question qs[], int max)
{
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int count = 0;

    if (path == NULL || qs == NULL || max <= 0)
        return -1;

    if (sqlite3_open_v2(path, &db, SQLITE_OPEN_READONLY, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return -1;
    }
    if (sqlite3_prepare_v2(db,
            "SELECT text, option_a, option_b, option_c, option_d, correct "
            "FROM questions ORDER BY id",
            -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return -1;
    }

    while (count < max && sqlite3_step(stmt) == SQLITE_ROW) {
        Question q;
        char line[ANSWER_LINE_LEN];
        char letter;
        int i;

        copy_column(q.text, sizeof q.text, sqlite3_column_text(stmt, 0));
        for (i = 0; i < 4; i++)
            copy_column(q.options[i], sizeof q.options[i],
                        sqlite3_column_text(stmt, 1 + i));

        copy_column(line, sizeof line, sqlite3_column_text(stmt, 5));
        letter = parse_answer(line);
        if (letter == 0) {
            fprintf(stderr,
                    "Warning: skipping question \"%.40s\" (correct answer must be A-D).\n",
                    q.text);
            continue;
        }

        q.correct = letter;
        qs[count++] = q;               /* only valid questions are stored */
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return count;
}

/* Reads one line from the keyboard into buf. Extra characters on the line
   are discarded so the buffer cannot overflow (SEC-3).
   Returns 0 on success, -1 if there is no more input. */
static int read_answer_line(char *buf, size_t size)
{
    size_t len;
    int c;

    if (fgets(buf, (int)size, stdin) == NULL)
        return -1;
    len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[--len] = '\0';
    } else if (!feof(stdin)) {
        while ((c = getchar()) != '\n' && c != EOF)
            ;
    }
    if (len > 0 && buf[len - 1] == '\r')
        buf[len - 1] = '\0';
    return 0;
}

/* Shows the questions one by one and records the answer to each
   (REQ-18 to REQ-22). If the input ends early, the quiz stops and the
   remaining answers stay 0 (unanswered). */
void run_quiz(const Question qs[], int n, char answers[])
{
    char line[ANSWER_LINE_LEN];
    char ans;
    int i, k;

    if (qs == NULL || answers == NULL || n <= 0)
        return;

    /* Start with every answer unanswered, even if the caller did not clear it. */
    for (i = 0; i < n; i++)
        answers[i] = 0;

    for (i = 0; i < n; i++) {
        printf("\n%s%sQuestion %d of %d%s\n%s%s%s\n", ui(UI_BOLD), ui(UI_CYAN), i + 1, n, ui(UI_RESET),
               ui(UI_BOLD), qs[i].text, ui(UI_RESET));
        for (k = 0; k < 4; k++)
            printf("  %s%c)%s %s\n", ui(UI_YELLOW), 'A' + k, ui(UI_RESET), qs[i].options[k]);

        /* Keep asking until the answer is A, B, C or D (REQ-19). */
        ans = 0;
        while (ans == 0) {
            printf("%sYour answer (A-D): %s", ui(UI_BOLD), ui(UI_RESET));
            if (read_answer_line(line, sizeof line) != 0) {
                printf("\n");
                return;
            }
            ans = parse_answer(line);
            if (ans == 0)
                printf("%sError: please enter A, B, C or D.%s\n", ui(UI_RED), ui(UI_RESET));
        }
        ui_sound(UI_SOUND_NEXT);
        answers[i] = ans;   /* record it, then show the next question (REQ-20, REQ-21) */
    }
}