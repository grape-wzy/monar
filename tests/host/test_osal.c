#include <assert.h>
#include <string.h>

#include <monar/monar.h>

void mn_test_osal(void)
{
    assert(mn_osal_is_initialized() == false);
    assert(mn_osal_init() == MN_EOK);
    assert(mn_osal_is_initialized() == true);
    assert(strcmp(mn_osal_runtime_name(), "baremetal") == 0);
    assert(mn_osal_is_in_isr() == false);
}
