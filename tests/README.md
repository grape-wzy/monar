# Tests

Host-side and future target-side tests belong here.

The current bootstrap milestone adds a small host test at `tests/host/`.

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
syncs `compile_commands.json` to the repository root for editor tooling.
