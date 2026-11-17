#ifndef UI_H
#define UI_H

/**
 * @file ui.h
 * @brief Optional terminal colours and sounds.
 *
 * Colours are used only when the output goes to a terminal. When the output is
 * piped or redirected to a file (as in the tests), or when NO_COLOR is set,
 * ui() returns an empty string, so the text is exactly the same as before.
 * Set FORCE_COLOR=1 to switch colours on even when the output is piped.
 */

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <io.h>
#include <windows.h>
#define UI_ISATTY_STDOUT() _isatty(1)
#else
#include <unistd.h>
#define UI_ISATTY_STDOUT() isatty(1)
#endif

/* ANSI escape codes for the colours and styles used by the program. */
#define UI_RESET   "\033[0m"
#define UI_BOLD    "\033[1m"
#define UI_DIM     "\033[2m"
#define UI_RED     "\033[91m"
#define UI_GREEN   "\033[92m"
#define UI_YELLOW  "\033[93m"
#define UI_BLUE    "\033[94m"
#define UI_MAGENTA "\033[95m"
#define UI_CYAN    "\033[96m"

/**
 * @brief Tells whether colours are switched on.
 * @return 1 if colours are on, 0 if not. Worked out once per source file.
 */
static inline int uiOn(void)
{
    static int colourState = -1;

    if (colourState < 0) {
        colourState = 0;
        if (getenv("NO_COLOR") == NULL) {
            if (getenv("FORCE_COLOR") != NULL) {
                colourState = 1;
            } else if (UI_ISATTY_STDOUT()) {
                colourState = 1;
#ifdef _WIN32
                {
                    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
                    DWORD consoleMode = 0;

                    /* ask the Windows console to understand colour codes */
                    if (consoleHandle == INVALID_HANDLE_VALUE ||
                        !GetConsoleMode(consoleHandle, &consoleMode) ||
                        !SetConsoleMode(consoleHandle, consoleMode | 0x0004))
                        colourState = 0;
                }
#endif
            }
        }
    }
    return colourState;
}

/**
 * @brief Returns the colour code, or "" when colours are off.
 * @param code  One of the UI_* colour or style macros.
 */
static inline const char *ui(const char *code)
{
    return uiOn() ? code : "";
}

/* Optional sounds. Played only on a real terminal (never when piped, so the
   tests stay silent) and not when NO_SOUND is set. Windows plays short tones;
   other systems use the terminal bell. */
#define UI_SOUND_NEXT 0
#define UI_SOUND_PASS 1
#define UI_SOUND_FAIL 2

/**
 * @brief Plays a short sound if sounds are enabled.
 * @param soundKind  UI_SOUND_NEXT, UI_SOUND_PASS or UI_SOUND_FAIL.
 */
static inline void uiSound(int soundKind)
{
    static int soundState = -1;

    if (soundState < 0)
        soundState = (getenv("NO_SOUND") == NULL && UI_ISATTY_STDOUT()) ? 1 : 0;
    if (!soundState)
        return;
#ifdef _WIN32
    if (soundKind == UI_SOUND_PASS) {
        Beep(784, 90); Beep(988, 90); Beep(1319, 160);
    } else if (soundKind == UI_SOUND_FAIL) {
        Beep(392, 150); Beep(262, 250);
    } else {
        Beep(880, 60);
    }
#else
    (void)soundKind;
    putchar('\a');
    fflush(stdout);
#endif
}

#endif
