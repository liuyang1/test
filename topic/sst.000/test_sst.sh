#!/bin/bash
# test_sst.sh - regression tests for sst screensaver
cd "$(dirname "$0")"

PASS=0 FAIL=0
check() {
    local desc="$1"; shift
    if eval "$@" >/dev/null 2>&1; then
        echo "  PASS: $desc"; ((PASS++))
    else
        echo "  FAIL: $desc"; ((FAIL++))
    fi
}
check_fail() {
    local desc="$1"; shift
    if ! eval "$@" >/dev/null 2>&1; then
        echo "  PASS: $desc (expected failure)"; ((PASS++))
    else
        echo "  FAIL: $desc (should have failed)"; ((FAIL++))
    fi
}

echo "=== Build ==="
make clean >/dev/null 2>&1 || true
check "compiles without error" "make sst"

# verify linked against ncursesw (not ncurses)
echo ""
echo "=== Library linkage ==="
check "links ncursesw" "ldd ./sst | grep -q libncursesw"
check_fail "does NOT link narrow ncurses" "ldd ./sst | grep 'libncurses\.so' | grep -vq ncursesw"

echo ""
echo "=== CLI: help and error handling ==="
check "--help exits 0"       "./sst --help"
check "-h exits 0"           "./sst -h"
check "--help lists modes"   "./sst --help 2>&1 | grep -q breakout"
check "--help lists random"  "./sst --help 2>&1 | grep -q random"
check "--help lists --fancy" "./sst --help 2>&1 | grep -q fancy"
check_fail "bad mode exits nonzero" "./sst nosuchmode"
check_fail "bad option exits nonzero" "./sst --badopt"
check "bad mode shows available" "./sst nosuchmode 2>&1 | grep -q Available"

echo ""
echo "=== Runtime: all modes (compat) ==="
for mode in square body3 rulex life breakout random; do
    check "$mode compat runs" "echo q | timeout 3 ./sst --compat $mode"
done

echo ""
echo "=== Runtime: all modes (fancy / Unicode) ==="
for mode in square body3 rulex life breakout random; do
    check "$mode fancy runs" "echo q | timeout 3 ./sst --fancy $mode"
done

echo ""
echo "=== Signal handling ==="
check "SIGTERM clean exit" '
    ./sst life </dev/null &
    PID=$!; sleep 0.5; kill -TERM $PID 2>/dev/null; wait $PID 2>/dev/null'
check "SIGINT clean exit" '
    ./sst life </dev/null &
    PID=$!; sleep 0.5; kill -INT $PID 2>/dev/null; wait $PID 2>/dev/null'

echo ""
echo "=== Source code checks ==="
check "setlocale present"    "grep -q setlocale sst.000.c"
check "locale.h included"    "grep -q locale.h sst.000.c"
check "ncursesw in Makefile"  "grep -q ncursesw Makefile"
check_fail "no narrow ncurses in Makefile" "grep 'lncurses[^w]' Makefile"

echo ""
echo "================================"
echo "Results: $PASS passed, $FAIL failed"
[ "$FAIL" -eq 0 ] && echo "ALL TESTS PASSED" || echo "SOME TESTS FAILED"
exit $FAIL
