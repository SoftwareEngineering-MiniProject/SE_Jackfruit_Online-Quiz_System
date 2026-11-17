/**
 * @file storage.c
 * @brief Result storage and leaderboard (owner: Vaishnavi).
 *
 * Results are kept as "Name,Score" lines in results.txt. The leaderboard
 * reads the file again each time, skips badly formatted lines, and prints the
 * scores from highest to lowest.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h"
#include "ui.h"

#define RESULTS_FILE "results.txt"
#define MAX_ENTRIES  1000          /* more results than this are ignored */
#define NAME_LEN     64
#define LINE_LEN     256

/* Appends "Name,Score" as one line to results.txt.
   Returns 0 on success, -1 on any error. */
int saveResult(const char *playerName, const Result *result)
{
    FILE *resultsFile;
    const char *nameChar;

    if (playerName == NULL || result == NULL) {
        return -1;
    }

    resultsFile = fopen(RESULTS_FILE, "a");   /* "a" = append, keeps old results */
    if (resultsFile == NULL) {
        return -1;                            /* no paths or details printed */
    }

    /* Commas/newlines in a name would break the Name,Score format,
       so replace them with spaces. */
    for (nameChar = playerName; *nameChar != '\0'; nameChar++) {
        fputc((*nameChar == ',' || *nameChar == '\n' || *nameChar == '\r') ? ' ' : *nameChar,
              resultsFile);
    }
    fprintf(resultsFile, ",%d\n", result->score);

    if (fclose(resultsFile) != 0) {
        return -1;
    }
    return 0;
}

/** One line of the results file: a player name and a score. */
typedef struct {
    char name[NAME_LEN];
    int score;
} Entry;

/* Reads one line (without the line ending) into buffer and drops any extra
   characters so the next read starts on a new line. Returns 0 on success,
   -1 at end of file. */
static int readLine(FILE *file, char *buffer, size_t size)
{
    size_t length;
    int discarded;

    if (fgets(buffer, (int)size, file) == NULL)
        return -1;
    length = strlen(buffer);
    if (length > 0 && buffer[length - 1] == '\n') {
        buffer[--length] = '\0';
    } else if (!feof(file)) {
        while ((discarded = fgetc(file)) != '\n' && discarded != EOF)
            ;
    }
    if (length > 0 && buffer[length - 1] == '\r')
        buffer[length - 1] = '\0';
    return 0;
}

/* Splits "Name,Score" into an Entry. Returns 0 if the line is valid,
   -1 if it is in the wrong format (such lines are skipped). */
static int parseEntry(const char *line, Entry *entry)
{
    const char *comma = strrchr(line, ',');
    char *scoreEnd;
    long score;
    size_t nameLength;

    if (comma == NULL || comma == line)
        return -1;
    score = strtol(comma + 1, &scoreEnd, 10);
    if (scoreEnd == comma + 1 || *scoreEnd != '\0' || score < 0 || score > 1000000)
        return -1;
    nameLength = (size_t)(comma - line);
    if (nameLength >= NAME_LEN)
        nameLength = NAME_LEN - 1;
    memcpy(entry->name, line, nameLength);
    entry->name[nameLength] = '\0';
    entry->score = (int)score;
    return 0;
}

/* Gold, silver and bronze colours for the first three places. */
static const char *rankColour(int rankIndex)
{
    if (rankIndex == 0)
        return ui(UI_BOLD UI_YELLOW);
    if (rankIndex == 1)
        return ui(UI_BOLD);
    if (rankIndex == 2)
        return ui(UI_MAGENTA);
    return "";
}

/* Reads results.txt and prints the entries from the highest score to the
   lowest. Entries with the same score keep the order they were saved in. */
void showLeaderboard(void)
{
    static Entry entries[MAX_ENTRIES];
    char line[LINE_LEN];
    Entry entry;
    FILE *resultsFile;
    int entryCount = 0;
    int insertIndex, printIndex;

    resultsFile = fopen(RESULTS_FILE, "r");
    if (resultsFile == NULL) {
        printf("\n%sNo results yet. Play a quiz first.%s\n", ui(UI_YELLOW), ui(UI_RESET));
        return;
    }

    while (readLine(resultsFile, line, sizeof line) == 0) {
        if (parseEntry(line, &entry) != 0)
            continue;                       /* skip lines in the wrong format */
        if (entryCount >= MAX_ENTRIES)
            break;
        /* Insert so the list stays sorted high to low; "<" keeps equal
           scores in the order they were saved. */
        for (insertIndex = entryCount;
             insertIndex > 0 && entries[insertIndex - 1].score < entry.score;
             insertIndex--)
            entries[insertIndex] = entries[insertIndex - 1];
        entries[insertIndex] = entry;
        entryCount++;
    }
    fclose(resultsFile);

    if (entryCount == 0) {
        printf("\n%sNo results yet. Play a quiz first.%s\n", ui(UI_YELLOW), ui(UI_RESET));
        return;
    }

    printf("\n%s%s===== Leaderboard =====%s\n", ui(UI_BOLD), ui(UI_CYAN), ui(UI_RESET));
    printf("%s%-5s %-30s %s%s\n", ui(UI_BOLD), "Rank", "Name", "Score", ui(UI_RESET));
    for (printIndex = 0; printIndex < entryCount; printIndex++)
        printf("%s%-5d %-30.30s %d%s\n", rankColour(printIndex), printIndex + 1,
               entries[printIndex].name, entries[printIndex].score, ui(UI_RESET));
}
