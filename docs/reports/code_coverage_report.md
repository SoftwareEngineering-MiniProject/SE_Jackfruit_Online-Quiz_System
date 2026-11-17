# Code coverage report (lines)

Date: 2026-11-17  |  Commit: `47e7d37`  |  Project: Online Quiz System

Measured with `gcc --coverage` and `gcov`. I built the program with coverage turned on and ran it two ways.

## Unit tests (scoring module only)

| File | Lines | Lines run |
|---|---|---|
| `src/score.c` | 25 | 100.0% |

## System tests (the 22 test cases against the whole program)

| File | Lines | Lines run |
|---|---|---|
| `src/main.c` | 77 | 89.6% |
| `src/quiz.c` | 86 | 82.6% |
| `src/score.c` | 25 | 88.0% |
| `src/storage.c` | 71 | 83.1% |
| **All source files** | 259 | **85.3%** |

### Lines the system tests did not run

- `src/main.c` line 84: `return -1;`
- `src/main.c` line 88: `} else if (!feof(stdin)) {`
- `src/main.c` line 90: `while ((discarded = getchar()) != '\n' && discarded != EOF)`
- `src/main.c` line 94: `buffer[--length] = '\0';`
- `src/main.c` line 109: `return;`
- `src/main.c` line 111: `strcpy(playerName, "Player");`
- `src/main.c` line 116: `printf("%sError: could not load any questions.%s\n", ui(UI_RED), ui(UI_RESET));`
- `src/main.c` line 117: `return;`
- `src/quiz.c` line 28: `text++;`
- `src/quiz.c` line 34: `text++;`
- `src/quiz.c` line 46: `return 0;`
- `src/quiz.c` line 48: `wantedCount = loadedCount;`
- `src/quiz.c` line 66: `source = (const unsigned char *)"";`
- `src/quiz.c` line 81: `return -1;`
- `src/quiz.c` line 84: `sqlite3_close(database);`
- `src/quiz.c` line 85: `return -1;`
- `src/quiz.c` line 91: `sqlite3_close(database);`
- `src/quiz.c` line 92: `return -1;`
- `src/quiz.c` line 109: `fprintf(stderr,`
- `src/quiz.c` line 112: `continue;`
- `src/quiz.c` line 137: `} else if (!feof(stdin)) {`
- `src/quiz.c` line 138: `while ((discarded = getchar()) != '\n' && discarded != EOF)`
- `src/quiz.c` line 142: `buffer[length - 1] = '\0';`
- `src/score.c` line 27: `return result;`
- `src/score.c` line 51: `printf("%sError: result is unavailable.%s\n", ui(UI_RED), ui(UI_RESET));`
- `src/score.c` line 52: `return;`
- `src/storage.c` line 29: `return -1;`
- `src/storage.c` line 46: `return -1;`
- `src/storage.c` line 70: `} else if (!feof(file)) {`
- `src/storage.c` line 71: `while ((discarded = fgetc(file)) != '\n' && discarded != EOF)`
- `src/storage.c` line 75: `buffer[length - 1] = '\0';`
- `src/storage.c` line 89: `return -1;`
- `src/storage.c` line 92: `return -1;`
- `src/storage.c` line 95: `nameLength = NAME_LEN - 1;`
- `src/storage.c` line 133: `continue;                       /* skip lines in the wrong format */`
- `src/storage.c` line 135: `break;`
- `src/storage.c` line 148: `printf("\n%sNo results yet. Play a quiz first.%s\n", ui(UI_YELLOW), ui(UI_RESET)`
- `src/storage.c` line 149: `return;`

Most of these are error paths and unusual input: very long lines, input ending at a prompt, a missing question file, an empty or damaged results file. A few are covered by the unit tests instead.

To regenerate this report: `python tests/make_reports.py`
