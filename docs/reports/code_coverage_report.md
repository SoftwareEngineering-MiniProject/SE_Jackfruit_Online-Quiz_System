# Code Coverage Report (line coverage)

Date: 2026-11-03  |  Commit: `3f4a53c`  |  Project: Online Quiz System

Measured with `gcc --coverage` and `gcov`. The program was built with coverage on and run in two ways.

## 1. Unit tests (`tests/test_score.c`, scoring module only)

| File | Lines | Lines executed |
|---|---|---|
| `src/score.c` | 23 | 100.0% |

## 2. System tests (the 20 scripted test cases run against the whole program)

| File | Lines | Lines executed |
|---|---|---|
| `src/main.c` | 72 | 88.9% |
| `src/quiz.c` | 78 | 71.8% |
| `src/score.c` | 23 | 87.0% |
| `src/storage.c` | 60 | 80.0% |
| **All source files** | 233 | **80.7%** |

### Lines not executed by the system tests

- `src/main.c` line 66: `return -1;`
- `src/main.c` line 70: `} else if (!feof(stdin)) {`
- `src/main.c` line 72: `while ((c = getchar()) != '\n' && c != EOF)`
- `src/main.c` line 76: `buf[--len] = '\0';`
- `src/main.c` line 91: `return;`
- `src/main.c` line 93: `strcpy(name, "Player");`
- `src/main.c` line 98: `printf("Error: could not load any questions.\n");`
- `src/main.c` line 99: `return;`
- `src/quiz.c` line 16: `s++;`
- `src/quiz.c` line 22: `s++;`
- `src/quiz.c` line 40: `} else if (!feof(fp)) {`
- `src/quiz.c` line 41: `while ((c = fgetc(fp)) != '\n' && c != EOF)`
- `src/quiz.c` line 45: `buf[len - 1] = '\0';`
- `src/quiz.c` line 62: `return -1;`
- `src/quiz.c` line 66: `return -1;`
- `src/quiz.c` line 84: `fclose(fp);`
- `src/quiz.c` line 85: `return count;          /* truncated last question: ignore it */`
- `src/quiz.c` line 91: `fclose(fp);`
- `src/quiz.c` line 92: `return count;`
- `src/quiz.c` line 97: `fprintf(stderr,`
- `src/quiz.c` line 100: `continue;`
- `src/quiz.c` line 107: `fclose(fp);`
- `src/quiz.c` line 108: `return count;`
- `src/quiz.c` line 120: `return -1;`
- `src/quiz.c` line 124: `} else if (!feof(stdin)) {`
- `src/quiz.c` line 125: `while ((c = getchar()) != '\n' && c != EOF)`
- `src/quiz.c` line 129: `buf[len - 1] = '\0';`
- `src/quiz.c` line 143: `return;`
- `src/quiz.c` line 159: `printf("\n");`
- `src/quiz.c` line 160: `return;`
- `src/score.c` line 21: `return r;`
- `src/score.c` line 43: `printf("Error: result is unavailable.\n");`
- `src/score.c` line 44: `return;`
- `src/storage.c` line 19: `return -1;`
- `src/storage.c` line 35: `return -1;`
- `src/storage.c` line 58: `} else if (!feof(fp)) {`
- `src/storage.c` line 59: `while ((c = fgetc(fp)) != '\n' && c != EOF)`
- `src/storage.c` line 63: `buf[len - 1] = '\0';`
- `src/storage.c` line 77: `return -1;`
- `src/storage.c` line 80: `return -1;`
- `src/storage.c` line 83: `n = NAME_LEN - 1;`
- `src/storage.c` line 109: `continue;                       /* skip lines in the wrong format */`
- `src/storage.c` line 111: `break;`
- `src/storage.c` line 122: `printf("\nNo results yet. Play a quiz first.\n");`
- `src/storage.c` line 123: `return;`

The system tests drive the real program from the keyboard. The lines listed above are mostly error paths and rare input cases (very long input lines, end of input at a prompt, a missing question file, an empty or damaged results file). Several of them are covered by the unit tests or by the manual checks in the Test Plan.

**How to reproduce:** `python tests/make_reports.py`
