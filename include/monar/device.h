#ifndef MONAR_DEVICE_H
#define MONAR_DEVICE_H

#include <stddef.h>

#include <monar/types.h>

#define MN_DEVICE_NAME_MAX 32u

#define MN_DEVICE_CAP_READ  (1u << 0)
#define MN_DEVICE_CAP_WRITE (1u << 1)
#define MN_DEVICE_CAP_IOCTL (1u << 2)

/*
 * Open flags declare which data-path capabilities the caller intends to use.
 * A zero flag value is a control-only open with no read/write request. This is
 * intended for metadata or ioctl-style access, such as a control endpoint that
 * does not expose a data stream.
 */
#define MN_DEVICE_OPEN_READ  (1u << 0)
#define MN_DEVICE_OPEN_WRITE (1u << 1)
#define MN_DEVICE_OPEN_RDWR  (MN_DEVICE_OPEN_READ | MN_DEVICE_OPEN_WRITE)

typedef struct mn_device *mn_device_t;

typedef enum mn_device_class {
    MN_DEVICE_CLASS_GENERIC = 0,
    MN_DEVICE_CLASS_ENDPOINT = 1,
    MN_DEVICE_CLASS_BUS = 2,
    MN_DEVICE_CLASS_CARRIER = 3
} mn_device_class_t;

typedef struct mn_device_ops {
    mn_status_t (*open)(mn_device_t device, mn_u32_t flags);
    mn_status_t (*close)(mn_device_t device);
    mn_status_t (*read)(mn_device_t device, void *buffer, size_t size,
        size_t *out_size);
    mn_status_t (*write)(mn_device_t device, const void *buffer, size_t size,
        size_t *out_size);
    mn_status_t (*ioctl)(mn_device_t device, mn_u32_t cmd, void *arg);
} mn_device_ops_t;

/*
 * Monar phase-1 registration is static-allocation-friendly: the registry keeps
 * these pointer fields directly instead of copying them. The following members
 * must therefore remain valid for the full lifetime of the registered device:
 *
 * - name
 * - ops
 * - resource_key
 * - driver_data
 *
 * Do not pass stack-allocated temporary names, operation tables, or other
 * short-lived objects to mn_device_register().
 */
typedef struct mn_device_descriptor {
    const char *name;
    mn_device_class_t device_class;
    mn_u32_t capability_flags;
    const void *resource_key;
    const mn_device_ops_t *ops;
    void *driver_data;
} mn_device_descriptor_t;

/*
 * Registration is intended for framework integration code such as BSP,
 * silicon adapters, and tests. Application code should normally consume
 * already-registered devices through mn_device_open().
 *
 * The descriptor's pointer members are retained by reference. Their storage
 * must outlive the registered device.
 */
mn_status_t mn_device_register(const mn_device_descriptor_t *descriptor,
    mn_device_t *out_device);
mn_status_t mn_device_find(const char *name, mn_device_t *out_device);

/*
 * mn_device_open() validates requested read/write intent against the device
 * capability flags before calling the driver open operation. Unsupported read
 * or write requests fail with -MN_ENOTSUP.
 */
mn_status_t mn_device_open(const char *name, mn_u32_t flags,
    mn_device_t *out_device);
mn_status_t mn_device_close(mn_device_t device);
mn_status_t mn_device_read(mn_device_t device, void *buffer, size_t size,
    size_t *out_size);
mn_status_t mn_device_write(mn_device_t device, const void *buffer,
    size_t size, size_t *out_size);
mn_status_t mn_device_ioctl(mn_device_t device, mn_u32_t cmd, void *arg);

const char *mn_device_get_name(mn_device_t device);
mn_device_class_t mn_device_get_class(mn_device_t device);
mn_u32_t mn_device_get_capabilities(mn_device_t device);
void *mn_device_get_driver_data(mn_device_t device);

#endif /* MONAR_DEVICE_H */
