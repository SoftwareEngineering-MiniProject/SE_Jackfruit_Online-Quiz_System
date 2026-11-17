#ifndef QUIZ_H
#define QUIZ_H

/**
 * @file quiz.h
 * @brief Quiz engine: loading questions, choosing which ones to ask, and
 *        running the question-and-answer loop (owner: Sanjana).
 */

/** One multiple-choice question. */
typedef struct {
    char text[256];          /**< The question text. */
    char options[4][128];    /**< Option texts for A, B, C and D. */
    char correct;            /**< Correct option letter, 'A' to 'D'. */
} Question;

/**
 * @brief Loads questions from the SQLite question bank.
 * @param path          Path of the database file.
 * @param questions     Output array, filled with the valid questions found.
 * @param maxQuestions  Size of the output array.
 * @return Number of questions loaded, or -1 on error.
 */
int loadQuestions(const char *path, Question questions[], int maxQuestions);

/**
 * @brief Chooses the questions for one quiz.
 *
 * Keeps wantedCount questions out of the loadedCount loaded (all of them if
 * wantedCount is 0 or more than loadedCount). With shuffle != 0 the kept
 * questions are chosen at random, without repeats, in random order.
 *
 * @return How many questions are kept.
 */
int pickQuestions(Question questions[], int loadedCount, int wantedCount, int shuffle);

/**
 * @brief Asks every question and records the answers.
 * @param questions      The questions to ask.
 * @param questionCount  How many questions there are.
 * @param answers        Output: answers[i] is 'A' to 'D', or 0 if unanswered.
 */
void runQuiz(const Question questions[], int questionCount, char answers[]);

#endif
