# AI Use Note

**Project:** Online Quiz System (Software Engineering mini-project)
**Tool:** Claude Code (Anthropic), used by Tejas (repository owner) in a VS Code session, as permitted by the course.

## 1. Approach

AI was used as an assistant under human direction, not as an autonomous author. For each task the workflow was:

1. **Define the task and its source of truth.** The requirement came from the course deliverable PDFs (Part-1 and Part-2), the SRS, or the instructor's review comments.
2. **Prompt for a specific, bounded outcome**, for example "review this pull request and merge it if the build and tests pass", or "rewrite the backlog as user stories".
3. **Review the output.** Code was built and run, documents were read, and changes were requested where needed (simpler wording, fewer tables, shorter sections).
4. **Verify with evidence.** Every code change was checked with `make` (no warnings), the unit tests (`make test`) and the system tests (`tests/run_system_tests.sh`). GitHub Actions rebuilt every push and pull request.
5. **Record the result** in the repository (issues, pull requests, commits, Test Plan results).

## 2. Methodologies followed

| Methodology | How it was applied |
|---|---|
| **Agile / Scrum** | The SRS requirements became a product backlog of 32 user stories ("As a ..., I want ..., so that ...") with story points (64 in total), assignees, 6 epics and two sprints on a GitHub Project board. Each sprint ended with a demo video. |
| **IEEE-style documentation** | The SRS, the Software Architecture and Design Specification and the Software Test Plan follow the course templates (Part-1 deliverables), including a use case diagram, a security section, component and sequence diagrams, API design and error handling. |
| **Requirements traceability** | Each requirement (REQ-1 to REQ-22, NFR-1 to NFR-5) is traced to a backlog item, to a component in the SAD and to a test case in the Test Plan. |
| **Verification and validation** | Unit tests (scoring module), integration and system tests (20 scripted test cases), boundary tests (the 40% pass mark), negative tests (invalid input, missing or damaged files) and security checks (SEC-1 to SEC-6). |
| **Code review and continuous integration** | Pull requests were reviewed before merging and the build ran on every push and pull request. Merges were checked against the build, the tests and a full run of the program. |
| **Change control** | Differences from the submitted documents are recorded in `docs/DEVIATIONS.md`. The development cycle was frozen with the tag `v1.0-freeze` at the end of Sprint-2. |

## 3. Where AI assistance was used

| Area | AI contribution | Human direction and verification |
|---|---|---|
| Code review and merging | Reviewed teammates' pull requests, ran the build and tests, and merged them | Tejas requested each review and approved each merge |
| Implementation | Implemented the quiz flow (`run_quiz`, REQ-18 to REQ-22) and the leaderboard (`show_leaderboard`, REQ-14 to REQ-16); added boundary and invalid-input unit tests (tests 6 to 8 in `tests/test_score.c`); small clean-ups such as the named pass-mark constant | Tejas ran the program and the test suite |
| Documentation | Drafted the SAD and Test Plan, corrected and simplified the SRS, and produced the `.docx` and `.pdf` versions | Tejas reviewed the drafts and directed the revisions |
| Test automation | Wrote `tests/run_system_tests.sh`, which executes the 20 system test cases | Results come from real runs of the program |
| Project management | Rewrote the backlog as user stories, created the epics, and migrated the repository and project board to the organisation | Tejas created the organisation and set the requirements |
| Demo videos | Produced `docs/demo/sprint1-demo.mp4` and `sprint2-demo.mp4` as screen captures of a real terminal window running the program, with the commands entered by a script | Tejas reviewed the videos |
| Learning support | Explained the project, the SRS and the testing approach | |

## 4. Prompting summary

Work was directed through a chat session in stages that followed the project plan:

| Stage | Main tasks requested |
|---|---|
| Sprint-1 | Review and merge open pull requests; fix small code issues ahead of the quiz-engine integration |
| Documentation (Part-1) | Prepare the SRS corrections, the SAD and the Test Plan in line with the mini-project guidelines; simplify the wording; shorten the SAD and the Test Plan to the required sections |
| Project management (Part-2) | Prepare the repository and the project board for the instructor's checkpoint; restate the backlog as user stories grouped under epics; move the repository and the board into the organisation |
| Sprint-2 and wrap-up | Complete the remaining items (leaderboard, test run, speed and security checks); produce the demo videos; cross-check the deliverables against the course PDFs |

A record of the session can be provided if required.

## 5. Attribution

Commits made with AI assistance are committed under Tejas's name. The menu was written by Tejas, the scoring module by Vikas, question loading and input hardening by Sanjana, and result saving by Vaishnavi; their commits keep their own names in the Git history.
