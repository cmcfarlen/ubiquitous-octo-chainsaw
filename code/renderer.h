#ifndef _GAME_RENDERER_H_
#define _GAME_RENDERER_H_

#include "game.h"

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

struct screen_viewport {
      int x;
      int y;
      int width;
      int height;
};

struct renderer_implementation;

typedef struct renderer
{
   screen_viewport viewport;

   renderer_implementation* impl;
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
