#ifndef MONAR_SRC_INTERNAL_MN_OSAL_BACKEND_H
#define MONAR_SRC_INTERNAL_MN_OSAL_BACKEND_H

#include <stdbool.h>

#include <monar/osal.h>
#include <monar/types.h>

typedef struct mn_runtime_ops {
    const char *name;
    mn_status_t (*init)(void);
    bool (*is_in_isr)(void);
    mn_osal_critical_state_t (*critical_enter)(void);
    void (*critical_exit)(mn_osal_critical_state_t state);
} mn_runtime_ops_t;

const mn_runtime_ops_t *mn_runtime_get_ops(void);

#endif /* MONAR_SRC_INTERNAL_MN_OSAL_BACKEND_H */
