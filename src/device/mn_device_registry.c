#include <stdbool.h>

#include <monar/device.h>
#include <monar/errno.h>
#include <monar/osal.h>

#include "mn_device_internal.h"

static struct mn_device g_mn_device_registry[MN_CFG_DEVICE_REGISTRY_MAX];

static bool mn_device_name_is_valid(const char *name)
{
    size_t len;

    if (name == NULL || name[0] == '\0') {
        return false;
    }

    len = 0;
    while (name[len] != '\0') {
        if (len >= MN_DEVICE_NAME_MAX - 1u) {
            return false;
        }
        ++len;
    }

    return true;
}

static bool mn_device_name_equals(const char *lhs, const char *rhs)
{
    size_t i;

    if (lhs == NULL || rhs == NULL) {
        return false;
    }

    for (i = 0; lhs[i] != '\0' || rhs[i] != '\0'; ++i) {
        if (lhs[i] != rhs[i]) {
            return false;
        }
    }

    return true;
}

static bool mn_device_class_is_valid(mn_device_class_t device_class)
{
    return device_class == MN_DEVICE_CLASS_GENERIC ||
        device_class == MN_DEVICE_CLASS_ENDPOINT ||
        device_class == MN_DEVICE_CLASS_BUS ||
        device_class == MN_DEVICE_CLASS_CARRIER;
}

static mn_status_t mn_device_registry_validate_descriptor(
    const mn_device_descriptor_t *descriptor)
{
    if (!mn_osal_is_initialized()) {
        return -MN_EPERM;
    }

    if (descriptor == NULL) {
        return -MN_EINVAL;
    }

    if (!mn_device_name_is_valid(descriptor->name)) {
        return -MN_EINVAL;
    }

    if (!mn_device_class_is_valid(descriptor->device_class)) {
        return -MN_EINVAL;
    }

    if (descriptor->ops == NULL) {
        return -MN_EINVAL;
    }

    return MN_EOK;
}

mn_status_t mn_device_register(const mn_device_descriptor_t *descriptor,
    mn_device_t *out_device)
{
    mn_osal_critical_state_t state;
    size_t i;
    mn_status_t ret;

    if (out_device != NULL) {
        *out_device = NULL;
    }

    ret = mn_device_registry_validate_descriptor(descriptor);
    if (ret != MN_EOK) {
        return ret;
    }

    state = mn_osal_critical_enter();
    for (i = 0; i < MN_CFG_DEVICE_REGISTRY_MAX; ++i) {
        if (!g_mn_device_registry[i].is_registered) {
            continue;
        }

        if (mn_device_name_equals(g_mn_device_registry[i].name,
                descriptor->name)) {
            mn_osal_critical_exit(state);
            return -MN_EEXIST;
        }

        if (descriptor->resource_key != NULL &&
                g_mn_device_registry[i].resource_key ==
                descriptor->resource_key) {
            mn_osal_critical_exit(state);
            return -MN_EEXIST;
        }
    }

    for (i = 0; i < MN_CFG_DEVICE_REGISTRY_MAX; ++i) {
        if (g_mn_device_registry[i].is_registered) {
            continue;
        }

        g_mn_device_registry[i].name = descriptor->name;
        g_mn_device_registry[i].device_class = descriptor->device_class;
        g_mn_device_registry[i].capability_flags =
            descriptor->capability_flags;
        g_mn_device_registry[i].resource_key = descriptor->resource_key;
        g_mn_device_registry[i].ops = descriptor->ops;
        g_mn_device_registry[i].driver_data = descriptor->driver_data;
        g_mn_device_registry[i].state = MN_DEVICE_STATE_REGISTERED;
        g_mn_device_registry[i].is_registered = true;

        if (out_device != NULL) {
            *out_device = &g_mn_device_registry[i];
        }

        mn_osal_critical_exit(state);
        return MN_EOK;
    }

    mn_osal_critical_exit(state);
    return -MN_ENOMEM;
}

mn_status_t mn_device_registry_lookup(const char *name, mn_device_t *out_device)
{
    mn_osal_critical_state_t state;
    size_t i;

    if (out_device != NULL) {
        *out_device = NULL;
    }

    if (!mn_osal_is_initialized()) {
        return -MN_EPERM;
    }

    if (!mn_device_name_is_valid(name) || out_device == NULL) {
        return -MN_EINVAL;
    }

    state = mn_osal_critical_enter();
    for (i = 0; i < MN_CFG_DEVICE_REGISTRY_MAX; ++i) {
        if (!g_mn_device_registry[i].is_registered) {
            continue;
        }

        if (mn_device_name_equals(g_mn_device_registry[i].name, name)) {
            *out_device = &g_mn_device_registry[i];
            mn_osal_critical_exit(state);
            return MN_EOK;
        }
    }

    mn_osal_critical_exit(state);
    return -MN_ENOENT;
}

void mn_device_registry_reset_for_test(void)
{
    mn_osal_critical_state_t state;
    size_t i;

    state = mn_osal_critical_enter();
    for (i = 0; i < MN_CFG_DEVICE_REGISTRY_MAX; ++i) {
        g_mn_device_registry[i].name = NULL;
        g_mn_device_registry[i].device_class = MN_DEVICE_CLASS_GENERIC;
        g_mn_device_registry[i].capability_flags = 0u;
        g_mn_device_registry[i].resource_key = NULL;
        g_mn_device_registry[i].ops = NULL;
        g_mn_device_registry[i].driver_data = NULL;
        g_mn_device_registry[i].state = MN_DEVICE_STATE_UNUSED;
        g_mn_device_registry[i].is_registered = false;
    }
    mn_osal_critical_exit(state);
}

bool mn_device_registry_is_valid_handle(mn_device_t device)
{
    size_t i;
    bool found;
    mn_osal_critical_state_t state;

    if (device == NULL) {
        return false;
    }

    found = false;
    state = mn_osal_critical_enter();
    for (i = 0; i < MN_CFG_DEVICE_REGISTRY_MAX; ++i) {
        if (&g_mn_device_registry[i] == device &&
                g_mn_device_registry[i].is_registered) {
            found = true;
            break;
        }
    }
    mn_osal_critical_exit(state);

    return found;
}

mn_status_t mn_device_find(const char *name, mn_device_t *out_device)
{
    return mn_device_registry_lookup(name, out_device);
}
