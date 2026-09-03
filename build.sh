#!/bin/bash
# build.sh — Build full PS-EXE from decompiled THPS2 PSX prototype sources
#
# Usage:
#   bash build.sh              # Full build
#   bash build.sh --compile    # Compile TUs only (no link)
#   bash build.sh --compile --jobs 8 # Compile with eight TU workers
#   bash build.sh --link       # Link only (assumes TUs already compiled)
#   bash build.sh --native-link # Compile, then run relocation-complete PSYLINK
#   bash build.sh --diff       # Build + compare against original
#   bash build.sh --clean      # Remove build artifacts
#
# Produces: output/rebuilt.exe (PS-EXE format, diffable against SLUS_900.86)

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
SRC_DIR="$SCRIPT_DIR/src"
OBJ_DIR="$SCRIPT_DIR/output/obj"
PSYQ_OBJ_DIR="$SCRIPT_DIR/output/psyq_obj"
TMP_DIR="$SCRIPT_DIR/output/tmp"
ASM_DIR="$SCRIPT_DIR/output/asm"
OUTPUT_DIR="$SCRIPT_DIR/output"
SYMBOLS_LD="$SCRIPT_DIR/output/symbols.ld"
LINKER_SCRIPT="$SCRIPT_DIR/psxexe.ld"
COMPILER_DIR="$SCRIPT_DIR/toolchain/gcc-2.91.66-psx"
CC1_C="$COMPILER_DIR/cc1"
CC1="$COMPILER_DIR/cc1plus"
EGCS_JUMP_PATCH_HELPER="$SCRIPT_DIR/toolchain/patch_egcs_1_1_1_jump.py"
ASPSX_EXE="$SCRIPT_DIR/toolchain/psyq4.6/ASPSX.EXE"
PSYLINK_EXE="$SCRIPT_DIR/toolchain/psyq4.6/PSYLINK.EXE"
FILL_DS="$SCRIPT_DIR/tools/fill_delay_slots.py"
ASPSX_ASSEMBLE="$SCRIPT_DIR/tools/aspsx_assemble.py"
PREPARE_FOR_ASPSX="$SCRIPT_DIR/tools/prepare_for_aspsx.py"
DMPSX_TOKENS="$SCRIPT_DIR/tools/dmpsx_tokens.py"
COMBINE_ASM_SOURCES="$SCRIPT_DIR/tools/combine_asm_sources.py"
GLABEL2GAS="$SCRIPT_DIR/tools/glabel2gas.py"
VERIFY_HANDWRITTEN_OWNERS="$SCRIPT_DIR/tools/verify_handwritten_owners.py"
VERIFY_ADDRESS_OWNER="$SCRIPT_DIR/tools/verify_address_owner.py"
VERIFY_DATA_SECTIONS="$SCRIPT_DIR/tools/verify_data_sections.py"
GENERATE_SQRTTAB="$SCRIPT_DIR/tools/generate_sqrttab.py"
BUILD_CACHE_HELPER="$SCRIPT_DIR/tools/build_object_cache.sh"
TU_DEPENDENCY_HELPER="$SCRIPT_DIR/tools/list_tu_dependencies.py"
PSYQ_SDK_HEADER_HELPER="$SCRIPT_DIR/tools/psyq_sdk_headers.py"
PSYQ_SDK_HEADER_MANIFEST="$SCRIPT_DIR/tools/psyq_4_6_headers.sha256"
PSYQ_RUNNER_HELPER="$SCRIPT_DIR/tools/psyq_runner.py"
source "$BUILD_CACHE_HELPER"

assembly_pipeline_identity() {
    sha256sum -- \
        "$PREPARE_FOR_ASPSX" \
        "$ASPSX_ASSEMBLE" \
        "$DMPSX_TOKENS" \
        | sha256sum \
        | awk '{print $1}'
}

# Build options
TU_FLAGS_FILE="$SCRIPT_DIR/tools/tu_compiler_flags.sh"
source "$TU_FLAGS_FILE"
DO_COMPILE=true
DO_LINK=true
DO_NATIVE_LINK=false
DO_NATIVE_DIFF=false
DO_DIFF=false
BUILD_JOBS="${THPS2_BUILD_JOBS:-4}"

while [[ $# -gt 0 ]]; do
    case "$1" in
        --compile) DO_LINK=false; shift ;;
        --link) DO_COMPILE=false; shift ;;
        --native-link) DO_LINK=false; DO_NATIVE_LINK=true; shift ;;
        --native-diff) DO_LINK=false; DO_NATIVE_LINK=true; DO_NATIVE_DIFF=true; shift ;;
        --diff) DO_DIFF=true; shift ;;
        --jobs)
            if (( $# < 2 )); then
                echo "Error: --jobs requires a positive integer." >&2
                exit 2
            fi
            BUILD_JOBS="$2"
            shift 2
            ;;
        --jobs=*) BUILD_JOBS="${1#--jobs=}"; shift ;;
        --clean)
            echo "Cleaning build artifacts..."
            rm -rf "$OBJ_DIR"
            rm -rf "$PSYQ_OBJ_DIR"
            rm -rf "$TMP_DIR"
            rm -rf "$OUTPUT_DIR/native"
            rm -f "$OUTPUT_DIR/rebuilt.bin" "$OUTPUT_DIR/rebuilt.exe"
            rm -f "$OUTPUT_DIR/data_blob.bin" "$OUTPUT_DIR/data_blob.s"
            rm -f "$OUTPUT_DIR/psyq_blob.bin" "$OUTPUT_DIR/psyq_blob.s"
            exit 0
            ;;
        *) echo "Unknown option: $1"; exit 1 ;;
    esac
done

if [[ ! "$BUILD_JOBS" =~ ^[1-9][0-9]*$ ]]; then
    echo "Error: --jobs requires a positive integer (got '$BUILD_JOBS')." >&2
    exit 2
fi

if $DO_NATIVE_LINK && $DO_DIFF; then
    echo "Error: --native-link cannot be combined with --diff; --diff only compares the legacy rebuilt executable." >&2
    exit 2
fi

PYTHON_BIN="$(command -v python3)"
PSYQ_INCLUDE_ROOT=""
ASSEMBLY_PIPELINE_START_IDENTITY=""
if $DO_COMPILE; then
    if ! "$PYTHON_BIN" "$EGCS_JUMP_PATCH_HELPER" \
            --check "$COMPILER_DIR" >/dev/null; then
        echo "Canonical compiler jump-behavior validation failed; refusing compilation." >&2
        exit 1
    fi
    if ! PSYQ_INCLUDE_ROOT="$("$PYTHON_BIN" "$PSYQ_SDK_HEADER_HELPER" \
            --check --print-include-root)"; then
        echo "Psy-Q 4.6 SDK header validation failed; refusing compilation." >&2
        exit 1
    fi
    if ! ASSEMBLY_PIPELINE_START_IDENTITY="$(assembly_pipeline_identity)"; then
        echo "Assembly-pipeline validation failed; refusing compilation." >&2
        exit 1
    fi
fi

mkdir -p "$OBJ_DIR" "$PSYQ_OBJ_DIR" "$TMP_DIR"

# ===========================================================================
# Phase 1: Compile all TUs to .o files
# ===========================================================================
if $DO_COMPILE; then
    echo "=== Phase 1: Compiling TUs ==="

    # A cached object is valid only when every source-level and toolchain input
    # which can affect it is older. Each TU gets its actual transitive project
    # headers below; unrelated headers no longer invalidate the entire build.
    CPP_BIN="/usr/bin/cpp"
    AWK_BIN="$(command -v awk)"

    # The checked-in SQRTTAB initializer is a reproducible source artifact.
    # Refuse to compile a hand-edited or stale table.
    "$PYTHON_BIN" "$GENERATE_SQRTTAB" --check

    # Cache the complete runner selection identity as well as the resolved host
    # executable. This invalidates objects when an override switches between
    # wibo and Wine, without making Wine a mandatory dependency for wibo users.
    RUNNER_QUERY_FILE="$TMP_DIR/psyq_runner.query.$$"
    if ! "$PYTHON_BIN" - "$SCRIPT_DIR/tools" > "$RUNNER_QUERY_FILE" <<'PY'
import json
import os
import shutil
import sys
from pathlib import Path

sys.path.insert(0, sys.argv[1])
from psyq_runner import select_psyq_runner

runner = select_psyq_runner()
resolved = []
for index, token in enumerate(runner.command):
    candidate = Path(token).expanduser()
    path = None
    if candidate.is_file():
        path = candidate.resolve()
    elif index == 0 or (not token.startswith("-") and "=" not in token):
        found = shutil.which(token)
        if found:
            path = Path(found).resolve()
    if path is not None and path not in resolved:
        resolved.append(path)

if not resolved:
    raise SystemExit(
        f"selected Psy-Q runner executable was not found: {runner.command[0]}"
    )

identity = json.dumps(
    {
        "command": runner.command,
        "resolved": [os.fspath(path) for path in resolved],
        "source": runner.source,
    },
    sort_keys=True,
    separators=(",", ":"),
)
items = [identity, *(os.fspath(path) for path in resolved)]
sys.stdout.buffer.write(b"\0".join(item.encode() for item in items) + b"\0")
PY
    then
        rm -f "$RUNNER_QUERY_FILE"
        echo "Unable to resolve the selected Psy-Q runner." >&2
        exit 1
    fi
    mapfile -d '' -t PSYQ_RUNNER_DETAILS < "$RUNNER_QUERY_FILE"
    rm -f "$RUNNER_QUERY_FILE"
    PSYQ_RUNNER_IDENTITY="${PSYQ_RUNNER_DETAILS[0]}"
    PSYQ_RUNNER_DEPENDENCIES=("${PSYQ_RUNNER_DETAILS[@]:1}")
    PSYQ_RUNNER_STAMP="$TMP_DIR/psyq_runner.selection"
    update_dependency_stamp "$PSYQ_RUNNER_STAMP" "$PSYQ_RUNNER_IDENTITY"

    TU_PIPELINE_DEPENDENCIES=(
        "$SCRIPT_DIR/build.sh"
        "$BUILD_CACHE_HELPER"
        "$TU_DEPENDENCY_HELPER"
        "$PSYQ_SDK_HEADER_HELPER"
        "$PSYQ_SDK_HEADER_MANIFEST"
        "$PSYQ_INCLUDE_ROOT/.psyq-sdk-header-view"
        "$PSYQ_RUNNER_HELPER"
        "$TU_FLAGS_FILE"
        "$FILL_DS"
        "$ASPSX_ASSEMBLE"
        "$PREPARE_FOR_ASPSX"
        "$DMPSX_TOKENS"
        "$COMBINE_ASM_SOURCES"
        "$GLABEL2GAS"
        "$GENERATE_SQRTTAB"
        "$EGCS_JUMP_PATCH_HELPER"
        "$CC1_C"
        "$CC1"
        "$ASPSX_EXE"
        "$PSYLINK_EXE"
        "$CPP_BIN"
        "$AWK_BIN"
        "$PYTHON_BIN"
        "$PSYQ_RUNNER_STAMP"
        "${PSYQ_RUNNER_DEPENDENCIES[@]}"
        "$SYMBOLS_LD"
        "$SCRIPT_DIR/output/symdefs.obj"
    )
    for DEPENDENCY in "${TU_PIPELINE_DEPENDENCIES[@]}"; do
        if [ ! -f "$DEPENDENCY" ]; then
            echo "Missing object-build dependency: $DEPENDENCY" >&2
            exit 1
        fi
    done

    mapfile -d '' -t TUS < <(
        find "$SRC_DIR" -maxdepth 1 -type f -name '*.cpp' -print0 \
            | LC_ALL=C sort -z
    )
    TOTAL=${#TUS[@]}
    echo "  Workers: $BUILD_JOBS"
    TU_DEPENDENCY_DIR="$TMP_DIR/tu_dependencies"
    mkdir -p "$TU_DEPENDENCY_DIR"

    compile_translation_unit() {
        local TU="$1"
        local BASENAME
        BASENAME="$(basename "$TU" .cpp)"
        local OBJ_FILE="$OBJ_DIR/$BASENAME.o"
        local OBJ_SIDECAR_FILE="${OBJ_FILE}.psyq.json"
        local PSYQ_OBJ_FILE="$PSYQ_OBJ_DIR/$BASENAME.obj"
        local -a CANONICAL_OUTPUTS=(
            "$OBJ_FILE"
            "$OBJ_SIDECAR_FILE"
            "$PSYQ_OBJ_FILE"
        )

        # MIPS.obj and vlc_bit.obj are proven hand-written application modules.
        # Their clean C++ recoveries remain compile-checked semantic references,
        # while the canonical link objects come from the independently
        # reconstructed target assembly below.
        local HANDWRITTEN_OWNER=false
        local HANDWRITTEN_CODE_START=""
        local HANDWRITTEN_CODE_END=""
        local -a HANDWRITTEN_SOURCES=()
        case "$BASENAME" in
            MIPS)
                HANDWRITTEN_OWNER=true
                HANDWRITTEN_CODE_START=0x8006B0B8
                HANDWRITTEN_CODE_END=0x8006B1E4
                HANDWRITTEN_SOURCES=(
                    "$ASM_DIR/Mips_DotProd2Sign.s"
                    "$ASM_DIR/Mips_FixMul__Fii.s"
                    "$ASM_DIR/Mips_DotProd2.s"
                    "$ASM_DIR/Mips_CVecDotProd__FP7CVectorT0.s"
                )
                ;;
            VLC_BIT)
                HANDWRITTEN_OWNER=true
                HANDWRITTEN_CODE_START=0x8007E87C
                HANDWRITTEN_CODE_END=0x8007EDD0
                HANDWRITTEN_SOURCES=(
                    "$ASM_DIR/DecDCTvlcBuild3.s"
                    "$ASM_DIR/DecDCTvlc3.s"
                )
                ;;
        esac

        local DEPENDENCY_LIST="$TU_DEPENDENCY_DIR/$BASENAME.list"
        local DEPENDENCY_TEMP="$TU_DEPENDENCY_DIR/$BASENAME.list.tmp"
        local DEPENDENCY_LOG="$TU_DEPENDENCY_DIR/$BASENAME.log"
        local -a TU_PROJECT_DEPENDENCIES=()
        local DEPENDENCY_LIST_CURRENT=false
        if [ -f "$DEPENDENCY_LIST" ]; then
            mapfile -d '' -t TU_PROJECT_DEPENDENCIES < "$DEPENDENCY_LIST"
            local SOURCE_IS_LISTED=false
            local PROJECT_DEPENDENCY
            for PROJECT_DEPENDENCY in "${TU_PROJECT_DEPENDENCIES[@]}"; do
                if [ "$PROJECT_DEPENDENCY" = "$TU" ]; then
                    SOURCE_IS_LISTED=true
                    break
                fi
            done
            if $SOURCE_IS_LISTED \
                && object_is_newer_than_all "$DEPENDENCY_LIST" \
                    "$SCRIPT_DIR/build.sh" "$TU_DEPENDENCY_HELPER" \
                    "$PSYQ_SDK_HEADER_HELPER" "$PSYQ_SDK_HEADER_MANIFEST" \
                    "$PSYQ_INCLUDE_ROOT/.psyq-sdk-header-view" \
                    "$CPP_BIN" "${TU_PROJECT_DEPENDENCIES[@]}"; then
                DEPENDENCY_LIST_CURRENT=true
            fi
        fi

        if ! $DEPENDENCY_LIST_CURRENT; then
            rm -f "$DEPENDENCY_TEMP"
            if ! "$PYTHON_BIN" "$TU_DEPENDENCY_HELPER" \
                    --src-dir "$SRC_DIR" --cpp "$CPP_BIN" --null "$TU" \
                    > "$DEPENDENCY_TEMP" 2> "$DEPENDENCY_LOG"; then
                echo "  FAIL: $BASENAME (dependency scan returned nonzero; see $DEPENDENCY_LOG)"
                rm -f "$DEPENDENCY_TEMP"
                rm -f "${CANONICAL_OUTPUTS[@]}"
                return 1
            fi
            mv -f "$DEPENDENCY_TEMP" "$DEPENDENCY_LIST"
            TU_PROJECT_DEPENDENCIES=()
            mapfile -d '' -t TU_PROJECT_DEPENDENCIES < "$DEPENDENCY_LIST"
        fi
        rm -f "$DEPENDENCY_LOG"
        local -a TU_DEPENDENCIES=(
            "${TU_PIPELINE_DEPENDENCIES[@]}"
            "${TU_PROJECT_DEPENDENCIES[@]}"
            "${HANDWRITTEN_SOURCES[@]}"
        )

        # Both formats come from the same ASPSX invocation.  The ELF object is
        # retained for the legacy GNU diagnostic link; the native LNK v2 object
        # is the source of truth for the relocation-complete PSYLINK path.
        if object_set_is_newer_than_all \
                "${CANONICAL_OUTPUTS[@]}" -- "${TU_DEPENDENCIES[@]}"; then
            return 0
        fi

        # Never accept preprocessing, assembly, or objects left by an earlier
        # failed attempt for a TU which the dependency audit marked stale.
        rm -f "$TMP_DIR/$BASENAME.i" "$TMP_DIR/$BASENAME.s"
        rm -f "${CANONICAL_OUTPUTS[@]}"

        # Step 1: Preprocess
        local -a CPP_FLAGS=(-E -nostdinc "-I$SRC_DIR" "-I$PSYQ_INCLUDE_ROOT")
        local CPP_LOG="$TMP_DIR/$BASENAME.cpp.log"
        if ! "$CPP_BIN" "${CPP_FLAGS[@]}" "$TU" \
                -o "$TMP_DIR/$BASENAME.i" 2>"$CPP_LOG"; then
            echo "  FAIL: $BASENAME (preprocessor returned nonzero; see $CPP_LOG)"
            rm -f "$TMP_DIR/$BASENAME.i" "$TMP_DIR/$BASENAME.s"
            rm -f "${CANONICAL_OUTPUTS[@]}"
            return 1
        fi
        rm -f "$CPP_LOG"

        # Step 2: Compile
        local -a TU_FLAGS=()
        read -r -a TU_FLAGS <<< "$(thps2_tu_compiler_flags "$BASENAME")"
        local TU_GP_SIZE
        TU_GP_SIZE="$(thps2_tu_gp_size "$BASENAME")"
        local CC1_LOG="$TMP_DIR/$BASENAME.cc1.log"
        if ! "$CC1" -quiet "${TU_FLAGS[@]}" -G"$TU_GP_SIZE" \
                -msoft-float -mno-abicalls \
                "$TMP_DIR/$BASENAME.i" -o "$TMP_DIR/$BASENAME.s" 2>"$CC1_LOG"; then
            echo "  FAIL: $BASENAME (compiler returned nonzero; see $CC1_LOG)"
            rm -f "$TMP_DIR/$BASENAME.s"
            rm -f "${CANONICAL_OUTPUTS[@]}"
            return 1
        fi

        if [ ! -s "$TMP_DIR/$BASENAME.s" ]; then
            echo "  FAIL: $BASENAME (no output)"
            rm -f "${CANONICAL_OUTPUTS[@]}"
            return 1
        fi
        rm -f "$CC1_LOG"

        # Step 2.5: li -> addiu fixup
        if ! "$AWK_BIN" '{
            if (match($0, /^\tli\t(\$[0-9]+),([0-9]+)/, m)) {
                val = int(m[2])
                if (val > 0 && val < 32768) {
                    printf "\taddiu\t%s,$zero,%d\n", m[1], val
                } else {
                    print
                }
            } else {
                print
            }
        }' "$TMP_DIR/$BASENAME.s" > "$TMP_DIR/${BASENAME}_fixed.s" \
            || ! mv "$TMP_DIR/${BASENAME}_fixed.s" "$TMP_DIR/$BASENAME.s"; then
            echo "  FAIL: $BASENAME (small-li normalization failed)"
            rm -f "${CANONICAL_OUTPUTS[@]}"
            return 1
        fi

        # Step 2.7: Fill delay slots (sw before pseudo-lw + jal)
        if ! "$PYTHON_BIN" "$FILL_DS" \
                "$TMP_DIR/$BASENAME.s" "$TMP_DIR/${BASENAME}_filled.s" \
                2>/dev/null \
            || ! mv "$TMP_DIR/${BASENAME}_filled.s" "$TMP_DIR/$BASENAME.s"; then
            echo "  FAIL: $BASENAME (delay-slot normalization failed)"
            rm -f "${CANONICAL_OUTPUTS[@]}"
            return 1
        fi

        # Step 3: Assemble via ASPSX.EXE (real PSY-Q assembler). For the two
        # verified hand-written owners, first assemble the C++ reference into
        # scratch outputs so declaration or compiler errors still fail closed,
        # then emit the sole canonical object from the recovered assembly.
        if $HANDWRITTEN_OWNER; then
            local REFERENCE_ASM="$TMP_DIR/${BASENAME}_reference.s"
            local REFERENCE_OBJ="$TMP_DIR/${BASENAME}_reference.o"
            local REFERENCE_SIDECAR="${REFERENCE_OBJ}.psyq.json"
            local REFERENCE_PSYQ_OBJ="$TMP_DIR/${BASENAME}_reference.obj"
            local -a REFERENCE_OUTPUTS=(
                "$REFERENCE_OBJ"
                "$REFERENCE_SIDECAR"
                "$REFERENCE_PSYQ_OBJ"
            )
            mv "$TMP_DIR/$BASENAME.s" "$REFERENCE_ASM"
            rm -f "${REFERENCE_OUTPUTS[@]}"
            if ! "$PYTHON_BIN" "$ASPSX_ASSEMBLE" \
                    "$REFERENCE_ASM" "$REFERENCE_OBJ" --gp "$TU_GP_SIZE" \
                    --psyq-obj "$REFERENCE_PSYQ_OBJ" 2>/dev/null; then
                echo "  FAIL: $BASENAME (semantic C++ reference assembly failed)"
                rm -f "${REFERENCE_OUTPUTS[@]}"
                rm -f "${CANONICAL_OUTPUTS[@]}"
                return 1
            fi
            if [ ! -f "$REFERENCE_OBJ" ] \
                || [ ! -f "$REFERENCE_SIDECAR" ] \
                || [ ! -f "$REFERENCE_PSYQ_OBJ" ]; then
                echo "  FAIL: $BASENAME (semantic C++ reference outputs incomplete)"
                rm -f "${REFERENCE_OUTPUTS[@]}"
                rm -f "${CANONICAL_OUTPUTS[@]}"
                return 1
            fi

            if ! "$PYTHON_BIN" "$COMBINE_ASM_SOURCES" \
                    --expected-start "$HANDWRITTEN_CODE_START" \
                    --expected-end "$HANDWRITTEN_CODE_END" \
                    "$TMP_DIR/${BASENAME}_extracted.s" \
                    "${HANDWRITTEN_SOURCES[@]}" \
                || ! "$PYTHON_BIN" "$GLABEL2GAS" --aspsx-cop2 \
                    "$TMP_DIR/${BASENAME}_extracted.s" \
                    "$TMP_DIR/$BASENAME.s"; then
                echo "  FAIL: $BASENAME (hand-written owner source preparation failed)"
                rm -f "${CANONICAL_OUTPUTS[@]}"
                return 1
            fi
        fi

        if ! "$PYTHON_BIN" "$ASPSX_ASSEMBLE" \
                "$TMP_DIR/$BASENAME.s" "$OBJ_FILE" --gp "$TU_GP_SIZE" \
                --psyq-obj "$PSYQ_OBJ_FILE" 2>/dev/null; then
            echo "  FAIL: $BASENAME (assembly returned nonzero)"
            rm -f "${CANONICAL_OUTPUTS[@]}"
            return 1
        fi

        if [ -f "$OBJ_FILE" ] \
            && [ -f "$OBJ_SIDECAR_FILE" ] \
            && [ -f "$PSYQ_OBJ_FILE" ]; then
            return 0
        else
            echo "  FAIL: $BASENAME (assembly outputs incomplete)"
            rm -f "${CANONICAL_OUTPUTS[@]}"
            return 1
        fi
    }

    BUILD_STATUS_DIR="$(mktemp -d "$TMP_DIR/build-status.XXXXXX")"
    JOB_PIDS=()
    for TU in "${TUS[@]}"; do
        BASENAME="$(basename "$TU" .cpp)"
        (
            STATUS=0
            compile_translation_unit "$TU" \
                > "$BUILD_STATUS_DIR/$BASENAME.log" 2>&1 || STATUS=$?
            printf '%s\n' "$STATUS" > "$BUILD_STATUS_DIR/$BASENAME.status"
        ) &
        JOB_PIDS+=("$!")

        if (( ${#JOB_PIDS[@]} >= BUILD_JOBS )); then
            wait "${JOB_PIDS[0]}" || true
            JOB_PIDS=("${JOB_PIDS[@]:1}")
        fi
    done
    for JOB_PID in "${JOB_PIDS[@]}"; do
        wait "$JOB_PID" || true
    done

    COMPILED=0
    FAILED=0
    for TU in "${TUS[@]}"; do
        BASENAME="$(basename "$TU" .cpp)"
        STATUS_FILE="$BUILD_STATUS_DIR/$BASENAME.status"
        STATUS=1
        if [ -f "$STATUS_FILE" ]; then
            IFS= read -r STATUS < "$STATUS_FILE" || STATUS=1
        fi
        if [ "$STATUS" = 0 ]; then
            COMPILED=$((COMPILED + 1))
        else
            FAILED=$((FAILED + 1))
            if [ -s "$BUILD_STATUS_DIR/$BASENAME.log" ]; then
                cat "$BUILD_STATUS_DIR/$BASENAME.log"
            else
                echo "  FAIL: $BASENAME (worker produced no status log)"
            fi
        fi
    done
    rm -rf "$BUILD_STATUS_DIR"

    if ! "$PYTHON_BIN" "$EGCS_JUMP_PATCH_HELPER" \
            --check "$COMPILER_DIR" >/dev/null; then
        echo "Canonical compiler jump behavior changed during compilation; refusing success." >&2
        exit 1
    fi
    if ! "$PYTHON_BIN" "$PSYQ_SDK_HEADER_HELPER" --check >/dev/null; then
        echo "Psy-Q 4.6 SDK headers changed during compilation; refusing success." >&2
        exit 1
    fi
    if ! ASSEMBLY_PIPELINE_CHECK_IDENTITY="$(assembly_pipeline_identity)"; then
        echo "Assembly-pipeline validation failed during compilation." >&2
        exit 1
    fi
    if [ "$ASSEMBLY_PIPELINE_CHECK_IDENTITY" != "$ASSEMBLY_PIPELINE_START_IDENTITY" ]; then
        echo "Assembly pipeline changed during compilation; refusing stale objects." >&2
        exit 1
    fi

    echo "  Compiled: $COMPILED / $TOTAL TUs ($FAILED failures)"
    if (( FAILED > 0 )); then
        echo "Compilation failed; refusing to assemble or link stale objects."
        exit 1
    fi

    # Phase 1b: Assemble the original M3d scratch window and hand-written code
    echo ""
    echo "=== Phase 1b: Assembling M3dAsm hand-written functions ==="
    M3D_ASPSX_DEPENDENCIES=(
        "$SCRIPT_DIR/build.sh"
        "$BUILD_CACHE_HELPER"
        "$PSYQ_RUNNER_HELPER"
        "$ASPSX_ASSEMBLE"
        "$PREPARE_FOR_ASPSX"
        "$DMPSX_TOKENS"
        "$ASPSX_EXE"
        "$PSYLINK_EXE"
        "$PYTHON_BIN"
        "$PSYQ_RUNNER_STAMP"
        "${PSYQ_RUNNER_DEPENDENCIES[@]}"
        "$SYMBOLS_LD"
        "$SCRIPT_DIR/output/symdefs.obj"
    )
    M3D_CONVERTED_DEPENDENCIES=(
        "${M3D_ASPSX_DEPENDENCIES[@]}"
        "$GLABEL2GAS"
    )
    M3D_COMBINED_DEPENDENCIES=(
        "${M3D_CONVERTED_DEPENDENCIES[@]}"
        "$COMBINE_ASM_SOURCES"
    )
    ZSCRATCH_OBJ="$OBJ_DIR/M3d_ZScratch.o"
    ZSCRATCH_SIDECAR="${ZSCRATCH_OBJ}.psyq.json"
    ZSCRATCH_PSYQ_OBJ="$PSYQ_OBJ_DIR/M3d_ZScratch.obj"
    ZSCRATCH_OUTPUTS=(
        "$ZSCRATCH_OBJ"
        "$ZSCRATCH_SIDECAR"
        "$ZSCRATCH_PSYQ_OBJ"
    )
    ZSCRATCH_DEPENDENCIES=(
        "$ASM_DIR/M3d_ZScratch.s"
        "${M3D_ASPSX_DEPENDENCIES[@]}"
    )
    M3D_CACHED_COUNT=0
    M3D_REBUILT_COUNT=0
    if object_set_is_newer_than_all \
            "${ZSCRATCH_OUTPUTS[@]}" -- "${ZSCRATCH_DEPENDENCIES[@]}"; then
        M3D_CACHED_COUNT=$((M3D_CACHED_COUNT + 1))
    else
        rm -f "${ZSCRATCH_OUTPUTS[@]}"
        if ! "$PYTHON_BIN" "$ASPSX_ASSEMBLE" \
                "$ASM_DIR/M3d_ZScratch.s" "$ZSCRATCH_OBJ" --gp 8 \
                --psyq-obj "$ZSCRATCH_PSYQ_OBJ" 2>/dev/null; then
            echo "  FAIL: M3d ZScratch window (assembly returned nonzero)"
            rm -f "${ZSCRATCH_OUTPUTS[@]}"
            exit 1
        fi
        if [ ! -f "$ZSCRATCH_OBJ" ] \
            || [ ! -f "$ZSCRATCH_SIDECAR" ] \
            || [ ! -f "$ZSCRATCH_PSYQ_OBJ" ]; then
            echo "  FAIL: M3d ZScratch window (assembly outputs incomplete)"
            rm -f "${ZSCRATCH_OUTPUTS[@]}"
            exit 1
        fi
        M3D_REBUILT_COUNT=$((M3D_REBUILT_COUNT + 1))
    fi

    M3DASM_COUNT=0
    M3DASM_SOURCES=()
    for ASM_FILE in "$ASM_DIR"/M3dAsm_*.s; do
        NAME="$(basename "$ASM_FILE" .s)"
        # Skip mangled-name duplicates (contain __)
        if [[ "$NAME" == *__* ]]; then
            continue
        fi
        M3DASM_SOURCES+=("$ASM_FILE")
        OBJ_FILE="$OBJ_DIR/$NAME.o"
        OBJ_SIDECAR_FILE="${OBJ_FILE}.psyq.json"
        PSYQ_OBJ_FILE="$PSYQ_OBJ_DIR/$NAME.obj"
        M3D_FUNCTION_OUTPUTS=(
            "$OBJ_FILE"
            "$OBJ_SIDECAR_FILE"
            "$PSYQ_OBJ_FILE"
        )
        M3D_FUNCTION_DEPENDENCIES=(
            "$ASM_FILE"
            "${M3D_CONVERTED_DEPENDENCIES[@]}"
        )
        if object_set_is_newer_than_all \
                "${M3D_FUNCTION_OUTPUTS[@]}" -- \
                "${M3D_FUNCTION_DEPENDENCIES[@]}"; then
            M3D_CACHED_COUNT=$((M3D_CACHED_COUNT + 1))
        else
            rm -f "${M3D_FUNCTION_OUTPUTS[@]}"

            # Convert glabel format to GAS format.
            if ! "$PYTHON_BIN" "$GLABEL2GAS" \
                    "$ASM_FILE" "$TMP_DIR/${NAME}_gas.s" \
                || ! "$PYTHON_BIN" "$ASPSX_ASSEMBLE" \
                    "$TMP_DIR/${NAME}_gas.s" "$OBJ_FILE" --gp 8 \
                    --psyq-obj "$PSYQ_OBJ_FILE" 2>/dev/null; then
                echo "  FAIL: $NAME (source conversion or assembly returned nonzero)"
                rm -f "${M3D_FUNCTION_OUTPUTS[@]}"
                exit 1
            fi
            if [ ! -f "$OBJ_FILE" ] \
                || [ ! -f "$OBJ_SIDECAR_FILE" ] \
                || [ ! -f "$PSYQ_OBJ_FILE" ]; then
                echo "  FAIL: $NAME (assembly outputs incomplete)"
                rm -f "${M3D_FUNCTION_OUTPUTS[@]}"
                exit 1
            fi
            M3D_REBUILT_COUNT=$((M3D_REBUILT_COUNT + 1))
        fi
        M3DASM_COUNT=$((M3DASM_COUNT + 1))
    done
    echo "  Available: $M3DASM_COUNT M3dAsm functions"
    printf -v M3DASM_SOURCE_IDENTITY '%s|' "${M3DASM_SOURCES[@]}"
    M3DASM_SOURCE_STAMP="$TMP_DIR/M3dAsm.sources"
    update_dependency_stamp "$M3DASM_SOURCE_STAMP" "$M3DASM_SOURCE_IDENTITY"

    # The review sources stay split by entry point, while the native link uses
    # the original single m3dasm.obj contribution.  Combining before glabel
    # conversion also lets cross-entry local branches resolve naturally.
    M3DASM_NATIVE_OBJ="$TMP_DIR/M3dAsm_native.o"
    M3DASM_NATIVE_SIDECAR="${M3DASM_NATIVE_OBJ}.psyq.json"
    M3DASM_PSYQ_OBJ="$PSYQ_OBJ_DIR/M3dAsm.obj"
    M3DASM_NATIVE_OUTPUTS=(
        "$M3DASM_NATIVE_OBJ"
        "$M3DASM_NATIVE_SIDECAR"
        "$M3DASM_PSYQ_OBJ"
    )
    M3DASM_NATIVE_DEPENDENCIES=(
        "$ASM_DIR/M3d_ZScratch.s"
        "${M3DASM_SOURCES[@]}"
        "$M3DASM_SOURCE_STAMP"
        "${M3D_COMBINED_DEPENDENCIES[@]}"
    )
    if object_set_is_newer_than_all \
            "${M3DASM_NATIVE_OUTPUTS[@]}" -- \
            "${M3DASM_NATIVE_DEPENDENCIES[@]}"; then
        M3D_CACHED_COUNT=$((M3D_CACHED_COUNT + 1))
    else
        rm -f "${M3DASM_NATIVE_OUTPUTS[@]}"
        if ! "$PYTHON_BIN" "$COMBINE_ASM_SOURCES" \
                --expected-start 0x80097EA8 --expected-end 0x8009E374 \
                "$TMP_DIR/M3dAsm_combined.s" \
                "$ASM_DIR/M3d_ZScratch.s" "${M3DASM_SOURCES[@]}" \
            || ! "$PYTHON_BIN" "$GLABEL2GAS" \
                "$TMP_DIR/M3dAsm_combined.s" \
                "$TMP_DIR/M3dAsm_combined_gas.s" \
            || ! "$PYTHON_BIN" "$ASPSX_ASSEMBLE" \
                "$TMP_DIR/M3dAsm_combined_gas.s" \
                "$M3DASM_NATIVE_OBJ" --gp 8 \
                --psyq-obj "$M3DASM_PSYQ_OBJ" 2>/dev/null; then
            echo "  FAIL: monolithic M3dAsm native object (preparation or assembly failed)"
            rm -f "${M3DASM_NATIVE_OUTPUTS[@]}"
            exit 1
        fi
        if [ ! -f "$M3DASM_NATIVE_OBJ" ] \
            || [ ! -f "$M3DASM_NATIVE_SIDECAR" ] \
            || [ ! -f "$M3DASM_PSYQ_OBJ" ]; then
            echo "  FAIL: monolithic M3dAsm native object (outputs incomplete)"
            rm -f "${M3DASM_NATIVE_OUTPUTS[@]}"
            exit 1
        fi
        M3D_REBUILT_COUNT=$((M3D_REBUILT_COUNT + 1))
    fi

    # Phase 1c: Assemble M3dMaths hand-written assembly
    echo ""
    echo "=== Phase 1c: Assembling M3dMaths hand-written functions ==="
    M3DMATHS_COUNT=0
    M3DMATHS_SOURCES=()
    for ASM_FILE in "$ASM_DIR"/M3dMaths_*.s; do
        NAME="$(basename "$ASM_FILE" .s)"
        # Skip mangled-name duplicates (contain __)
        if [[ "$NAME" == *__* ]]; then
            continue
        fi
        M3DMATHS_SOURCES+=("$ASM_FILE")
        OBJ_FILE="$OBJ_DIR/$NAME.o"
        OBJ_SIDECAR_FILE="${OBJ_FILE}.psyq.json"
        PSYQ_OBJ_FILE="$PSYQ_OBJ_DIR/$NAME.obj"
        M3D_FUNCTION_OUTPUTS=(
            "$OBJ_FILE"
            "$OBJ_SIDECAR_FILE"
            "$PSYQ_OBJ_FILE"
        )
        M3D_FUNCTION_DEPENDENCIES=(
            "$ASM_FILE"
            "${M3D_CONVERTED_DEPENDENCIES[@]}"
        )
        if object_set_is_newer_than_all \
                "${M3D_FUNCTION_OUTPUTS[@]}" -- \
                "${M3D_FUNCTION_DEPENDENCIES[@]}"; then
            M3D_CACHED_COUNT=$((M3D_CACHED_COUNT + 1))
        else
            rm -f "${M3D_FUNCTION_OUTPUTS[@]}"

            # Convert glabel format to GAS format.
            if ! "$PYTHON_BIN" "$GLABEL2GAS" \
                    "$ASM_FILE" "$TMP_DIR/${NAME}_gas.s" \
                || ! "$PYTHON_BIN" "$ASPSX_ASSEMBLE" \
                    "$TMP_DIR/${NAME}_gas.s" "$OBJ_FILE" --gp 8 \
                    --psyq-obj "$PSYQ_OBJ_FILE" 2>/dev/null; then
                echo "  FAIL: $NAME (source conversion or assembly returned nonzero)"
                rm -f "${M3D_FUNCTION_OUTPUTS[@]}"
                exit 1
            fi
            if [ ! -f "$OBJ_FILE" ] \
                || [ ! -f "$OBJ_SIDECAR_FILE" ] \
                || [ ! -f "$PSYQ_OBJ_FILE" ]; then
                echo "  FAIL: $NAME (assembly outputs incomplete)"
                rm -f "${M3D_FUNCTION_OUTPUTS[@]}"
                exit 1
            fi
            M3D_REBUILT_COUNT=$((M3D_REBUILT_COUNT + 1))
        fi
        M3DMATHS_COUNT=$((M3DMATHS_COUNT + 1))
    done
    echo "  Available: $M3DMATHS_COUNT M3dMaths functions"
    printf -v M3DMATHS_SOURCE_IDENTITY '%s|' "${M3DMATHS_SOURCES[@]}"
    M3DMATHS_SOURCE_STAMP="$TMP_DIR/M3dMaths.sources"
    update_dependency_stamp "$M3DMATHS_SOURCE_STAMP" "$M3DMATHS_SOURCE_IDENTITY"

    M3DMATHS_NATIVE_OBJ="$TMP_DIR/M3dMaths_native.o"
    M3DMATHS_NATIVE_SIDECAR="${M3DMATHS_NATIVE_OBJ}.psyq.json"
    M3DMATHS_PSYQ_OBJ="$PSYQ_OBJ_DIR/M3dMaths.obj"
    M3DMATHS_NATIVE_OUTPUTS=(
        "$M3DMATHS_NATIVE_OBJ"
        "$M3DMATHS_NATIVE_SIDECAR"
        "$M3DMATHS_PSYQ_OBJ"
    )
    M3DMATHS_NATIVE_DEPENDENCIES=(
        "${M3DMATHS_SOURCES[@]}"
        "$M3DMATHS_SOURCE_STAMP"
        "${M3D_COMBINED_DEPENDENCIES[@]}"
    )
    if object_set_is_newer_than_all \
            "${M3DMATHS_NATIVE_OUTPUTS[@]}" -- \
            "${M3DMATHS_NATIVE_DEPENDENCIES[@]}"; then
        M3D_CACHED_COUNT=$((M3D_CACHED_COUNT + 1))
    else
        rm -f "${M3DMATHS_NATIVE_OUTPUTS[@]}"
        if ! "$PYTHON_BIN" "$COMBINE_ASM_SOURCES" \
                --expected-start 0x8009E374 --expected-end 0x8009EA3C \
                "$TMP_DIR/M3dMaths_combined.s" \
                "${M3DMATHS_SOURCES[@]}" \
            || ! "$PYTHON_BIN" "$GLABEL2GAS" \
                "$TMP_DIR/M3dMaths_combined.s" \
                "$TMP_DIR/M3dMaths_combined_gas.s" \
            || ! "$PYTHON_BIN" "$ASPSX_ASSEMBLE" \
                "$TMP_DIR/M3dMaths_combined_gas.s" \
                "$M3DMATHS_NATIVE_OBJ" --gp 8 \
                --psyq-obj "$M3DMATHS_PSYQ_OBJ" 2>/dev/null; then
            echo "  FAIL: monolithic M3dMaths native object (preparation or assembly failed)"
            rm -f "${M3DMATHS_NATIVE_OUTPUTS[@]}"
            exit 1
        fi
        if [ ! -f "$M3DMATHS_NATIVE_OBJ" ] \
            || [ ! -f "$M3DMATHS_NATIVE_SIDECAR" ] \
            || [ ! -f "$M3DMATHS_PSYQ_OBJ" ]; then
            echo "  FAIL: monolithic M3dMaths native object (outputs incomplete)"
            rm -f "${M3DMATHS_NATIVE_OUTPUTS[@]}"
            exit 1
        fi
        M3D_REBUILT_COUNT=$((M3D_REBUILT_COUNT + 1))
    fi
    echo "  Hand-written M3d objects: $M3D_REBUILT_COUNT rebuilt, $M3D_CACHED_COUNT cached"

    # Phase 1d: Assemble the original ADDRESS.S boundary-word owner.  Keep the
    # ELF conversion in tmp so the legacy GNU link does not consume this
    # native-only object beside its extracted data blob.
    echo ""
    echo "=== Phase 1d: Assembling ADDRESS.S boundary owner ==="
    ADDRESS_NATIVE_OBJ="$TMP_DIR/ADDRESS_native.o"
    ADDRESS_NATIVE_SIDECAR="${ADDRESS_NATIVE_OBJ}.psyq.json"
    ADDRESS_PSYQ_OBJ="$PSYQ_OBJ_DIR/ADDRESS.obj"
    ADDRESS_OUTPUTS=(
        "$ADDRESS_NATIVE_OBJ"
        "$ADDRESS_NATIVE_SIDECAR"
        "$ADDRESS_PSYQ_OBJ"
    )
    ADDRESS_DEPENDENCIES=(
        "$ASM_DIR/ADDRESS.s"
        "$VERIFY_ADDRESS_OWNER"
        "${M3D_ASPSX_DEPENDENCIES[@]}"
    )
    if object_set_is_newer_than_all \
            "${ADDRESS_OUTPUTS[@]}" -- "${ADDRESS_DEPENDENCIES[@]}"; then
        echo "  ADDRESS owner: cached"
    else
        rm -f "${ADDRESS_OUTPUTS[@]}"
        if ! "$PYTHON_BIN" "$ASPSX_ASSEMBLE" \
                "$ASM_DIR/ADDRESS.s" "$ADDRESS_NATIVE_OBJ" --gp 8 \
                --psyq-obj "$ADDRESS_PSYQ_OBJ" 2>/dev/null; then
            echo "  FAIL: ADDRESS owner (assembly returned nonzero)"
            rm -f "${ADDRESS_OUTPUTS[@]}"
            exit 1
        fi
        if [ ! -f "$ADDRESS_NATIVE_OBJ" ] \
            || [ ! -f "$ADDRESS_NATIVE_SIDECAR" ] \
            || [ ! -f "$ADDRESS_PSYQ_OBJ" ]; then
            echo "  FAIL: ADDRESS owner (outputs incomplete)"
            rm -f "${ADDRESS_OUTPUTS[@]}"
            exit 1
        fi
        echo "  ADDRESS owner: rebuilt"
    fi
    if ! "$PYTHON_BIN" "$VERIFY_ADDRESS_OWNER" \
            --psyq-object "$ADDRESS_PSYQ_OBJ"; then
        echo "ADDRESS owner verification failed; refusing native link." >&2
        exit 1
    fi
fi

# ===========================================================================
# Phase 2n: Diagnostic native link with complete Psy-Q relocations/sections
# ===========================================================================
if $DO_NATIVE_LINK; then
    echo ""
    echo "=== Phase 2n: Native PSYLINK diagnostic ==="
    NATIVE_DIR="$OUTPUT_DIR/native"
    NATIVE_ORDER_FILE="$TMP_DIR/native_object_order.txt"
    mkdir -p "$NATIVE_DIR"
    python3 "$SCRIPT_DIR/tools/native_object_order.py" --check > "$NATIVE_ORDER_FILE"
    mapfile -t NATIVE_OBJECTS < "$NATIVE_ORDER_FILE"

    # The two independently reconstructed hand-written owners sit outside the
    # canonical function-progress denominator. Refuse to feed their native
    # objects to PSYLINK unless their complete text and relocations still match.
    if ! python3 "$VERIFY_HANDWRITTEN_OWNERS" \
            --psyq-object-dir "$PSYQ_OBJ_DIR"; then
        echo "Hand-written owner verification failed; refusing native link." >&2
        exit 1
    fi
    if ! python3 "$VERIFY_ADDRESS_OWNER" \
            --psyq-object "$PSYQ_OBJ_DIR/ADDRESS.obj"; then
        echo "ADDRESS owner verification failed; refusing native link." >&2
        exit 1
    fi
    if ! python3 "$VERIFY_DATA_SECTIONS"; then
        echo "Recovered data verification failed; refusing native link." >&2
        exit 1
    fi

    # First-seen archive order recovered from the 167 library-member FILE
    # blocks in original/MAIN.SYM.  Archive members remain lazily selected by
    # PSYLINK, matching the original toolchain model.
    NATIVE_LIBRARIES=(
        LIBGTE.LIB
        LIBC.LIB
        LIBGPU.LIB
        LIBCARD.LIB
        LIBAPI.LIB
        LIBSN.LIB
        LIBETC.LIB
        LIBCD.LIB
        LIBPAD.LIB
        LIBSPU.LIB
        LIBSND.LIB
        LIBPRESS.LIB
        LIBDS.LIB
    )
    NATIVE_LIBRARY_ARGS=()
    for LIBRARY in "${NATIVE_LIBRARIES[@]}"; do
        NATIVE_LIBRARY_ARGS+=(
            --library "$SCRIPT_DIR/toolchain/psyq-4.6-sdk/LIB/$LIBRARY"
        )
    done
    python3 "$SCRIPT_DIR/tools/psylink_build.py" \
        "${NATIVE_OBJECTS[@]}" \
        "${NATIVE_LIBRARY_ARGS[@]}" \
        --zero-bss \
        --image-origin 0x80010000 \
        --output "$NATIVE_DIR/rebuilt.bin"
    python3 "$VERIFY_ADDRESS_OWNER" \
        --psyq-object "$PSYQ_OBJ_DIR/ADDRESS.obj" \
        --linked-sym "$NATIVE_DIR/rebuilt.sym"
    if $DO_NATIVE_DIFF; then
        echo ""
        echo "=== Native image comparison ==="
        # The map is mandatory: it proves the raw /p image is address-ordered
        # and supplies the rebuilt-side symbol addresses.
        python3 "$SCRIPT_DIR/tools/bindiff.py" "$NATIVE_DIR/rebuilt.bin" \
            --rebuilt-load-address 0x80010000 \
            --rebuilt-map "$NATIVE_DIR/rebuilt.map" \
            --per-function
    fi
fi

# ===========================================================================
# Phase 2: Generate linker script + extract blobs
# ===========================================================================
if $DO_LINK; then
    echo ""
    echo "=== Phase 2: Generating linker script + extracting blobs ==="
    python3 "$SCRIPT_DIR/tools/gen_linker_script.py" 2>&1

    # Assemble blob wrappers (must run from output dir for .incbin)
    cd "$OUTPUT_DIR"
    mips-linux-gnu-as -EL -march=r3000 -mabi=32 -o obj/psyq_blob.o psyq_blob.s 2>/dev/null
    echo "  Assembled PSY-Q library blob"
    mips-linux-gnu-as -EL -march=r3000 -mabi=32 -o obj/data_blob.o data_blob.s 2>/dev/null
    echo "  Assembled data section blob"
    cd "$SCRIPT_DIR"
fi

# ===========================================================================
# Phase 3: Link all objects
# ===========================================================================
if $DO_LINK; then
    echo ""
    echo "=== Phase 3: Linking ==="

    # Count object files without parsing path names or failing on an empty glob.
    shopt -s nullglob
    BUILT_OBJECTS=("$OBJ_DIR"/*.o)
    shopt -u nullglob
    OBJ_COUNT=${#BUILT_OBJECTS[@]}
    echo "  Object files: $OBJ_COUNT"

    # Link with the master linker script
    # The linker script specifies the exact input file ordering
    rm -f "$OUTPUT_DIR/rebuilt.bin" "$OUTPUT_DIR/rebuilt.exe"
    mips-linux-gnu-ld -T "$LINKER_SCRIPT" \
        -L "$OBJ_DIR" \
        -o "$OUTPUT_DIR/rebuilt.bin" 2>&1 || {
        echo "  Link failed — check errors above"
        echo "  Object files are available in $OBJ_DIR/"
        exit 1
    }

    if [ -f "$OUTPUT_DIR/rebuilt.bin" ]; then
        SIZE=$(stat -c%s "$OUTPUT_DIR/rebuilt.bin" 2>/dev/null || stat -f%z "$OUTPUT_DIR/rebuilt.bin")
        echo "  Linked binary: $SIZE bytes"

        # Wrap in PS-EXE
        python3 "$SCRIPT_DIR/tools/mkpsxexe.py" \
            "$OUTPUT_DIR/rebuilt.bin" \
            "$OUTPUT_DIR/rebuilt.exe"
    fi
fi

# ===========================================================================
# Phase 4: Binary diff
# ===========================================================================
if $DO_DIFF && [ -f "$OUTPUT_DIR/rebuilt.bin" ]; then
    echo ""
    echo "=== Phase 4: Binary comparison ==="
    python3 "$SCRIPT_DIR/tools/bindiff.py" "$OUTPUT_DIR/rebuilt.exe" --summary
fi

if $DO_COMPILE; then
    if ! "$PYTHON_BIN" "$EGCS_JUMP_PATCH_HELPER" \
            --check "$COMPILER_DIR" >/dev/null; then
        echo "Canonical compiler jump behavior changed during build; refusing success." >&2
        exit 1
    fi
    if ! ASSEMBLY_PIPELINE_FINAL_IDENTITY="$(assembly_pipeline_identity)"; then
        echo "Assembly-pipeline validation failed after compilation." >&2
        exit 1
    fi
    if [ "$ASSEMBLY_PIPELINE_FINAL_IDENTITY" != "$ASSEMBLY_PIPELINE_START_IDENTITY" ]; then
        echo "Assembly pipeline changed during compilation; refusing success." >&2
        exit 1
    fi
fi

echo ""
echo "Done."
