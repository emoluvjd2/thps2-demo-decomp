#!/usr/bin/env bash
# Install the compiler used by the source-only translation-unit check.
set -euo pipefail

ROOT="$(cd "$(dirname "$0")" && pwd)"
TOOLCHAIN_DIR="$ROOT/toolchain"
COMPILER_DIR="$TOOLCHAIN_DIR/gcc-2.91.66-psx"
SDK_DIR="$TOOLCHAIN_DIR/psyq-4.6-sdk"
SDK_HELPER="$ROOT/tools/psyq_sdk_headers.py"

missing=()
for command_name in cpp python3 tar wget; do
    if ! command -v "$command_name" >/dev/null 2>&1; then
        missing+=("$command_name")
    fi
done

if (( ${#missing[@]} != 0 )); then
    echo "ERROR: missing required host commands: ${missing[*]}" >&2
    exit 1
fi

if [[ ! -d "$SDK_DIR/INCLUDE" ]]; then
    echo "ERROR: install your licensed Psy-Q 4.6 (DTL-S2360) SDK headers at:" >&2
    echo "       $SDK_DIR/INCLUDE" >&2
    echo "       This repository does not download or distribute the SDK." >&2
    exit 1
fi

python3 "$SDK_HELPER" --prepare

mkdir -p "$COMPILER_DIR"
if [[ ! -x "$COMPILER_DIR/cc1" || ! -x "$COMPILER_DIR/cc1plus" ]]; then
    tarball="$TOOLCHAIN_DIR/gcc-2.91.66-psx.tar.gz"
    echo "Downloading GCC 2.91.66-psx..."
    wget -q --show-progress -O "$tarball" \
        "https://github.com/decompals/old-gcc/releases/download/0.13/gcc-2.91.66-psx.tar.gz"
    tar xzf "$tarball" -C "$COMPILER_DIR"
    chmod +x "$COMPILER_DIR"/*
    rm -- "$tarball"
fi

"$COMPILER_DIR/cc1" -version </dev/null 2>&1 | head -n 1
"$COMPILER_DIR/cc1plus" -version </dev/null 2>&1 | head -n 1
python3 "$SDK_HELPER" --check

echo "Toolchain ready. Run: bash build.sh"
