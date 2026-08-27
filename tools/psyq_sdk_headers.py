#!/usr/bin/env python3
"""Validate the required Psy-Q 4.6 headers and their case-folded include view.

The SDK is user-supplied and intentionally ignored by Git.  Compilation must
nevertheless be reproducible: every header is pinned by the checked-in
manifest, and Linux hosts compile through a generated view which provides both
the original DOS spelling and a lower-case alias for every header path.

Normal build and matcher commands use the read-only ``--check`` path.  Only
``setup_toolchain.sh`` (or an explicit ``--prepare`` invocation) may create or
repair the generated symlink view.
"""

from __future__ import annotations

import argparse
import hashlib
import os
import re
import sys
import tempfile
from dataclasses import dataclass
from pathlib import Path, PurePosixPath


PROJECT_ROOT = Path(__file__).resolve().parent.parent
DEFAULT_SDK_ROOT = PROJECT_ROOT / "toolchain" / "psyq-4.6-sdk"
DEFAULT_MANIFEST_PATH = PROJECT_ROOT / "tools" / "psyq_4_6_headers.sha256"
DEFAULT_INCLUDE_VIEW_NAME = ".casefold-include"
VIEW_STAMP_NAME = ".psyq-sdk-header-view"
VIEW_STAMP_MAGIC = "thps2-psyq-4.6-header-view-v1"
MANIFEST_ROW_RE = re.compile(r"^(?P<digest>[0-9a-fA-F]{64})  (?P<path>.+)$")


class PsyqSdkHeaderError(RuntimeError):
    """The required SDK headers or generated include view are not trustworthy."""


@dataclass(frozen=True)
class ManifestEntry:
    """One exact header pinned by the checked-in SDK manifest."""

    digest: str
    relative_path: PurePosixPath


@dataclass(frozen=True)
class PsyqSdkLayout:
    """Validated source and generated include roots for the Psy-Q headers."""

    sdk_root: Path
    source_root: Path
    include_root: Path
    manifest_path: Path
    headers: tuple[Path, ...]


def _layout_paths(
    *,
    project_root: Path | None = None,
    sdk_root: Path | None = None,
    manifest_path: Path | None = None,
    include_root: Path | None = None,
) -> tuple[Path, Path, Path, Path]:
    root = (project_root or PROJECT_ROOT).resolve()
    resolved_sdk_root = (sdk_root or root / "toolchain" / "psyq-4.6-sdk").resolve()
    source_root = resolved_sdk_root / "INCLUDE"
    resolved_manifest = (
        manifest_path or root / "tools" / "psyq_4_6_headers.sha256"
    ).resolve()
    # Preserve the final path component so validation can reject an include
    # root which is itself a symlink. Path.resolve() would erase that evidence.
    include_root_path = include_root or resolved_sdk_root / DEFAULT_INCLUDE_VIEW_NAME
    resolved_include_root = Path(os.path.abspath(os.fspath(include_root_path)))
    return (
        resolved_sdk_root,
        source_root,
        resolved_manifest,
        resolved_include_root,
    )


def load_manifest(manifest_path: Path) -> tuple[ManifestEntry, ...]:
    """Load and structurally validate the exact SDK-header manifest."""
    try:
        lines = manifest_path.read_text(encoding="utf-8").splitlines()
    except OSError as exc:
        raise PsyqSdkHeaderError(
            f"required SDK header manifest is unavailable: {manifest_path}: {exc}"
        ) from exc

    entries: list[ManifestEntry] = []
    exact_paths: set[PurePosixPath] = set()
    folded_paths: dict[str, PurePosixPath] = {}
    for line_number, line in enumerate(lines, 1):
        if not line or line.startswith("#"):
            continue
        match = MANIFEST_ROW_RE.fullmatch(line)
        if match is None:
            raise PsyqSdkHeaderError(
                f"malformed SDK header manifest row {line_number}: {line!r}"
            )

        raw_path = match.group("path")
        relative_path = PurePosixPath(raw_path)
        if (
            relative_path.is_absolute()
            or raw_path.startswith("/")
            or "\\" in raw_path
            or any(part in ("", ".", "..") for part in relative_path.parts)
        ):
            raise PsyqSdkHeaderError(
                f"unsafe SDK header path on manifest row {line_number}: {raw_path!r}"
            )
        if relative_path in exact_paths:
            raise PsyqSdkHeaderError(
                f"duplicate SDK header manifest path: {relative_path.as_posix()}"
            )

        folded = relative_path.as_posix().casefold()
        previous = folded_paths.get(folded)
        if previous is not None:
            raise PsyqSdkHeaderError(
                "case-folding SDK header collision: "
                f"{previous.as_posix()} and {relative_path.as_posix()}"
            )
        exact_paths.add(relative_path)
        folded_paths[folded] = relative_path
        entries.append(
            ManifestEntry(match.group("digest").lower(), relative_path)
        )

    if not entries:
        raise PsyqSdkHeaderError(f"SDK header manifest is empty: {manifest_path}")
    return tuple(entries)


def _sha256(path: Path) -> str:
    digest = hashlib.sha256()
    try:
        with path.open("rb") as stream:
            for chunk in iter(lambda: stream.read(1024 * 1024), b""):
                digest.update(chunk)
    except OSError as exc:
        raise PsyqSdkHeaderError(f"cannot read required SDK header {path}: {exc}") from exc
    return digest.hexdigest()


def validate_sdk_headers(
    source_root: Path,
    manifest_path: Path,
) -> tuple[tuple[ManifestEntry, ...], tuple[Path, ...]]:
    """Validate the complete user-supplied header tree against the manifest."""
    entries = load_manifest(manifest_path)
    if not source_root.is_dir():
        raise PsyqSdkHeaderError(
            f"required Psy-Q 4.6 header directory is missing: {source_root}"
        )

    expected_paths = {entry.relative_path for entry in entries}
    actual_paths: set[PurePosixPath] = set()
    try:
        for path in source_root.rglob("*"):
            if path.is_symlink():
                raise PsyqSdkHeaderError(
                    f"SDK source header tree must be verbatim, not symlinked: {path}"
                )
            if path.is_file():
                actual_paths.add(PurePosixPath(path.relative_to(source_root).as_posix()))
    except OSError as exc:
        raise PsyqSdkHeaderError(
            f"cannot inventory Psy-Q 4.6 header directory {source_root}: {exc}"
        ) from exc

    missing = sorted(expected_paths - actual_paths, key=PurePosixPath.as_posix)
    extra = sorted(actual_paths - expected_paths, key=PurePosixPath.as_posix)
    if missing or extra:
        details: list[str] = []
        if missing:
            details.append(
                "missing " + ", ".join(path.as_posix() for path in missing)
            )
        if extra:
            details.append("unexpected " + ", ".join(path.as_posix() for path in extra))
        raise PsyqSdkHeaderError(
            f"Psy-Q 4.6 header inventory differs from {manifest_path}: "
            + "; ".join(details)
        )

    headers: list[Path] = []
    for entry in entries:
        header = source_root.joinpath(*entry.relative_path.parts)
        actual_digest = _sha256(header)
        if actual_digest != entry.digest:
            raise PsyqSdkHeaderError(
                f"Psy-Q 4.6 header checksum mismatch for "
                f"{entry.relative_path.as_posix()}: expected {entry.digest}, "
                f"got {actual_digest}"
            )
        headers.append(header.resolve())
    return entries, tuple(headers)


def _manifest_digest(manifest_path: Path) -> str:
    return _sha256(manifest_path)


def _stamp_text(manifest_path: Path) -> str:
    return f"{VIEW_STAMP_MAGIC}\nmanifest-sha256 {_manifest_digest(manifest_path)}\n"


def _alias_sources(
    entries: tuple[ManifestEntry, ...], source_root: Path
) -> dict[PurePosixPath, Path]:
    aliases: dict[PurePosixPath, Path] = {}
    for entry in entries:
        source = source_root.joinpath(*entry.relative_path.parts).resolve()
        aliases[entry.relative_path] = source
        aliases[PurePosixPath(entry.relative_path.as_posix().casefold())] = source
    return aliases


def _validate_include_view(
    include_root: Path,
    source_root: Path,
    manifest_path: Path,
    entries: tuple[ManifestEntry, ...],
) -> None:
    if not include_root.is_dir() or include_root.is_symlink():
        raise PsyqSdkHeaderError(
            f"generated Psy-Q include view is missing: {include_root}; "
            "run setup_toolchain.sh"
        )

    stamp = include_root / VIEW_STAMP_NAME
    if stamp.is_symlink() or not stamp.is_file():
        raise PsyqSdkHeaderError(
            f"generated Psy-Q include view has no valid regular-file stamp: "
            f"{stamp}; run setup_toolchain.sh"
        )
    try:
        actual_stamp = stamp.read_text(encoding="utf-8")
    except OSError as exc:
        raise PsyqSdkHeaderError(
            f"generated Psy-Q include view has no valid stamp: {stamp}: {exc}; "
            "run setup_toolchain.sh"
        ) from exc
    expected_stamp = _stamp_text(manifest_path)
    if actual_stamp != expected_stamp:
        raise PsyqSdkHeaderError(
            f"generated Psy-Q include view is stale: {include_root}; "
            "run setup_toolchain.sh"
        )

    aliases = _alias_sources(entries, source_root)
    expected_files = {PurePosixPath(VIEW_STAMP_NAME), *aliases.keys()}
    actual_files: set[PurePosixPath] = set()
    for path in include_root.rglob("*"):
        if path.is_symlink() or path.is_file():
            actual_files.add(PurePosixPath(path.relative_to(include_root).as_posix()))

    extra = sorted(actual_files - expected_files, key=PurePosixPath.as_posix)
    if extra:
        raise PsyqSdkHeaderError(
            "generated Psy-Q include view is inconsistent: unexpected "
            + ", ".join(path.as_posix() for path in extra)
            + "; "
            "run setup_toolchain.sh"
        )

    # Test every spelling directly instead of requiring rglob() to report both
    # directory entries. On a case-insensitive host the original and folded
    # names intentionally resolve through one on-disk symlink.
    for alias, source in aliases.items():
        link = include_root.joinpath(*alias.parts)
        if not link.is_symlink():
            raise PsyqSdkHeaderError(
                f"generated Psy-Q include entry is not a symlink: {link}; "
                "run setup_toolchain.sh"
            )
        try:
            target = link.resolve(strict=True)
        except OSError as exc:
            raise PsyqSdkHeaderError(
                f"generated Psy-Q include symlink is broken: {link}: {exc}; "
                "run setup_toolchain.sh"
            ) from exc
        if target != source:
            raise PsyqSdkHeaderError(
                f"generated Psy-Q include symlink has the wrong target: {link} "
                f"-> {target}, expected {source}; run setup_toolchain.sh"
            )


def checked_sdk_layout(
    *,
    project_root: Path | None = None,
    sdk_root: Path | None = None,
    manifest_path: Path | None = None,
    include_root: Path | None = None,
) -> PsyqSdkLayout:
    """Return the compile layout after read-only source and view validation."""
    (
        resolved_sdk_root,
        source_root,
        resolved_manifest,
        resolved_include_root,
    ) = _layout_paths(
        project_root=project_root,
        sdk_root=sdk_root,
        manifest_path=manifest_path,
        include_root=include_root,
    )
    entries, headers = validate_sdk_headers(source_root, resolved_manifest)
    _validate_include_view(
        resolved_include_root,
        source_root,
        resolved_manifest,
        entries,
    )
    return PsyqSdkLayout(
        resolved_sdk_root,
        source_root,
        resolved_include_root,
        resolved_manifest,
        headers,
    )


def _managed_view(include_root: Path) -> bool:
    stamp = include_root / VIEW_STAMP_NAME
    if stamp.is_symlink() or not stamp.is_file():
        return False
    try:
        first_line = stamp.read_text(encoding="utf-8").splitlines()[0]
    except (OSError, IndexError):
        return False
    return first_line == VIEW_STAMP_MAGIC


def _write_view_stamp(include_root: Path, manifest_path: Path) -> None:
    """Atomically mark an accepted directory as managed by this helper."""
    stamp = include_root / VIEW_STAMP_NAME
    descriptor, temporary_name = tempfile.mkstemp(
        prefix=f".{VIEW_STAMP_NAME}.",
        suffix=".tmp",
        # Keep an interrupted pre-replace temporary outside the view itself.
        # The view then remains empty and is safely accepted by the next
        # --prepare instead of looking like an unmanaged user directory.
        dir=include_root.parent,
        text=True,
    )
    temporary_stamp = Path(temporary_name)
    try:
        with os.fdopen(descriptor, "w", encoding="utf-8") as stream:
            stream.write(_stamp_text(manifest_path))
            stream.flush()
            os.fsync(stream.fileno())
        try:
            os.replace(temporary_stamp, stamp)
        except OSError as exc:
            raise PsyqSdkHeaderError(
                f"cannot establish managed Psy-Q include-view stamp {stamp}: {exc}"
            ) from exc
    finally:
        try:
            temporary_stamp.unlink()
        except FileNotFoundError:
            pass


def prepare_sdk_layout(
    *,
    project_root: Path | None = None,
    sdk_root: Path | None = None,
    manifest_path: Path | None = None,
    include_root: Path | None = None,
) -> PsyqSdkLayout:
    """Validate the SDK source and create or repair its managed symlink view."""
    (
        resolved_sdk_root,
        source_root,
        resolved_manifest,
        resolved_include_root,
    ) = _layout_paths(
        project_root=project_root,
        sdk_root=sdk_root,
        manifest_path=manifest_path,
        include_root=include_root,
    )
    entries, _headers = validate_sdk_headers(source_root, resolved_manifest)

    if resolved_include_root.is_symlink():
        raise PsyqSdkHeaderError(
            f"refusing to replace symlinked include-view root: {resolved_include_root}"
        )
    if resolved_include_root.exists() and not resolved_include_root.is_dir():
        raise PsyqSdkHeaderError(
            f"include-view path is not a directory: {resolved_include_root}"
        )
    if resolved_include_root.is_dir():
        try:
            has_entries = next(resolved_include_root.iterdir(), None) is not None
        except OSError as exc:
            raise PsyqSdkHeaderError(
                f"cannot inspect include-view directory {resolved_include_root}: {exc}"
            ) from exc
        if has_entries and not _managed_view(resolved_include_root):
            raise PsyqSdkHeaderError(
                f"refusing to modify unmanaged include-view directory: "
                f"{resolved_include_root}"
            )
    else:
        resolved_include_root.mkdir(parents=True)

    # Establish ownership before mutating aliases. If setup is interrupted,
    # read-only validation still rejects the incomplete view, while a later
    # --prepare can distinguish and safely repair its own partial directory.
    _write_view_stamp(resolved_include_root, resolved_manifest)

    aliases = _alias_sources(entries, source_root)
    expected_files = {PurePosixPath(VIEW_STAMP_NAME), *aliases.keys()}
    expected_dirs = {
        parent
        for alias in aliases
        for parent in alias.parents
        if parent != PurePosixPath(".")
    }

    # Remove only entries inside a directory carrying our management stamp.
    # This also retires aliases from an older manifest before new links appear.
    for path in sorted(
        resolved_include_root.rglob("*"),
        key=lambda candidate: len(candidate.parts),
        reverse=True,
    ):
        relative = PurePosixPath(path.relative_to(resolved_include_root).as_posix())
        if path.is_symlink() or path.is_file():
            if relative not in expected_files:
                path.unlink()
        elif relative not in expected_dirs:
            try:
                path.rmdir()
            except OSError as exc:
                raise PsyqSdkHeaderError(
                    f"cannot clean managed include-view directory {path}: {exc}"
                ) from exc

    for alias, source in sorted(aliases.items(), key=lambda item: item[0].as_posix()):
        link = resolved_include_root.joinpath(*alias.parts)
        link.parent.mkdir(parents=True, exist_ok=True)
        if link.is_symlink():
            try:
                if link.resolve(strict=True) == source:
                    continue
            except OSError:
                pass
            link.unlink()
        elif link.exists():
            if link.is_dir():
                raise PsyqSdkHeaderError(
                    f"managed include alias unexpectedly names a directory: {link}"
                )
            link.unlink()
        target = os.path.relpath(source, start=link.parent)
        try:
            link.symlink_to(target)
        except OSError as exc:
            raise PsyqSdkHeaderError(
                f"cannot create managed Psy-Q include symlink {link}: {exc}"
            ) from exc

    return checked_sdk_layout(
        project_root=project_root,
        sdk_root=resolved_sdk_root,
        manifest_path=resolved_manifest,
        include_root=resolved_include_root,
    )


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description="validate the required Psy-Q 4.6 SDK header tree"
    )
    mode = parser.add_mutually_exclusive_group()
    mode.add_argument(
        "--check",
        action="store_true",
        help="validate without modifying the SDK tree (default)",
    )
    mode.add_argument(
        "--prepare",
        action="store_true",
        help="create or repair the generated case-folded symlink view",
    )
    parser.add_argument(
        "--sdk-root",
        type=Path,
        default=DEFAULT_SDK_ROOT,
        help="Psy-Q 4.6 SDK directory containing INCLUDE",
    )
    parser.add_argument(
        "--manifest",
        type=Path,
        default=DEFAULT_MANIFEST_PATH,
        help="checked-in SHA-256 header manifest",
    )
    parser.add_argument(
        "--include-root",
        type=Path,
        help="generated include-view directory (default: SDK/.casefold-include)",
    )
    parser.add_argument(
        "--print-include-root",
        action="store_true",
        help="print only the validated generated include-root path",
    )
    return parser


def main(argv: list[str] | None = None) -> int:
    args = build_parser().parse_args(argv)
    try:
        if args.prepare:
            layout = prepare_sdk_layout(
                sdk_root=args.sdk_root,
                manifest_path=args.manifest,
                include_root=args.include_root,
            )
        else:
            layout = checked_sdk_layout(
                sdk_root=args.sdk_root,
                manifest_path=args.manifest,
                include_root=args.include_root,
            )
    except PsyqSdkHeaderError as exc:
        print(f"ERROR: {exc}", file=sys.stderr)
        print(
            "Install the unmodified Psy-Q 4.6 (DTL-S2360) SDK at "
            "toolchain/psyq-4.6-sdk and run bash setup_toolchain.sh.",
            file=sys.stderr,
        )
        return 1

    if args.print_include_root:
        print(layout.include_root)
    else:
        print(
            f"Psy-Q 4.6 headers verified: {len(layout.headers)} files; "
            f"include root {layout.include_root}"
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
