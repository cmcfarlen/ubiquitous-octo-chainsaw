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
   void (*freeMemory)(void*);
   u32 (*pageSize)(void);
   u32 (*processorCount)(void);
} platform_api;

extern platform_api Platform;

#endif
