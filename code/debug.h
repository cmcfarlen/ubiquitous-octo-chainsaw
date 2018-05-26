#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "types.h"

enum debug_item_types
{
   DEBUG_FUNCTION_ENTER = 1,
   DEBUG_FUNCTION_EXIT
};

struct debug_item
{
   int id;  // uses __COUNTER__
   int type;
   int line;
   const char* file;
   const char* name;
   u64 mark;
};

struct debug_frame
{
   u32 itemCount;
   debug_item items[5120];
};

struct debug_system
{
   double count_per_second; // debug counts / sec
   int frame_index; // double buffered debug frames
   debug_frame frame[2];
};

extern debug_system* GlobalDebug;

inline
u64 debug_clocks()
{
   unsigned int aux;
   return __rdtscp(&aux);
}

inline
void initializeDebugSystem(debug_system* s)
{
   s->frame_index = 0;
   s->frame[0].itemCount = 0;
   s->frame[1].itemCount = 0;
}

inline
void swapDebugFrame(debug_system* s)
{
   int frame = 1 - s->frame_index;
   s->frame_index = frame;
   s->frame[frame].itemCount = 0;
}

struct auto_timed_block
{
   int id;
   const char* file;
   const char* func;
   int line;

   auto_timed_block(int Id, const char* File, const char* Func, int Line) : id(Id), file(File), func(Func), line(Line)
   {
      int frame_index = GlobalDebug->frame_index;
      int idx = GlobalDebug->frame[frame_index].itemCount++;
      debug_item* item = GlobalDebug->frame[frame_index].items + idx;

      item->type = DEBUG_FUNCTION_ENTER;
      item->id = id;
      item->name = func;
      item->file = file;
      item->line = line;
      item->mark = debug_clocks();
   }

   ~auto_timed_block()
   {
      int frame_index = GlobalDebug->frame_index;
      int idx = GlobalDebug->frame[frame_index].itemCount++;
      debug_item* item = GlobalDebug->frame[frame_index].items + idx;

      item->type = DEBUG_FUNCTION_EXIT;
      item->id = id;
      item->name = func;
      item->file = file;
      item->line = line;
      item->mark = debug_clocks();
   }
};

#define timed_function() auto_timed_block _debug_time_function(__COUNTER__, __FILE__, __FUNCTION__, __LINE__)
#define timed_block(name) auto_timed_block _debug_time_block(__COUNTER__, __FILE__, name, __LINE__)

#endif
