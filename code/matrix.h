#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "types.h"
#include "vector.h"

struct mat4
{
   f32 m[16];
};

mat4 operator*(const mat4& ma, const mat4& mb)
{
   const f32* a = ma.m;
   const f32* b = mb.m;
   mat4 r;

   f32 b0 = b[0];
   f32 b1 = b[1];
   f32 b2 = b[2];
   f32 b3 = b[3];
   r.m[0] = a[0]*b0 + a[4]*b1 + a[ 8]*b2 + a[12]*b3;
   r.m[1] = a[1]*b0 + a[5]*b1 + a[ 9]*b2 + a[13]*b3;
   r.m[2] = a[2]*b0 + a[6]*b1 + a[10]*b2 + a[14]*b3;
   r.m[3] = a[3]*b0 + a[7]*b1 + a[11]*b2 + a[15]*b3;

   b0 = b[4];
   b1 = b[5];
   b2 = b[6];
   b3 = b[7];
   r.m[4] = a[0]*b0 + a[4]*b1 + a[ 8]*b2 + a[12]*b3;
   r.m[5] = a[1]*b0 + a[5]*b1 + a[ 9]*b2 + a[13]*b3;
   r.m[6] = a[2]*b0 + a[6]*b1 + a[10]*b2 + a[14]*b3;
   r.m[7] = a[3]*b0 + a[7]*b1 + a[11]*b2 + a[15]*b3;

   b0 = b[8];
   b1 = b[9];
   b2 = b[10];
   b3 = b[11];
   r.m[8] = a[0]*b0 + a[4]*b1 + a[ 8]*b2 + a[12]*b3;
   r.m[9] = a[1]*b0 + a[5]*b1 + a[ 9]*b2 + a[13]*b3;
   r.m[10] = a[2]*b0 + a[6]*b1 + a[10]*b2 + a[14]*b3;
   r.m[11] = a[3]*b0 + a[7]*b1 + a[11]*b2 + a[15]*b3;

   b0 = b[12];
   b1 = b[13];
   b2 = b[14];
   b3 = b[15];
   r.m[12] = a[0]*b0 + a[4]*b1 + a[ 8]*b2 + a[12]*b3;
   r.m[13] = a[1]*b0 + a[5]*b1 + a[ 9]*b2 + a[13]*b3;
   r.m[14] = a[2]*b0 + a[6]*b1 + a[10]*b2 + a[14]*b3;
   r.m[15] = a[3]*b0 + a[7]*b1 + a[11]*b2 + a[15]*b3;

   return r;
}

mat4 IdentityMatrix()
{
   mat4 r;

   r.m[0] = 1; r.m[4] = 0; r.m[ 8] = 0; r.m[12] = 0;
   r.m[1] = 0; r.m[5] = 1; r.m[ 9] = 0; r.m[13] = 0;
   r.m[2] = 0; r.m[6] = 0; r.m[10] = 1; r.m[14] = 0;
   r.m[3] = 0; r.m[7] = 0; r.m[11] = 0; r.m[15] = 1;

   return r;
}

mat4 translationMatrix(vec3 t)
{
   mat4 r;

   r.m[0] = 1; r.m[4] = 0; r.m[ 8] = 0; r.m[12] = t.x;
   r.m[1] = 0; r.m[5] = 1; r.m[ 9] = 0; r.m[13] = t.y;
   r.m[2] = 0; r.m[6] = 0; r.m[10] = 1; r.m[14] = t.z;
   r.m[3] = 0; r.m[7] = 0; r.m[11] = 0; r.m[15] = 1;

   return r;
}

mat4 scaleMatrix(vec3 t)
{
   mat4 r;

   r.m[0] = t.x; r.m[4] =   0; r.m[ 8] =   0; r.m[12] = 0;
   r.m[1] =   0; r.m[5] = t.y; r.m[ 9] =   0; r.m[13] = 0;
   r.m[2] =   0; r.m[6] =   0; r.m[10] = t.z; r.m[14] = 0;
   r.m[3] =   0; r.m[7] =   0; r.m[11] =   0; r.m[15] = 1;

   return r;
}

mat4 rotationMatrix(f32 radians, vec3 axis_in)
{
   vec3 axis = normalize(axis_in);

   f32 x = axis.x;
   f32 y = axis.y;
   f32 z = axis.z;

   f32 s = sinf(radians);
   f32 c = cosf(radians);
   f32 t = 1 - c;

   mat4 r;

   r.m[0] =     x * x * t + c; r.m[4] = x * y * t - z * s; r.m[ 8] = x * z * t + y * s; r.m[12] = 0;
   r.m[1] = y * x * t + z * s; r.m[5] =     y * y * t + c; r.m[ 9] = y * z * t - x * s; r.m[13] = 0;
   r.m[2] = z * x * t - y * s; r.m[6] = z * y * t + x * s; r.m[10] =     z * z * t + c; r.m[14] = 0;
   r.m[3] =                 0; r.m[7] =                 0; r.m[11] =                 0; r.m[15] = 1;

   return r;
}
   
mat4 rotationX(f32 radians)
{
   f32 s = sinf(radians);
   f32 c = cosf(radians);

   mat4 r;

   r.m[0] = 1; r.m[4] = 0; r.m[ 8] = 0; r.m[12] = 0;
   r.m[1] = 0; r.m[5] = c; r.m[ 9] =-s; r.m[13] = 0;
   r.m[2] = 0; r.m[6] = s; r.m[10] = c; r.m[14] = 0;
   r.m[3] = 0; r.m[7] = 0; r.m[11] = 0; r.m[15] = 1;

   return r;
}

mat4 rotationY(f32 radians)
{
   f32 s = sinf(radians);
   f32 c = cosf(radians);

   mat4 r;

   r.m[0] = c; r.m[4] = 0; r.m[ 8] = s; r.m[12] = 0;
   r.m[1] = 0; r.m[5] = 1; r.m[ 9] = 0; r.m[13] = 0;
   r.m[2] =-s; r.m[6] = 0; r.m[10] = c; r.m[14] = 0;
   r.m[3] = 0; r.m[7] = 0; r.m[11] = 0; r.m[15] = 1;

   return r;
}

mat4 rotationZ(f32 radians)
{
   f32 s = sinf(radians);
   f32 c = cosf(radians);

   mat4 r;

   r.m[0] = c; r.m[4] =-s; r.m[ 8] = 0; r.m[12] = 0;
   r.m[1] = s; r.m[5] = c; r.m[ 9] = 0; r.m[13] = 0;
   r.m[2] = 0; r.m[6] = 0; r.m[10] = 1; r.m[14] = 0;
   r.m[3] = 0; r.m[7] = 0; r.m[11] = 0; r.m[15] = 1;

   return r;
}

#endif
