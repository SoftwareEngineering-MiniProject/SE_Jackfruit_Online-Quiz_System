/**
 * @file test_score.c
 * @brief Unit tests for the scoring module (REQ-11). Run with: make test
 *
 * Each test case builds a small set of questions and answers, calls
 * calculateResult() and checks the counts, percentage and Pass/Fail with assert().
 */

#include "score.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  printf("=========================================\n");
  printf("        Testing score.c (REQ-11)        \n");
  printf("=========================================\n\n");

  /* Test 1: Standard quiz attempt (3 out of 5 correct -> 60%, Pass) */
  printf("--- Test Case 1: 3/5 Correct (60%%, Pass) ---\n");
  Question fiveQuestions[5] = {
      {"What is the capital of France?",
       {"Paris", "London", "Rome", "Berlin"},
       'A'},
      {"2 + 2 = ?", {"3", "4", "5", "6"}, 'B'},
      {"Which is a data type in C?", {"float", "banana", "car", "sky"}, 'A'},
      {"Which symbol denotes pointer?", {"*", "&", "%", "$"}, 'A'},
      {"Which function prints text?",
       {"scanf", "printf", "puts", "write"},
       'B'}};
  char answers1[5] = {
      'A', 'B', 'A', 'B',
      'A'}; /* Q1=A(ok), Q2=B(ok), Q3=A(ok), Q4=B(wrong), Q5=A(wrong) */

  Result result1 = calculateResult(fiveQuestions, answers1, 5);
  assert(result1.score == 3);
  assert(result1.correct == 3);
  assert(result1.incorrect == 2);
  assert(result1.total == 5);
  assert(result1.percent == 60.0);
  assert(result1.passed == 1);

  showResult(&result1);
  printf("--> Test 1 Passed!\n\n");

  /* Test Case 2: Failing score (1 out of 5 correct -> 20%, Fail) */
  printf("--- Test Case 2: 1/5 Correct (20%%, Fail) ---\n");
  char answers2[5] = {'A', 'C', 'C', 'C', 'C'}; /* Only Q1 is correct */
  Result result2 = calculateResult(fiveQuestions, answers2, 5);
  assert(result2.score == 1);
  assert(result2.correct == 1);
  assert(result2.incorrect == 4);
  assert(result2.total == 5);
  assert(result2.percent == 20.0);
  assert(result2.passed == 0);

  showResult(&result2);
  printf("--> Test 2 Passed!\n\n");

  /* Test Case 3: Perfect score (5 out of 5 correct -> 100%, Pass) */
  printf("--- Test Case 3: Perfect Score (100%%, Pass) ---\n");
  char answers3[5] = {'A', 'B', 'A', 'A', 'B'}; /* All correct */
  Result result3 = calculateResult(fiveQuestions, answers3, 5);
  assert(result3.score == 5);
  assert(result3.correct == 5);
  assert(result3.incorrect == 0);
  assert(result3.total == 5);
  assert(result3.percent == 100.0);
  assert(result3.passed == 1);

  showResult(&result3);
  printf("--> Test 3 Passed!\n\n");

  /* Test Case 4: Zero score (0 out of 5 correct -> 0%, Fail) */
  printf("--- Test Case 4: Zero Score (0%%, Fail) ---\n");
  char answers4[5] = {'C', 'C', 'C', 'C', 'C'}; /* None correct */
  Result result4 = calculateResult(fiveQuestions, answers4, 5);
  assert(result4.score == 0);
  assert(result4.correct == 0);
  assert(result4.incorrect == 5);
  assert(result4.total == 5);
  assert(result4.percent == 0.0);
  assert(result4.passed == 0);

  showResult(&result4);
  printf("--> Test 4 Passed!\n\n");

  /* Test Case 5: Null pointer handling (must not crash) */
  printf("--- Test Case 5: Safety Handling (NULL result) ---\n");
  showResult(NULL);
  printf("--> Test 5 Passed!\n\n");

  /* Test Case 6: Pass threshold boundary (4 of 10 correct -> 40%, Pass) */
  printf("--- Test Case 6: Boundary 4/10 (40%%, Pass) ---\n");
  Question tenQuestions[10];
  char answers10[10];
  int index;
  for (index = 0; index < 10; index++) {
    tenQuestions[index] = fiveQuestions[0];
    tenQuestions[index].correct = 'A';
    answers10[index] = (index < 4) ? 'A' : 'B';
  }
  Result result6 = calculateResult(tenQuestions, answers10, 10);
  assert(result6.score == 4);
  assert(result6.percent == 40.0);
  assert(result6.passed == 1);
  printf("--> Test 6 Passed!\n\n");

  /* Test Case 7: Just below the threshold (3 of 10 correct -> 30%, Fail) */
  printf("--- Test Case 7: Below boundary 3/10 (30%%, Fail) ---\n");
  answers10[3] = 'B';
  Result result7 = calculateResult(tenQuestions, answers10, 10);
  assert(result7.score == 3);
  assert(result7.percent == 30.0);
  assert(result7.passed == 0);
  printf("--> Test 7 Passed!\n\n");

  /* Test Case 8: Invalid arguments must not crash (zero result) */
  printf("--- Test Case 8: Invalid arguments ---\n");
  Result result8 = calculateResult(NULL, answers10, 10);
  assert(result8.total == 0 && result8.score == 0 && result8.passed == 0);
  result8 = calculateResult(tenQuestions, answers10, 0);
  assert(result8.total == 0 && result8.score == 0 && result8.passed == 0);
  result8 = calculateResult(tenQuestions, NULL, 10);
  assert(result8.total == 0 && result8.score == 0 && result8.passed == 0);
  printf("--> Test 8 Passed!\n\n");

  printf("=========================================\n");
  printf("   All test cases passed successfully!   \n");
  printf("=========================================\n");

  return 0;
}
