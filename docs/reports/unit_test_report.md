# Unit test report

Date: 2026-11-12  |  Commit: `e806d69`  |  Project: Online Quiz System

These tests cover the scoring module (`src/score.c`) on its own. They are in `tests/test_score.c` and use `assert`. Run them with `make test`.

Result: all 8 tests passed.

| Test | What it checks | Requirement |
|---|---|---|
| 1 to 4 | 3, 1, 5 and 0 answers right out of 5: score, counts, percentage, Pass or Fail | REQ-7, REQ-8, REQ-9 |
| 5 | `show_result` given a NULL result does not crash | REQ-11, NFR-4 |
| 6, 7 | The pass mark: 4 of 10 (40%) passes, 3 of 10 (30%) fails | REQ-10 |
| 8 | Bad arguments (NULL, no questions) give an all-zero result | NFR-4 |

Output of `make test`:

```
gcc -Wall -std=c99 -Iinclude -MMD -MP -o tests/test_score tests/test_score.c src/score.c
./tests/test_score
=========================================
        Testing score.c (REQ-11)        
=========================================

--- Test Case 1: 3/5 Correct (60%, Pass) ---

===== Quiz Results =====
Score: 3/5
Percentage: 60.00%
Correct answers: 3
Incorrect answers: 2
Result: Pass
--> Test 1 Passed!

--- Test Case 2: 1/5 Correct (20%, Fail) ---

===== Quiz Results =====
Score: 1/5
Percentage: 20.00%
Correct answers: 1
Incorrect answers: 4
Result: Fail
--> Test 2 Passed!

--- Test Case 3: Perfect Score (100%, Pass) ---

===== Quiz Results =====
Score: 5/5
Percentage: 100.00%
Correct answers: 5
Incorrect answers: 0
Result: Pass
--> Test 3 Passed!

--- Test Case 4: Zero Score (0%, Fail) ---

===== Quiz Results =====
Score: 0/5
Percentage: 0.00%
Correct answers: 0
Incorrect answers: 5
Result: Fail
--> Test 4 Passed!

--- Test Case 5: Safety Handling (NULL result) ---
Error: result is unavailable.
--> Test 5 Passed!

--- Test Case 6: Boundary 4/10 (40%, Pass) ---
--> Test 6 Passed!

--- Test Case 7: Below boundary 3/10 (30%, Fail) ---
--> Test 7 Passed!

--- Test Case 8: Invalid arguments ---
--> Test 8 Passed!

=========================================
   All test cases passed successfully!   
=========================================
```
