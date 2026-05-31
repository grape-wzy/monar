# Silicon

`silicon/` owns MCU vendor, family, and SDK adaptation layers.

Rules:

- vendor SDK or HAL wrappers belong here, not under `bsp/`
- silicon code should adapt MCU-family capabilities into Monar driver-facing
  primitives
- public Monar headers must not expose vendor HAL handles, registers, IRQ
  internals, or silicon-private objects
- board-level pin maps, concrete clocks, startup files, linker scripts, and
  device-instance registration do not belong here

A future STM32 HAL adapter belongs under `silicon/st/...`, while concrete board
selection stays under `bsp/`.
