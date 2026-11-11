/*
 * Scoring module (Vikas).
 * Responsible for calculating scores, percentages, answer counts,
 * Pass/Fail status, and displaying the final result.
 * Question loading and result file operations belong to other modules.
 */

#include "score.h"
#include "ui.h"

#include <stddef.h>
#include <stdio.h>

#define PASS_PERCENT 40.0

Result calculate_result(const Question qs[], const char answers[], int n)
{
    Result r = {0, 0, 0, 0, 0.0, 0};
    int i;

    if (qs == NULL || answers == NULL || n <= 0)
        return r;

    r.total = n;

    for (i = 0; i < n; i++) {
        if (answers[i] == qs[i].correct) {
            r.score++;
            r.correct++;
        } else {
            r.incorrect++;
        }
    }

    r.percent = (double)r.score / r.total * 100.0;
    r.passed = (r.percent >= PASS_PERCENT) ? 1 : 0;

    return r;
}

void show_result(const Result *r)
{
    if (r == NULL) {
        printf("%sError: result is unavailable.%s\n", ui(UI_RED), ui(UI_RESET));
        return;
    }

    printf("\n%s%s===== Quiz Results =====%s\n", ui(UI_BOLD), ui(UI_CYAN), ui(UI_RESET));
    printf("Score: %d/%d\n", r->score, r->total);
    printf("Percentage: %.2f%%\n", r->percent);
    printf("Correct answers: %d\n", r->correct);
    printf("Incorrect answers: %d\n", r->incorrect);
    ui_sound(r->passed ? UI_SOUND_PASS : UI_SOUND_FAIL);
    printf("Result: %s%s%s%s\n", ui(UI_BOLD), r->passed ? ui(UI_GREEN) : ui(UI_RED),
           r->passed ? "Pass" : "Fail", ui(UI_RESET));
}

