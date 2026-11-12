#ifndef QUIZ_H
#define QUIZ_H

/* quiz.h (Sanjana) */
typedef struct {
    char text[256];
    char options[4][128];
    char correct;              /* 'A'..'D' */
} Question;
int  load_questions(const char *path, Question qs[], int max);  /* returns count, -1 on error */
/* Keeps count questions from the n loaded (all of them if count is 0 or more than n).
   With shuffle != 0 the kept questions are chosen at random, with no repeats, in a
   random order. Returns how many questions are kept. */
int  pick_questions(Question qs[], int n, int count, int shuffle);
void run_quiz(const Question qs[], int n, char answers[]);      /* fills answers[i] with 'A'..'D' */

#endif
