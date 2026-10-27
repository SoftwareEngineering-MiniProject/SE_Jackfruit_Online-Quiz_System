#ifndef SCORE_H
#define SCORE_H

#include "quiz.h"

/*
 * Scoring module (Vikas).
 * This module calculates and displays quiz results.
 * It does not load questions or access result storage.
 */
typedef struct {
    int score;       /* Number of correct answers */
    int correct;     /* Number of correct answers */
    int incorrect;   /* Number of incorrect answers */
    int total;       /* Total number of questions */
    double percent;  /* Score as a percentage */
    int passed;      /* 1 = Pass, 0 = Fail */
} Result;

/* Calculate the result using questions and submitted answers. */
Result calculate_result(const Question qs[], const char answers[], int n);

/* Display the completed result to the user. */
void show_result(const Result *r);

#endif
