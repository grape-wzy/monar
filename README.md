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

## Current design decisions

- public symbols use the `mn_` / `MN_` prefixes
- public device handles are opaque
- the framework core does not depend directly on RTOS or vendor HAL headers
- `mn_osal_init()` is expected before device registry mutation and device use
- device registration is static-allocation-friendly and bounded by
  `MN_CFG_DEVICE_REGISTRY_MAX`
- duplicate `resource_key` registration is rejected to avoid ambiguous
  device/bus/carrier ownership of the same hardware resource

## Host validation

The initial tests live in `tests/host/test_main.c` and cover:

- deterministic Monar error codes
- minimal device registry registration rules
- basic `mn_device_open` / `close` / `read` guards

The current runtime backend is a minimal baremetal placeholder. Unsupported
runtime services are intentionally left for later milestones.
