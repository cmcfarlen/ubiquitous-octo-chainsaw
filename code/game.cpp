
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <stdlib.h>
#include <math.h>

#include "types.h"
#include "game.h"
#include "assert.h"

/*
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
*/

extern "C" {

   game_state* CreateGameState()
   {
      // TODO(dad): No stdlib
      game_state* st = (game_state*)malloc(sizeof(game_state));
      return st;
   }

   void UpdateGameState(game_state*, f32 )
   {

   }

   void InitializeGame(game_state* state, int width, int height)
   {
      state->WindowWidth = width;
      state->WindowHeight = height;
   }
}

