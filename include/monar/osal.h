#ifndef MONAR_OSAL_H
#define MONAR_OSAL_H

#include <stdbool.h>

#include <monar/types.h>

typedef mn_uptr_t mn_osal_critical_state_t;

/*
 * The public OSAL surface is the only runtime-facing API visible to Monar
 * applications. Runtime, RTOS, and bare-metal backend details remain private
 * under runtimes/ and internal headers.
 */
mn_status_t mn_osal_init(void);
bool mn_osal_is_initialized(void);
const char *mn_osal_runtime_name(void);
bool mn_osal_is_in_isr(void);

/*
 * Critical sections are delegated to the selected runtime backend. Future RTOS
 * integrations should let the backend own scheduler/context-switch interrupt
 * behavior instead of duplicating CPU-specific logic in framework core code.
 */
mn_osal_critical_state_t mn_osal_critical_enter(void);
void mn_osal_critical_exit(mn_osal_critical_state_t state);

#endif /* MONAR_OSAL_H */
