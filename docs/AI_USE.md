# AI Use Note

**Project:** Online Quiz System (Software Engineering mini-project)
**Tool:** Claude Code (Anthropic), used by Tejas (repo owner) in his own VS Code session.

## What the AI was used for

| Area | What the AI did | Who decided and checked |
|---|---|---|
| Pull requests | Read the teammates' pull requests, ran the build and tests, and merged them after review | Tejas asked for each review and merge |
| Code | Wrote `run_quiz` (the quiz flow, REQ-18 to REQ-22), `show_leaderboard` (REQ-14 to REQ-16) and extra unit tests (tests 6 to 8 in `tests/test_score.c`); small fixes such as the pass-mark constant | Tejas ran the program and the tests |
| Documents | Wrote the SAD and Test Plan, fixed and simplified the SRS, and converted all three to `.docx` and `.pdf` | Tejas reviewed and asked for changes (shorter, simpler wording) |
| Test scripts | Wrote `tests/run_system_tests.sh`, which runs the 20 system test cases | Results are from real runs of the program |
| GitHub setup | Rewrote the issues as user stories, created the epics, moved the repo and the board to the organisation | Tejas created the organisation and gave the instructions |
| Demo videos | Generated `docs/demo/sprint1-demo.mp4` and `sprint2-demo.mp4`. They are replays built from the real output of the program, not screen recordings | The videos say so on their first slide |
| Teaching | Explained the project, the SRS and the testing so Tejas could answer questions | |

## Prompts

The prompts were given in a chat. In short, they asked the AI to:

- check and merge the open pull requests, with a review,
- fix small code problems before the quiz engine was ready,
- write the SRS fixes, the SAD and the Test Plan following the mini-project guideline PDFs,
- use simpler words in the documents,
- prepare the repository and project board for the instructor's checkpoint,
- change the user stories to the "As a ..., I want ..., so that ..." format and group them under epics,
- move the repository and the board into the organisation,
- shorten the SAD and the Test Plan,
- finish the leaderboard and the remaining sprint items, and
- produce the demo videos.

The full conversation is available on request.

## Commits

Commits made with the AI's help are under Tejas's name. Teammates' own work (the menu, scoring tests, storage, question loading and the robustness changes) was written and committed by them and keeps their names in the Git history.
