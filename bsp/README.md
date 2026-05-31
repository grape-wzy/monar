# BSP

`bsp/` owns concrete board integration and device-instance selection.

Rules:

- board startup files, linker scripts, pins, clocks, and MCU model selection
  belong here
- future BSP code may register concrete Monar device instances for a board
- BSP code may depend on selected silicon adaptation, but application code
  should consume Monar APIs rather than BSP internals
- vendor-family adaptation logic does not belong here; it belongs under
  `silicon/`

The current `bsp/boards/st/nucleo_f407zg/` directory is the phase-2 reference
board skeleton for STM32F407 build validation.
