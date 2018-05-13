#ifndef _GAME_RENDERER_H_
#define _GAME_RENDERER_H_

#include "game.h"

#pragma pack(push, 1)
struct textured_vertex
{
   vec3 P;
   vec2 T;
};
#pragma pack(pop)

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

typedef struct renderer
{
   struct {
      int x;
      int y;
      int width;
      int height;
   } viewport;

   unsigned int VBO;
   unsigned int VAO;
   unsigned int EBO;
   unsigned int shaderProgram;
   unsigned int texture;
   screen_font* TheFont;
   unsigned int fontProgram;
   unsigned int FontTexture;
   textured_vertex_buffer* FontVertexBuffer;

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
