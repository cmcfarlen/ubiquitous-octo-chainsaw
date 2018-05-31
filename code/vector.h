#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "types.h"

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

   vec3& operator+=(const vec3& o)
   {
      x += o.x;
      y += o.y;
      z += o.z;
      return *this;
   }

   struct {
      f32 x;
      f32 y;
      f32 z;
   };
   f32 v[3];
};

union vec4
{
   vec4() {}
   vec4(f32 R, f32 G, f32 B, f32 A) : r(R), g(G), b(B), a(A) {}
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

struct rect
{
   rect(f32 x1, f32 y1, f32 x2, f32 y2) : ll(x1, y1), ur(x2, y2) {}
   rect(vec2 LL, vec2 UR) : ll(LL), ur(UR) {}

   vec2 ll;
   vec2 ur;
};

#pragma pack(pop)

f32 width(const rect& r)
{
   return r.ur.x - r.ll.x;
}

f32 height(const rect& r)
{
   return r.ur.y - r.ll.y;
}

float magnitude(vec3 v)
{
   return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
}

vec3 normalize(vec3 v)
{
   float m = magnitude(v);
   if (m > EPSILON) {
      vec3 r = { v.x / m, v.y / m, v.z / m };
      return r;
   }
   return v;
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

vec3 cross(vec3 a, vec3 b)
{
   vec3 r;

   r.x = a.y * b.z - a.z * b.y;
   r.y = a.z * b.x - a.x * b.z;
   r.z = a.x * b.y - a.y * b.x;

   return r;
}

vec3 operator+(vec3 a, vec3 b)
{
   vec3 r = { a.x + b.x, a.y + b.y, a.z + b.z };
   return r;
}

vec3 operator-(vec3 a, vec3 b)
{
   vec3 r = { a.x - b.x, a.y - b.y, a.z - b.z };
   return r;
}

vec3 operator-(const vec3& a)
{
   vec3 r = { -a.x, -a.y, -a.z };
   return r;
}

vec3 operator*(vec3 a, float b)
{
   vec3 r = { a.x * b, a.y * b, a.z * b };
   return r;
}

vec2 operator*(vec2 a, float b)
{
   vec2 r = { a.x * b, a.y * b };
   return r;
}


vec2 operator-(const vec2&a, const vec2& b)
{
   vec2 r;

   r.x = a.x - b.x;
   r.y = a.y - b.y;

   return r;
}

vec3 directionFromPitchYaw(f32 pitch, f32 yaw)
{
   pitch *= PI / 180.0f;
   yaw *= PI / 180.0f;

   vec3 r;

   r.x = cosf(yaw) * cosf(pitch);
   r.y = sinf(pitch);
   r.z = sinf(yaw) * cosf(pitch);

   return normalize(r);
}

vec3 directionFromYaw(f32 yaw)
{
   yaw *= PI / 180.0f;

   vec3 r;

   r.x = cosf(yaw);
   r.y = 0;
   r.z = sinf(yaw);

   return normalize(r);
}

#endif
