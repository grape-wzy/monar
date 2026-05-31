# BSP

`bsp/` owns concrete board integration and device-instance selection.

Rules:

- board startup files, linker scripts, pins, clocks, and MCU model selection
  belong here
- BSP code may register concrete Monar device instances for a board
- BSP code may depend on selected silicon adaptation, but application code
  should consume Monar APIs rather than BSP internals
- vendor-family adaptation logic does not belong here; it belongs under
  `silicon/`
- each concrete board resource must keep a single owner through `resource_key`

Current Phase-3 reference content:

- `bsp/boards/host/reference/` provides the first minimal device-registration
  path by registering `status0`
- `bsp/boards/st/nucleo_f407zg/` remains the STM32F407 board scaffold for
  embedded configure/build validation
