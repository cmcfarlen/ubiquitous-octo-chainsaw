#ifndef _TYPES_H
#define _TYPES_H

#ifdef _MSC_VER
#define COMPILER_MSVC
#include <intrin.h>
#else
#error Support other compilers
#endif

#include <math.h>
#include <stdint.h>

typedef unsigned long long u64;
typedef unsigned long u32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef long long s64;
typedef long s32;
typedef short s16;
typedef char s8;

typedef float f32;
typedef double f64;

#define arraycount(v) (sizeof(v)/sizeof(v[0]))

#define EPSILON 1.19e-07
#define PI 3.1415927f

#endif
