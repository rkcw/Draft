#!/usr/bin/env bash
# Mini Scrabble test runner (bash version - for macOS / Linux).
#
# For each folder in testcases/ :
#   state.txt      - board state loaded via the LOAD menu option
#   input.txt      - the exact stdin script fed to the game
#   output.txt     - the exact output of the reference solution for input.txt,
#                    so students can see the intended output (solution not given)
#   expected.txt   - one required output substring per line; a line starting
#                    with '!' is a substring that must NOT appear. Lines
#                    starting with '#' are comments and are ignored.
#
# Run from the Task-2 folder:
#   bash run_tests.sh
#   (or: ./run_tests.sh  after  chmod +x run_tests.sh)
#
# NOTE: the PowerShell runner passes "-Wl,--stack,2097152" to gcc. That is a
# Windows/MinGW-specific flag (it grows the reserved stack size) and is not
# understood by macOS / Linux linkers, so it is omitted here - the default
# stack on those platforms is large enough for this game.

# Strip leading and trailing whitespace.
trim() {
    local s="$1"
    s="${s#"${s%%[![:space:]]*}"}"
    s="${s%"${s##*[![:space:]]}"}"
    printf '%s' "$s"
}

# Colours only when stdout is a terminal (so logs/CI stay clean).
if [ -t 1 ]; then
    GREEN=$'\033[0;32m'
    RED=$'\033[0;31m'
    YELLOW=$'\033[0;33m'
    NC=$'\033[0m'
else
    GREEN=''
    RED=''
    YELLOW=''
    NC=''
fi

# Always rebuild so the tests exercise the current code.
echo "Building main ..."
if ! gcc main.c Task2sol.c given.c lib/dict.c -o main; then
    echo "${RED}Build failed.${NC}" >&2
    exit 1
fi

testsDir="testcases"
passed=0
failed=0

for dir in "$testsDir"/*/; do
    [ -d "$dir" ] || continue
    name=$(basename "$dir")
    inputFile="$dir/input.txt"
    expectedFile="$dir/expected.txt"

    if [ ! -f "$inputFile" ] || [ ! -f "$expectedFile" ]; then
        echo "${YELLOW}[SKIP] $name (missing input.txt or expected.txt)${NC}"
        continue
    fi

    # Feed the scripted input to the game and capture everything it prints.
    actual="$(./main < "$inputFile" 2>&1)"

    ok=1
    problems=()

    while IFS= read -r line || [ -n "$line" ]; do
        line="$(trim "$line")"
        if [ -z "$line" ] || [[ "$line" == \#* ]]; then
            continue
        fi
        if [[ "$line" == !* ]]; then
            sub="$(trim "${line:1}")"
            if [[ "$actual" == *"$sub"* ]]; then
                ok=0
                problems+=("  must NOT contain: '$sub'")
            fi
        else
            if [[ "$actual" != *"$line"* ]]; then
                ok=0
                problems+=("  missing: '$line'")
            fi
        fi
    done < "$expectedFile"

    if [ "$ok" -eq 1 ]; then
        echo "${GREEN}[PASS] $name${NC}"
        passed=$((passed+1))
    else
        echo "${RED}[FAIL] $name${NC}"
        for p in "${problems[@]}"; do
            echo "${RED}$p${NC}" >&2
        done
        failed=$((failed+1))
    fi
done

echo ""
echo "Passed: $passed   Failed: $failed"
if [ "$failed" -gt 0 ]; then
    exit 1
fi
