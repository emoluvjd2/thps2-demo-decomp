# THPS2 Demo Decompilation

Source-only work-in-progress reconstruction of the PlayStation THPS2 demo.

This minimal public snapshot contains the recovered C/C++ under `src/` and the
scripts needed to compile-check every translation unit. It is an unofficial
research project and is not affiliated with or endorsed by Tony Hawk,
Neversoft, Activision, Sony, or any other rights holder.

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
bash build.sh
```

`setup_toolchain.sh` validates the user-supplied SDK headers and downloads the
separately distributed GCC compiler. It does not download Sony software or
game files.

`build.sh` compiles all recovered translation units as a source-integrity
check. It does not link a game executable or perform binary matching; those
workflows require proprietary and target-derived inputs that are deliberately
outside this repository.

## Evidence and rights

The reconstruction is based on permitted binary/debug evidence and
reproducible compiler experiments. Unlicensed original source, editor/tag
databases, and development-project files are not accepted as evidence.

This repository has no blanket open-source license. See
[`LICENSE_SCOPE.md`](LICENSE_SCOPE.md) for the copyright and third-party-rights
scope.
