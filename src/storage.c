#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h"

#define RESULTS_FILE "results.txt"
#define MAX_NAME     64      /* longest player name kept */
#define MAX_ENTRIES  1000    /* most records loaded from the file */
#define LINE_LEN     128     /* a valid "Name,Score" line is well under this */

/* One saved record from results.txt */
typedef struct {
    char name[MAX_NAME + 1];
    int  score;
} Entry;

/* Appends "Name,Score" as one line to results.txt.
   Returns 0 on success, -1 on any error. */
int save_result(const char *name, const Result *r)
{
    FILE *fp;
    const char *p;

    if (name == NULL || r == NULL) {
        return -1;
    }

    fp = fopen(RESULTS_FILE, "a");      /* "a" = append, keeps old results */
    if (fp == NULL) {
        return -1;                       /* no paths or details printed */
    }

    /* Commas/newlines in a name would break the Name,Score format,
       so replace them with spaces. */
    for (p = name; *p != '\0'; p++) {
        fputc((*p == ',' || *p == '\n' || *p == '\r') ? ' ' : *p, fp);
    }
    fprintf(fp, ",%d\n", r->score);

    if (fclose(fp) != 0) {
        return -1;
    }
    return 0;
}

/* REQ-14: turn one "Name,Score" line into an Entry.
   Returns 1 if the line is valid, 0 if it is damaged (the caller skips it). */
static int parse_line(char *line, Entry *e)
{
    char *comma, *end;
    long  score;

    line[strcspn(line, "\r\n")] = '\0';          /* remove the line ending */

    comma = strrchr(line, ',');                  /* score comes after the last comma */
    if (comma == NULL || comma == line) {
        return 0;                                /* no comma, or empty name */
    }
    if ((size_t)(comma - line) > MAX_NAME) {
        return 0;                                /* name too long for the Entry */
    }

    errno = 0;
    score = strtol(comma + 1, &end, 10);
    if (end == comma + 1 || *end != '\0' || errno == ERANGE ||
        score < 0 || score > INT_MAX) {
        return 0;                                /* score is not a valid number */
    }

    memcpy(e->name, line, (size_t)(comma - line));
    e->name[comma - line] = '\0';
    e->score = (int)score;
    return 1;
}

/* REQ-14: read the saved results from results.txt into list[].
   Returns how many records were read. A file that does not exist yet counts
   as 0 records. Returns -1 if the file exists but cannot be read.
   Damaged lines are skipped. */
static int load_results(Entry list[], int max)
{
    FILE *fp = fopen(RESULTS_FILE, "r");
    char  line[LINE_LEN];
    int   count = 0;

    if (fp == NULL) {
        return (errno == ENOENT) ? 0 : -1;
    }

    while (count < max && fgets(line, sizeof line, fp) != NULL) {
        size_t len = strlen(line);

        if (len > 0 && line[len - 1] != '\n' && !feof(fp)) {
            int c;                               /* line longer than the buffer: */
            while ((c = fgetc(fp)) != '\n' && c != EOF) {
                ;                                /* skip the rest of it */
            }
            continue;
        }
        if (parse_line(line, &list[count])) {
            count++;
        }
    }

    if (ferror(fp)) {
        fclose(fp);
        return -1;
    }
    fclose(fp);
    return count;
}

/* For now this prints the saved results in file order, so REQ-14 can be
   checked. Sorting is REQ-15; messages for no results / read errors are
   REQ-16 and NFR-2. */
void show_leaderboard(void)
{
    static Entry list[MAX_ENTRIES];     /* static: keeps the big array off the stack */
    int n = load_results(list, MAX_ENTRIES);
    int i;

    for (i = 0; i < n; i++) {
        printf("%-30s %d\n", list[i].name, list[i].score);
    }
}
