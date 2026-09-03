#!/usr/bin/env python3
"""Verify the unmodified, pinned EGCS 1.1.2 PSX compiler binaries.

The compiler archive installed by ``setup_toolchain.sh`` is a reproducible
input to every canonical match.  This helper is deliberately read-only: it
accepts only the two stock binaries from that archive and never patches or
repairs compiler code.
"""

from __future__ import annotations

import argparse
import hashlib
import stat
import sys
from pathlib import Path


EXPECTED_SHA256 = {
    "cc1": "dfd5077618c62b4e439d6efdbfec8b9a6a157d5c15a515ca50752f7f76b8e30a",
    "cc1plus": "75d1f249a5c879ca583cbb9ad99e9e88f5b7e8d766f8a792735c6c58e80b0890",
}


class VerificationError(RuntimeError):
    """Raised when a compiler input is missing, altered, or surprising."""


def sha256_file(path: Path) -> str:
    """Hash one regular compiler file without following a symbolic link."""

    try:
        path_stat = path.lstat()
    except FileNotFoundError as exc:
        raise VerificationError(f"{path}: compiler binary is missing") from exc
    except OSError as exc:
        raise VerificationError(f"{path}: cannot inspect compiler binary: {exc}") from exc

    if stat.S_ISLNK(path_stat.st_mode):
        raise VerificationError(f"{path}: compiler binary must not be a symbolic link")
    if not stat.S_ISREG(path_stat.st_mode):
        raise VerificationError(f"{path}: compiler binary is not a regular file")

    digest = hashlib.sha256()
    try:
        with path.open("rb") as compiler:
            for chunk in iter(lambda: compiler.read(1024 * 1024), b""):
                digest.update(chunk)
    except OSError as exc:
        raise VerificationError(f"{path}: cannot read compiler binary: {exc}") from exc
    return digest.hexdigest()


def verify_directory(compiler_dir: Path) -> tuple[tuple[str, str], ...]:
    """Require the complete stock compiler pair and return its identities."""

    verified: list[tuple[str, str]] = []
    for name, expected in EXPECTED_SHA256.items():
        path = compiler_dir / name
        actual = sha256_file(path)
        if actual != expected:
            raise VerificationError(
                f"{path}: SHA-256 {actual} is not the unmodified EGCS 1.1.2 "
                f"identity {expected}"
            )
        verified.append((name, actual))
    return tuple(verified)


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--check",
        action="store_true",
        required=True,
        help="require the unmodified stock compiler pair",
    )
    parser.add_argument(
        "compiler_dir",
        nargs="?",
        type=Path,
        default=Path(__file__).resolve().parent / "gcc-2.91.66-psx",
        help="directory containing cc1 and cc1plus",
    )
    args = parser.parse_args(argv)

    try:
        for name, digest in verify_directory(args.compiler_dir):
            print(f"{name}: unmodified EGCS 1.1.2 verified ({digest})")
    except VerificationError as error:
        print(f"ERROR: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
