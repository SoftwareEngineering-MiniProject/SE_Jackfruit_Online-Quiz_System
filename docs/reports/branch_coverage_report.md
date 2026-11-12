# Branch coverage report

Date: 2026-11-12  |  Commit: `e806d69`  |  Project: Online Quiz System

Measured with `gcc --coverage` and `gcov -b`. "Reached" is the share of branches the tests got to. "Taken" is the share that were followed in at least one direction.

## Unit tests (scoring module only)

| File | Branches | Reached | Taken |
|---|---|---|---|
| `src/score.c` | 18 | 100.0% | 100.0% |

## System tests (the 22 test cases against the whole program)

| File | Branches | Reached | Taken |
|---|---|---|---|
| `src/main.c` | 62 | 90.3% | 69.3% |
| `src/quiz.c` | 80 | 92.5% | 65.0% |
| `src/score.c` | 18 | 100.0% | 77.8% |
| `src/storage.c` | 68 | 91.2% | 63.2% |
| **All source files** | 228 | **92.1%** | **66.7%** |

### Branches the system tests never took

- `src/main.c` line 80
- `src/main.c` line 82
- `src/quiz.c` line 126
- `src/quiz.c` line 127
- `src/storage.c` line 59
- `src/storage.c` line 60

To regenerate this report: `python tests/make_reports.py`
