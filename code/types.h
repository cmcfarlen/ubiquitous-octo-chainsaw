#ifndef _TYPES_H
#define _TYPES_H

#include <math.h>

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

#define PI 3.1415927f

union vec3
{
   struct {
      f32 x;
      f32 y;
      f32 z;
   };
   f32 v[3];
};

float magnitude(vec3 v)
{
   return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
}

vec3 normalize(vec3 v)
{
   float m = magnitude(v);
   vec3 r = { v.x / m, v.y / m, v.z / m };
   return r;
}

vec3 mul(vec3 a, float b)
{
   vec3 r = { a.x * b, a.y * b, a.z * b };
   return r;
}

vec3 add(vec3 a, vec3 b)
{
   vec3 r = { a.x + b.x, a.y + b.y, a.z + b.z };
   return r;
}

vec3 operator+(vec3 a, vec3 b)
{
   vec3 r = { a.x + b.x, a.y + b.y, a.z + b.z };
   return r;
}

vec3 operator*(vec3 a, float b)
{
   vec3 r = { a.x * b, a.y * b, a.z * b };
   return r;
}

union vec4
{
   struct {
      f32 x;
      f32 y;
      f32 z;
      f32 w;
   };
   struct {
      f32 r;
      f32 g;
      f32 b;
      f32 a;
   };
   f32 v[4];
};

struct screen_font_entry
{
   char codePoint;
   s32 u;
   s32 v;
   s32 w;
   s32 h;
};

struct screen_font_size
{
   s32 size;
   screen_font_entry* entries;
};

struct screen_font {
   char* fontName;
   int firstChar;
   int lastChar;
   int numSizes;
   s8* textureData;
   screen_font_size* sizes;
};

#endif
