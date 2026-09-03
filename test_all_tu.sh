#!/bin/bash
# Test compile all TU files (supports both .c and .cpp)
set -u
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PSYQ_SDK_HEADER_HELPER="$SCRIPT_DIR/tools/psyq_sdk_headers.py"
EGCS_JUMP_PATCH_HELPER="$SCRIPT_DIR/toolchain/patch_egcs_1_1_1_jump.py"
COMPILER_DIR="$SCRIPT_DIR/toolchain/gcc-2.91.66-psx"

if ! python3 "$EGCS_JUMP_PATCH_HELPER" --check "$COMPILER_DIR" >/dev/null; then
    echo "Canonical compiler jump-behavior validation failed; refusing compilation." >&2
    exit 1
fi

if ! PSYQ_INCLUDE_ROOT="$(python3 "$PSYQ_SDK_HEADER_HELPER" \
        --check --print-include-root)"; then
    echo "Psy-Q 4.6 SDK header validation failed; refusing compilation." >&2
    exit 1
fi

# Copy src to WSL native filesystem for faster I/O (avoids /mnt/c/ 9P overhead)
# Toolchain stays on /mnt/c/ (large, read-only, cached after first access)
FAST_DIR="/tmp/thps2_build_$$"
mkdir -p "$FAST_DIR/output/tmp"
trap 'rm -rf "$FAST_DIR"' EXIT

cp -r "$SCRIPT_DIR/src" "$FAST_DIR/src"

TU_DIR="$FAST_DIR/src"
TMP_DIR="$FAST_DIR/output/tmp"
CC1="$COMPILER_DIR/cc1"
CC1PLUS="$COMPILER_DIR/cc1plus"
SRC_DIR="$FAST_DIR/src"

pass=0
fail=0

# Process .cpp files first, then .c files (skip .c if .cpp exists)
for f in "$TU_DIR"/*.cpp "$TU_DIR"/*.c; do
    [ -f "$f" ] || continue
    IS_CPP=false
    if [[ "$f" == *.cpp ]]; then
        IS_CPP=true
        name=$(basename "$f" .cpp)
    else
        name=$(basename "$f" .c)
        # Skip .c if .cpp exists
        if [ -f "$TU_DIR/$name.cpp" ]; then
            continue
        fi
    fi

    # Preprocess
    CPP_FLAGS=(-E -nostdinc "-I$SRC_DIR" "-I$PSYQ_INCLUDE_ROOT")
    if ! $IS_CPP; then
        CPP_FLAGS+=(-lang-c)
    fi
    /usr/bin/cpp "${CPP_FLAGS[@]}" "$f" -o "$TMP_DIR/${name}_test.i" 2>/dev/null
    if [ $? -ne 0 ]; then
        echo "FAIL $name (preprocess error)"
        fail=$((fail + 1))
        continue
    fi

    # Compile: use cc1plus for .cpp, cc1 for .c
    # A successful compiler run must return zero and produce real assembly.
    if $IS_CPP; then
        COMPILER="$CC1PLUS"
    else
        COMPILER="$CC1"
    fi
    ASM_OUTPUT="$TMP_DIR/${name}_test.s"
    result=$("$COMPILER" -quiet -O2 -fno-strength-reduce -G8 -msoft-float -mno-abicalls \
        "$TMP_DIR/${name}_test.i" -o "$ASM_OUTPUT" 2>&1)
    rc=$?
    errs=$(echo "$result" | grep -cE "^[^:]+:[0-9]+: [^w]" || true)
    warns=$(echo "$result" | grep -c "warning:" || true)

    if $IS_CPP; then
        if [ "$rc" -eq 0 ] && [ "$errs" -eq 0 ] && [ -s "$ASM_OUTPUT" ]; then
            echo "OK   $name ($warns warnings) [C++]"
            pass=$((pass + 1))
        else
            echo "FAIL $name ($errs errors, $warns warnings) [C++]"
            echo "$result" | grep -E "^[^:]+:[0-9]+: [^w]" | head -5
            fail=$((fail + 1))
        fi
    else
        if [ "$rc" -eq 0 ] && [ -s "$ASM_OUTPUT" ]; then
            echo "OK   $name ($warns warnings)"
            pass=$((pass + 1))
        else
            echo "FAIL $name ($errs errors, $warns warnings)"
            echo "$result" | grep ": " | grep -v "warning:" | head -5
            fail=$((fail + 1))
        fi
    fi
done

echo ""
echo "Results: $pass passed, $fail failed out of $((pass + fail)) TU files"

if ! python3 "$EGCS_JUMP_PATCH_HELPER" --check "$COMPILER_DIR" >/dev/null; then
    echo "Canonical compiler jump behavior changed during compilation; refusing success." >&2
    exit 1
fi
if ! python3 "$PSYQ_SDK_HEADER_HELPER" --check >/dev/null; then
    echo "Psy-Q 4.6 SDK headers changed during compilation; refusing success." >&2
    exit 1
fi

if [ "$fail" -ne 0 ]; then
    exit 1
fi
