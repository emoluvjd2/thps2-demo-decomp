#!/usr/bin/env python3
"""Restore the target-proven EGCS 1.1.1 recursive jump-deletion behavior.

The downloadable PSX compiler is built from EGCS 1.1.2.  That release added
a guard in ``jump.c:delete_computation`` which disables recursive deletion
after the second scheduler.  EGCS 1.1.1 did not contain the guard, and the
target's PHYSICS object independently requires the earlier behavior.

This helper makes the source-level reversion reproducible for the exact static
``cc1`` and ``cc1plus`` binaries installed by ``setup_toolchain.sh``.  It is
fail-closed: an unknown compiler or unexpected instruction sequence is never
modified.  Re-running it on an already-patched compiler is harmless.
"""

from __future__ import annotations

import argparse
import hashlib
import os
import stat
import sys
import tempfile
from dataclasses import dataclass
from pathlib import Path


@dataclass(frozen=True)
class BinaryPatch:
    """One verified machine-code realization of the historical source change."""

    name: str
    stock_sha256: str
    patched_sha256: str
    offset: int
    stock_guard: bytes
    resume_signature: bytes


# At each offset the first instruction loads ``reload_completed``.  Replacing
# it with ``jmp +0x26`` skips the complete 1.1.2-only guard and enters the
# unchanged 1.1.1 loop over REG_DEAD notes.  ``stock_guard`` covers every byte
# skipped by the jump, while ``resume_signature`` proves the destination is the
# expected loop entry rather than merely a byte at the right distance.
PATCH_BYTES = bytes.fromhex("e9 26 00 00 00")
PATCHES = (
    BinaryPatch(
        name="cc1",
        stock_sha256="dfd5077618c62b4e439d6efdbfec8b9a6a157d5c15a515ca50752f7f76b8e30a",
        patched_sha256="cffa6c2bdfb1425559f2a11bd213a146604107f6eba642781c183fc4530e9466",
        offset=0xD22F1,
        stock_guard=bytes.fromhex(
            "c7 c0 78 b3 34 08 8b 00 85 c0 74 1f "
            "c7 c0 1c 6c 33 08 8b 00 85 c0 74 13 "
            "83 ec 0c ff 75 08 e8 f5 01 00 00 83 "
            "c4 10 e9 e8 01 00 00"
        ),
        resume_signature=bytes.fromhex("8b 45 08 8b 40 1c 89 45 e4"),
    ),
    BinaryPatch(
        name="cc1plus",
        stock_sha256="75d1f249a5c879ca583cbb9ad99e9e88f5b7e8d766f8a792735c6c58e80b0890",
        patched_sha256="c590af1c78de851f2df12e89ad6cfe84adc5e965746c8be07f853420af4ff0a5",
        offset=0x13D451,
        stock_guard=bytes.fromhex(
            "c7 c0 b8 f9 3d 08 8b 00 85 c0 74 1f "
            "c7 c0 84 a4 3c 08 8b 00 85 c0 74 13 "
            "83 ec 0c ff 75 08 e8 f5 01 00 00 83 "
            "c4 10 e9 e8 01 00 00"
        ),
        resume_signature=bytes.fromhex("8b 45 08 8b 40 1c 89 45 e4"),
    ),
)


class PatchError(RuntimeError):
    """Raised when a compiler is missing or does not match a known state."""


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def _resume_offset(spec: BinaryPatch) -> int:
    """Return the destination encoded by the replacement near jump."""

    if len(PATCH_BYTES) != 5 or PATCH_BYTES[0] != 0xE9:
        raise PatchError("internal patch is not an x86 E9 rel32 instruction")
    displacement = int.from_bytes(PATCH_BYTES[1:], "little", signed=True)
    return spec.offset + len(PATCH_BYTES) + displacement


def _validate_spec(spec: BinaryPatch) -> int:
    """Validate the relation between the skipped guard and resume point."""

    if spec.offset < 0:
        raise PatchError(f"{spec.name}: patch offset must not be negative")
    if len(spec.stock_guard) < len(PATCH_BYTES):
        raise PatchError(f"{spec.name}: stock guard is shorter than the patch")
    guard_end = spec.offset + len(spec.stock_guard)
    resume_offset = _resume_offset(spec)
    if resume_offset != guard_end:
        raise PatchError(
            f"{spec.name}: jump resumes at 0x{resume_offset:x}, not immediately "
            f"after the verified guard at 0x{guard_end:x}"
        )
    if not spec.resume_signature:
        raise PatchError(f"{spec.name}: resume signature is empty")
    return resume_offset


def _verify_stock_layout(data: bytes, spec: BinaryPatch) -> None:
    """Require the complete skipped guard and its destination signature."""

    resume_offset = _validate_spec(spec)
    guard_end = spec.offset + len(spec.stock_guard)
    if data[spec.offset:guard_end] != spec.stock_guard:
        raise PatchError(
            f"{spec.name}: verified stock hash has an unexpected jump guard"
        )
    resume_end = resume_offset + len(spec.resume_signature)
    if data[resume_offset:resume_end] != spec.resume_signature:
        raise PatchError(
            f"{spec.name}: verified stock hash has an unexpected resume point"
        )


def _verify_patched_layout(data: bytes, spec: BinaryPatch) -> None:
    """Require the replacement jump and the destination it promises to use."""

    resume_offset = _validate_spec(spec)
    if data[spec.offset : spec.offset + len(PATCH_BYTES)] != PATCH_BYTES:
        raise PatchError(
            f"{spec.name}: verified patched hash has unexpected patch bytes"
        )
    resume_end = resume_offset + len(spec.resume_signature)
    if data[resume_offset:resume_end] != spec.resume_signature:
        raise PatchError(
            f"{spec.name}: verified patched hash has an unexpected resume point"
        )


def patch_payload(data: bytes, spec: BinaryPatch) -> tuple[bytes, bool]:
    """Return the verified patched payload and whether it changed."""

    _validate_spec(spec)
    digest = sha256(data)
    if digest == spec.patched_sha256:
        _verify_patched_layout(data, spec)
        return data, False
    if digest != spec.stock_sha256:
        raise PatchError(
            f"{spec.name}: unknown SHA-256 {digest}; refusing to modify it"
        )

    _verify_stock_layout(data, spec)

    patched = bytearray(data)
    patched[spec.offset : spec.offset + len(PATCH_BYTES)] = PATCH_BYTES
    result = bytes(patched)
    result_digest = sha256(result)
    if result_digest != spec.patched_sha256:
        raise PatchError(
            f"{spec.name}: patched SHA-256 {result_digest} is not the expected "
            f"{spec.patched_sha256}"
        )
    return result, True


def _read_regular_file(path: Path, spec: BinaryPatch) -> tuple[bytes, int]:
    """Read one compiler without following a surprising filesystem object."""

    try:
        path_stat = path.lstat()
    except FileNotFoundError as exc:
        raise PatchError(f"{path}: compiler binary is missing") from exc
    except OSError as exc:
        raise PatchError(f"{path}: cannot inspect compiler binary: {exc}") from exc
    if stat.S_ISLNK(path_stat.st_mode):
        raise PatchError(f"{path}: compiler binary must not be a symbolic link")
    if not stat.S_ISREG(path_stat.st_mode):
        raise PatchError(f"{path}: compiler binary is not a regular file")
    try:
        data = path.read_bytes()
    except OSError as exc:
        raise PatchError(f"{path}: cannot read compiler binary: {exc}") from exc
    # Validate the specification here too so callers cannot mistake a malformed
    # local fixture or future table entry for an ordinary unknown binary.
    _validate_spec(spec)
    return data, stat.S_IMODE(path_stat.st_mode)


def compiler_state(path: Path, spec: BinaryPatch) -> str:
    """Return ``stock``, ``patched``, or fail for an unknown compiler."""

    data, _ = _read_regular_file(path, spec)
    digest = sha256(data)
    if digest == spec.stock_sha256:
        _verify_stock_layout(data, spec)
        return "stock"
    if digest == spec.patched_sha256:
        _verify_patched_layout(data, spec)
        return "patched"
    raise PatchError(f"{path}: unknown SHA-256 {digest}")


def _preflight_file(path: Path, spec: BinaryPatch) -> tuple[bytes, int, bool]:
    """Read and validate one input without modifying it."""

    data, original_mode = _read_regular_file(path, spec)
    patched, changed = patch_payload(data, spec)
    return patched, original_mode, changed


def _replace_file(path: Path, spec: BinaryPatch, payload: bytes, mode: int) -> None:
    """Atomically replace one preflighted compiler and verify it from disk."""

    temporary_name: str | None = None
    try:
        with tempfile.NamedTemporaryFile(
            dir=path.parent, prefix=f".{path.name}.jump-patch.", delete=False
        ) as temporary:
            temporary_name = temporary.name
            temporary.write(payload)
            temporary.flush()
            os.fsync(temporary.fileno())
        os.chmod(temporary_name, mode)
        os.replace(temporary_name, path)
        temporary_name = None
    except OSError as exc:
        raise PatchError(f"{path}: cannot install patched compiler: {exc}") from exc
    finally:
        if temporary_name is not None:
            try:
                Path(temporary_name).unlink(missing_ok=True)
            except OSError:
                # Preserve the actionable install failure rather than masking
                # it with best-effort cleanup of an unpublished temp file.
                pass

    if compiler_state(path, spec) != "patched":
        # ``compiler_state`` currently either returns ``patched`` or raises, but
        # retain an explicit guard if its API grows another accepted state.
        raise PatchError(f"{path}: patched compiler failed on-disk verification")


def patch_file(path: Path, spec: BinaryPatch) -> bool:
    """Atomically patch one compiler, returning whether it changed."""

    patched, original_mode, changed = _preflight_file(path, spec)
    if not changed:
        return False
    _replace_file(path, spec, patched, original_mode)
    return True


def patch_directory(
    compiler_dir: Path,
    specs: tuple[BinaryPatch, ...] = PATCHES,
) -> tuple[tuple[BinaryPatch, bool], ...]:
    """Preflight every compiler before replacing any member of the pair."""

    prepared: list[tuple[Path, BinaryPatch, bytes, int, bool]] = []
    for spec in specs:
        path = compiler_dir / spec.name
        payload, mode, changed = _preflight_file(path, spec)
        prepared.append((path, spec, payload, mode, changed))

    for path, spec, payload, mode, changed in prepared:
        if changed:
            _replace_file(path, spec, payload, mode)
    return tuple((spec, changed) for _, spec, _, _, changed in prepared)


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    action = parser.add_mutually_exclusive_group(required=True)
    action.add_argument("--apply", action="store_true", help="apply the verified patch")
    action.add_argument("--check", action="store_true", help="require patched binaries")
    parser.add_argument(
        "compiler_dir",
        nargs="?",
        type=Path,
        default=Path(__file__).resolve().parent / "gcc-2.91.66-psx",
        help="directory containing cc1 and cc1plus",
    )
    args = parser.parse_args(argv)

    try:
        if args.apply:
            for spec, changed in patch_directory(args.compiler_dir):
                state = "patched" if changed else "already patched"
                print(f"{spec.name}: {state} ({spec.patched_sha256})")
        else:
            for spec in PATCHES:
                path = args.compiler_dir / spec.name
                state = compiler_state(path, spec)
                if state != "patched":
                    raise PatchError(
                        f"{path}: stock EGCS 1.1.2 jump behavior; run --apply"
                    )
                print(f"{spec.name}: historical jump behavior verified")
    except PatchError as error:
        print(f"ERROR: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
