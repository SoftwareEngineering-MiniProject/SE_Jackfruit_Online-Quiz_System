#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h"

#define RESULTS_FILE "results.txt"
#define MAX_ENTRIES  1000          /* more results than this are ignored */
#define NAME_LEN     64
#define LINE_LEN     256

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

typedef struct {
    char name[NAME_LEN];
    int score;
} Entry;

/* Reads one line (without the line ending) into buf and drops any extra
   characters so the next read starts on a new line. Returns 0 on success,
   -1 at end of file. */
static int read_line(FILE *fp, char *buf, size_t size)
{
    size_t len;
    int c;

    if (fgets(buf, (int)size, fp) == NULL)
        return -1;
    len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[--len] = '\0';
    } else if (!feof(fp)) {
        while ((c = fgetc(fp)) != '\n' && c != EOF)
            ;
    }
    if (len > 0 && buf[len - 1] == '\r')
        buf[len - 1] = '\0';
    return 0;
}

/* Splits "Name,Score" into an Entry. Returns 0 if the line is valid,
   -1 if it is in the wrong format (such lines are skipped). */
static int parse_entry(const char *line, Entry *e)
{
    const char *comma = strrchr(line, ',');
    char *end;
    long score;
    size_t n;

    if (comma == NULL || comma == line)
        return -1;
    score = strtol(comma + 1, &end, 10);
    if (end == comma + 1 || *end != '\0' || score < 0 || score > 1000000)
        return -1;
    n = (size_t)(comma - line);
    if (n >= NAME_LEN)
        n = NAME_LEN - 1;
    memcpy(e->name, line, n);
    e->name[n] = '\0';
    e->score = (int)score;
    return 0;
}

/* Reads results.txt and prints the entries from the highest score to the
   lowest. Entries with the same score keep the order they were saved in. */
void show_leaderboard(void)
{
    static Entry list[MAX_ENTRIES];
    char line[LINE_LEN];
    Entry e;
    FILE *fp;
    int count = 0;
    int i, j;

    fp = fopen(RESULTS_FILE, "r");
    if (fp == NULL) {
        printf("\nNo results yet. Play a quiz first.\n");
        return;
    }

    while (read_line(fp, line, sizeof line) == 0) {
        if (parse_entry(line, &e) != 0)
            continue;                       /* skip lines in the wrong format */
        if (count >= MAX_ENTRIES)
            break;
        /* Insert so the list stays sorted high to low; "<" keeps equal
           scores in the order they were saved. */
        for (i = count; i > 0 && list[i - 1].score < e.score; i--)
            list[i] = list[i - 1];
        list[i] = e;
        count++;
    }
    fclose(fp);

    if (count == 0) {
        printf("\nNo results yet. Play a quiz first.\n");
        return;
    }

    printf("\n===== Leaderboard =====\n");
    printf("%-5s %-30s %s\n", "Rank", "Name", "Score");
    for (j = 0; j < count; j++)
        printf("%-5d %-30.30s %d\n", j + 1, list[j].name, list[j].score);
}
