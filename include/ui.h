#ifndef UI_H
#define UI_H

/* ui.h: optional terminal colours.
   Colours are used only when the output goes to a terminal. When the output is
   piped or redirected to a file (as in the tests), or when NO_COLOR is set,
   ui() returns an empty string, so the text is exactly the same as before.
   Set FORCE_COLOR=1 to switch colours on even when the output is piped. */

#include <stdlib.h>

#ifdef _WIN32
#include <io.h>
#include <windows.h>
#define UI_ISATTY_STDOUT() _isatty(1)
#else
#include <unistd.h>
#define UI_ISATTY_STDOUT() isatty(1)
#endif

#define UI_RESET   "\033[0m"
#define UI_BOLD    "\033[1m"
#define UI_DIM     "\033[2m"
#define UI_RED     "\033[91m"
#define UI_GREEN   "\033[92m"
#define UI_YELLOW  "\033[93m"
#define UI_BLUE    "\033[94m"
#define UI_MAGENTA "\033[95m"
#define UI_CYAN    "\033[96m"

/* 1 if colours are on, 0 if not. Worked out once per source file. */
static inline int ui_on(void)
{
    static int state = -1;

    if (state < 0) {
        state = 0;
        if (getenv("NO_COLOR") == NULL) {
            if (getenv("FORCE_COLOR") != NULL) {
                state = 1;
            } else if (UI_ISATTY_STDOUT()) {
                state = 1;
#ifdef _WIN32
                {
                    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
                    DWORD mode = 0;

                    /* ask the Windows console to understand colour codes */
                    if (h == INVALID_HANDLE_VALUE || !GetConsoleMode(h, &mode) ||
                        !SetConsoleMode(h, mode | 0x0004))
                        state = 0;
                }
#endif
            }
        }
    }
    return state;
}

/* Returns the colour code, or "" when colours are off. */
static inline const char *ui(const char *code)
{
    return ui_on() ? code : "";
}

#endif
