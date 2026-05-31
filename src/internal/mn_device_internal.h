#ifndef MONAR_SRC_INTERNAL_MN_DEVICE_INTERNAL_H
#define MONAR_SRC_INTERNAL_MN_DEVICE_INTERNAL_H

#include <stdbool.h>

#include <monar/config.h>
#include <monar/device.h>
#include <monar/types.h>

typedef enum mn_device_state {
    MN_DEVICE_STATE_UNUSED = 0,
    MN_DEVICE_STATE_REGISTERED,
    MN_DEVICE_STATE_OPENING,
    MN_DEVICE_STATE_OPENED,
    MN_DEVICE_STATE_CLOSING
} mn_device_state_t;

struct mn_device {
    const char *name;
    mn_device_class_t device_class;
    mn_u32_t capability_flags;
    const void *resource_key;
    const mn_device_ops_t *ops;
    void *driver_data;
    mn_device_state_t state;
    bool is_registered;
};

mn_status_t mn_device_registry_lookup(const char *name, mn_device_t *out_device);
bool mn_device_registry_is_valid_handle(mn_device_t device);

#endif /* MONAR_SRC_INTERNAL_MN_DEVICE_INTERNAL_H */
