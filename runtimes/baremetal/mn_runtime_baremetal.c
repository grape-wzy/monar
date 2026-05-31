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
        .init = mn_runtime_baremetal_init,
        .is_in_isr = mn_runtime_baremetal_is_in_isr,
        .critical_enter = mn_runtime_baremetal_critical_enter,
        .critical_exit = mn_runtime_baremetal_critical_exit,
    };

    return &runtime;
}
