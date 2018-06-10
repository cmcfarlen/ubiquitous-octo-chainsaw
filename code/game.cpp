
#include "types.h"

#include "platform.h"

#include "game.h"
#include "assert.h"

platform_api Platform;

void initializeWorld(game_world* world)
{
   if (world->isInitialized) {
      return;
   }

   world->isInitialized = true;

   world->move_target = 0;

   world->camera.p = Vec3(0, 0, 20);
   world->camera.v = Vec3(0, 0, 0);
   world->camera.pitch = 0;
   world->camera.yaw = 270.0;

   f32 w = 5.0;
   for (u32 i = 0; i < arraycount(world->cubes); i++)
   {
      world->cubes[i].p = Vec3(i * 2 * w, 0, 0);
      world->cubes[i].dim = Vec3(w, 2*w, w);
      world->cubes[i].angle = i * 0.25f;
   }

   world->picked_cube = 0;

   world->lightP = Vec3(5, 5, 5);
}

bool isDown(game_input* i, int button)
{
   return i->buttons_down[button] != 0;
}

bool isPressed(game_input* i, int button)
{
   return i->buttons_pressed[button] != 0;
}

bool letterDown(game_input* i, int letter)
{
   return i->letters_down[letter] != 0;
}

void accelerateCamera(world_camera* c, f32 dt, vec3 a)
{
   vec3 v = c->v;
   a += v * -8.0f; // dampen

   c->p += a * (0.5f * dt * dt) + v * dt;
   c->v += a * dt;
}

void accelerateLight(game_world* w, f32 dt, vec3 a)
{
   vec3 v = w->lightV;
   a += v * -8.0f; // dampen

   w->lightP += a * (0.5f * dt * dt) + v * dt;
   w->lightV += a * dt;
}

extern "C" {
   void UpdateGameState(game_state* state, game_input* input)
   {
      Platform = state->Platform;

      state->current_input = *input;

      if (letterDown(input, 'R')) {
         state->world.camera.p = Vec3(0, 0, 20);
         state->world.camera.v = Vec3(0, 0, 0);
         state->world.camera.pitch = 0.0f;
         state->world.camera.yaw = 270.0f;
      }

      if (letterDown(input, 'P')) {
         state->draw_pick_buffer = true;
      } else {
         state->draw_pick_buffer = false;
      }

      if (letterDown(input, 'N')) {
         state->rate += 0.1;
      }

      if (letterDown(input, 'M')) {
         state->rate -= 0.1;
      }

      if (letterDown(input, 'L')) {
         state->world.move_target = 1;
      } else {
         state->world.move_target = 0;
      }

      state->angle += state->rate * input->dt;

      game_world* world = &state->world;

      f32 w = 2.0;
      for (u32 i = 0; i < arraycount(world->cubes); i++)
      {
         world->cubes[i].p = Vec3(i * 2 * w, 0, 0);
         world->cubes[i].dim = Vec3(w, 2*w, w);
         world->cubes[i].angle += state->rate * input->dt;
      }

      // accelerate camera
      vec3 f = Vec3(0, 0, 0);
      if (isDown(input, BUTTON_FORWARD)) {
         f += directionFromYaw(state->world.camera.yaw);
      }
      if (isDown(input, BUTTON_BACK)) {
         f += -directionFromYaw(state->world.camera.yaw);
      }
      if (isDown(input, BUTTON_LEFT)) {
         vec3 dir = directionFromYaw(state->world.camera.yaw);
         vec3 right = cross(dir, Vec3(0, 1, 0));
         f += -right;
      }
      if (isDown(input, BUTTON_RIGHT)) {
         vec3 dir = directionFromYaw(state->world.camera.yaw);
         vec3 right = cross(dir, Vec3(0, 1, 0));
         f += right;
      }
      if (isDown(input, BUTTON_UP)) {
         f += Vec3(0, 1, 0);
      }
      if (isDown(input, BUTTON_DOWN)) {
         f += Vec3(0, -1, 0);
      }

      f = normalize(f) * 50.0f;

      if (state->world.move_target == 0) {
         accelerateCamera(&state->world.camera, input->dt, f);
      } else {
         accelerateLight(&state->world, input->dt, f);
      }

      if (input->mouse_buttons_down[MOUSE_BUTTON1]) {
         vec2 py = input->mouse_dp * 0.1;

         state->world.camera.pitch -= py.y;
         if (state->world.camera.pitch > 89.0f) {
            state->world.camera.pitch = 89.0f;
         }
         if (state->world.camera.pitch < -89.0f) {
            state->world.camera.pitch = -89.0f;
         }

         state->world.camera.yaw += py.x;
      }
   }

   void InitializeGame(game_state* state)
   {
      Platform = state->Platform;
      initializeWorld(&state->world);
   }
}

