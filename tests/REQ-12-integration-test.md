\# REQ-12 Integration Test



\## Objective



Verify that the completed quiz result is passed to the storage module after each quiz attempt.



\## Test Procedure



1\. Build and run the Online Quiz System.

2\. Select \*\*Start Quiz\*\* from the main menu.

3\. Enter a player name.

4\. Answer all five questions.

5\. Observe the final result screen.

6\. Check that the program calls the storage function with the player's name and completed `Result` structure.

7\. Check the storage function's return value and confirm that no storage error is reported.



\## Expected Result



\- The quiz calculates the final score and percentage.

\- The final result is displayed after the quiz ends.

\- The player's name and completed result are passed to `save\_result()`.

\- The storage function successfully appends the player's name and score to `results.txt`.



\## Acceptance Criteria



The player's name, score, and percentage are available in the completed result passed to the storage module after each quiz attempt.



\## Scope



This test verifies REQ-12 integration. It does not change the storage file format or implement leaderboard functionality, which belong to the storage module requirements.



