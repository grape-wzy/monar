#include <string.h>

#include <monar/device.h>
#include <monar/errno.h>

typedef struct mn_host_status_device {
    int unused;
} mn_host_status_device_t;

static mn_status_t mn_host_status_open(mn_device_t device, mn_u32_t flags)
{
    (void)device;
    (void)flags;

    return MN_EOK;
}

static mn_status_t mn_host_status_close(mn_device_t device)
{
    (void)device;

    return MN_EOK;
}

static mn_status_t mn_host_status_read(mn_device_t device, void *buffer,
    size_t size, size_t *out_size)
{
    static const char payload[] = "host-ready";
    size_t count;

    (void)device;

    if (size > 0u && buffer == NULL) {
        return -MN_EINVAL;
    }

    count = size;
    if (count > sizeof(payload) - 1u) {
        count = sizeof(payload) - 1u;
    }

    if (count > 0u) {
        memcpy(buffer, payload, count);
    }

    if (out_size != NULL) {
        *out_size = count;
    }

    return MN_EOK;
}

static mn_host_status_device_t g_mn_host_status_device;
static const int g_mn_host_status_resource;
static const mn_device_ops_t g_mn_host_status_ops = {
    .open = mn_host_status_open,
    .close = mn_host_status_close,
    .read = mn_host_status_read,
    .write = NULL,
    .ioctl = NULL,
};
static const mn_device_descriptor_t g_mn_host_status_descriptor = {
    .name = "status0",
    .device_class = MN_DEVICE_CLASS_ENDPOINT,
    .capability_flags = MN_DEVICE_CAP_READ,
    .resource_key = &g_mn_host_status_resource,
    .ops = &g_mn_host_status_ops,
    .driver_data = &g_mn_host_status_device,
};

mn_status_t mn_bsp_init(void)
{
    mn_device_t device;

    device = NULL;
    return mn_device_register(&g_mn_host_status_descriptor, &device);
}
