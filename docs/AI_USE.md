# AI Use Note

Project: Online Quiz System (Software Engineering mini-project)

I used Claude Code (Anthropic) in VS Code while working on this project. The course allows the use of AI tools, so this note explains how I used it.

## How I worked with it

I gave it one task at a time, usually taken straight from the course PDFs, our SRS, or the instructor's feedback after the checkpoint. I read what came back, ran the code, and asked for changes when something was off. For example, I asked for simpler wording in the documents, shorter sections in the SAD and Test Plan, and a different format for the user stories.

Nothing was accepted just because it looked right. Code changes had to build without warnings and pass `make test`. Before the final submission I also ran the 20 system test cases with `tests/run_system_tests.sh`. GitHub Actions built every push and pull request.

## Process we followed

- **Scrum:** the SRS requirements became a backlog of 32 user stories on a GitHub Project board, with story points (64 in total), assignees, 6 epics and two sprints. Each sprint ended with a demo video.
- **Documents:** the SRS, SAD and Test Plan follow the templates and the Part-1 guideline.
- **Traceability:** every requirement is linked to a backlog item, a component in the SAD and a test case in the Test Plan.
- **Testing:** unit tests for scoring, plus scripted tests for valid input, invalid input, a missing or damaged results file, the 40% pass mark and the security requirements.
- **Review:** pull requests were looked at before merging. Where our documents and the final program differ, `docs/DEVIATIONS.md` says so.

## What the AI did

- Reviewed and merged the teammates' pull requests after checking the build and tests.
- Wrote the quiz flow (`run_quiz`) and the leaderboard (`show_leaderboard`), and added a few unit tests (tests 6 to 8 in `tests/test_score.c`).
- Drafted the SAD and the Test Plan, fixed and simplified the SRS, and made the Word and PDF versions.
- Wrote `tests/run_system_tests.sh`.
- Rewrote the issues as user stories, set up the epics, and moved the repo and board to the organisation.
- Made the two demo videos. They are screen captures of a real terminal running the program, with the commands typed in by a script.
- Explained the project and the testing to me so I could answer questions about it.

## What I did

I set the tasks and checked the results, and I decided what stayed. I created the organisation, ran the program and the tests, and approved every merge.

## Who wrote what

Commits made with the AI's help are under my name. The menu is mine. The scoring module is Vikas's, question loading and input checks are Sanjana's, and saving results is Vaishnavi's. Their commits keep their own names in the Git history.

I can share the chat record if it is needed.
