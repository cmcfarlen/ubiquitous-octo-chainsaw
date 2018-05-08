
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <stdlib.h>
#include <math.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "types.h"
#include "game.h"
#include "assert.h"

void render_centered_circle(vec3 c, float r, int cnt)
{
   vec3 a = { 1, 0, 0 };
   vec3 b = { 0, 1, 0 };

   float step = 2*PI / cnt;

   glBegin(GL_TRIANGLE_FAN);
   glVertex3fv(c.v);

   for (int i = 0; i <= cnt; i++) {
      float t = step * i;
      float rct = r * cosf(t);
      float rst = r * sinf(t);

      vec3 pi = c + a * rct + b * rst; // add(c, add(mul(a, rct), mul(b, rst)));

      glVertex3fv(pi.v);
   }
   glEnd();
}

void render_frame(game_state *, float dt)
{

   // Drawing code here.
   glClearColor(0, 0, 0, 0);
   glClear(GL_COLOR_BUFFER_BIT);

   glEnable(GL_LIGHTING);
   glShadeModel(GL_SMOOTH);

   vec4 light1_color = { 1.0f, 1.0f, 1.0f, 1.0f };
   vec3 light1_pos = { 4.0f, 4.0f, 4.0f };
   vec4 ambient_light = { 0.2f, 0.2f, 0.2f, 1.0f };
   vec4 red = { 1.0f, 0.0f, 0.0f, 1.0f };

   glEnable(GL_LIGHT0);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light1_color.v);
   glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light.v);
   glLightfv(GL_LIGHT0, GL_POSITION, light1_pos.v);

   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light.v);
   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red.v);

   glPushMatrix();
   glTranslatef(-1, 0, -1);
   glNormal3f(0.0f, 1.0f, 0.0f);
   vec3 center = { 0, 0, 0 };
   render_centered_circle(center, .5, 40);

   glPopMatrix();

   glPushMatrix();

   glRotatef(10 * dt, 1, 0, 0);

   glBegin(GL_TRIANGLES);
   {
      glNormal3f(0.0f, 0.0f, 1.0f);
      glVertex3f(0.0f, 0.6f, 0.0f);
      glVertex3f(-0.2f, -0.3f, 0.0f);
      glVertex3f(0.2f, -0.3f, 0.0f);
   }
   glEnd();


   glPopMatrix();

   glPushMatrix();

   glTranslated(1, 0, 1);
   glRotatef(45 + 45 * dt, 1, 0, 0);

   glBegin(GL_TRIANGLES);
   {
      glNormal3f(0.0f, 0.0f, 1.0f);
      glVertex3f(0.0f, 0.6f, 0.0f);
      glVertex3f(-0.2f, -0.3f, 0.0f);
      glVertex3f(0.2f, -0.3f, 0.0f);
   }
   glEnd();


   glPopMatrix();


}

f32 drawChar(screen_font* font, screen_font_size* s, char c, f32 X, f32 Y)
{
   int entry = (int)(c - font->firstChar);
   screen_font_entry* e =  &s->entries[entry];

   f32 W = (f32)e->w;
   f32 H = (f32)e->h;
   f32 u1 = e->u1;
   f32 v1 = e->v1;

   f32 u2 = e->u2;
   f32 v2 = e->v2;

   glTexCoord2f(u1, v2);
   glVertex3f(X, Y + H, 0);
   glTexCoord2f(u2, v2);
   glVertex3f(X+W, Y + H, 0);
   glTexCoord2f(u2, v1);
   glVertex3f(X+W, Y, 0);
   glTexCoord2f(u1, v1);
   glVertex3f(X, Y, 0);

   return W;
}

screen_font_size* findFontSize(screen_font* font, int size)
{
   screen_font_size* p = font->sizes;
   for (int i = 0; i < font->numSizes; i++) {
      if (font->sizes[i].size == size) {
         p = font->sizes + i;
         break;
      }
   }

   return p;
}

f32 drawString(screen_font* font, int size, f32 XBegin, f32 Y, const char* text)
{
   f32 X = XBegin;
   screen_font_size* p = findFontSize(font, size);
   glBegin(GL_QUADS);
   for (const char* c = text; *c; c++) {
      if (*c == ' ') {
         X += 10;
      } else {
         X += drawChar(font, p, *c, X, Y);
      }
   }
   glEnd();

   return X - XBegin;
}

f32 drawInt(screen_font* font, int size, f32 XBegin, f32 Y, int number, unsigned int padding = 1)
{
   f32 X = XBegin;
   screen_font_size* p = findFontSize(font, size);

   char buffer[32] = "0000000000";
   if (padding > 10) {
      padding = 10;
   }

   buffer[padding] = 0;

   if (number < 0) {
      X += drawChar(font, p, '-', X, Y);
      number = -number;
   }

   unsigned int idx = 0;
   while (number > 0) {
      buffer[idx++] = (number % 10) + '0';
      number /= 10;
   }

   glBegin(GL_QUADS);

   padding = padding < idx ? idx : padding;

   for (unsigned int i = 0; i < padding; i++) {
      X += drawChar(font, p, buffer[padding - i - 1], X, Y);
   }

   glEnd();

   return X - XBegin;
}

f32 drawFloat(screen_font* font, int size, f32 XBegin, f32 Y, f32 number, int precision)
{
   f32 X = XBegin;
   int whole = (int)floorf(number);
   int fraction = (int)roundf((number - whole) * powf(10.0f, (f32)precision));

   X += drawInt(font, size, X, Y, whole);
   X += drawString(font, size, X, Y, ".");
   X += drawInt(font, size, X, Y, fraction, precision);

   return X - XBegin;
}

void render_ui(game_state* state, float dt)
{
   int Width = state->WindowWidth;
   int Height = state->WindowHeight;

   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity();

   gluOrtho2D(0, Width, 0, Height);

   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
   glLoadIdentity();

   glDisable(GL_LIGHTING);
   glDisable(GL_DEPTH_TEST);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_TEXTURE_2D);
   
   glBindTexture(GL_TEXTURE_2D, state->FontTexture);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

   glColor4f(1.0, 1.0f, 1.0f, 0.3f);


   int FontSize = 36;
   f32 X = 5;
   f32 Y = (f32)Height - FontSize;

   drawString(state->TheFont, FontSize, X, Y, "Hello World");
   drawString(state->TheFont, FontSize, X, Y - FontSize, "Hello World");
   drawInt(state->TheFont, FontSize, X, Y - FontSize - FontSize, 0);
   drawInt(state->TheFont, FontSize, X + 100, Y - FontSize - FontSize, -43523432);
   drawFloat(state->TheFont, FontSize, X, Y - FontSize * 3, dt, 4);
   drawFloat(state->TheFont, FontSize, X, Y - FontSize * 4, 600.0f, 4);


   //glBindTexture(GL_TEXTURE_2D, 0);
   glEnable(GL_LIGHTING);
   glDisable(GL_TEXTURE_2D);

   glMatrixMode(GL_PROJECTION);
   glPopMatrix();

   glMatrixMode(GL_MODELVIEW);
   glPopMatrix();
}

#define GL_SHADING_LANGUAGE_VERSION       0x8B8C
#define GL_BGRA 0x80E1

extern "C" {

   __declspec(dllexport) game_state* createGameState()
   {
      // TODO(dad): No stdlib
      game_state* st = (game_state*)malloc(sizeof(game_state));
      return st;
   }

   __declspec(dllexport) void render(game_state* state, float dt)
   {
      render_frame(state, dt);
      render_ui(state, dt);

      glFlush();
   }

   typedef struct gl_information
   {
      const unsigned char* vendor;
      const unsigned char* renderer;
      const unsigned char* version;
      const unsigned char* shader_version;
      const unsigned char* extensions;
   } gl_information;

   __declspec(dllexport) void initialize(game_state* state, int width, int height)
   {
      gl_information info = {};

      info.vendor = glGetString(GL_VENDOR);
      info.renderer = glGetString(GL_RENDERER);
      info.version = glGetString(GL_VERSION);
      info.shader_version = glGetString(GL_SHADING_LANGUAGE_VERSION);
      info.extensions = glGetString(GL_EXTENSIONS);

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluPerspective(35.0L, (f64)width / (f64)height, 0.2L, 25.0L);

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      gluLookAt(-5, -5, 5, 0, 0, 0, 0, 0, 1);

      glViewport(0, 0, width, height);

      state->WindowWidth = width;
      state->WindowHeight = height;

      if (state->FontTexture == 0 && state->TheFont) {
         glGenTextures(1, &state->FontTexture);
         glBindTexture(GL_TEXTURE_2D, state->FontTexture);

         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

         /*
         u32 red = 0xff0000ff;
         u32 white = 0xffffffff;
         u32 data[10][10];

         for (int r = 0; r < 5; r++) {
            for (int c = 0; c < 5; c++) {
               data[r][c] = red;
            }
            for (int c = 5; c < 10; c++) {
               data[r][c] = white;
            }
         }
         for (int r = 5; r < 10; r++) {
            for (int c = 0; c < 5; c++) {
               data[r][c] = white;
            }
            for (int c = 5; c < 10; c++) {
               data[r][c] = red;
            }
         }

         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 10, 10, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
         */
         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, state->TheFont->textureData);
         //glGenerateMipmap(GL_TEXTURE_2D);

         glBindTexture(GL_TEXTURE_2D, 0);
      }
   }
}

