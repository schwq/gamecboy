#pragma once

#include <errno.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef unsigned int uint;

// Debugging defines
#define DG_STOP_AT_RST_IN true
#define DG_CPU_NO_CRITICAL_EXIT 0

#define BIT(a, n) ((a & (1 << n)) ? 1 : 0)

#define BIT_SET(a, n, on) \
  {                       \
    if (on)               \
      a |= (1 << n);      \
    else                  \
      a &= ~(1 << n);     \
  }

#define BETWEEN(a, b, c) ((a >= b) && (a <= c))

#ifndef DG_CPU_NO_CRITICAL_EXIT
#define _CRITICAL                                        \
  {                                                      \
    fprintf(stderr, "Critical error, cannot handle!\n"); \
    exit(-5);                                            \
  }
#else
#define _CRITICAL                                        \
  {                                                      \
    fprintf(stderr, "Critical error, cannot handle!\n"); \
  }
#endif

#define _ERROR(...)               \
  {                               \
    fprintf(stderr, __VA_ARGS__); \
  }

#define HEX_PATTERN "0x%2.2X"

/* --- PRINTF_BYTE_TO_BINARY macro's --- */
#define PRINTF_BINARY_SEPARATOR
#define PRINTF_BINARY_PATTERN_INT8 "%c%c%c%c%c%c%c%c"
#define PRINTF_BYTE_TO_BINARY_INT8(i)                             \
  (((i) & 0x80ll) ? '1' : '0'), (((i) & 0x40ll) ? '1' : '0'),     \
      (((i) & 0x20ll) ? '1' : '0'), (((i) & 0x10ll) ? '1' : '0'), \
      (((i) & 0x08ll) ? '1' : '0'), (((i) & 0x04ll) ? '1' : '0'), \
      (((i) & 0x02ll) ? '1' : '0'), (((i) & 0x01ll) ? '1' : '0')

#define PRINTF_BINARY_PATTERN_INT16 \
  PRINTF_BINARY_PATTERN_INT8 PRINTF_BINARY_SEPARATOR PRINTF_BINARY_PATTERN_INT8
#define PRINTF_BYTE_TO_BINARY_INT16(i) \
  PRINTF_BYTE_TO_BINARY_INT8((i) >> 8), PRINTF_BYTE_TO_BINARY_INT8(i)
#define PRINTF_BINARY_PATTERN_INT32                   \
  PRINTF_BINARY_PATTERN_INT16 PRINTF_BINARY_SEPARATOR \
      PRINTF_BINARY_PATTERN_INT16
#define PRINTF_BYTE_TO_BINARY_INT32(i) \
  PRINTF_BYTE_TO_BINARY_INT16((i) >> 16), PRINTF_BYTE_TO_BINARY_INT16(i)
#define PRINTF_BINARY_PATTERN_INT64                   \
  PRINTF_BINARY_PATTERN_INT32 PRINTF_BINARY_SEPARATOR \
      PRINTF_BINARY_PATTERN_INT32
#define PRINTF_BYTE_TO_BINARY_INT64(i) \
  PRINTF_BYTE_TO_BINARY_INT32((i) >> 32), PRINTF_BYTE_TO_BINARY_INT32(i)
/* --- end macros --- */

typedef enum { linfo, lerror, lwarn } log_type;

extern void logfmt(log_type type, const char* fmt, ...);
