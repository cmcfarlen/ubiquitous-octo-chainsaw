#ifndef WIN32_OPENGL_H_
#define WIN32_OPENGL_H_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "platform.h"

typedef struct win32_opengl_render_context
{
   HWND window;
   HDC dc;
   HGLRC context;
   struct {
      const unsigned char* vendor;
      const unsigned char* renderer;
      const unsigned char* version;
      const unsigned char* shader_version;
      const unsigned char* extensions;
   } info;
   platform_api Platform;
} win32_opengl_render_context;

typedef bool (*Win32SetupRenderContext_t)(HWND window, platform_api*, win32_opengl_render_context* context);
typedef bool (*Win32SelectRenderContext_t)(win32_opengl_render_context* context);


#endif
