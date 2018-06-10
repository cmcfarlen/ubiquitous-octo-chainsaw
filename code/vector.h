#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "types.h"

#pragma pack(push, 1)
union vec2
{
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
   vec2 ll;
   vec2 ur;
};

#pragma pack(pop)

inline
vec2 Vec2(f32 X, f32 Y)
{
   vec2 r = {X, Y};
   return r;
}

inline
vec2 Vec3(const vec3& v)
{
   vec2 r = {v.x, v.y};
   return r;
}

inline
vec3 Vec3(f32 X, f32 Y)
{
   vec3 r = {X, Y, 0};
   return r;
}

inline
vec3 Vec3(f32 X, f32 Y, f32 Z)
{
   vec3 r = {X, Y, Z};
   return r;
}

inline
vec3 Vec3(vec2& v)
{
   vec3 r = {v.x, v.y, 0};
   return r;
}

inline
vec3 Vec3(vec2& v, f32 Z)
{
   vec3 r = {v.x, v.y, Z};
   return r;
}

inline
vec4 Vec4(f32 R, f32 G, f32 B, f32 A)
{
   vec4 r = {R, G, B, A};
   return r;
};

inline
rect Rect2(f32 x1, f32 y1, f32 x2, f32 y2)
{
   rect r = {{x1, y1}, {x2, y2}};
   return r;
}

inline
rect Rect2(vec2 ll, vec2 ur)
{
   rect r = {ll, ur};
   return r;
}

inline
f32 width(const rect& r)
{
   return r.ur.x - r.ll.x;
}

inline
f32 height(const rect& r)
{
   return r.ur.y - r.ll.y;
}

inline
float magnitude(vec3 v)
{
   return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
}

inline
vec3 normalize(vec3 v)
{
   float m = magnitude(v);
   if (m > EPSILON) {
      vec3 r = { v.x / m, v.y / m, v.z / m };
      return r;
   }
   return v;
}

inline
vec3 mul(vec3 a, float b)
{
   vec3 r = { a.x * b, a.y * b, a.z * b };
   return r;
}

inline
vec3 add(vec3 a, vec3 b)
{
   vec3 r = { a.x + b.x, a.y + b.y, a.z + b.z };
   return r;
}

inline
vec3 cross(vec3 a, vec3 b)
{
   vec3 r;

   r.x = a.y * b.z - a.z * b.y;
   r.y = a.z * b.x - a.x * b.z;
   r.z = a.x * b.y - a.y * b.x;

   return r;
}

inline
vec3 operator+(vec3 a, vec3 b)
{
   vec3 r = { a.x + b.x, a.y + b.y, a.z + b.z };
   return r;
}

inline
vec3 operator-(vec3 a, vec3 b)
{
   vec3 r = { a.x - b.x, a.y - b.y, a.z - b.z };
   return r;
}

inline
vec3 operator-(const vec3& a)
{
   vec3 r = { -a.x, -a.y, -a.z };
   return r;
}

inline
vec3 operator*(vec3 a, float b)
{
   vec3 r = { a.x * b, a.y * b, a.z * b };
   return r;
}

inline
vec2 operator*(vec2 a, float b)
{
   vec2 r = { a.x * b, a.y * b };
   return r;
}


inline
vec2 operator-(const vec2&a, const vec2& b)
{
   vec2 r;

   r.x = a.x - b.x;
   r.y = a.y - b.y;

   return r;
}

inline
bool operator==(const vec2& a , const vec2 & b)
{
   return a.x == b.x && a.y == b.y;
}

inline
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

inline
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
