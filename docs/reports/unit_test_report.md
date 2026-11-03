# Unit Test Report

Date: 2026-11-03  |  Commit: `3f4a53c`  |  Project: Online Quiz System

**What was tested:** the scoring module (`src/score.c`) on its own, using `assert` in `tests/test_score.c`.
**How to run:** `make test`

**Result: 8 of 8 unit tests passed.**

| Test | Checks | Requirement |
|---|---|---|
| 1 to 4 | 3, 1, 5 and 0 of 5 correct: score, counts, percentage, Pass/Fail | REQ-7, REQ-8, REQ-9 |
| 5 | `show_result` with a NULL result does not crash | REQ-11, NFR-4 |
| 6, 7 | Pass mark boundary: 4 of 10 (40%) is Pass, 3 of 10 (30%) is Fail | REQ-10 |
| 8 | Invalid arguments (NULL, zero questions) give an all-zero result | NFR-4 |

## Raw output of `make test`

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
