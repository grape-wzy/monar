#include <stdbool.h>

#include <monar/device.h>
#include <monar/errno.h>
#include <monar/osal.h>

#include "mn_device_internal.h"

static bool mn_device_open_flags_are_valid(mn_u32_t flags)
{
    mn_u32_t allowed;

    allowed = MN_DEVICE_OPEN_READ | MN_DEVICE_OPEN_WRITE;
    return (flags & ~allowed) == 0u;
}

static mn_status_t mn_device_open_flags_match_capabilities(
    const struct mn_device *device, mn_u32_t flags)
{
    if ((flags & MN_DEVICE_OPEN_READ) != 0u &&
            (device->capability_flags & MN_DEVICE_CAP_READ) == 0u) {
        return -MN_ENOTSUP;
    }

    if ((flags & MN_DEVICE_OPEN_WRITE) != 0u &&
            (device->capability_flags & MN_DEVICE_CAP_WRITE) == 0u) {
        return -MN_ENOTSUP;
    }

    return MN_EOK;
}

static mn_status_t mn_device_prepare_transition(mn_device_t device,
    mn_device_state_t expected, mn_device_state_t next)
{
    mn_osal_critical_state_t state;

    state = mn_osal_critical_enter();
    if (device->state != expected) {
        mn_osal_critical_exit(state);
        return (expected == MN_DEVICE_STATE_OPENED) ? -MN_EBADF : -MN_EBUSY;
    }

    device->state = next;
    mn_osal_critical_exit(state);
    return MN_EOK;
}

static void mn_device_finish_transition(mn_device_t device,
    mn_device_state_t next)
{
    mn_osal_critical_state_t state;

    state = mn_osal_critical_enter();
    device->state = next;
    mn_osal_critical_exit(state);
}

mn_status_t mn_device_open(const char *name, mn_u32_t flags,
    mn_device_t *out_device)
{
    mn_device_t device;
    mn_status_t ret;

    if (out_device == NULL) {
        return -MN_EINVAL;
    }

    *out_device = NULL;

    if (!mn_device_open_flags_are_valid(flags)) {
        return -MN_EINVAL;
    }

    ret = mn_device_registry_lookup(name, &device);
    if (ret != MN_EOK) {
        return ret;
    }

    ret = mn_device_open_flags_match_capabilities(device, flags);
    if (ret != MN_EOK) {
        return ret;
    }

    ret = mn_device_prepare_transition(device, MN_DEVICE_STATE_REGISTERED,
        MN_DEVICE_STATE_OPENING);
    if (ret != MN_EOK) {
        return ret;
    }

    if (device->ops->open != NULL) {
        ret = device->ops->open(device, flags);
        if (ret != MN_EOK) {
            mn_device_finish_transition(device, MN_DEVICE_STATE_REGISTERED);
            return ret;
        }
    }

    mn_device_finish_transition(device, MN_DEVICE_STATE_OPENED);
    *out_device = device;
    return MN_EOK;
}

mn_status_t mn_device_close(mn_device_t device)
{
    mn_status_t ret;

    if (!mn_osal_is_initialized()) {
        return -MN_EPERM;
    }

    if (!mn_device_registry_is_valid_handle(device)) {
        return -MN_EBADF;
    }

    ret = mn_device_prepare_transition(device, MN_DEVICE_STATE_OPENED,
        MN_DEVICE_STATE_CLOSING);
    if (ret != MN_EOK) {
        return ret;
    }

    if (device->ops->close != NULL) {
        ret = device->ops->close(device);
        if (ret != MN_EOK) {
            mn_device_finish_transition(device, MN_DEVICE_STATE_OPENED);
            return ret;
        }
    }

    mn_device_finish_transition(device, MN_DEVICE_STATE_REGISTERED);
    return MN_EOK;
}

mn_status_t mn_device_read(mn_device_t device, void *buffer, size_t size,
    size_t *out_size)
{
    if (!mn_osal_is_initialized()) {
        return -MN_EPERM;
    }

    if (!mn_device_registry_is_valid_handle(device)) {
        return -MN_EBADF;
    }

    if (size > 0u && buffer == NULL) {
        return -MN_EINVAL;
    }

    if (device->state != MN_DEVICE_STATE_OPENED) {
        return -MN_EBADF;
    }

    if ((device->capability_flags & MN_DEVICE_CAP_READ) == 0u ||
            device->ops->read == NULL) {
        return -MN_ENOTSUP;
    }

    return device->ops->read(device, buffer, size, out_size);
}

mn_status_t mn_device_write(mn_device_t device, const void *buffer,
    size_t size, size_t *out_size)
{
    if (!mn_osal_is_initialized()) {
        return -MN_EPERM;
    }

    if (!mn_device_registry_is_valid_handle(device)) {
        return -MN_EBADF;
    }

    if (size > 0u && buffer == NULL) {
        return -MN_EINVAL;
    }

    if (device->state != MN_DEVICE_STATE_OPENED) {
        return -MN_EBADF;
    }

    if ((device->capability_flags & MN_DEVICE_CAP_WRITE) == 0u ||
            device->ops->write == NULL) {
        return -MN_ENOTSUP;
    }

    return device->ops->write(device, buffer, size, out_size);
}

mn_status_t mn_device_ioctl(mn_device_t device, mn_u32_t cmd, void *arg)
{
    (void)cmd;
    (void)arg;

    if (!mn_osal_is_initialized()) {
        return -MN_EPERM;
    }

    if (!mn_device_registry_is_valid_handle(device)) {
        return -MN_EBADF;
    }

    if (device->state != MN_DEVICE_STATE_OPENED) {
        return -MN_EBADF;
    }

    if ((device->capability_flags & MN_DEVICE_CAP_IOCTL) == 0u ||
            device->ops->ioctl == NULL) {
        return -MN_ENOTSUP;
    }

    return device->ops->ioctl(device, cmd, arg);
}

const char *mn_device_get_name(mn_device_t device)
{
    if (!mn_device_registry_is_valid_handle(device)) {
        return NULL;
    }

    return device->name;
}

mn_device_class_t mn_device_get_class(mn_device_t device)
{
    if (!mn_device_registry_is_valid_handle(device)) {
        return MN_DEVICE_CLASS_GENERIC;
    }

    return device->device_class;
}

mn_u32_t mn_device_get_capabilities(mn_device_t device)
{
    if (!mn_device_registry_is_valid_handle(device)) {
        return 0u;
    }

    return device->capability_flags;
}

void *mn_device_get_driver_data(mn_device_t device)
{
    if (!mn_device_registry_is_valid_handle(device)) {
        return NULL;
    }

    return device->driver_data;
}
