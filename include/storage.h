#ifndef STORAGE_H
#define STORAGE_H

#include "score.h"

/**
 * @file storage.h
 * @brief Saving results to results.txt and showing the leaderboard
 *        (owner: Vaishnavi).
 */

/**
 * @brief Appends "Name,Score" as one line to results.txt.
 * @return 0 on success, -1 on error.
 */
int saveResult(const char *playerName, const Result *result);

/** @brief Reads results.txt and prints the scores from highest to lowest. */
void showLeaderboard(void);

#endif
