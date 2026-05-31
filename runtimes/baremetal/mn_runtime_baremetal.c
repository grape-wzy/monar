#include <stdbool.h>

#include <monar/errno.h>

#include "mn_osal_backend.h"

static mn_status_t mn_runtime_baremetal_init(void)
{
    return MN_EOK;
}

static bool mn_runtime_baremetal_is_in_isr(void)
{
    return false;
}

static mn_status_t mn_runtime_baremetal_get_time_ms(mn_u32_t *out_time_ms)
{
    if (out_time_ms == NULL) {
        return -MN_EINVAL;
    }

    *out_time_ms = 0u;
    return -MN_ENOTSUP;
}

static mn_status_t mn_runtime_baremetal_sleep_ms(mn_u32_t duration_ms)
{
    (void)duration_ms;

    return -MN_ENOTSUP;
}

static mn_osal_critical_state_t mn_runtime_baremetal_critical_enter(void)
{
    return 0;
}

static void mn_runtime_baremetal_critical_exit(
    mn_osal_critical_state_t state)
{
    (void)state;
}

const mn_runtime_ops_t *mn_runtime_get_ops(void)
{
    static const mn_runtime_ops_t runtime = {
        .name = "baremetal",
        .capabilities = MN_OSAL_CAP_CRITICAL_SECTION,
        .init = mn_runtime_baremetal_init,
        .is_in_isr = mn_runtime_baremetal_is_in_isr,
        .get_time_ms = mn_runtime_baremetal_get_time_ms,
        .sleep_ms = mn_runtime_baremetal_sleep_ms,
        .critical_enter = mn_runtime_baremetal_critical_enter,
        .critical_exit = mn_runtime_baremetal_critical_exit,
    };

    return &runtime;
}
