#ifndef _GAME_H_
#define _GAME_H_

#include "types.h"

struct game_state
{
   int WindowWidth;
   int WindowHeight;
};

typedef game_state* (*CreateGameState_t)(void);
typedef void (*UpdateGameState_t)(game_state*, f32);
typedef void (*InitializeGame_t)(game_state* state, int width, int height);

struct game_api
{
   CreateGameState_t CreateGameState;
   UpdateGameState_t UpdateGameState;
   InitializeGame_t InitializeGame;
};

#endif
