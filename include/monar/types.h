#ifndef MONAR_TYPES_H
#define MONAR_TYPES_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef int8_t mn_i8_t;
typedef uint8_t mn_u8_t;
typedef int16_t mn_i16_t;
typedef uint16_t mn_u16_t;
typedef int32_t mn_i32_t;
typedef uint32_t mn_u32_t;
typedef int64_t mn_i64_t;
typedef uint64_t mn_u64_t;
typedef intptr_t mn_iptr_t;
typedef uintptr_t mn_uptr_t;

typedef mn_i32_t mn_status_t;
typedef mn_i32_t mn_timeout_t;
typedef void *mn_handle_t;

#endif /* MONAR_TYPES_H */
