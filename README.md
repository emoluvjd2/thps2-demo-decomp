# THPS2 Demo Decompilation

Source-only work-in-progress reconstruction of the PlayStation THPS2 demo.

This minimal public snapshot contains the recovered C/C++ under `src/` and the
scripts needed to compile-check every translation unit. It is an unofficial
research project and is not affiliated with or endorsed by Tony Hawk,
Neversoft, Activision, Sony, or any other rights holder.

## Status

**1,637 of 1,646 functions reproduce the original object code exactly
(99.45%).** "Exactly" means every instruction agrees, relocations included —
not merely equivalent behaviour.

The nine remaining are `StartGrind` (PHYSICS), `Front_Display` (FRONT),
`Redbook_XAPlay` (REDBOOK), `Bit_Display` (BIT), `CreateZonePacket`
(LEVELGEN), `setData` (IMAGE), `ReactToCollision` (CAMERA),
`M3dColij_LineToThisItemCached` (M3DCOLIJ) and `Wireframe_ProcessPolys` (M3D).

The measurement itself is not reproducible from this repository: verifying it
requires the target disassembly and debug symbols, which are target-derived and
deliberately excluded. What is published is the source those results were
produced from.

### Progress reporting

`build/report.json` is a [decomp.dev](https://decomp.dev) progress report in
objdiff's report schema, uploaded by `.github/workflows/progress.yml` as the
`SLUS_900.86_report` artifact.

It is committed rather than built in CI, because this repository deliberately
cannot build: producing either the target or the base objects needs the
original executable, the Sony PSY-Q SDK headers and ASPSX/PSYLINK, none of
which is redistributable. The report is generated from the project's canonical
measurement gate — which refuses to publish against stale or incomplete
artifacts — and is refreshed in the same commit as the source it describes. CI
validates its schema, internal consistency and unit totals before publishing
it.

## Not included

This repository intentionally excludes:

- the original executable, game assets, `MAIN.SYM`, and `MAIN_STRUCTS.h`;
- Psy-Q SDK files, ASPSX, PSYLINK, and other Sony software;
- downloaded compiler or assembler binaries;
- target disassembly, generated objects, link products, and matcher caches;
- recovered or leaked original source and development-project files; and
- private work logs, experiments, IDE state, and local paths.

Use only external inputs that you are legally authorized to possess.

## Compile check

The supported environment is Linux or WSL with Bash, Python 3, GNU `cpp`,
`wget`, and `tar`. Supply your licensed Psy-Q 4.6 (DTL-S2360) SDK headers at:

```text
toolchain/psyq-4.6-sdk/INCLUDE/
```

Then run:

```bash
bash setup_toolchain.sh
bash test_all_tu.sh
```

`setup_toolchain.sh` validates the user-supplied SDK headers and downloads the
separately distributed GCC compiler. It does not download Sony software or
game files.

`test_all_tu.sh` compiles every recovered translation unit as a
source-integrity check. It does not link a game executable or perform binary
matching; those workflows require proprietary and target-derived inputs that
are deliberately outside this repository.

`build.sh` is included for reference only. It drives the full assemble, link
and match pipeline and depends on tooling and target data this snapshot
excludes, so it will not run here.

### The compiler is patched, deliberately

The compiler `setup_toolchain.sh` downloads is built from egcs 1.1.2, which
added a guard in `jump.c:delete_computation` disabling recursive jump deletion
after the second scheduler. egcs 1.1.1 had no such guard, and the target
requires the earlier behaviour. `toolchain/patch_egcs_1_1_1_jump.py`
reproduces that reversion against pinned SHA-256 hashes and refuses an unknown
binary; `--check` verifies without modifying. Both `setup_toolchain.sh` and
`test_all_tu.sh` run that check and fail closed, so a stock or tampered
compiler is rejected rather than quietly producing different code.

## Evidence and rights

The reconstruction is based on permitted binary/debug evidence and
reproducible compiler experiments. Unlicensed original source, editor/tag
databases, and development-project files are not accepted as evidence.

This repository has no blanket open-source license. See
[`LICENSE_SCOPE.md`](LICENSE_SCOPE.md) for the copyright and third-party-rights
scope.
