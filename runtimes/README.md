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
- mutex
- semaphore
- thread
- timer

The current bare-metal backend intentionally reports only
`MN_OSAL_CAP_CRITICAL_SECTION`. Unsupported services return `-MN_ENOTSUP`.
