#include <assert.h>
#include <string.h>

#include <monar/monar.h>

#include "mn_init_internal.h"

void mn_test_osal(void)
{
    mn_osal_capability_t capabilities;

    mn_system_reset_for_test();
    assert(mn_osal_is_initialized() == false);
    capabilities = mn_osal_get_capabilities();
    assert(capabilities == MN_OSAL_CAP_CRITICAL_SECTION);
    assert(mn_osal_has_capability(MN_OSAL_CAP_CRITICAL_SECTION) == true);
    assert(mn_osal_has_capability(MN_OSAL_CAP_ISR_CONTEXT) == false);
    assert(mn_osal_has_capability(MN_OSAL_CAP_SLEEP) == false);
    assert(mn_osal_init() == MN_EOK);
    assert(mn_osal_is_initialized() == true);
    assert(strcmp(mn_osal_runtime_name(), "baremetal") == 0);
    assert(mn_osal_is_in_isr() == false);
    capabilities = mn_osal_get_capabilities();
    assert(capabilities == MN_OSAL_CAP_CRITICAL_SECTION);
    assert((capabilities & MN_OSAL_CAP_TIMEBASE) == 0u);
    assert((capabilities & MN_OSAL_CAP_ISR_CONTEXT) == 0u);
}
