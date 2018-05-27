#ifndef _GAME_RENDERER_H_
#define _GAME_RENDERER_H_

#include "game.h"

#pragma pack(push, 1)
struct textured_vertex
{
   vec3 P;
   vec2 T;
};

struct colored_vertex
{
   vec3 P;
   vec4 C;
};
#pragma pack(pop)

struct plot
{
   vec4 color;
   u32 sampleCount;
   u32 cursor;
   vec2* samples;
};

struct graph
{
   vec2 domain;
   vec2 range;
   vec2 p;        // screen coord
   rect dim;      // screen
};

struct colored_vertex_buffer
{
   u32 max;
   u32 vcnt;
   u32 icnt;
   colored_vertex* vertices;
   u32* indices;
   unsigned int buffers[2];
   unsigned int vao;
};

struct textured_vertex_buffer
{
   u32 max;
   u32 vcnt;
   u32 icnt;
   textured_vertex* vertices;
   u32* indices;
   unsigned int buffers[2];
   unsigned int vao;
};

struct screen_font;

typedef struct renderer
{
   struct {
      int x;
      int y;
      int width;
      int height;
   } viewport;

   screen_font* TheFont;
   unsigned int fontProgram;
   unsigned int FontTexture;
   textured_vertex_buffer* FontVertexBuffer;
   colored_vertex_buffer* Colors;
   unsigned int ColorProgram;

   mat4 worldProj;
   mat4 uiProj;
} renderer;

typedef bool (*InitializeRenderer_t)(renderer*);
typedef bool (*ResizeWindow_t)(renderer* r, int width, int height);
typedef bool (*RenderFrame_t)(renderer* r, game_state*);

struct renderer_api
{
   InitializeRenderer_t InitializeRenderer;
   ResizeWindow_t ResizeWindow;
   RenderFrame_t RenderFrame;
};

#endif
