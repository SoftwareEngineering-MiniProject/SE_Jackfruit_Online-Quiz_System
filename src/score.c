/**
 * @file score.c
 * @brief Scoring module (owner: Vikas).
 *
 * Calculates scores, percentages, answer counts and Pass/Fail status, and
 * displays the final result. Question loading and result file operations
 * belong to other modules.
 */

#include "score.h"
#include "ui.h"

#include <stddef.h>
#include <stdio.h>

/** Minimum percentage needed to pass. */
#define PASS_PERCENT 40.0

/* Counts the answers that match the correct option and derives the
   percentage and Pass/Fail status. Invalid arguments give an all-zero result. */
Result calculateResult(const Question questions[], const char answers[], int questionCount)
{
    Result result = {0, 0, 0, 0, 0.0, 0};
    int questionIndex;

    if (questions == NULL || answers == NULL || questionCount <= 0)
        return result;

    result.total = questionCount;

    for (questionIndex = 0; questionIndex < questionCount; questionIndex++) {
        if (answers[questionIndex] == questions[questionIndex].correct) {
            result.score++;
            result.correct++;
        } else {
            result.incorrect++;
        }
    }

    result.percent = (double)result.score / result.total * 100.0;
    result.passed = (result.percent >= PASS_PERCENT) ? 1 : 0;

    return result;
}

/* Prints the result screen. A NULL result prints an error line instead of
   crashing. */
void showResult(const Result *result)
{
    if (result == NULL) {
        printf("%sError: result is unavailable.%s\n", ui(UI_RED), ui(UI_RESET));
        return;
    }

    printf("\n%s%s===== Quiz Results =====%s\n", ui(UI_BOLD), ui(UI_CYAN), ui(UI_RESET));
    printf("Score: %d/%d\n", result->score, result->total);
    printf("Percentage: %.2f%%\n", result->percent);
    printf("Correct answers: %d\n", result->correct);
    printf("Incorrect answers: %d\n", result->incorrect);
    uiSound(result->passed ? UI_SOUND_PASS : UI_SOUND_FAIL);
    printf("Result: %s%s%s%s\n", ui(UI_BOLD), result->passed ? ui(UI_GREEN) : ui(UI_RED),
           result->passed ? "Pass" : "Fail", ui(UI_RESET));
}
