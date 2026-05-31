# Build Guide

## Supported validation commands

```text
make test
cmake --build --preset host-debug
ctest --preset host-debug
cmake --build --preset stm32f407-debug
```

## Presets

Committed presets stay portable:

- `host-debug`
- `stm32f407-debug`

Machine-local compiler paths should go in `CMakeUserPresets.json`, which is
ignored by git. A starter template is provided in
`CMakeUserPresets.example.json`.

## Windows local setup

Host build requirements:

- `cmake`
- `ctest`
- `gcc`
- `mingw32-make`

STM32F407 build requirements:

- everything above
- Arm GNU Toolchain with `arm-none-eabi-gcc`
- either `arm-none-eabi-gcc` on `PATH`, or `MONAR_ARM_GCC_ROOT` set

The repository `make.cmd` wrapper exists so `make test` can work in a normal
Windows terminal when `mingw32-make` is available on `PATH`.

## Generated files

Normal builds keep generated files under `build/`.

- `compile_commands.json` stays in the selected build directory by default
- if a repository-root copy is needed for tooling, use an explicit sync target
  rather than making source-tree mutation part of every build

## CI expectation

Pull requests should at least validate host build and host tests
automatically. The repository CI also keeps a separate STM32F407 build job so
the embedded output path stays visible.
