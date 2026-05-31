# Monar

Monar is an embedded framework whose goal is:

> Free your application from MCU and RTOS dependencies.

This repository, `grape-wzy/monar`, stores the open-source implementation code.
The private design guidance, architecture notes, and review checklists live in
`grape-wzy/monar_desing_guidance`.

## First-milestone status

This branch bootstraps the first minimal framework skeleton:

- public headers under `include/monar/`
- platform-independent core under `src/`
- a minimal baremetal runtime backend under `runtimes/baremetal/`
- host-side smoke tests under `tests/host/`

The current scope intentionally stays small. It does not implement STM32
silicon adapters, RTOS backends, Modbus, WS2812B, or complete SPI/I2C/UART
drivers yet.

## Phase-2 foundation

Phase 2 focuses on engineering foundation work before large driver features:

- CI validation
- runtime backend interface clarification
- runtime / silicon / BSP boundary documentation
- minimal lifecycle conventions
- host test structure cleanup
- documentation for future programmers

## Repository layout

```text
include/monar/  public Monar headers
src/            platform-independent framework core
runtimes/       runtime backends
silicon/        silicon and vendor SDK adaptation
bsp/            board-level integration
tests/          host-side and future target-side tests
examples/       user-facing examples
tools/          optional development helpers
```

The CMake build is organized the same way: each buildable directory owns its
own `CMakeLists.txt`, and the repository-root `CMakeLists.txt` only coordinates
options and conditional subdirectories.

Additional architecture documentation:

- `docs/architecture.md`
- `docs/build.md`
- `docs/runtime-silicon-bsp.md`

## Build system

The repository now includes:

- a root `CMakeLists.txt` that coordinates modular subdirectory builds
- per-directory `CMakeLists.txt` files under `src/`, `runtimes/`, `bsp/`,
  `examples/`, and `tests/`
- portable `CMakePresets.json` for host and STM32F407 configure/build presets
- `CMakeUserPresets.example.json` as a local-machine override template
- a root `Makefile` as a thin wrapper around CMake commands
- `.vscode/` configuration for CMake Tools, IntelliSense, and navigation

The key root-level CMake cache variables are:

```text
MONAR_PLATFORM       host | stm32f407
MONAR_RUNTIME        baremetal
MONAR_BUILD_TESTS    ON | OFF
MONAR_BUILD_EXAMPLES ON | OFF
```

Host and STM32 builds are intentionally isolated:

- the host preset builds `src/`, the selected runtime backend, and host tests
- the STM32 preset builds `src/`, the selected runtime backend, the selected
  BSP, and the selected example target
- host tests are not compiled into the STM32 target build
- STM32 startup/BSP/example files are not compiled into the host test build

### Host build

```text
make test
make host-build
make host-test
```

`make test` is the short repository-root entry for the current host smoke test.
It configures `build/host-debug/`, builds the
host test with private `src/internal/` headers on the include path, and runs it.
On Windows, the repository also provides `make.cmd` as a thin wrapper around
`mingw32-make`, so `make test` works in the VSCode integrated terminal when a
local MinGW toolchain is available.

### STM32 example build

```text
make stm32-build
```

This configures `build/stm32f407-debug/` and produces:

- `monar_stm32f407_minimal.elf`
- `monar_stm32f407_minimal.bin`
- `monar_stm32f407_minimal.hex`

under the STM32 build directory.

The STM32 target is currently a BSP/startup skeleton for `nucleo_f407zg`. It is
meant to validate the embedded build/output path without pulling in STM32 HAL
or silicon adaptation yet.

## Local toolchain setup

The committed `CMakePresets.json` is intentionally portable and does not embed
machine-local compiler paths.

Recommended options:

- put `cmake`, `gcc`, and `mingw32-make` on `PATH` for host builds
- put `arm-none-eabi-gcc` on `PATH`, or set `MONAR_ARM_GCC_ROOT`, for STM32
  builds
- copy `CMakeUserPresets.example.json` to `CMakeUserPresets.json` and adjust it
  for your local Windows installation if you need pinned local paths or PATH
  overrides

The build directory remains the default location for generated
`compile_commands.json`. If you want a repository-root copy for tooling, use an
explicit command such as:

```text
make host-sync-compile-commands
```

## VSCode integration

The committed `.vscode/` settings are set up so that:

- CMake Tools uses the repository presets directly
- the repository root is on terminal `PATH`, so `make.cmd` is directly usable
- `ms-vscode.cmake-tools` remains the cpptools configuration provider
- `clangd` uses the host build directory compile database by default
- switching the active CMake preset in VSCode keeps host and STM32 builds
  separate

If the Arm GNU toolchain is not already on your system `PATH`, set
`MONAR_ARM_GCC_ROOT` before configuring the `stm32f407-debug` preset.

## Lifecycle convention

The current intended initialization order is:

```text
runtime / OSAL init
framework core init point (currently implicit)
BSP board init and device registration
application logic
```

The current minimal example keeps `main()` simple and follows this sequence by
calling `mn_osal_init()` before `mn_board_init()`.

## Current design decisions

- public symbols use the `mn_` / `MN_` prefixes
- public device handles are opaque
- the framework core does not depend directly on RTOS or vendor HAL headers
- public APIs must not expose RTOS, HAL, vendor SDK, register, IRQ-internal, or
  silicon-private objects
- `mn_osal_init()` is expected before device registry mutation and device use
- device registration is static-allocation-friendly and bounded by
  `MN_CFG_DEVICE_REGISTRY_MAX`
- registered descriptors are retained by reference, so `name`, `ops`,
  `resource_key`, and `driver_data` must stay valid for the lifetime of the
  device
- duplicate `resource_key` registration is rejected to avoid ambiguous
  device/bus/carrier ownership of the same hardware resource
- `runtimes/`, `silicon/`, and `bsp/` have distinct responsibilities and must
  not be collapsed into deprecated `ports/` or `chips/` source trees

## CI

GitHub Actions workflows live under `.github/workflows/`.

The current CI foundation validates:

- `make test`
- `cmake --build --preset host-debug`
- `ctest --preset host-debug`

and also keeps a separate STM32F407 build job for:

- `cmake --build --preset stm32f407-debug`

## Host validation

The host tests live under `tests/host/` and currently cover:

- deterministic Monar error codes
- minimal device registry registration rules
- bare-metal OSAL init/runtime reporting
- `mn_device_open` flag, capability, and stale-handle guards
- internal-only registry reset between host test cases

The current runtime backend is a minimal baremetal placeholder. Unsupported
runtime services are intentionally left for later milestones.
