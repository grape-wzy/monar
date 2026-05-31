# NUCLEO-F407ZG BSP Skeleton

This board directory provides a minimal linker script, startup file, and board
initialization stub so the repository can produce STM32F407 `elf`, `bin`, and
`hex` artifacts through CMake.

It is intentionally small:

- no STM32 HAL integration
- no full silicon adapter yet, only a family-level scaffold under `silicon/`
- no peripheral driver registration yet

The purpose is to establish a usable embedded build/output path for future
Monar stages.
