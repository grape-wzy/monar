# Runtimes

`runtimes/` owns Monar runtime backends, including bare-metal and future OS or
RTOS integrations.

Rules:

- public runtime-facing APIs stay under `include/monar/osal.h`
- framework core calls the private backend contract from
  `src/internal/mn_osal_backend.h`
- runtime backends decide capability reporting, critical-section mechanics,
  ISR detection, and any future scheduler interaction
- framework core must not duplicate context-switch interrupt handling or expose
  RTOS-private objects through public Monar APIs

Phase-3 runtime capability bits currently cover:

- timebase
- sleep/delay
- critical section
- ISR-context detection
- mutex
- semaphore
- thread
- timer

Capability queries are backend metadata. They are safe to call before
`mn_osal_init()` and after init.

The current bare-metal backend intentionally reports only
`MN_OSAL_CAP_CRITICAL_SECTION`.

Future framework or driver code must check capabilities before assuming a
runtime service exists. If a required capability is missing, the higher-level
feature should fail with a deterministic Monar error such as `-MN_ENOTSUP`
instead of crashing or guessing.
