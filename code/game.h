#ifndef _GAME_H_
#define _GAME_H_

#include "types.h"
#include "memory.h"
#include "debug.h"
#include "platform.h"
#include "vector.h"

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

enum MouseButtons
{
   MOUSE_BUTTON1 = 1,
   MOUSE_BUTTON2,
   MOUSE_BUTTON3,
   MOUSE_BUTTON4,
   MOUSE_BUTTON5,
   MOUSE_BUTTON6,

   MOUSE_BUTTON_MAX
};

struct game_input
{
   f32 dt; // time delta to update game by in seconds
   u8  buttons_down[BUTTON_MAX_DEFINED]; // true if the button is currently down
   u8  buttons_pressed[BUTTON_MAX_DEFINED]; // true if button was pressed this frame
   u8  letters_down[256]; // untranslated keys that are down
   vec2 mouse_p; // mouse position this frame
   vec2 mouse_dp; // mouse change this frame
   vec2 mouse_down_p; // position when mouse button went down


   u8 mouse_buttons_down[MOUSE_BUTTON_MAX];
   u8 mouse_buttons_click[MOUSE_BUTTON_MAX];
};

struct world_camera
{
   vec3 p;
   vec3 v;
   f32 yaw;
   f32 pitch;
};

struct cube
{
   vec3 p;
   vec3 dim;

   f32 angle;
};

struct game_world
{
   bool isInitialized;
   world_camera camera;

   u32 picked_cube;
   cube cubes[10];
};

struct game_state
{
   platform_api Platform;
   debug_system* DebugSystem;

   f32 rate;
   f32 angle;

   bool draw_pick_buffer;

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
