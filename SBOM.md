# Software Bill of Materials (SBOM)

Online Quiz System, version 1.0. This lists every third-party component the program is built from or needs to run.

## Runtime and build dependencies

| Component | Version | Licence | Used for | Where |
|---|---|---|---|---|
| SQLite | 3.46.1 (built and tested with); any 3.x works | Public domain | Stores and reads the question bank (`data/quiz.db`) | `src/quiz.c` (`sqlite3_open_v2`, `sqlite3_prepare_v2`, `sqlite3_step`); linked with `-lsqlite3` in the `Makefile` |

On Ubuntu (GitHub Actions) the package is `libsqlite3-dev`. On Windows it is `mingw-w64-ucrt-x86_64-sqlite3` (MSYS2).

## Build and test tools (not shipped with the program)

| Tool | Licence | Used for |
|---|---|---|
| GCC (`-std=c99 -Wall`) | GPL (with runtime library exception) | Compiling the code |
| GNU Make | GPL | Build and `make test` |
| gcov (part of GCC) | GPL | Line and branch coverage reports |
| GitHub Actions (`actions/checkout@v4`) | MIT | Continuous integration |
| cppcheck | GPL | Static analysis in CI |

## Other notes

- No other libraries are used. The program uses only the C standard library apart from SQLite.
- The project's own licence is MIT (see `LICENSE`). SQLite is public domain, so there is no licence conflict.
- Format: this is a plain Markdown list, not a machine-readable SPDX or CycloneDX file.
