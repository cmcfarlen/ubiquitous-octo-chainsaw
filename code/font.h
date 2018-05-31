#ifndef _FONT_H_
#define _FONT_H_

#include "types.h"
#include "stb_truetype.h"
#include "vector.h"

// TODO: general vertex buffer code
struct textured_vertex_buffer;
void addTextured2DQuad(textured_vertex_buffer* b, vec2 pll, vec2 pur, vec2 tll, vec2 tur);


struct screen_font_size
{
   f32 size;
   int firstChar;
   int lastChar;
   int width;
   int height;
   stbtt_bakedchar cdata[96];
};


struct screen_font
{
   const char* fontName;
   int numSizes;
   u8* textureData;
   screen_font_size sizes[5];
};

screen_font* createFont(int Width, int Height, const char* fontName);
screen_font_size* findFontSize(screen_font* font, int size);

f32 drawChar(textured_vertex_buffer* b, screen_font_size* s, int c, f32 X, f32 Y);
f32 drawString(textured_vertex_buffer* b, screen_font_size* p, f32 XBegin, f32 Y, const char* text);

f32 drawInt(textured_vertex_buffer* b, screen_font_size* p, f32 XBegin, f32 Y, s64 number, unsigned int padding = 1);
f32 drawFloat(textured_vertex_buffer* b, screen_font_size* p, f32 XBegin, f32 Y, f32 number, int precision);
f32 drawDouble(textured_vertex_buffer* b, screen_font_size* p, f32 XBegin, f32 Y, f64 number, int precision);
f32 drawVec3(textured_vertex_buffer* b, screen_font_size* p, f32 XBegin, f32 Y, vec3 v);
f32 drawVec4(textured_vertex_buffer* b, screen_font_size* p, f32 XBegin, f32 Y, vec4 v);
f32 drawVec2(textured_vertex_buffer* b, screen_font_size* p, f32 XBegin, f32 Y, vec2 v);

void free_screen_font(screen_font* f);

#endif
