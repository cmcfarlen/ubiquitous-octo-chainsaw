#ifndef _TYPES_H
#define _TYPES_H

#ifdef _MSC_VER
#include <intrin.h>
#elif __clang__
#include <x86intrin.h>
#else
#error Support other compilers
#endif

#include <math.h>
#include <stdint.h>

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;
typedef int64_t s64;
typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;

typedef float f32;
typedef double f64;

#define arraycount(v) (sizeof(v)/sizeof(v[0]))

#define EPSILON 1.19e-07
#define PI 3.1415927f

#endif
