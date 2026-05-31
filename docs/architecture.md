# Monar Architecture

## Mission

Monar is an embedded framework whose goal is to keep application code away from
direct MCU, RTOS, and vendor SDK dependencies.

## Directory responsibilities

```text
include/monar/  public Monar APIs only
src/            platform-independent framework core
runtimes/       bare-metal, OS, and RTOS runtime backends
silicon/        MCU vendor/family/SDK adaptation
bsp/            concrete board resources and device-instance selection
tests/          host-side tests and future target-side validation
examples/       minimal application examples
tools/          optional development helpers
```

Rules:

- public APIs stay under `include/monar/`
- internal headers stay private under `src/internal/` or other private paths
- `runtimes/`, `silicon/`, and `bsp/` must be treated as different layers
- formal source directories named `ports/` or `chips/` must not be introduced

## Public API rule

Public Monar APIs must not expose:

- RTOS task, semaphore, queue, or scheduler types
- vendor HAL or SDK handle types
- MCU registers or raw peripheral base addresses
- IRQ internal objects
- silicon-private adaptation objects

## OSAL capability contract

`include/monar/osal.h` exposes a small capability bitset so runtime-dependent
framework code can discover what the selected backend supports without touching
RTOS-private types.

Rules:

- capability queries are deterministic before and after `mn_osal_init()`
- capability queries report backend metadata only; they do not initialize the
  runtime
- missing capabilities must be handled by higher-level code with deterministic
  Monar errors such as `-MN_ENOTSUP`
- context-switch-related interrupt behavior remains a runtime-backend concern,
  not generic framework-core code

## Current lifecycle convention

Phase-3 keeps initialization simple but moves the ordering behind one Monar
entry point:

```text
mn_system_init()
  -> runtime / OSAL init
  -> silicon init
  -> BSP device declaration and registration
  -> application loop / entry logic
```

This keeps application entry code independent from BSP-private init calls while
preserving explicit boundaries between runtime, silicon, and BSP layers.
