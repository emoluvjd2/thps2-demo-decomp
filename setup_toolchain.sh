#!/bin/bash
# setup_toolchain.sh — Set up GCC 2.91.66 PSX cross-compiler for decompilation matching
#
# Run this once from the repository root on Linux or inside WSL Ubuntu:
#   cd /path/to/thps2-psx-proto
#   bash setup_toolchain.sh
#
# Creates: compiler tools and a validated case-folded Psy-Q header include view.

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
TOOLCHAIN_DIR="$SCRIPT_DIR/toolchain"
COMPILER_DIR="$TOOLCHAIN_DIR/gcc-2.91.66-psx"
MASPSX_DIR="$TOOLCHAIN_DIR/maspsx"
PSYQ_SDK_DIR="$TOOLCHAIN_DIR/psyq-4.6-sdk"
PSYQ_SDK_HEADER_HELPER="$SCRIPT_DIR/tools/psyq_sdk_headers.py"
EGCS_COMPILER_VERIFIER="$TOOLCHAIN_DIR/verify_egcs_1_1_2.py"
COMPILER_ARCHIVE_SHA256="25667f9cc2be6fcce38ada9b466fdf9dccf7fd02f86af2fda7019173f308c833"

echo "=== PSX Cross-Compiler Setup ==="
echo "Target: $TOOLCHAIN_DIR"
echo ""

# Step 1: Check/install system packages
echo "[1/5] Checking system packages..."
MISSING=""
command -v mips-linux-gnu-as >/dev/null 2>&1 || MISSING="$MISSING binutils-mips-linux-gnu"
command -v cpp >/dev/null 2>&1 || MISSING="$MISSING cpp"
command -v python3 >/dev/null 2>&1 || MISSING="$MISSING python3"

if [ -n "$MISSING" ]; then
    echo "  Installing:$MISSING"
    sudo apt-get update -qq
    sudo apt-get install -y -qq $MISSING
else
    echo "  All packages present."
fi

# Step 2: Validate the user-supplied official SDK and prepare its include view.
# The SDK cannot be redistributed or downloaded by this repository.
echo "[2/5] Validating Psy-Q 4.6 SDK headers..."
if [ ! -d "$PSYQ_SDK_DIR/INCLUDE" ]; then
    echo "  ERROR: Install the unmodified Psy-Q 4.6 (DTL-S2360) SDK at:" >&2
    echo "    $PSYQ_SDK_DIR" >&2
    echo "  The required SDK is not redistributable and is not downloaded here." >&2
    exit 1
fi
python3 "$PSYQ_SDK_HEADER_HELPER" --prepare

# Step 3: Download GCC 2.91.66-psx
echo "[3/5] Downloading GCC 2.91.66-psx..."
mkdir -p "$TOOLCHAIN_DIR"
if [ -f "$COMPILER_DIR/cc1" ] \
        && [ -f "$COMPILER_DIR/cc1plus" ] \
        && python3 "$EGCS_COMPILER_VERIFIER" \
            --check "$COMPILER_DIR" >/dev/null 2>&1; then
    echo "  Already downloaded."
else
    DOWNLOAD_STAGE="$(mktemp -d "$TOOLCHAIN_DIR/.gcc-2.91.66-psx.XXXXXXXX")"
    TARBALL="$DOWNLOAD_STAGE/gcc-2.91.66-psx.tar.gz"
    STAGED_COMPILER_DIR="$DOWNLOAD_STAGE/compiler"
    mkdir "$STAGED_COMPILER_DIR"
    wget -q --show-progress -O "$TARBALL" \
        "https://github.com/decompals/old-gcc/releases/download/0.13/gcc-2.91.66-psx.tar.gz"
    printf '%s  %s\n' "$COMPILER_ARCHIVE_SHA256" "$TARBALL" \
        | sha256sum --check --status
    tar xzf "$TARBALL" -C "$STAGED_COMPILER_DIR"
    if [ ! -f "$STAGED_COMPILER_DIR/cc1" ] \
            || [ ! -f "$STAGED_COMPILER_DIR/cc1plus" ]; then
        echo "  ERROR: compiler archive is missing cc1 or cc1plus." >&2
        exit 1
    fi
    chmod +x "$STAGED_COMPILER_DIR"/*

    # Verify the complete stock pair before replacing any live compiler files.
    # A partial or formerly patched installation therefore repairs itself
    # without ever becoming a canonical input.
    python3 "$EGCS_COMPILER_VERIFIER" --check "$STAGED_COMPILER_DIR" >/dev/null

    COMPILER_BACKUP="$TOOLCHAIN_DIR/.gcc-2.91.66-psx.backup.$$"
    if [ -e "$COMPILER_DIR" ]; then
        mv "$COMPILER_DIR" "$COMPILER_BACKUP"
    fi
    if ! mv "$STAGED_COMPILER_DIR" "$COMPILER_DIR"; then
        if [ -e "$COMPILER_BACKUP" ]; then
            mv "$COMPILER_BACKUP" "$COMPILER_DIR"
        fi
        exit 1
    fi
    rm -rf -- "$COMPILER_BACKUP" "$DOWNLOAD_STAGE"
    echo "  Installed: cc1, gcc, cpp, cc1plus, g++"
fi

# Canonical builds use the downloaded compiler without binary modifications.
python3 "$EGCS_COMPILER_VERIFIER" --check "$COMPILER_DIR" >/dev/null

# Step 4: Clone maspsx
echo "[4/5] Setting up maspsx..."
if [ -d "$MASPSX_DIR" ]; then
    echo "  Already cloned."
else
    git clone --depth 1 https://github.com/mkst/maspsx.git "$MASPSX_DIR"
    echo "  Cloned maspsx."
fi

# Step 5: Create 'as' wrapper so gcc driver routes through maspsx
echo "[5/5] Creating assembler wrapper..."
cat > "$COMPILER_DIR/as" << 'WRAPPER'
#!/bin/bash
# Route assembly through maspsx (ASPSX emulation) before GNU as
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
python3 "$SCRIPT_DIR/../maspsx/maspsx.py" --run-assembler --gnu-as-path=mips-linux-gnu-as "$@"
WRAPPER
chmod +x "$COMPILER_DIR/as"

# Verify
echo ""
echo "=== Verification ==="
echo -n "cc1:      " && "$COMPILER_DIR/cc1" -version < /dev/null 2>&1 | head -1 || echo "FAILED"
echo -n "EGCS stock: " && python3 "$EGCS_COMPILER_VERIFIER" --check "$COMPILER_DIR" >/dev/null \
    && echo "verified"
echo -n "as:       " && mips-linux-gnu-as --version | head -1
echo -n "objdump:  " && mips-linux-gnu-objdump --version | head -1
echo -n "maspsx:   " && python3 "$MASPSX_DIR/maspsx.py" --version 2>&1 || echo "(no --version flag, but present)"
echo -n "SDK hdrs: " && python3 "$PSYQ_SDK_HEADER_HELPER" --check
echo ""
echo "=== Setup complete! ==="
echo "Toolchain: $TOOLCHAIN_DIR"
echo ""
echo "Next: Run 'bash test_all_tu.sh' to test the complete source build."
