# Runtime, Silicon, and BSP Boundary

## Runtime

`runtimes/` provides the selected runtime backend.

Responsibilities:

- OSAL backend init
- critical-section entry and exit
- ISR-context detection
- future scheduler interaction for RTOS backends

Non-responsibilities:

- board pin selection
- linker scripts
- vendor HAL peripheral adaptation

## Silicon

`silicon/` adapts MCU vendor, family, and SDK capabilities into Monar-facing
driver primitives.

Examples of future silicon responsibilities:

- STM32 HAL or LL wrapper code
- family-specific interrupt helpers
- SDK-specific peripheral primitive adaptation

A future STM32 HAL adapter belongs under `silicon/st/...`.

## BSP

`bsp/` selects concrete board resources and device instances.

Responsibilities:

- startup file selection
- linker script selection
- pin and clock configuration
- board-level init order
- registration of concrete Monar device instances for the selected board

Examples:

- `bsp/boards/st/nucleo_f407zg/` is the current STM32F407 board skeleton

## Application boundary

Application code should depend on Monar APIs. It should not call into
silicon-private or BSP-private objects as its stable interface.
