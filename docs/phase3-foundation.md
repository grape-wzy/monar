# Phase-3 Foundation

## Runtime capability model

`include/monar/osal.h` exposes capability bits for:

- timebase
- sleep/delay
- critical section
- ISR-context detection
- mutex
- semaphore
- thread
- timer

Phase-3 keeps the bare-metal backend intentionally small. It reports only
`MN_OSAL_CAP_CRITICAL_SECTION`.

Capability queries are deterministic before and after `mn_osal_init()`. Future
framework or driver code should treat missing required capabilities as
deterministic `-MN_ENOTSUP` feature failures instead of assuming the backend
implements every service.

## Lifecycle

`mn_system_init()` is the preferred application entry point.

It performs:

1. runtime backend initialization
2. silicon-layer initialization
3. BSP registration of board-owned resources
4. ready state transition for application code

The lifecycle state is queryable through `mn_system_get_stage()` and
`mn_system_is_initialized()`.

## BSP resource declaration

Phase-3 keeps resource declaration static-allocation-friendly:

- descriptors are static
- registration happens through `mn_device_register()`
- `resource_key` remains the single-ownership guard

The host reference BSP demonstrates this with `status0`, a minimal read-only
endpoint registered during BSP init.

## Silicon boundary

Silicon code stays under `silicon/` and exposes only Monar-private hooks into
framework core. It does not publish vendor or SDK types through
`include/monar/`.

Current scaffolds:

- host silicon stub
- STM32F4 silicon stub
