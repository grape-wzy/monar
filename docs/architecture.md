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

## Current lifecycle convention

Phase-2 keeps initialization simple and explicit:

```text
runtime / OSAL init
framework core init point (currently implicit after OSAL init)
BSP board init and device registration
application loop / entry logic
```

The current minimal example reflects that order by calling `mn_osal_init()`
before `mn_board_init()`.

Monar does not add a large init framework in phase 2. Future milestones may add
a dedicated framework-core init hook or linker-section registration model, but
that is not required yet.
