// platform.h
//
// definitions for functionality provided by the platform
#ifndef _PLATFORM_H_
#define _PLATFORM_H_

typedef struct platform_api
{
   void (*log)(const char* fmt, ...);
} platform_api;

extern platform_api Platform;

#endif
