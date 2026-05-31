#ifndef MONAR_SRC_INTERNAL_MN_OSAL_BACKEND_H
#define MONAR_SRC_INTERNAL_MN_OSAL_BACKEND_H

#include <stdbool.h>

#include <monar/osal.h>
#include <monar/types.h>

typedef struct mn_runtime_ops {
    const char *name;
    mn_osal_capability_t capabilities;
    mn_status_t (*init)(void);
    bool (*is_in_isr)(void);
    mn_status_t (*get_time_ms)(mn_u32_t *out_time_ms);
    mn_status_t (*sleep_ms)(mn_u32_t duration_ms);
    mn_osal_critical_state_t (*critical_enter)(void);
    void (*critical_exit)(mn_osal_critical_state_t state);
} mn_runtime_ops_t;

/*
 * This backend contract is private to framework core and runtime
 * implementations. Public headers must not expose RTOS handles, vendor HAL
 * types, registers, IRQ objects, or other silicon-private details here.
 */
const mn_runtime_ops_t *mn_runtime_get_ops(void);

#endif /* MONAR_SRC_INTERNAL_MN_OSAL_BACKEND_H */
