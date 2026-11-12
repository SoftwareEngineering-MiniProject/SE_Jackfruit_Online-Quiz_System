# Deviations from the Submitted Documents

This file lists where the final project differs from the SRS, SAD and Test Plan as first submitted, and why. It is for the final demo (Part-2, item 2i).

## Changes to the documents

| # | What changed | Why |
|---|---|---|
| 1 | SRS went from v1.0 to v1.1: added a use case diagram and use case table (3.5), security objectives and requirements (5.3), IDs and measurable criteria for the NFRs (NFR-1 to NFR-5), and the 40% pass mark (REQ-10, BR-6) | Part-1 guideline asked for a use case diagram, a security section and measurable requirements |
| 2 | SAD and Test Plan were written against the SRS v1.1 and then shortened to the sections the template needs | Part-1 guideline and instructor feedback |
| 3 | The Test Plan results were updated after each sprint: 18 of 20 passed on 12-Oct (leaderboard not built yet), 20 of 20 passed on 27-Oct | Test results are recorded as the code is finished |
| 4 | GitHub issues were rewritten as user stories ("As a ..., I want ..., so that ...") and grouped under 6 epics; the repo and board moved to the organisation `SoftwareEngineering-MiniProject` | Instructor review of the checkpoint |
| 5 | The question bank moved from `data/questions.txt` to a SQLite database (`data/quiz.db`, built from `data/questions.sql`). The SRS, SAD, Test Plan, README and diagrams now say so; results stay in `results.txt` | A database keeps the questions as proper data and new questions are added with a normal SQL `INSERT`. This replaces the earlier "no database" decision |
| 6 | The question bank now has 25 questions and each quiz asks 10 of them at random without repeats (REQ-17 updated). Two test cases were added (TC-21, TC-22). The tests set `QUIZ_ORDERED=1` and `QUIZ_COUNT=5` so their scores stay predictable | Different questions on every attempt; the system tests need fixed questions |

## Changes in the program compared with the documents

| # | Document says | Program does | Reason |
|---|---|---|---|
| 1 | REQ-12: pass "score and percentage" to storage | The whole `Result` is passed (score, percentage, counts), but only `Name,Score` is saved to `results.txt` | BR-4 sets the saved format to name and score |
| 2 | The question file layout was only shown by the sample file | Each question is 6 lines: text, options A to D, correct letter. A question with a bad letter is skipped (with a `Warning:` line on the error output) and a cut-off last question is dropped | NFR-4: bad data must not crash the program |
| 3 | REQ-19: accept A, B, C or D | Lowercase letters and spaces around the letter are also accepted | Easier to use |
| 4 | REQ-15: leaderboard sorted high to low | Equal scores keep the order they were saved in; at most 1000 results are read; lines in the wrong format are skipped | Needed a fixed rule for ties and for damaged files |
| 5 | REQ-16: missing file handled | A missing, empty or unreadable `results.txt` shows "No results yet. Play a quiz first." | Short message, no crash (SEC-4) |
| 6 | Terminal colours (`include/ui.h`) | Added at the end: colour on titles, menu numbers, errors, Pass/Fail and the top three leaderboard rows. Text is the same; colours switch off when output is piped or `NO_COLOR` is set | Easier to read; the demo videos were recorded with the colours on (the beeps are not in the recordings) |
| 7 | Sounds (`ui_sound` in `include/ui.h`) | A short beep after each answer and a different tone for Pass or Fail. Played only on a real terminal; `NO_SOUND` turns it off and piped output is silent | Feedback while moving to the next question |

## Changes in how the work was done

| # | Guideline | What happened | Reason |
|---|---|---|---|
| 1 | Record a video of the outcome | The two demo videos in `docs/demo/` are screen captures of a real terminal window running the program, with the commands typed in by a script instead of by hand | Made with AI help, not with a manual screen recorder |
