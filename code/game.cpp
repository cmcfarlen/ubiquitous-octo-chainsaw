
#include "types.h"

#include "platform.h"

#include "game.h"
#include "assert.h"

platform_api Platform;

extern "C" {

   void UpdateGameState(game_state* state, f32 )
   {
      Platform = state->Platform;
   }

   void InitializeGame(game_state* state)
   {
      Platform = state->Platform;
   }
}

