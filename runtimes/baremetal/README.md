# Baremetal Runtime

This backend is the first-stage minimal runtime placeholder for Monar.

Current behavior:

- `mn_osal_init()` succeeds
- critical sections are implemented as no-op stubs
- ISR detection always reports `false`

This is intentional for the bootstrap milestone. Future milestones can replace
the no-op critical section behavior with target-specific interrupt save/restore
logic without changing the public Monar API.
