#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "quiz.h"

#define ANSWER_LINE_LEN 64

/* Returns 'A'..'D' if the line is exactly one of those letters (either case,
   spaces around it allowed), otherwise 0. */
static char parse_answer(const char *s)
{
    char c;

    while (*s == ' ' || *s == '\t')
        s++;
    c = (char)toupper((unsigned char)*s);
    if (c < 'A' || c > 'D')
        return 0;
    s++;
    while (*s == ' ' || *s == '\t')
        s++;
    return (*s == '\0') ? c : 0;
}

/* Reads one line from the questions file into buf (size bytes). Strips \r\n
   and discards any characters that do not fit, so the next read starts on a
   fresh line and the fields never get out of step (NFR-4).
   Returns 0 on success, -1 at end of file. */
static int read_file_line(FILE *fp, char *buf, size_t size)
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

/* Loads up to max questions from path. Each question is 6 lines: the text,
   four options and the correct letter (A-D). Blank lines between questions
   are ignored, a question with a bad answer letter is skipped, and an
   incomplete last question is dropped.
   Returns the number of questions loaded, or -1 if the file cannot be
   opened or the arguments are invalid (NFR-4). */
int load_questions(const char *path, Question qs[], int max)
{
    FILE *fp;
    char line[256];
    int count = 0;

    if (path == NULL || qs == NULL || max <= 0)
        return -1;

    fp = fopen(path, "r");
    if (!fp)
        return -1;

    while (count < max) {
        Question q;
        char letter;
        int i;

        /* Skip blank lines between questions */
        do {
            if (read_file_line(fp, q.text, sizeof q.text) != 0) {
                fclose(fp);
                return count;
            }
        } while (q.text[0] == '\0');

        /* Read all 4 options */
        for (i = 0; i < 4; i++) {
            if (read_file_line(fp, q.options[i], sizeof q.options[i]) != 0) {
                fclose(fp);
                return count;          /* truncated last question: ignore it */
            }
        }

        /* Read the correct option letter ('A'..'D') */
        if (read_file_line(fp, line, sizeof line) != 0) {
            fclose(fp);
            return count;
        }

        letter = parse_answer(line);
        if (letter == 0) {
            fprintf(stderr,
                    "Warning: skipping question \"%.40s\" (correct answer must be A-D).\n",
                    q.text);
            continue;
        }

        q.correct = letter;
        qs[count++] = q;               /* only complete, valid questions are stored */
    }

    fclose(fp);
    return count;
}

/* Reads one line from the keyboard into buf. Extra characters on the line
   are discarded so the buffer cannot overflow (SEC-3).
   Returns 0 on success, -1 if there is no more input. */
static int read_answer_line(char *buf, size_t size)
{
    size_t len;
    int c;

    if (fgets(buf, (int)size, stdin) == NULL)
        return -1;
    len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[--len] = '\0';
    } else if (!feof(stdin)) {
        while ((c = getchar()) != '\n' && c != EOF)
            ;
    }
    if (len > 0 && buf[len - 1] == '\r')
        buf[len - 1] = '\0';
    return 0;
}

/* Shows the questions one by one and records the answer to each
   (REQ-18 to REQ-22). If the input ends early, the quiz stops and the
   remaining answers stay 0 (unanswered). */
void run_quiz(const Question qs[], int n, char answers[])
{
    char line[ANSWER_LINE_LEN];
    char ans;
    int i, k;

    if (qs == NULL || answers == NULL || n <= 0)
        return;

    /* Start with every answer unanswered, even if the caller did not clear it. */
    for (i = 0; i < n; i++)
        answers[i] = 0;

    for (i = 0; i < n; i++) {
        printf("\nQuestion %d of %d\n%s\n", i + 1, n, qs[i].text);
        for (k = 0; k < 4; k++)
            printf("  %c) %s\n", 'A' + k, qs[i].options[k]);

        /* Keep asking until the answer is A, B, C or D (REQ-19). */
        ans = 0;
        while (ans == 0) {
            printf("Your answer (A-D): ");
            if (read_answer_line(line, sizeof line) != 0) {
                printf("\n");
                return;
            }
            ans = parse_answer(line);
            if (ans == 0)
                printf("Error: please enter A, B, C or D.\n");
        }
        answers[i] = ans;   /* record it, then show the next question (REQ-20, REQ-21) */
    }
}