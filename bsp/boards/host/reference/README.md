# Host Reference BSP

This board provides the first minimal reference device path for Phase-3.

- `status0` is a statically registered read-only endpoint.
- The device is registered from BSP init through the normal Monar registry.
- `resource_key` remains unique and continues to enforce single ownership.

The device is intentionally simple so Phase-3 can validate lifecycle and
registration flow without introducing a real silicon driver.
