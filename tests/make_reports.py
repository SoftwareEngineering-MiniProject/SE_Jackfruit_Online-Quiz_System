"""Generates the final test reports in docs/reports/.

Run from the repository root:  python tests/make_reports.py
Needs gcc, gcov, make (or mingw32-make) and bash on the PATH.
Everything is built and run in a temporary copy, so the repository stays clean.
"""
import datetime
import os
import re
import shutil
import subprocess
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(ROOT, "docs", "reports")
MAKE = "make" if shutil.which("make") else "mingw32-make"


def find_bash():
    """Git's bash, not the Windows WSL bash that a bare 'bash' can resolve to."""
    found = shutil.which("bash")
    if found and "system32" not in found.lower():
        return found
    for c in (r"C:\Program Files\Git\bin\bash.exe", r"C:\Program Files\Git\usr\bin\bash.exe"):
        if os.path.isfile(c):
            return c
    return found or "bash"


BASH = find_bash()

TC_REQ = {
    "TC-01": "REQ-1", "TC-02": "REQ-2, REQ-4", "TC-03": "REQ-3, NFR-4, SEC-3", "TC-04": "REQ-5",
    "TC-05": "REQ-6, SEC-6", "TC-06": "REQ-7, REQ-8, REQ-9", "TC-07": "REQ-10", "TC-08": "REQ-11",
    "TC-09": "REQ-13, SEC-2", "TC-10": "REQ-12", "TC-11": "REQ-17 to REQ-22", "TC-12": "REQ-19",
    "TC-13": "REQ-14, REQ-15", "TC-14": "REQ-16, SEC-4", "TC-15": "NFR-1", "TC-16": "SEC-1, SEC-4",
    "TC-17": "SEC-3, NFR-4", "TC-18": "SEC-5, SEC-3", "TC-19": "NFR-5", "TC-20": "NFR-3",
}


MAKE_PATH = (shutil.which(MAKE) or MAKE).replace(chr(92), "/")
if len(MAKE_PATH) > 2 and MAKE_PATH[1] == ":":           # C:/x/y -> /c/x/y, the form bash understands
    MAKE_PATH = "/" + MAKE_PATH[0].lower() + MAKE_PATH[2:]


def run(cmd, cwd, inp=None):
    env = dict(os.environ)
    r = subprocess.run(cmd, cwd=cwd, capture_output=True, text=True, input=inp, encoding="utf-8", errors="replace", env=env)
    return (r.stdout + r.stderr).replace("\r\n", "\n"), r.returncode


def need(ok, msg):
    if not ok:
        sys.exit("FAILED: " + msg)


commit = run(["git", "rev-parse", "--short", "HEAD"], ROOT)[0].strip()
stamp = datetime.date.today().isoformat()
head = f"Date: {stamp}  |  Commit: `{commit}`  |  Project: Online Quiz System\n"

tmp = tempfile.mkdtemp()
try:
    for d in ("src", "include", "tests", "data"):
        shutil.copytree(os.path.join(ROOT, d), os.path.join(tmp, d), ignore=shutil.ignore_patterns("*.exe", "*.o", "*.d", "*.gc*"))
    shutil.copy(os.path.join(ROOT, "Makefile"), tmp)

    # ------------------------------------------------------------ unit tests
    unit_out, rc = run([MAKE, "test"], tmp)
    need(rc == 0 and "All test cases passed" in unit_out, "unit tests did not pass\n" + unit_out[-800:])
    passed = len(re.findall(r"Test \d+ Passed", unit_out))

    # ------------------------------------------------------------ system tests (normal build)
    out, rc = run([MAKE], tmp)
    need(rc == 0, "build failed\n" + out[-500:])
    sys_out, rc = run([BASH, "tests/run_system_tests.sh", MAKE_PATH], tmp)
    rows = re.findall(r"^(PASS|FAIL)\s+(TC-\d+)\s+(.*)$", sys_out, re.M)
    summary = re.search(r"Result: (\d+) passed, (\d+) failed", sys_out)
    rows.sort(key=lambda r: int(r[1].split('-')[1]))
    need(rows and summary, "system test output not understood\n" + sys_out[-500:])

    # ------------------------------------------------------------ coverage
    def build_cov(objdir, sources, exe):
        os.makedirs(os.path.join(tmp, objdir), exist_ok=True)
        objs = []
        for s in sources:
            o = os.path.join(objdir, os.path.splitext(os.path.basename(s))[0] + ".o")
            out, rc = run(["gcc", "-std=c99", "-O0", "-Iinclude", "--coverage", "-c", s, "-o", o], tmp)
            need(rc == 0, "compile failed\n" + out)
            objs.append(o)
        out, rc = run(["gcc", "--coverage", "-o", exe] + objs, tmp)
        need(rc == 0, "link failed\n" + out)

    def gcov(objdir, sources):
        res = {}
        for s in sources:
            out, rc = run(["gcov", "-b", "-c", "-o", objdir, s], tmp)
            m = re.search(r"File '[^']*" + re.escape(s) + r"'\n(.*?)(?:Creating|\Z)", out, re.S)
            need(m, "gcov gave no data for " + s + "\n" + out)
            blk = m.group(1)
            lines = re.search(r"Lines executed:([\d.]+)% of (\d+)", blk)
            br = re.search(r"Branches executed:([\d.]+)% of (\d+)", blk)
            tk = re.search(r"Taken at least once:([\d.]+)% of (\d+)", blk)
            gfile = os.path.join(tmp, os.path.basename(s) + ".gcov")
            unl, ubr, cur = [], [], None
            if os.path.exists(gfile):
                for ln in open(gfile, encoding="utf-8", errors="replace").read().split("\n"):
                    m2 = re.match(r"\s*([\d#=-]+\*?):\s*(\d+):(.*)", ln)
                    if m2:
                        cur = int(m2.group(2))
                        if m2.group(1).startswith("#####"):
                            unl.append((cur, m2.group(3).strip()))
                    elif ln.startswith("branch") and cur and ("never executed" in ln or re.search(r"taken 0%", ln)):
                        if cur not in ubr:
                            ubr.append(cur)
            res[s] = dict(lp=float(lines.group(1)), ln=int(lines.group(2)),
                          bp=float(br.group(1)) if br else None, bn=int(br.group(2)) if br else 0,
                          tp=float(tk.group(1)) if tk else None, uncovered=unl, ubranches=ubr)
        return res

    srcs = ["src/main.c", "src/quiz.c", "src/score.c", "src/storage.c"]
    # (a) unit tests: only score.c is exercised
    build_cov("cov_unit", ["tests/test_score.c", "src/score.c"], "test_score_cov.exe")
    run([os.path.join(tmp, "test_score_cov.exe")], tmp)
    unit_cov = gcov("cov_unit", ["src/score.c"])
    # (b) system tests: instrumented program driven by the 20 scripted test cases
    build_cov("cov_sys", srcs, "quiz.exe")
    for f in os.listdir(tmp):
        if f.endswith(".gcda") or f.endswith(".gcov"):
            os.remove(os.path.join(tmp, f))
    run([BASH, "tests/run_system_tests.sh", MAKE_PATH], tmp)
    sys_cov = gcov("cov_sys", srcs)
finally:
    shutil.rmtree(tmp, ignore_errors=True)

os.makedirs(OUT, exist_ok=True)


def write(name, text):
    open(os.path.join(OUT, name), "w", encoding="utf-8", newline="\n").write(text)


# ---------------------------------------------------------------- unit test report
write("unit_test_report.md", f"""# Unit Test Report

{head}
**What was tested:** the scoring module (`src/score.c`) on its own, using `assert` in `tests/test_score.c`.
**How to run:** `make test`

**Result: {passed} of {passed} unit tests passed.**

| Test | Checks | Requirement |
|---|---|---|
| 1 to 4 | 3, 1, 5 and 0 of 5 correct: score, counts, percentage, Pass/Fail | REQ-7, REQ-8, REQ-9 |
| 5 | `show_result` with a NULL result does not crash | REQ-11, NFR-4 |
| 6, 7 | Pass mark boundary: 4 of 10 (40%) is Pass, 3 of 10 (30%) is Fail | REQ-10 |
| 8 | Invalid arguments (NULL, zero questions) give an all-zero result | NFR-4 |

## Raw output of `make test`

```
{unit_out.strip()}
```
""")

# ---------------------------------------------------------------- system validation report
tbl = "\n".join(f"| {tc} | {TC_REQ.get(tc, '')} | {desc} | {'Pass' if st == 'PASS' else '**FAIL**'} |" for st, tc, desc in rows)
write("system_validation_report.md", f"""# System Validation Report

{head}
**What was tested:** the whole program, by sending input to the compiled `quiz` and checking the output against the SRS.
**How to run:** `make` and then `bash tests/run_system_tests.sh`

**Result: {summary.group(1)} test cases passed, {summary.group(2)} failed.** The test cases are defined in `docs/TEST_PLAN.md` (section 13).

| Test case | Requirement(s) | Check | Result |
|---|---|---|---|
{tbl}

## Raw output

```
{sys_out.strip()}
```
""")


# ---------------------------------------------------------------- coverage reports
def tot(res, key_p, key_n):
    n = sum(v[key_n] for v in res.values())
    c = sum(v[key_p] / 100.0 * v[key_n] for v in res.values() if v[key_p] is not None)
    return (100.0 * c / n) if n else 100.0, n


def lines_table(res):
    out = ["| File | Lines | Lines executed |", "|---|---|---|"]
    for f, v in res.items():
        out.append(f"| `{f}` | {v['ln']} | {v['lp']:.1f}% |")
    if len(res) > 1:
        p, n = tot(res, "lp", "ln")
        out.append(f"| **All source files** | {n} | **{p:.1f}%** |")
    return "\n".join(out)


def branch_table(res):
    out = ["| File | Branches | Branches executed | Taken at least once |", "|---|---|---|---|"]
    for f, v in res.items():
        be = "n/a" if v["bp"] is None else f"{v['bp']:.1f}%"
        bt = "n/a" if v["tp"] is None else f"{v['tp']:.1f}%"
        out.append(f"| `{f}` | {v['bn']} | {be} | {bt} |")
    if len(res) > 1:
        p, n = tot(res, "bp", "bn")
        t, _ = tot({k: dict(v, tp=v["tp"]) for k, v in res.items()}, "tp", "bn")
        out.append(f"| **All source files** | {n} | **{p:.1f}%** | **{t:.1f}%** |")
    return "\n".join(out)


def uncovered(res, kind):
    items = []
    for f, v in res.items():
        if kind == "lines":
            for n, code in v["uncovered"]:
                items.append(f"- `{f}` line {n}: `{code[:80]}`")
        else:
            for n in v["ubranches"]:
                items.append(f"- `{f}` line {n}")
    return "\n".join(items) if items else "None."


write("code_coverage_report.md", f"""# Code Coverage Report (line coverage)

{head}
Measured with `gcc --coverage` and `gcov`. The program was built with coverage on and run in two ways.

## 1. Unit tests (`tests/test_score.c`, scoring module only)

{lines_table(unit_cov)}

## 2. System tests (the 20 scripted test cases run against the whole program)

{lines_table(sys_cov)}

### Lines not executed by the system tests

{uncovered(sys_cov, "lines")}

The system tests drive the real program from the keyboard. The lines listed above are mostly error paths and rare input cases (very long input lines, end of input at a prompt, a missing question file, an empty or damaged results file). Several of them are covered by the unit tests or by the manual checks in the Test Plan.

**How to reproduce:** `python tests/make_reports.py`
""")

write("branch_coverage_report.md", f"""# Branch Coverage Report

{head}
Measured with `gcc --coverage` and `gcov -b`. "Branches executed" is the share of branches that were reached at all. "Taken at least once" is the share that were followed in at least one direction.

## 1. Unit tests (`tests/test_score.c`, scoring module only)

{branch_table(unit_cov)}

## 2. System tests (the 20 scripted test cases run against the whole program)

{branch_table(sys_cov)}

### Branches never taken by the system tests

{uncovered(sys_cov, "branches")}

**How to reproduce:** `python tests/make_reports.py`
""")

print("unit tests passed:", passed)
print("system tests:", summary.group(0))
print("score.c unit coverage: lines", unit_cov["src/score.c"]["lp"], "branches", unit_cov["src/score.c"]["bp"], "taken", unit_cov["src/score.c"]["tp"])
for f, v in sys_cov.items():
    print(f, "lines", v["lp"], "branches", v["bp"], "taken", v["tp"])
print("written to", OUT)
