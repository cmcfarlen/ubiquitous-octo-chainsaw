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

#pragma pack(push, 1)
union vec2
{
   vec2() : x(0), y(0) {}
   vec2(f32 X, f32 Y) : x(X), y(Y) {}

   struct {
      f32 x;
      f32 y;
   };
   struct {
      f32 s;
      f32 t;
   };
   f32 v[2];
};

union vec3
{
   vec3() : x(0), y(0), z(0) {}
   vec3(f32 X, f32 Y) : x(X), y(Y), z(0) {}
   vec3(f32 X, f32 Y, f32 Z) :x(X), y(Y), z(Z) {}
   vec3(vec2& v) : x(v.x), y(v.y), z(0) {}
   vec3(vec2& v, f32 Z) : x(v.x), y(v.y), z(Z) {}

   struct {
      f32 x;
      f32 y;
      f32 z;
   };
   f32 v[3];
};

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

struct mat4
{
   f32 m[16];
};

#pragma pack(pop)

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

struct screen_font_entry
{
   char codePoint;
   f32 u1;
   f32 v1;
   f32 u2;
   f32 v2;
   s32 w;
   s32 h;
};

struct screen_font_size
{
   s32 size;
   int firstChar;
   int lastChar;
   int lineHeight;
   screen_font_entry* entries;
};

struct screen_font {
   char* fontName;
   int numSizes;
   s8* textureData;
   screen_font_size* sizes;
};

#endif
