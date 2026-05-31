# Tests

Host-side and future target-side tests belong here.

The current phase-2 baseline keeps host tests under `tests/host/` and does not
compile them into STM32 target builds.

Example host build command:

```text
cc -std=c11 -Wall -Wextra -Iinclude -Isrc/internal \
  src/osal/mn_osal.c \
  src/device/mn_device.c \
  src/device/mn_device_registry.c \
  runtimes/baremetal/mn_runtime_baremetal.c \
  tests/host/test_main.c \
  -o monar_host_test
```

The repository-level CMake and Make wrappers now automate this flow through:

```text
make test
make host-build
make host-test
```

`make test` is the repository-root smoke-test entry. It currently runs a
single-process host test, with an internal-only registry reset helper used to
keep test cases independent without exposing any public reset API.

`make host-test` runs the registered CTest target, and the active build also
leaves `compile_commands.json` in the host build directory by default.

The host tests are now split by module:

```text
tests/host/test_osal.c
tests/host/test_device.c
tests/host/test_main.c
```

Current coverage includes:

- OSAL init/runtime reporting
- device registry reset behavior
- stale-handle prevention on failed register/find/open paths
- open-mode semantics
- unsupported operation handling

If editor tooling needs a repository-root copy, use the explicit sync target:

```text
make host-sync-compile-commands
```
