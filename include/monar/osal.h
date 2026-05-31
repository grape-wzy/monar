#ifndef MONAR_OSAL_H
#define MONAR_OSAL_H

#include <stdbool.h>

#include <monar/types.h>

typedef mn_uptr_t mn_osal_critical_state_t;

mn_status_t mn_osal_init(void);
bool mn_osal_is_initialized(void);
const char *mn_osal_runtime_name(void);
bool mn_osal_is_in_isr(void);
mn_osal_critical_state_t mn_osal_critical_enter(void);
void mn_osal_critical_exit(mn_osal_critical_state_t state);

#endif /* MONAR_OSAL_H */
