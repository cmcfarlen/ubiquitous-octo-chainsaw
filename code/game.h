#ifndef _GAME_H_
#define _GAME_H_

#include "types.h"
#include "memory.h"
#include "debug.h"
#include "platform.h"

struct game_state
{
   platform_api Platform;
};

typedef void (*UpdateGameState_t)(game_state*, f32);
typedef void (*InitializeGame_t)(game_state* state);

struct game_api
{
   UpdateGameState_t UpdateGameState;
   InitializeGame_t InitializeGame;
};

#endif
