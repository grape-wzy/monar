# Runtimes

`runtimes/` owns Monar runtime backends, including bare-metal and future OS or
RTOS integrations.

Rules:

- public runtime-facing APIs stay under `include/monar/osal.h`
- framework core calls the private backend contract from
  `src/internal/mn_osal_backend.h`
- runtime backends decide critical-section mechanics, ISR detection, and any
  future scheduler interaction
- framework core must not duplicate context-switch interrupt handling or expose
  RTOS-private objects through public Monar APIs

The phase-2 baseline keeps only the minimal bare-metal backend buildable.
