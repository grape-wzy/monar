#include <assert.h>
#include <string.h>

#include <monar/monar.h>

#include "mn_init_internal.h"

void mn_test_osal(void)
{
    mn_u32_t time_ms;

    mn_system_reset_for_test();
    assert(mn_osal_is_initialized() == false);
    assert(mn_osal_init() == MN_EOK);
    assert(mn_osal_is_initialized() == true);
    assert(strcmp(mn_osal_runtime_name(), "baremetal") == 0);
    assert(mn_osal_is_in_isr() == false);
    assert(mn_osal_has_capability(MN_OSAL_CAP_CRITICAL_SECTION) == true);
    assert(mn_osal_has_capability(MN_OSAL_CAP_SLEEP) == false);
    assert((mn_osal_get_capabilities() & MN_OSAL_CAP_TIMEBASE) == 0u);
    assert(mn_osal_get_time_ms(&time_ms) == -MN_ENOTSUP);
    assert(time_ms == 0u);
    assert(mn_osal_sleep_ms(1u) == -MN_ENOTSUP);
}
