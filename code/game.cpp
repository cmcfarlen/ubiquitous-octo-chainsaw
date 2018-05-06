
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <stdlib.h>
#include <math.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "types.h"
#include "game.h"

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

void render_frame(float dt)
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


   glFlush();
}

void render_ui()
{

}

#define GL_SHADING_LANGUAGE_VERSION       0x8B8C


extern "C" {

   __declspec(dllexport) game_state* createGameState()
   {
      // TODO(dad): No stdlib
      game_state* st = (game_state*)malloc(sizeof(game_state));
      return st;
   }

   __declspec(dllexport) void render(float dt)
   {
      render_frame(dt);
   }

   typedef struct gl_information
   {
      const unsigned char* vendor;
      const unsigned char* renderer;
      const unsigned char* version;
      const unsigned char* shader_version;
      const unsigned char* extensions;
   } gl_information;

   __declspec(dllexport) void initialize(int width, int height)
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

   }
}

