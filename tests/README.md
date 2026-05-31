# Tests

Host-side and future target-side tests belong here.

The current bootstrap milestone adds a small host test at `tests/host/`.

Example host build command:

```text
cc -std=c11 -Wall -Wextra -Iinclude -Isrc/internal \
  src/osal/mn_osal.c \
  src/device/mn_device.c \
  src/device/mn_device_registry.c \
  runtimes/baremetal/mn_runtime_baremetal.c \
  tests/host/test_main.c \
  -o monar_host_test
```
