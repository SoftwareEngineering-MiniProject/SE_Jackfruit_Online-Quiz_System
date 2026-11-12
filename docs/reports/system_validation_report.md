# System validation report

Date: 2026-11-12  |  Commit: `e806d69`  |  Project: Online Quiz System

These tests run the whole program. The script sends input to `quiz` and checks the output against the SRS. Build with `make`, then run `bash tests/run_system_tests.sh`.

Result: 22 test cases passed, 0 failed. The test cases are described in section 13 of `docs/TEST_PLAN.md`.

| Test case | Requirement(s) | Check | Result |
|---|---|---|---|
| TC-01 | REQ-1 | menu shows 4 options and the prompt | Pass |
| TC-02 | REQ-2, REQ-4 | option 3 shows the instructions | Pass |
| TC-03 | REQ-3, NFR-4, SEC-3 | 5 invalid menu inputs give 5 errors, then exit works | Pass |
| TC-04 | REQ-5 | menu is shown again after the instructions | Pass |
| TC-05 | REQ-6, SEC-6 | option 4 and empty input both exit cleanly (exit code 0) | Pass |
| TC-06 | REQ-7, REQ-8, REQ-9 | scoring unit tests: counts and percentage | Pass |
| TC-07 | REQ-10 | scoring unit tests: 40% boundary | Pass |
| TC-08 | REQ-11 | show_result unit tests incl. NULL | Pass |
| TC-09 | REQ-13, SEC-2 | comma in a name is replaced; one line per record | Pass |
| TC-10 | REQ-12 | full quiz as Asha saves one Asha,<score> line | Pass |
| TC-11 | REQ-17 to REQ-22 | 5 questions one at a time, result at the end | Pass |
| TC-12 | REQ-19 | E, empty line and 1 give 3 errors, then B is accepted | Pass |
| TC-13 | REQ-14, REQ-15 | leaderboard order is 10, 9, 8 | Pass |
| TC-14 | REQ-16, SEC-4 | missing results.txt: message, no crash | Pass |
| TC-15 | NFR-1 | full run 860 ms, leaderboard of 100 results 100 ms (limit 1000 ms) | Pass |
| TC-16 | SEC-1, SEC-4 | write failure shows only a short Error line, no paths | Pass |
| TC-17 | SEC-3, NFR-4 | 5000-character line gives one error, program continues | Pass |
| TC-18 | SEC-5, SEC-3 | no gets/scanf/sprintf; file names are fixed | Pass |
| TC-19 | NFR-5 | score.c builds and runs on its own (make test) | Pass |
| TC-20 | NFR-3 | every error message starts with Error: | Pass |
| TC-21 | REQ-17, REQ-22 | a normal quiz asks 10 questions and none repeats | Pass |
| TC-22 | REQ-17 | three quizzes in a row are not all the same | Pass |

Output of the script:

```
PASS  TC-01  menu shows 4 options and the prompt
PASS  TC-02  option 3 shows the instructions
PASS  TC-03  5 invalid menu inputs give 5 errors, then exit works
PASS  TC-04  menu is shown again after the instructions
PASS  TC-05  option 4 and empty input both exit cleanly (exit code 0)
PASS  TC-06  scoring unit tests: counts and percentage
PASS  TC-07  scoring unit tests: 40% boundary
PASS  TC-08  show_result unit tests incl. NULL
PASS  TC-19  score.c builds and runs on its own (make test)
PASS  TC-09  comma in a name is replaced; one line per record
PASS  TC-10  full quiz as Asha saves one Asha,<score> line
PASS  TC-11  5 questions one at a time, result at the end
PASS  TC-12  E, empty line and 1 give 3 errors, then B is accepted
PASS  TC-13  leaderboard order is 10, 9, 8
PASS  TC-14  missing results.txt: message, no crash
PASS  TC-15  full run 860 ms, leaderboard of 100 results 100 ms (limit 1000 ms)
PASS  TC-16  write failure shows only a short Error line, no paths
PASS  TC-17  5000-character line gives one error, program continues
PASS  TC-18  no gets/scanf/sprintf; file names are fixed
PASS  TC-21  a normal quiz asks 10 questions and none repeats
PASS  TC-22  three quizzes in a row are not all the same
PASS  TC-20  every error message starts with Error:

Result: 22 passed, 0 failed
```
