# Coding standard

This is the standard the C code in `src/`, `include/` and `tests/` follows.

## Language and build

- C99 (`gcc -std=c99`), compiled with `-Wall`. The code must build with zero warnings.
- Code that needs a platform-specific call (Windows or Linux) is kept inside `#ifdef _WIN32` blocks.

## Naming (camelCase)

| Item | Style | Examples |
|---|---|---|
| Functions | camelCase, starts with a verb | `calculateResult`, `loadQuestions`, `showLeaderboard` |
| Variables and parameters | camelCase, a full word that says what it holds | `questionCount`, `playerName`, `answerLetter` |
| Types (struct names) | PascalCase | `Question`, `Result`, `Entry` |
| Macros and constants | UPPER_SNAKE_CASE | `PASS_PERCENT`, `MAX_QUESTIONS`, `RESULTS_FILE` |
| Files | lowercase | `quiz.c`, `score.h` |

Rules: no one-letter names except short loop counters in tests; no abbreviations that are not obvious; booleans are 1 or 0 (`passed`).

## Documentation

- Every `.c` and `.h` file starts with a `@file` / `@brief` comment that names the module and its owner.
- Every function declared in a header has a Doxygen-style comment (`/** ... */`) with `@brief`, `@param` and `@return`.
- Every function in a `.c` file has a short comment above it saying what it does and what it returns, including error values.
- Requirement IDs (`REQ-`, `NFR-`, `SEC-`) are written in comments next to the code that satisfies them.

## Safe C (security)

- No `gets`, `scanf` or `sprintf`. Read lines with `fgets` into a fixed buffer and discard the rest of an over-long line.
- Convert numbers with `strtol` and check the end pointer. Use `snprintf` when formatting into a buffer.
- Check every `fopen` and `fclose`. Never print file paths or system error text to the user (SEC-1).
- Test case TC-18 scans `src/*.c` for the banned functions and fails the build if any appear.

## Layout

- 4 spaces for indentation, no tabs. Opening brace of a function on its own line; for `if`, `for` and `while` the brace stays on the same line.
- One statement per line. Keep functions short enough to read on one screen where possible.

## Checks before a pull request is merged

1. `make` builds with no warnings.
2. `make test` passes (8 unit test cases).
3. `bash tests/run_system_tests.sh` passes (TC-01 to TC-22).
4. A teammate reviews the pull request. The GitHub Actions workflow runs steps 1 to 3 on every push.
