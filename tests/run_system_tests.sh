#!/bin/bash
# System tests for the Online Quiz System (Test Plan section 13, TC-01 to TC-20).
# Run from the repository root after building:  make && bash tests/run_system_tests.sh
# It sends input to the program, checks the output, and prints PASS or FAIL for each test case.

cd "$(dirname "$0")/.." || exit 1
if [ -x ./quiz.exe ]; then Q=./quiz.exe; elif [ -x ./quiz ]; then Q=./quiz; else echo "Build first: make"; exit 1; fi
if command -v make >/dev/null 2>&1; then MAKE=make; else MAKE=mingw32-make; fi

pass=0; fail=0
check() {   # check "TC-xx" "description" <command that succeeds when the test passes>
    local id="$1" desc="$2"; shift 2
    if "$@"; then echo "PASS  $id  $desc"; pass=$((pass+1)); else echo "FAIL  $id  $desc"; fail=$((fail+1)); fi
}
fresh()   { rm -rf results.txt; }
play()    { printf "$1" | $Q 2>/dev/null; }
count()   { grep -c -F "$2" <<<"$1"; }

BACKUP=""
[ -f results.txt ] && BACKUP=$(mktemp) && cp results.txt "$BACKUP"

# TC-01 menu is shown
fresh; out=$(play '4\n')
t01() { for s in "1. Start Quiz" "2. View Leaderboard" "3. Instructions" "4. Exit" "Enter your choice (1-4):"; do grep -qF "$s" <<<"$out" || return 1; done; }
check TC-01 "menu shows 4 options and the prompt" t01

# TC-02 option 3 shows the instructions
out=$(play '3\n4\n'); t02() { grep -qF -- "--- Instructions ---" <<<"$out"; }
check TC-02 "option 3 shows the instructions" t02

# TC-03 invalid menu input: 5 errors, program keeps running
out=$(play 'abc\n0\n5\n\n4 x\n4\n'); t03() { [ "$(count "$out" 'Error: please enter a number from 1 to 4.')" = 5 ] && grep -qF Goodbye <<<"$out"; }
check TC-03 "5 invalid menu inputs give 5 errors, then exit works" t03

# TC-04 menu returns after a feature
out=$(play '3\n4\n'); t04() { [ "$(count "$out" '===== Online Quiz System =====')" = 2 ]; }
check TC-04 "menu is shown again after the instructions" t04

# TC-05 clean exit, also with empty input
out=$(play '4\n'); rc1=$?; out2=$($Q </dev/null 2>/dev/null); rc2=$?
t05() { grep -qF Goodbye <<<"$out" && [ $rc1 = 0 ] && [ $rc2 = 0 ]; }
check TC-05 "option 4 and empty input both exit cleanly (exit code 0)" t05

# TC-06, TC-07, TC-08, TC-19: unit tests
ut=$($MAKE test 2>&1); t06() { grep -qF "All test cases passed" <<<"$ut"; }
check TC-06 "scoring unit tests: counts and percentage" t06
check TC-07 "scoring unit tests: 40% boundary" t06
check TC-08 "show_result unit tests incl. NULL" t06
check TC-19 "score.c builds and runs on its own (make test)" t06

# TC-09 name with a comma
fresh; play '1\nTara,Bad\nA\nB\nC\nD\nA\n4\n' >/dev/null
t09() { grep -qE '^Tara Bad,[0-9]+$' results.txt && [ "$(wc -l < results.txt)" = 1 ]; }
check TC-09 "comma in a name is replaced; one line per record" t09

# TC-10 integration: full quiz saves Name,Score
fresh; play '1\nAsha\nA\nB\nC\nD\nA\n4\n' >/dev/null
t10() { grep -qE '^Asha,[0-9]+$' results.txt && [ "$(wc -l < results.txt)" = 1 ]; }
check TC-10 "full quiz as Asha saves one Asha,<score> line" t10

# TC-11 whole quiz
fresh; out=$(play '1\nAsha\nA\nB\nC\nD\nA\n4\n')
t11() { for i in 1 2 3 4 5; do grep -qF "Question $i of 5" <<<"$out" || return 1; done; grep -qF "===== Quiz Results =====" <<<"$out"; }
check TC-11 "5 questions one at a time, result at the end" t11

# TC-12 invalid answers
fresh; out=$(play '1\nAsha\nE\n\n1\nB\nA\nA\nA\nA\n4\n')
t12() { [ "$(count "$out" 'Error: please enter A, B, C or D.')" = 3 ] && grep -qF "Question 2 of 5" <<<"$out"; }
check TC-12 "E, empty line and 1 give 3 errors, then B is accepted" t12

# TC-13 leaderboard order
printf 'Vikas,10\nVaishnavi,8\nTejas,9\n' > results.txt; out=$(play '2\n4\n')
t13() { a=$(grep -n Vikas <<<"$out" | tail -1 | cut -d: -f1); b=$(grep -n Tejas <<<"$out" | tail -1 | cut -d: -f1); c=$(grep -n Vaishnavi <<<"$out" | tail -1 | cut -d: -f1); [ -n "$a" ] && [ "$a" -lt "$b" ] && [ "$b" -lt "$c" ]; }
check TC-13 "leaderboard order is 10, 9, 8" t13

# TC-14 no results file
fresh; out=$(play '2\n4\n'); rc=$?
t14() { grep -qF "No results yet" <<<"$out" && [ $rc = 0 ]; }
check TC-14 "missing results.txt: message, no crash" t14

# TC-15 speed (NFR-1)
fresh; s=$(date +%s%N); play '1\nAsha\nA\nB\nC\nD\nA\n2\n4\n' >/dev/null; e=$(date +%s%N); ms=$(( (e - s) / 1000000 ))
python3 -c "
import random
open('results.txt','w').write(''.join('P%d,%d\n' % (i, random.randint(0,5)) for i in range(100)))" 2>/dev/null || python -c "
import random
open('results.txt','w').write(''.join('P%d,%d\n' % (i, random.randint(0,5)) for i in range(100)))"
s=$(date +%s%N); play '2\n4\n' >/dev/null; e=$(date +%s%N); ms2=$(( (e - s) / 1000000 ))
t15() { [ "$ms" -lt 1000 ] && [ "$ms2" -lt 1000 ]; }
check TC-15 "full run ${ms} ms, leaderboard of 100 results ${ms2} ms (limit 1000 ms)" t15

# TC-16 results.txt cannot be written
fresh; mkdir results.txt; out=$(play '1\nAsha\nA\nB\nC\nD\nA\n4\n'); rmdir results.txt
t16() { grep -qF "Error: could not save your result." <<<"$out" && ! grep -qiE 'results\.txt|errno|permission|0x' <<<"$out"; }
check TC-16 "write failure shows only a short Error line, no paths" t16

# TC-17 very long input
fresh; out=$( { head -c 5000 /dev/zero | tr '\0' 'a'; printf '\n4\n'; } | $Q 2>/dev/null ); rc=$?
t17() { [ "$(count "$out" 'Error: please enter a number from 1 to 4.')" = 1 ] && grep -qF Goodbye <<<"$out" && [ $rc = 0 ]; }
check TC-17 "5000-character line gives one error, program continues" t17

# TC-18 source inspection
t18() {
    ! grep -nE '\bgets\(|\bscanf\(|\bsprintf\(' src/*.c >/dev/null || return 1
    [ "$(grep -c 'strcpy' src/*.c | awk -F: '{s+=$2} END {print s}')" -le 1 ] || return 1
    grep -n 'fopen' src/*.c | grep -vE 'RESULTS_FILE|"r"\)' >/dev/null && return 1
    return 0
}
check TC-18 "no gets/scanf/sprintf; file names are fixed" t18

# TC-20 every error message starts with "Error:"
t20() { ! grep -hE 'printf\(".*(could not|please enter|unavailable)' src/*.c | grep -v 'Error:' >/dev/null; }
check TC-20 "every error message starts with Error:" t20

fresh; [ -n "$BACKUP" ] && cp "$BACKUP" results.txt && rm -f "$BACKUP"
echo; echo "Result: $pass passed, $fail failed"
[ "$fail" = 0 ]
