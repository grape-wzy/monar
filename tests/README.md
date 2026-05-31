# Tests

Host-side and future target-side tests belong here.

The current Phase-3 baseline keeps host tests under `tests/host/` and does not
compile them into STM32 target builds.

The repository-level CMake and Make wrappers automate the host flow through:

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
tests/host/test_lifecycle.c
tests/host/test_device.c
tests/host/test_main.c
```

Current coverage includes:

- OSAL init/runtime reporting
- runtime capability reporting
- unsupported runtime-feature handling
- lifecycle stage behavior
- BSP-backed reference device registration
- device registry reset behavior
- stale-handle prevention on failed register/find/open paths
- open-mode semantics
- unsupported operation handling

If editor tooling needs a repository-root copy, use the explicit sync target:

```text
make host-sync-compile-commands
```
