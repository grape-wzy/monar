#include <assert.h>
#include <string.h>

#include <monar/monar.h>

#include "mn_device_internal.h"
#include "mn_init_internal.h"

typedef struct test_device_context {
    size_t open_calls;
    size_t close_calls;
    size_t read_calls;
    size_t write_calls;
    size_t ioctl_calls;
} test_device_context_t;

static mn_status_t test_open(mn_device_t device, mn_u32_t flags)
{
    test_device_context_t *ctx;

    (void)flags;

    ctx = mn_device_get_driver_data(device);
    assert(ctx != NULL);
    ctx->open_calls++;
    return MN_EOK;
}

static mn_status_t test_close(mn_device_t device)
{
    test_device_context_t *ctx;

    ctx = mn_device_get_driver_data(device);
    assert(ctx != NULL);
    ctx->close_calls++;
    return MN_EOK;
}

static mn_status_t test_read(mn_device_t device, void *buffer, size_t size,
    size_t *out_size)
{
    static const char payload[] = "monar";
    size_t count;
    test_device_context_t *ctx;

    ctx = mn_device_get_driver_data(device);
    assert(ctx != NULL);
    ctx->read_calls++;

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

static mn_status_t test_write(mn_device_t device, const void *buffer,
    size_t size, size_t *out_size)
{
    test_device_context_t *ctx;

    assert(buffer != NULL || size == 0u);

    ctx = mn_device_get_driver_data(device);
    assert(ctx != NULL);
    ctx->write_calls++;

    if (out_size != NULL) {
        *out_size = size;
    }

    return MN_EOK;
}

static mn_status_t test_ioctl(mn_device_t device, mn_u32_t cmd, void *arg)
{
    test_device_context_t *ctx;

    (void)cmd;
    (void)arg;

    ctx = mn_device_get_driver_data(device);
    assert(ctx != NULL);
    ctx->ioctl_calls++;
    return MN_EOK;
}

static void test_registry_rules(void)
{
    static test_device_context_t ctx_a;
    static test_device_context_t ctx_b;
    static const int resource_a;
    static const int resource_b;
    static const mn_device_ops_t ops = {
        .open = test_open,
        .close = test_close,
        .read = test_read,
        .write = test_write,
        .ioctl = test_ioctl,
    };
    const mn_device_descriptor_t dev_a = {
        .name = "uart0",
        .device_class = MN_DEVICE_CLASS_ENDPOINT,
        .capability_flags = MN_DEVICE_CAP_READ | MN_DEVICE_CAP_WRITE |
            MN_DEVICE_CAP_IOCTL,
        .resource_key = &resource_a,
        .ops = &ops,
        .driver_data = &ctx_a,
    };
    const mn_device_descriptor_t duplicate_name = {
        .name = "uart0",
        .device_class = MN_DEVICE_CLASS_ENDPOINT,
        .capability_flags = MN_DEVICE_CAP_READ,
        .resource_key = &resource_b,
        .ops = &ops,
        .driver_data = &ctx_b,
    };
    const mn_device_descriptor_t duplicate_resource = {
        .name = "spi0",
        .device_class = MN_DEVICE_CLASS_BUS,
        .capability_flags = MN_DEVICE_CAP_READ,
        .resource_key = &resource_a,
        .ops = &ops,
        .driver_data = &ctx_b,
    };
    mn_device_t unused;
    mn_device_t stale;

    mn_system_reset_for_test();
    assert(mn_osal_init() == MN_EOK);
    mn_device_registry_reset_for_test();
    stale = (mn_device_t)&ctx_b;

    assert(mn_device_register(&dev_a, NULL) == MN_EOK);
    assert(mn_device_register(&duplicate_name, &stale) == -MN_EEXIST);
    assert(stale == NULL);
    stale = (mn_device_t)&ctx_b;
    assert(mn_device_register(&duplicate_resource, &stale) == -MN_EEXIST);
    assert(stale == NULL);
    unused = (mn_device_t)&ctx_a;
    assert(mn_device_find("missing0", &unused) == -MN_ENOENT);
    assert(unused == NULL);
}

static void test_device_api_guards(void)
{
    static test_device_context_t ctx;
    static test_device_context_t control_ctx;
    static const int resource_c;
    static const int resource_d;
    static const mn_device_ops_t ops = {
        .open = test_open,
        .close = test_close,
        .read = test_read,
        .write = NULL,
        .ioctl = NULL,
    };
    static const mn_device_ops_t control_ops = {
        .open = test_open,
        .close = test_close,
        .read = NULL,
        .write = NULL,
        .ioctl = test_ioctl,
    };
    const mn_device_descriptor_t descriptor = {
        .name = "sensor0",
        .device_class = MN_DEVICE_CLASS_ENDPOINT,
        .capability_flags = MN_DEVICE_CAP_READ,
        .resource_key = &resource_c,
        .ops = &ops,
        .driver_data = &ctx,
    };
    const mn_device_descriptor_t control_descriptor = {
        .name = "control0",
        .device_class = MN_DEVICE_CLASS_ENDPOINT,
        .capability_flags = MN_DEVICE_CAP_IOCTL,
        .resource_key = &resource_d,
        .ops = &control_ops,
        .driver_data = &control_ctx,
    };
    mn_device_t device;
    mn_device_t looked_up;
    mn_device_t control_device;
    char buffer[8];
    size_t count;

    mn_system_reset_for_test();
    assert(mn_osal_init() == MN_EOK);
    mn_device_registry_reset_for_test();
    assert(mn_device_register(&descriptor, &device) == MN_EOK);
    assert(mn_device_register(&control_descriptor, &control_device) == MN_EOK);
    assert(mn_device_find("sensor0", &looked_up) == MN_EOK);
    assert(looked_up == device);

    looked_up = (mn_device_t)&ctx;
    assert(mn_device_open(NULL, 0u, &device) == -MN_EINVAL);
    assert(device == NULL);
    assert(mn_device_open("sensor0", 4u, &device) == -MN_EINVAL);
    assert(device == NULL);
    assert(mn_device_open("missing1", 0u, &device) == -MN_ENOENT);
    assert(device == NULL);
    assert(mn_device_read(device, buffer, sizeof(buffer), &count) == -MN_EBADF);
    assert(mn_device_open("sensor0", MN_DEVICE_OPEN_WRITE, &device) ==
        -MN_ENOTSUP);
    assert(device == NULL);
    assert(mn_device_open("sensor0", MN_DEVICE_OPEN_RDWR, &device) ==
        -MN_ENOTSUP);
    assert(device == NULL);
    assert(mn_device_open("control0", 0u, &control_device) == MN_EOK);
    assert(control_ctx.open_calls == 1u);
    assert(mn_device_ioctl(control_device, 0u, NULL) == MN_EOK);
    assert(control_ctx.ioctl_calls == 1u);
    assert(mn_device_close(control_device) == MN_EOK);
    assert(mn_device_open("sensor0", MN_DEVICE_OPEN_READ, &device) == MN_EOK);
    assert(mn_device_open("sensor0", MN_DEVICE_OPEN_READ, &looked_up) ==
        -MN_EBUSY);
    assert(looked_up == NULL);
    assert(mn_device_read(device, NULL, sizeof(buffer), &count) == -MN_EINVAL);
    assert(mn_device_read(device, buffer, sizeof(buffer), &count) == MN_EOK);
    assert(count == 5u);
    assert(mn_device_write(device, buffer, sizeof(buffer), &count) ==
        -MN_ENOTSUP);
    assert(mn_device_ioctl(device, 0u, NULL) == -MN_ENOTSUP);
    assert(mn_device_close(device) == MN_EOK);
    assert(mn_device_close(device) == -MN_EBADF);
    assert(ctx.open_calls == 1u);
    assert(ctx.close_calls == 1u);
    assert(ctx.read_calls == 1u);
    assert(ctx.write_calls == 0u);
    assert(ctx.ioctl_calls == 0u);
    assert(control_ctx.close_calls == 1u);
}

void mn_test_device(void)
{
    test_registry_rules();
    test_device_api_guards();
}
