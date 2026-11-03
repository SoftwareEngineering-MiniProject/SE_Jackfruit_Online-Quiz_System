# Branch coverage report

Date: 2026-11-03  |  Commit: `be1bfab`  |  Project: Online Quiz System

Measured with `gcc --coverage` and `gcov -b`. "Reached" is the share of branches the tests got to. "Taken" is the share that were followed in at least one direction.

## Unit tests (scoring module only)

| File | Branches | Reached | Taken |
|---|---|---|---|
| `src/score.c` | 14 | 100.0% | 100.0% |

## System tests (the 20 test cases against the whole program)

| File | Branches | Reached | Taken |
|---|---|---|---|
| `src/main.c` | 60 | 90.0% | 68.3% |
| `src/quiz.c` | 86 | 86.0% | 55.8% |
| `src/score.c` | 14 | 100.0% | 71.4% |
| `src/storage.c` | 62 | 90.3% | 59.7% |
| **All source files** | 222 | **89.2%** | **61.3%** |

### Branches the system tests never took

- `src/main.c` line 70
- `src/main.c` line 72
- `src/quiz.c` line 40
- `src/quiz.c` line 41
- `src/quiz.c` line 124
- `src/quiz.c` line 125
- `src/storage.c` line 58
- `src/storage.c` line 59

To regenerate this report: `python tests/make_reports.py`
