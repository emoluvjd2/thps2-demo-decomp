#!/usr/bin/env bash
# Source-only build entry point for the public decompilation snapshot.
set -euo pipefail

ROOT="$(cd "$(dirname "$0")" && pwd)"

case "${1:-}" in
    ""|--compile)
        exec bash "$ROOT/test_all_tu.sh"
        ;;
    -h|--help)
        echo "Usage: bash build.sh [--compile]"
        echo "Compile-check every recovered translation unit; no executable is linked."
        ;;
    *)
        echo "Unknown option: $1" >&2
        echo "Usage: bash build.sh [--compile]" >&2
        exit 2
        ;;
esac
