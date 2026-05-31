#include <monar/errno.h>
#include <monar/init.h>
#include <monar/osal.h>

#include "mn_device_internal.h"
#include "mn_init_internal.h"
#include "mn_osal_internal.h"
#include "mn_platform_hooks.h"

static mn_init_stage_t g_mn_init_stage;

mn_status_t mn_system_init(void)
{
    mn_status_t ret;

    if (g_mn_init_stage == MN_INIT_STAGE_READY) {
        return MN_EOK;
    }

    ret = mn_osal_init();
    if (ret != MN_EOK) {
        return ret;
    }
    g_mn_init_stage = MN_INIT_STAGE_RUNTIME_READY;

    ret = mn_silicon_init();
    if (ret != MN_EOK) {
        return ret;
    }
    g_mn_init_stage = MN_INIT_STAGE_SILICON_READY;

    ret = mn_bsp_init();
    if (ret != MN_EOK) {
        return ret;
    }
    g_mn_init_stage = MN_INIT_STAGE_BSP_READY;

    g_mn_init_stage = MN_INIT_STAGE_READY;
    return MN_EOK;
}

bool mn_system_is_initialized(void)
{
    return g_mn_init_stage == MN_INIT_STAGE_READY;
}

mn_init_stage_t mn_system_get_stage(void)
{
    return g_mn_init_stage;
}

void mn_system_reset_for_test(void)
{
    mn_device_registry_reset_for_test();
    mn_osal_reset_for_test();
    g_mn_init_stage = MN_INIT_STAGE_RESET;
}
