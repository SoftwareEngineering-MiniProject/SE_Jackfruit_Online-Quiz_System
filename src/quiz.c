/**
 * @file quiz.c
 * @brief Quiz engine (owner: Sanjana).
 *
 * Loads questions from the SQLite question bank, picks which ones to ask, and
 * runs the question-and-answer loop. All keyboard input is read with fgets
 * into fixed-size buffers so that long input cannot overflow them (SEC-3).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sqlite3.h>
#include "quiz.h"
#include "ui.h"

/** Size of the buffer used to read one answer line. */
#define ANSWER_LINE_LEN 64

/* Returns 'A'..'D' if the line is exactly one of those letters (either case,
   spaces around it allowed), otherwise 0. */
static char parseAnswer(const char *text)
{
    char letter;

    while (*text == ' ' || *text == '\t')
        text++;
    letter = (char)toupper((unsigned char)*text);
    if (letter < 'A' || letter > 'D')
        return 0;
    text++;
    while (*text == ' ' || *text == '\t')
        text++;
    return (*text == '\0') ? letter : 0;
}

/* Chooses which questions a quiz uses: a random selection of wantedCount
   questions without repeats (partial Fisher-Yates shuffle), or the first
   wantedCount in order. */
int pickQuestions(Question questions[], int loadedCount, int wantedCount, int shuffle)
{
    int position;

    if (questions == NULL || loadedCount <= 0)
        return 0;
    if (wantedCount <= 0 || wantedCount > loadedCount)
        wantedCount = loadedCount;
    if (shuffle) {
        for (position = 0; position < wantedCount; position++) {
            int swapWith = position + rand() % (loadedCount - position);
            Question temp = questions[position];

            questions[position] = questions[swapWith];
            questions[swapWith] = temp;
        }
    }
    return wantedCount;
}

/* Copies a text column into destination (size bytes). A NULL column becomes ""
   and anything too long is cut, so the fields can never overflow (NFR-4). */
static void copyColumn(char *destination, size_t size, const unsigned char *source)
{
    if (source == NULL)
        source = (const unsigned char *)"";
    snprintf(destination, size, "%s", (const char *)source);
}

/* Loads up to maxQuestions questions from the SQLite database at path, table
   "questions" (see data/questions.sql). A row with a bad answer letter is
   skipped. Returns the number of questions loaded, or -1 if the database
   cannot be opened or read, or the arguments are invalid (NFR-4). */
int loadQuestions(const char *path, Question questions[], int maxQuestions)
{
    sqlite3 *database = NULL;
    sqlite3_stmt *statement = NULL;
    int loadedCount = 0;

    if (path == NULL || questions == NULL || maxQuestions <= 0)
        return -1;

    if (sqlite3_open_v2(path, &database, SQLITE_OPEN_READONLY, NULL) != SQLITE_OK) {
        sqlite3_close(database);
        return -1;
    }
    if (sqlite3_prepare_v2(database,
            "SELECT text, option_a, option_b, option_c, option_d, correct "
            "FROM questions ORDER BY id",
            -1, &statement, NULL) != SQLITE_OK) {
        sqlite3_close(database);
        return -1;
    }

    while (loadedCount < maxQuestions && sqlite3_step(statement) == SQLITE_ROW) {
        Question question;
        char answerText[ANSWER_LINE_LEN];
        char answerLetter;
        int optionIndex;

        copyColumn(question.text, sizeof question.text, sqlite3_column_text(statement, 0));
        for (optionIndex = 0; optionIndex < 4; optionIndex++)
            copyColumn(question.options[optionIndex], sizeof question.options[optionIndex],
                       sqlite3_column_text(statement, 1 + optionIndex));

        copyColumn(answerText, sizeof answerText, sqlite3_column_text(statement, 5));
        answerLetter = parseAnswer(answerText);
        if (answerLetter == 0) {
            fprintf(stderr,
                    "Warning: skipping question \"%.40s\" (correct answer must be A-D).\n",
                    question.text);
            continue;
        }

        question.correct = answerLetter;
        questions[loadedCount++] = question;   /* only valid questions are stored */
    }

    sqlite3_finalize(statement);
    sqlite3_close(database);
    return loadedCount;
}

/* Reads one line from the keyboard into buffer. Extra characters on the line
   are discarded so the buffer cannot overflow (SEC-3).
   Returns 0 on success, -1 if there is no more input. */
static int readAnswerLine(char *buffer, size_t size)
{
    size_t length;
    int discarded;

    if (fgets(buffer, (int)size, stdin) == NULL)
        return -1;
    length = strlen(buffer);
    if (length > 0 && buffer[length - 1] == '\n') {
        buffer[--length] = '\0';
    } else if (!feof(stdin)) {
        while ((discarded = getchar()) != '\n' && discarded != EOF)
            ;
    }
    if (length > 0 && buffer[length - 1] == '\r')
        buffer[length - 1] = '\0';
    return 0;
}

/* Shows the questions one by one and records the answer to each
   (REQ-18 to REQ-22). If the input ends early, the quiz stops and the
   remaining answers stay 0 (unanswered). */
void runQuiz(const Question questions[], int questionCount, char answers[])
{
    char answerLine[ANSWER_LINE_LEN];
    char answerLetter;
    int questionIndex, optionIndex;

    if (questions == NULL || answers == NULL || questionCount <= 0)
        return;

    /* Start with every answer unanswered, even if the caller did not clear it. */
    for (questionIndex = 0; questionIndex < questionCount; questionIndex++)
        answers[questionIndex] = 0;

    for (questionIndex = 0; questionIndex < questionCount; questionIndex++) {
        printf("\n%s%sQuestion %d of %d%s\n%s%s%s\n", ui(UI_BOLD), ui(UI_CYAN),
               questionIndex + 1, questionCount, ui(UI_RESET),
               ui(UI_BOLD), questions[questionIndex].text, ui(UI_RESET));
        for (optionIndex = 0; optionIndex < 4; optionIndex++)
            printf("  %s%c)%s %s\n", ui(UI_YELLOW), 'A' + optionIndex, ui(UI_RESET),
                   questions[questionIndex].options[optionIndex]);

        /* Keep asking until the answer is A, B, C or D (REQ-19). */
        answerLetter = 0;
        while (answerLetter == 0) {
            printf("%sYour answer (A-D): %s", ui(UI_BOLD), ui(UI_RESET));
            if (readAnswerLine(answerLine, sizeof answerLine) != 0) {
                printf("\n");
                return;
            }
            answerLetter = parseAnswer(answerLine);
            if (answerLetter == 0)
                printf("%sError: please enter A, B, C or D.%s\n", ui(UI_RED), ui(UI_RESET));
        }
        uiSound(UI_SOUND_NEXT);
        answers[questionIndex] = answerLetter;   /* record it, then show the next question (REQ-20, REQ-21) */
    }
}
