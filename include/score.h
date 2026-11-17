#ifndef SCORE_H
#define SCORE_H

#include "quiz.h"

/**
 * @file score.h
 * @brief Scoring module (owner: Vikas).
 *
 * Calculates and displays quiz results. It does not load questions and does
 * not access result storage.
 */

/** The outcome of one quiz attempt. */
typedef struct {
    int score;        /**< Number of correct answers. */
    int correct;      /**< Number of correct answers. */
    int incorrect;    /**< Number of incorrect answers. */
    int total;        /**< Total number of questions. */
    double percent;   /**< Score as a percentage. */
    int passed;       /**< 1 = Pass, 0 = Fail. */
} Result;

/**
 * @brief Works out the result from the questions and the submitted answers.
 * @return The result; all zeros if an argument is invalid.
 */
Result calculateResult(const Question questions[], const char answers[], int questionCount);

/** @brief Prints the finished result to the user. Safe to call with NULL. */
void showResult(const Result *result);

#endif
