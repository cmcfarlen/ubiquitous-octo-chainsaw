#ifndef _VERTEX_BUFFER_H_
#define _VERTEX_BUFFER_H_

#include "types.h"
#include "vector.h"

#pragma pack(push, 1)
struct vertex
{
   vec3 P;
   vec2 T;
   vec4 C;
   vec3 N;
};
#pragma pack(pop)

struct vertex_buffer
{
   u32 max;
   u32 vcnt;
   u32 icnt;
   vertex* vertices;
   u32* indices;

   void* p;
};

enum PrimitiveType
{
   Triangles
};

void addTextured2DQuad(vertex_buffer* b, vec2 pll, vec2 pur, vec2 tll, vec2 tur);
void addColoredCube(vertex_buffer* bin, vec3 origin, vec3 dim, vec4 color[6]);
void addColored2DQuad(vertex_buffer* b, vec2 pll, vec2 pur, vec4 color);
void addColoredVertex(vertex_buffer* b, vec3 p, vec4 c);

void drawBufferElements(vertex_buffer* tvb, PrimitiveType prim);
void drawBuffer(vertex_buffer* tvb, PrimitiveType prim);

#endif
