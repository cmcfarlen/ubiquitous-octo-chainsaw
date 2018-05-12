#ifndef _GAME_RENDERER_H_
#define _GAME_RENDERER_H_

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
   unsigned int vertexShader;
   unsigned int fragmentShader;
   unsigned int shaderProgram;
} renderer;

typedef bool (*InitializeRenderer_t)(renderer*);
typedef bool (*ResizeWindow_t)(renderer* r, int width, int height);
typedef bool (*RenderFrame_t)(renderer* r);

typedef struct renderer_api
{
   InitializeRenderer_t InitializeRenderer;
   ResizeWindow_t ResizeWindow;
   RenderFrame_t RenderFrame;
} renderer_api;

#endif
