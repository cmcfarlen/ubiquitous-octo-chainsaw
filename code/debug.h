#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "types.h"

struct debug_item
{
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
void swapDebugFrame(debug_system* s)
{
   int frame = 1 - s->frame_index;
   s->frame_index = frame;
   s->frame[frame].itemCount = 0;
}

#endif
