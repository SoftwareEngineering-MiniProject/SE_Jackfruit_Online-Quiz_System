# AI Use Note

**Project:** Online Quiz System (Software Engineering mini-project)
**Tool:** Claude Code (Anthropic), used by Tejas (repository owner) in a VS Code session, as permitted by the course.

## Where AI assistance was used

| Area | AI contribution | Human direction and verification |
|---|---|---|
| Code review and merging | Reviewed teammates' pull requests, ran the build and tests, and merged them | Tejas requested each review and approved each merge |
| Implementation | Implemented the quiz flow (`run_quiz`, REQ-18 to REQ-22) and the leaderboard (`show_leaderboard`, REQ-14 to REQ-16); added boundary and invalid-input unit tests (tests 6 to 8 in `tests/test_score.c`); small clean-ups such as the named pass-mark constant | Tejas ran the program and the test suite |
| Documentation | Drafted the SAD and Test Plan, corrected and simplified the SRS, and produced the `.docx` and `.pdf` versions | Tejas reviewed the drafts and directed the revisions |
| Test automation | Wrote `tests/run_system_tests.sh`, which executes the 20 system test cases | Results come from real runs of the program |
| Project management | Rewrote the backlog as user stories, created the epics, and migrated the repository and project board to the organisation | Tejas created the organisation and set the requirements |
| Demo videos | Produced `docs/demo/sprint1-demo.mp4` and `sprint2-demo.mp4` as screen captures of a real terminal window running the program, with the commands entered by a script | Tejas reviewed the videos |
| Learning support | Explained the project, the SRS and the testing approach | |

## Prompts

Work was directed through a chat session. The main tasks requested were:

- review and merge the open pull requests;
- fix small code issues ahead of the quiz-engine integration;
- prepare the SRS corrections, the SAD and the Test Plan in line with the mini-project guidelines;
- simplify the wording of the documents;
- prepare the repository and project board for the instructor's checkpoint;
- restate the backlog as user stories ("As a ..., I want ..., so that ...") grouped under epics;
- move the repository and the board into the organisation;
- shorten the SAD and the Test Plan to the required sections;
- complete the remaining Sprint-2 items; and
- produce the demo videos.

A record of the session can be provided if required.

## Attribution

Commits made with AI assistance are committed under Tejas's name. The menu was written by Tejas, the scoring module by Vikas, question loading and input hardening by Sanjana, and result saving by Vaishnavi; their commits keep their own names in the Git history.
