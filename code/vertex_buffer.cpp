
#include "vertex_buffer.h"

void addTextured2DQuad(vertex_buffer* b, vec2 pll, vec2 pur, vec2 tll, vec2 tur)
{
   if ((b->vcnt + 6) > b->max) {
      drawBufferElements(b, Triangles);
   }

   u32 v = b->vcnt;
   u32 i = b->icnt;

   b->vertices[v+0].P = Vec3(pll); // lower-left
   b->vertices[v+0].T = tll;
   b->vertices[v+1].P = Vec3(pur.x, pll.y); // lower right
   b->vertices[v+1].T = Vec2(tur.x, tll.y);
   b->vertices[v+2].P = Vec3(pur); // upper-right
   b->vertices[v+2].T = tur;
   b->vertices[v+3].P = Vec3(pll.x, pur.y); // top left
   b->vertices[v+3].T = Vec2(tll.x, tur.y);

   b->indices[i+0] = v + 0;
   b->indices[i+1] = v + 1;
   b->indices[i+2] = v + 2;
   b->indices[i+3] = v + 0;
   b->indices[i+4] = v + 2;
   b->indices[i+5] = v + 3;


   b->vcnt += 4;
   b->icnt += 6;
}


void addColoredCube(vertex_buffer* bin, vec3 origin, vec3 dim, vec4 color[6])
{
   u32 v = bin->vcnt;
   u32 ioff = bin->icnt;
   vertex* vtx = bin->vertices;
   u32* idx = bin->indices;
   f32 hw = 0.5f * dim.x;
   f32 hh = 0.5f * dim.y;
   f32 hd = 0.5f * dim.z;
   f32 ox = origin.x;
   f32 oy = origin.y;
   f32 oz = origin.z;

   f32 l = ox - hw;
   f32 r = ox + hw;
   f32 f = oz - hd; // far
   f32 n = oz + hd; // near
   f32 b = oy - hh;
   f32 t = oy + hh;

   // 8 vertices
   // front face
   vtx[v+0].P = Vec3(l, b, n);
   vtx[v+0].C = color[0];
   vtx[v+1].P = Vec3(r, b, n);
   vtx[v+1].C = color[0];
   vtx[v+2].P = Vec3(r, t, n);
   vtx[v+2].C = color[0];
   vtx[v+3].P = Vec3(l, t, n);
   vtx[v+3].C = color[0];

   // back face
   vtx[v+4].P = Vec3(r, b, f);
   vtx[v+4].C = color[1];
   vtx[v+5].P = Vec3(l, b, f);
   vtx[v+5].C = color[1];
   vtx[v+6].P = Vec3(l, t, f);
   vtx[v+6].C = color[1];
   vtx[v+7].P = Vec3(r, t, f);
   vtx[v+7].C = color[1];

   // top face
   vtx[v+8].P = Vec3(l, t, n);
   vtx[v+8].C = color[2];
   vtx[v+9].P = Vec3(l, t, f);
   vtx[v+9].C = color[2];
   vtx[v+10].P = Vec3(r, t, f);
   vtx[v+10].C = color[2];
   vtx[v+11].P = Vec3(r, t, n);
   vtx[v+11].C = color[2];

   // bottom face
   vtx[v+12].P = Vec3(l, b, n);
   vtx[v+12].C = color[3];
   vtx[v+13].P = Vec3(r, b, n);
   vtx[v+13].C = color[3];
   vtx[v+14].P = Vec3(r, b, f);
   vtx[v+14].C = color[3];
   vtx[v+15].P = Vec3(l, b, f);
   vtx[v+15].C = color[3];

   // right face
   vtx[v+16].P = Vec3(r, b, n);
   vtx[v+16].C = color[4];
   vtx[v+17].P = Vec3(r, b, f);
   vtx[v+17].C = color[4];
   vtx[v+18].P = Vec3(r, t, f);
   vtx[v+18].C = color[4];
   vtx[v+19].P = Vec3(r, t, n);
   vtx[v+19].C = color[4];

   // left face
   vtx[v+20].P = Vec3(l, b, n);
   vtx[v+20].C = color[5];
   vtx[v+21].P = Vec3(l, b, f);
   vtx[v+21].C = color[5];
   vtx[v+22].P = Vec3(l, t, f);
   vtx[v+22].C = color[5];
   vtx[v+23].P = Vec3(l, t, n);
   vtx[v+23].C = color[5];

   static int cube_indices[] = {
      0, 1, 2, 0, 2, 3,
      4, 5, 6, 4, 6, 7,
      8, 9,10, 8,10,11,
     12,13,14,12,14,15,
     16,17,18,16,18,19,
     20,21,22,20,22,23
   };

   for (int i = 0; i < 36; i++) {
      idx[ioff + i] = v + cube_indices[i];
   }

   bin->vcnt += 24;
   bin->icnt += 36;
}

void addColored2DQuad(vertex_buffer* b, vec2 pll, vec2 pur, vec4 color)
{
   u32 v = b->vcnt;
   u32 i = b->icnt;

   b->vertices[v+0].P = Vec3(pur); // top right
   b->vertices[v+0].C = color;
   b->vertices[v+1].P = Vec3(pur.x, pll.y); // bottom right
   b->vertices[v+1].C = color;
   b->vertices[v+2].P = Vec3(pll); // bottom left
   b->vertices[v+2].C = color;
   b->vertices[v+3].P = Vec3(pll.x, pur.y); // top left
   b->vertices[v+3].C = color;

   b->indices[i+0] = v + 0;
   b->indices[i+1] = v + 1;
   b->indices[i+2] = v + 3;
   b->indices[i+3] = v + 1;
   b->indices[i+4] = v + 2;
   b->indices[i+5] = v + 3;


   b->vcnt += 4;
   b->icnt += 6;
}

void addColoredVertex(vertex_buffer* b, vec3 p, vec4 c)
{
   u32 i = b->vcnt++;
   b->vertices[i].P = p;
   b->vertices[i].C = c;
}

