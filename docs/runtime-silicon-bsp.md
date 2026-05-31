# Runtime, Silicon, and BSP Boundary

## Runtime

`runtimes/` owns the selected backend behind `include/monar/osal.h`.

Responsibilities:

- backend initialization
- capability reporting
- ISR-context detection
- critical-section entry and exit
- future timebase, delay, timer, and RTOS primitive adaptation

Non-responsibilities:

- board startup files or linker scripts
- pin maps or enabled peripheral selection
- vendor HAL peripheral adaptation

Capability query rules:

- `mn_osal_get_capabilities()` and `mn_osal_has_capability()` report backend
  metadata and are deterministic before and after OSAL init
- capability queries do not expose RTOS-native objects
- future framework or driver code should turn missing required capabilities into
  deterministic Monar errors such as `-MN_ENOTSUP`

## Silicon

`silicon/` owns MCU vendor, family, and SDK adaptation.

Responsibilities:

- map selected MCU-family capability into Monar-private hooks
- host or MCU-family bring-up steps that are not board selection
- future vendor HAL or LL wrapper code

Non-responsibilities:

- public Monar API surface
- linker scripts
- board-specific pin or device-instance declaration

Current scaffolds:

- `silicon/host/`
- `silicon/st/stm32f4/`

## BSP

`bsp/` owns concrete board resources and Monar device-instance declaration.

Responsibilities:

- startup file selection
- linker script selection
- board-specific compile definitions
- static Monar device descriptors and registration
- stable `resource_key` ownership for board resources

Resource declaration rules:

- one hardware resource must map to one `resource_key`
- a board must not register the same hardware instance as multiple independent
  device/bus/carrier owners
- BSP code should register concrete devices through `mn_device_register()`
  instead of bypassing the registry

Current scaffolds:

- `bsp/boards/host/reference/` registers `status0`
- `bsp/boards/st/nucleo_f407zg/` provides STM32F407 startup and board skeleton

## Lifecycle

Phase-3 introduces a minimal framework lifecycle entry point:

```text
mn_system_init()
  -> mn_osal_init()
  -> mn_silicon_init()
  -> mn_bsp_init()
  -> registry-backed device use
```

Application code should normally call `mn_system_init()` and then consume
devices through Monar APIs.
