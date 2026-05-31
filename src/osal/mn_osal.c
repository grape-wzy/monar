#include <monar/errno.h>
#include <monar/osal.h>

#include "mn_osal_backend.h"

static bool g_mn_osal_initialized;

mn_status_t mn_osal_init(void)
{
    const mn_runtime_ops_t *runtime;
    mn_status_t ret;

    if (g_mn_osal_initialized) {
        return MN_EOK;
    }

    runtime = mn_runtime_get_ops();
    if (runtime == NULL || runtime->init == NULL) {
        return -MN_ENOSYS;
    }

    ret = runtime->init();
    if (ret != MN_EOK) {
        return ret;
    }

    g_mn_osal_initialized = true;
    return MN_EOK;
}

bool mn_osal_is_initialized(void)
{
    return g_mn_osal_initialized;
}

const char *mn_osal_runtime_name(void)
{
    const mn_runtime_ops_t *runtime;

    runtime = mn_runtime_get_ops();
    if (runtime == NULL || runtime->name == NULL) {
        return "unknown";
    }

    return runtime->name;
}

bool mn_osal_is_in_isr(void)
{
    const mn_runtime_ops_t *runtime;

    runtime = mn_runtime_get_ops();
    if (runtime == NULL || runtime->is_in_isr == NULL) {
        return false;
    }

    return runtime->is_in_isr();
}

mn_osal_critical_state_t mn_osal_critical_enter(void)
{
    const mn_runtime_ops_t *runtime;

    runtime = mn_runtime_get_ops();
    if (runtime == NULL || runtime->critical_enter == NULL) {
        return 0;
    }

    return runtime->critical_enter();
}

void mn_osal_critical_exit(mn_osal_critical_state_t state)
{
    const mn_runtime_ops_t *runtime;

    runtime = mn_runtime_get_ops();
    if (runtime == NULL || runtime->critical_exit == NULL) {
        return;
    }

    runtime->critical_exit(state);
}
