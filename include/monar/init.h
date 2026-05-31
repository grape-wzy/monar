#ifndef MONAR_INIT_H
#define MONAR_INIT_H

#include <stdbool.h>

#include <monar/types.h>

typedef enum mn_init_stage {
    MN_INIT_STAGE_RESET = 0,
    MN_INIT_STAGE_RUNTIME_READY,
    MN_INIT_STAGE_SILICON_READY,
    MN_INIT_STAGE_BSP_READY,
    MN_INIT_STAGE_READY
} mn_init_stage_t;

mn_status_t mn_system_init(void);
bool mn_system_is_initialized(void);
mn_init_stage_t mn_system_get_stage(void);

#endif /* MONAR_INIT_H */
