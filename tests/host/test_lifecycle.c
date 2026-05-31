#include <assert.h>
#include <string.h>

#include <monar/monar.h>

#include "mn_init_internal.h"

void mn_test_lifecycle(void)
{
    char buffer[16];
    size_t count;
    mn_device_t device;

    mn_system_reset_for_test();

    assert(mn_system_get_stage() == MN_INIT_STAGE_RESET);
    assert(mn_system_is_initialized() == false);
    assert(mn_device_find("status0", &device) == -MN_EPERM);

    assert(mn_system_init() == MN_EOK);
    assert(mn_system_is_initialized() == true);
    assert(mn_system_get_stage() == MN_INIT_STAGE_READY);
    assert(mn_device_open("status0", MN_DEVICE_OPEN_READ, &device) == MN_EOK);
    assert(strcmp(mn_device_get_name(device), "status0") == 0);
    assert(mn_device_read(device, buffer, sizeof(buffer), &count) == MN_EOK);
    assert(count == strlen("host-ready"));
    assert(memcmp(buffer, "host-ready", count) == 0);
    assert(mn_device_close(device) == MN_EOK);
    assert(mn_system_init() == MN_EOK);
}
