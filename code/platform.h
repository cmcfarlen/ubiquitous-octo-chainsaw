// platform.h
//
// definitions for functionality provided by the platform
#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include "types.h"

typedef struct platform_api
{
   void (*log)(const char* fmt, ...);
   u8* (*slurp)(const char* resource, u32*size);  //read entire resource
   void* (*allocateMemory)(size_t);
   u32 (*pageSize)();
   u32 (*processorCount)();

} platform_api;

extern platform_api Platform;

#endif
