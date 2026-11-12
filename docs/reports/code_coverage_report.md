# Code coverage report (lines)

Date: 2026-11-12  |  Commit: `e806d69`  |  Project: Online Quiz System

Measured with `gcc --coverage` and `gcov`. I built the program with coverage turned on and ran it two ways.

## Unit tests (scoring module only)

| File | Lines | Lines run |
|---|---|---|
| `src/score.c` | 25 | 100.0% |

## System tests (the 22 test cases against the whole program)

| File | Lines | Lines run |
|---|---|---|
| `src/main.c` | 76 | 89.5% |
| `src/quiz.c` | 85 | 82.3% |
| `src/score.c` | 25 | 88.0% |
| `src/storage.c` | 68 | 82.3% |
| **All source files** | 254 | **85.0%** |

### Lines the system tests did not run

- `src/main.c` line 76: `return -1;`
- `src/main.c` line 80: `} else if (!feof(stdin)) {`
- `src/main.c` line 82: `while ((c = getchar()) != '\n' && c != EOF)`
- `src/main.c` line 86: `buf[--len] = '\0';`
- `src/main.c` line 101: `return;`
- `src/main.c` line 103: `strcpy(name, "Player");`
- `src/main.c` line 108: `printf("%sError: could not load any questions.%s\n", ui(UI_RED), ui(UI_RESET));`
- `src/main.c` line 109: `return;`
- `src/quiz.c` line 18: `s++;`
- `src/quiz.c` line 24: `s++;`
- `src/quiz.c` line 35: `return 0;`
- `src/quiz.c` line 37: `count = n;`
- `src/quiz.c` line 55: `src = (const unsigned char *)"";`
- `src/quiz.c` line 70: `return -1;`
- `src/quiz.c` line 73: `sqlite3_close(db);`
- `src/quiz.c` line 74: `return -1;`
- `src/quiz.c` line 80: `sqlite3_close(db);`
- `src/quiz.c` line 81: `return -1;`
- `src/quiz.c` line 98: `fprintf(stderr,`
- `src/quiz.c` line 101: `continue;`
- `src/quiz.c` line 126: `} else if (!feof(stdin)) {`
- `src/quiz.c` line 127: `while ((c = getchar()) != '\n' && c != EOF)`
- `src/quiz.c` line 131: `buf[len - 1] = '\0';`
- `src/score.c` line 22: `return r;`
- `src/score.c` line 44: `printf("%sError: result is unavailable.%s\n", ui(UI_RED), ui(UI_RESET));`
- `src/score.c` line 45: `return;`
- `src/storage.c` line 20: `return -1;`
- `src/storage.c` line 36: `return -1;`
- `src/storage.c` line 59: `} else if (!feof(fp)) {`
- `src/storage.c` line 60: `while ((c = fgetc(fp)) != '\n' && c != EOF)`
- `src/storage.c` line 64: `buf[len - 1] = '\0';`
- `src/storage.c` line 78: `return -1;`
- `src/storage.c` line 81: `return -1;`
- `src/storage.c` line 84: `n = NAME_LEN - 1;`
- `src/storage.c` line 122: `continue;                       /* skip lines in the wrong format */`
- `src/storage.c` line 124: `break;`
- `src/storage.c` line 135: `printf("\n%sNo results yet. Play a quiz first.%s\n", ui(UI_YELLOW), ui(UI_RESET)`
- `src/storage.c` line 136: `return;`

Most of these are error paths and unusual input: very long lines, input ending at a prompt, a missing question file, an empty or damaged results file. A few are covered by the unit tests instead.

To regenerate this report: `python tests/make_reports.py`
