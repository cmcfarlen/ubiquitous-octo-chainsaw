#ifndef _GAME_H_
#define _GAME_H_

#include <gl/GL.h>
#include "types.h"

struct game_state
{
   int WindowWidth;
   int WindowHeight;

   screen_font* TheFont;
   GLuint FontTexture;
};

#endif
