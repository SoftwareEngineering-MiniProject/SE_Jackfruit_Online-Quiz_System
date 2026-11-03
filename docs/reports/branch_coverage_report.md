# Branch Coverage Report

Date: 2026-11-03  |  Commit: `3f4a53c`  |  Project: Online Quiz System

Measured with `gcc --coverage` and `gcov -b`. "Branches executed" is the share of branches that were reached at all. "Taken at least once" is the share that were followed in at least one direction.

## 1. Unit tests (`tests/test_score.c`, scoring module only)

| File | Branches | Branches executed | Taken at least once |
|---|---|---|---|
| `src/score.c` | 14 | 100.0% | 100.0% |

## 2. System tests (the 20 scripted test cases run against the whole program)

| File | Branches | Branches executed | Taken at least once |
|---|---|---|---|
| `src/main.c` | 60 | 90.0% | 68.3% |
| `src/quiz.c` | 86 | 86.0% | 55.8% |
| `src/score.c` | 14 | 100.0% | 71.4% |
| `src/storage.c` | 62 | 90.3% | 59.7% |
| **All source files** | 222 | **89.2%** | **61.3%** |

### Branches never taken by the system tests

- `src/main.c` line 70
- `src/main.c` line 72
- `src/quiz.c` line 40
- `src/quiz.c` line 41
- `src/quiz.c` line 124
- `src/quiz.c` line 125
- `src/storage.c` line 58
- `src/storage.c` line 59

**How to reproduce:** `python tests/make_reports.py`
