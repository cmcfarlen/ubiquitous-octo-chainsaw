#ifndef _GAME_H_
#define _GAME_H_

#include "types.h"
#include "memory.h"
#include "debug.h"
#include "platform.h"

enum GameButtons
{
   BUTTON_FORWARD = 1,
   BUTTON_BACK,
   BUTTON_LEFT,
   BUTTON_RIGHT,
   BUTTON_UP,
   BUTTON_DOWN,

   BUTTON_MAX_DEFINED
};

struct game_input
{
   f32 dt; // time delta to update game by in seconds
   u8  buttons_down[BUTTON_MAX_DEFINED]; // true if the button is currently down
   u8  buttons_pressed[BUTTON_MAX_DEFINED]; // true if button was pressed this frame
   u8  letters_down[256]; // untranslated keys that are down
   vec2 mouse_p; // mouse position this frame
   vec2 mouse_dp; // mouse change this frame
};

struct world_camera
{
   vec3 p;
   vec3 v;
   f32 yaw;
   f32 pitch;
};

struct game_world
{
   bool isInitialized;
   world_camera camera;
};

struct game_state
{
   platform_api Platform;
   debug_system* DebugSystem;


   game_input current_input;
   game_world world;
};

typedef void (*UpdateGameState_t)(game_state*, game_input*);
typedef void (*InitializeGame_t)(game_state* state);

struct game_api
{
   UpdateGameState_t UpdateGameState;
   InitializeGame_t InitializeGame;
};

#endif
