#ifndef MONAR_OSAL_H
#define MONAR_OSAL_H

#include <stdbool.h>

#include <monar/types.h>

typedef mn_uptr_t mn_osal_critical_state_t;
typedef mn_u32_t mn_osal_capability_t;

#define MN_OSAL_CAP_TIMEBASE         (1u << 0)
#define MN_OSAL_CAP_SLEEP            (1u << 1)
#define MN_OSAL_CAP_CRITICAL_SECTION (1u << 2)
#define MN_OSAL_CAP_MUTEX            (1u << 3)
#define MN_OSAL_CAP_SEMAPHORE        (1u << 4)
#define MN_OSAL_CAP_THREAD           (1u << 5)
#define MN_OSAL_CAP_TIMER            (1u << 6)

/*
 * The public OSAL surface is the only runtime-facing API visible to Monar
 * applications. Runtime, RTOS, and bare-metal backend details remain private
 * under runtimes/ and internal headers.
 */
mn_status_t mn_osal_init(void);
bool mn_osal_is_initialized(void);
const char *mn_osal_runtime_name(void);
bool mn_osal_is_in_isr(void);
mn_osal_capability_t mn_osal_get_capabilities(void);
bool mn_osal_has_capability(mn_osal_capability_t capability);
mn_status_t mn_osal_get_time_ms(mn_u32_t *out_time_ms);
mn_status_t mn_osal_sleep_ms(mn_u32_t duration_ms);

/*
 * Critical sections are delegated to the selected runtime backend. Future RTOS
 * integrations should let the backend own scheduler/context-switch interrupt
 * behavior instead of duplicating CPU-specific logic in framework core code.
 */
mn_osal_critical_state_t mn_osal_critical_enter(void);
void mn_osal_critical_exit(mn_osal_critical_state_t state);

#endif /* MONAR_OSAL_H */
