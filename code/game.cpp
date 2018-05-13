
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <stdlib.h>
#include <math.h>

#include "types.h"
#include "game.h"
#include "assert.h"

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

