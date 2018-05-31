#include "platform.h"
#include "font.h"
#include "stb_image_write.h"

screen_font* createFont(int Width, int Height, const char* fontName)
{
   u32 ttf_size;
   u8*ttf = Platform.slurp(fontName, &ttf_size);
   u8* img = (u8*)malloc(Width*Height); //Platform.allocateMemory(Width*Height);

   screen_font* result = (screen_font*)malloc(sizeof(screen_font));

   f32 sizes[] = {12.0, 16.0, 18.0, 22.0,  32.0 };

   result->fontName = fontName;
   result->numSizes = sizeof(sizes)/sizeof(sizes[0]);
   result->textureData = img;


   u32 rows = 0;
   for (int i = 0; i < result->numSizes; i++) {
      screen_font_size* fs = result->sizes + i;
      f32 sz = sizes[i];

      fs->size = sz;
      fs->firstChar = 32;
      fs->lastChar = 96;
      fs->width = Width;
      fs->height = Height;

      u32 r = stbtt_BakeFontBitmap(ttf, 0, sz, img + Width * rows, Width, Height - rows, 32, 96, fs->cdata);

      for (int idx = 0; idx < 96; idx++) {
         fs->cdata[idx].y0 += (u16)rows;
         fs->cdata[idx].y1 += (u16)rows;
      }

      rows += r;
   }

   free(ttf);

   stbi_write_bmp("stbtt.bmp", Width, Height, 1, img);

   return result;
}

screen_font_size* findFontSize(screen_font* font, int size)
{
   for (int i = 0; i < font->numSizes; i++) {
      if (font->sizes[i].size > size) {
         return font->sizes + i;
      }
   }

   // return first if not found
   return font->sizes;
}

f32 drawChar(textured_vertex_buffer* b, screen_font_size* s, int c, f32 X, f32 Y)
{
   f32 x = X;
   f32 y = Y;
   stbtt_aligned_quad q;

   stbtt_GetBakedQuad(s->cdata, s->width, s->height, c - s->firstChar, &x, &y, &q, 1);

   addTextured2DQuad(b, vec2(q.x0, q.y0), vec2(q.x1, q.y1), vec2(q.s0, q.t0), vec2(q.s1, q.t1));

   return x - X;
}

f32 drawString(textured_vertex_buffer* b, screen_font_size* p, f32 XBegin, f32 Y, const char* text)
{
   f32 X = XBegin;
   for (const char* c = text; *c; c++) {
      if (*c == ' ') {
         X += 10;
      } else {
         X += drawChar(b, p, *c, X, Y);
      }
   }

   return X - XBegin;
}

f32 drawInt(textured_vertex_buffer* b, screen_font_size* p, f32 XBegin, f32 Y, s64 number, unsigned int padding)
{
   f32 X = XBegin;

   char buffer[32] = "0000000000";
   if (padding > 10) {
      padding = 10;
   }

   buffer[padding] = 0;

   if (number < 0) {
      X += drawChar(b, p, '-', X, Y);
      number = -number;
   }

   unsigned int idx = 0;
   while (number > 0) {
      buffer[idx++] = (number % 10) + '0';
      number /= 10;
   }

   padding = padding < idx ? idx : padding;

   for (unsigned int i = 0; i < padding; i++) {
      X += drawChar(b, p, buffer[padding - i - 1], X, Y);
   }

   return X - XBegin;
}

f32 drawFloat(textured_vertex_buffer* b, screen_font_size* p, f32 XBegin, f32 Y, f32 number, int precision)
{
   f32 X = XBegin;

   bool neg = number < 0 ? 1 : 0;

   number = fabsf(number);

   if (number < 2 * EPSILON) {
      number = 0.0f;
      neg = 0;
   }

   int whole = (int)floorf(number);
   int fraction = (int)roundf((number - whole) * powf(10.0f, (f32)precision));

   if (neg) {
      X += drawChar(b, p, '-', X, Y);
   }

   X += drawInt(b, p, X, Y, whole);
   X += drawString(b, p, X, Y, ".");
   X += drawInt(b, p, X, Y, fraction, precision);

   return X - XBegin;
}

f32 drawDouble(textured_vertex_buffer* b, screen_font_size* p, f32 XBegin, f32 Y, f64 number, int precision)
{
   f32 X = XBegin;
   s64 whole = (s64)floor(number);
   s64 fraction = (s64)round((number - whole) * powf(10.0f, (f32)precision));

   X += drawInt(b, p, X, Y, whole);
   X += drawString(b, p, X, Y, ".");
   X += drawInt(b, p, X, Y, fraction, precision);

   return X - XBegin;
}

f32 drawVec3(textured_vertex_buffer* b, screen_font_size* p, f32 XBegin, f32 Y, vec3 v)
{
   f32 X = XBegin;

   X += drawChar(b, p, '[', X, Y);
   X += drawFloat(b, p, X, Y, v.x, 3);
   X += drawChar(b, p, ' ', X, Y);
   X += drawFloat(b, p, X, Y, v.y, 3);
   X += drawChar(b, p, ' ', X, Y);
   X += drawFloat(b, p, X, Y, v.z, 3);
   X += drawChar(b, p, ']', X, Y);

   return X - XBegin;
}

f32 drawVec4(textured_vertex_buffer* b, screen_font_size* p, f32 XBegin, f32 Y, vec4 v)
{
   f32 X = XBegin;

   X += drawChar(b, p, '[', X, Y);
   X += drawFloat(b, p, X, Y, v.x, 3);
   X += drawChar(b, p, ' ', X, Y);
   X += drawFloat(b, p, X, Y, v.y, 3);
   X += drawChar(b, p, ' ', X, Y);
   X += drawFloat(b, p, X, Y, v.z, 3);
   X += drawChar(b, p, ' ', X, Y);
   X += drawFloat(b, p, X, Y, v.w, 3);
   X += drawChar(b, p, ']', X, Y);

   return X - XBegin;
}

f32 drawVec2(textured_vertex_buffer* b, screen_font_size* p, f32 XBegin, f32 Y, vec2 v)
{
   f32 X = XBegin;

   X += drawChar(b, p, '[', X, Y);
   X += drawFloat(b, p, X, Y, v.x, 3);
   X += drawChar(b, p, ' ', X, Y);
   X += drawFloat(b, p, X, Y, v.y, 3);
   X += drawChar(b, p, ']', X, Y);

   return X - XBegin;
}

void free_screen_font(screen_font* f)
{
   free(f->textureData);
   free(f);
}

