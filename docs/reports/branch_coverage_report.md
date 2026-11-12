# Branch coverage report

Date: 2026-11-12  |  Commit: `bd94a81`  |  Project: Online Quiz System

Measured with `gcc --coverage` and `gcov -b`. "Reached" is the share of branches the tests got to. "Taken" is the share that were followed in at least one direction.

## Unit tests (scoring module only)

| File | Branches | Reached | Taken |
|---|---|---|---|
| `src/score.c` | 18 | 100.0% | 100.0% |

## System tests (the 20 test cases against the whole program)

| File | Branches | Reached | Taken |
|---|---|---|---|
| `src/main.c` | 60 | 90.0% | 68.3% |
| `src/quiz.c` | 68 | 91.2% | 60.3% |
| `src/score.c` | 18 | 100.0% | 77.8% |
| `src/storage.c` | 68 | 91.2% | 63.2% |
| **All source files** | 214 | **91.6%** | **65.0%** |

### Branches the system tests never took

- `src/main.c` line 71
- `src/main.c` line 73
- `src/quiz.c` line 104
- `src/quiz.c` line 105
- `src/storage.c` line 59
- `src/storage.c` line 60

To regenerate this report: `python tests/make_reports.py`
