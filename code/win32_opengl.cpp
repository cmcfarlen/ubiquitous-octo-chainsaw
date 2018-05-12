
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// win32 has to manage its own gl function loading
#include "glcorearb.h"

// direct linked gl functions
extern "C" {
GLAPI void APIENTRY glClearColor (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
GLAPI void APIENTRY glClear (GLbitfield mask);
GLAPI void APIENTRY glPolygonMode (GLint, GLint);
GLAPI void APIENTRY glFinish (void);
GLAPI unsigned char* APIENTRY glGetString (GLint);
GLAPI void APIENTRY glViewport (GLint x, GLint y, GLsizei width, GLsizei height);
}

// loaded functions
PFNGLGETSTRINGIPROC glGetStringi = 0;
PFNGLGENBUFFERSPROC glGenBuffers = 0;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = 0;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = 0;
PFNGLBUFFERDATAPROC glBufferData = 0;
PFNGLCREATESHADERPROC glCreateShader = 0;
PFNGLSHADERSOURCEPROC glShaderSource = 0;
PFNGLCOMPILESHADERPROC glCompileShader = 0;
PFNGLGETSHADERIVPROC glGetShaderiv = 0;
PFNGLGETPROGRAMIVPROC glGetProgramiv = 0;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = 0;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = 0;
PFNGLCREATEPROGRAMPROC glCreateProgram = 0;
PFNGLATTACHSHADERPROC glAttachShader = 0;
PFNGLLINKPROGRAMPROC glLinkProgram = 0;
PFNGLUSEPROGRAMPROC glUseProgram = 0;
PFNGLDELETESHADERPROC glDeleteShader = 0;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = 0;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = 0;
PFNGLDRAWARRAYSPROC glDrawArrays = 0;
PFNGLDRAWELEMENTSPROC glDrawElements = 0;
PFNGLBINDBUFFERPROC glBindBuffer = 0;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = 0;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = 0;
PFNGLDELETEPROGRAMPROC glDeleteProgram = 0;


// wgl defines
#define WGL_NUMBER_PIXEL_FORMATS_ARB            0x2000
#define WGL_DRAW_TO_WINDOW_ARB                  0x2001
#define WGL_DRAW_TO_BITMAP_ARB                  0x2002
#define WGL_ACCELERATION_ARB                    0x2003
#define WGL_NEED_PALETTE_ARB                    0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB             0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB              0x2006
#define WGL_SWAP_METHOD_ARB                     0x2007
#define WGL_NUMBER_OVERLAYS_ARB                 0x2008
#define WGL_NUMBER_UNDERLAYS_ARB                0x2009
#define WGL_TRANSPARENT_ARB                     0x200A
#define WGL_TRANSPARENT_RED_VALUE_ARB           0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB         0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB          0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB         0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB         0x203B
#define WGL_SHARE_DEPTH_ARB                     0x200C
#define WGL_SHARE_STENCIL_ARB                   0x200D
#define WGL_SHARE_ACCUM_ARB                     0x200E
#define WGL_SUPPORT_GDI_ARB                     0x200F
#define WGL_SUPPORT_OPENGL_ARB                  0x2010
#define WGL_DOUBLE_BUFFER_ARB                   0x2011
#define WGL_STEREO_ARB                          0x2012
#define WGL_PIXEL_TYPE_ARB                      0x2013
#define WGL_COLOR_BITS_ARB                      0x2014
#define WGL_RED_BITS_ARB                        0x2015
#define WGL_RED_SHIFT_ARB                       0x2016
#define WGL_GREEN_BITS_ARB                      0x2017
#define WGL_GREEN_SHIFT_ARB                     0x2018
#define WGL_BLUE_BITS_ARB                       0x2019
#define WGL_BLUE_SHIFT_ARB                      0x201A
#define WGL_ALPHA_BITS_ARB                      0x201B
#define WGL_ALPHA_SHIFT_ARB                     0x201C
#define WGL_ACCUM_BITS_ARB                      0x201D
#define WGL_ACCUM_RED_BITS_ARB                  0x201E
#define WGL_ACCUM_GREEN_BITS_ARB                0x201F
#define WGL_ACCUM_BLUE_BITS_ARB                 0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB                0x2021
#define WGL_DEPTH_BITS_ARB                      0x2022
#define WGL_STENCIL_BITS_ARB                    0x2023
#define WGL_AUX_BUFFERS_ARB                     0x2024
#define WGL_NO_ACCELERATION_ARB                 0x2025
#define WGL_GENERIC_ACCELERATION_ARB            0x2026
#define WGL_FULL_ACCELERATION_ARB               0x2027
#define WGL_SWAP_EXCHANGE_ARB                   0x2028
#define WGL_SWAP_COPY_ARB                       0x2029
#define WGL_SWAP_UNDEFINED_ARB                  0x202A
#define WGL_TYPE_RGBA_ARB                       0x202B
#define WGL_TYPE_COLORINDEX_ARB                 0x202C

#define WGL_CONTEXT_MAJOR_VERSION_ARB           0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB           0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB             0x2093
#define WGL_CONTEXT_FLAGS_ARB                   0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB            0x9126
#define WGL_CONTEXT_DEBUG_BIT_ARB               0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB  0x0002
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB        0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
#define ERROR_INVALID_VERSION_ARB               0x2095
#define ERROR_INVALID_PROFILE_ARB               0x2096


// wgl extensions
typedef const char* (*wglGetExtensionsStringEXT_t)(void);
typedef bool (*wglChoosePixelFormatARB_t)(HDC hdc,
                                        const int* AttribIList,
                                        const FLOAT* AttribFList,
                                        unsigned int MaxFormats,
                                        int* Formats,
                                        unsigned int* nFormats);
typedef HGLRC (*wglCreateContextAttribsARB_t)(HDC hDC,
                                            HGLRC hshareContext,
                                            const int *attribList);

wglGetExtensionsStringEXT_t wglGetExtensionsStringEXT = 0;
wglChoosePixelFormatARB_t wglChoosePixelFormatARB = 0;
wglCreateContextAttribsARB_t wglCreateContextAttribsARB = 0;

#include "platform.h"

// This will make a fake window class, window, fake dc and fake opengl context
// in order to load the wgl extensions needed to create the real one.
void DoWin32OpenGLShenanigans()
{
   WNDCLASS fakeClass = {};

   fakeClass.lpfnWndProc = DefWindowProc;
   fakeClass.hInstance = GetModuleHandle(0);
   fakeClass.lpszClassName = "FakeGLClass";
   fakeClass.style = CS_OWNDC;

   if (RegisterClass(&fakeClass)) {
      HWND fakeWindow = CreateWindowEx(
            0,
            fakeClass.lpszClassName,
            "fakeopenglwindow",
            CS_OWNDC,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            0,
            0,
            0,
            0);
      PIXELFORMATDESCRIPTOR fakePF =
      {
         sizeof(PIXELFORMATDESCRIPTOR),
         1,
         PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
         PFD_TYPE_RGBA,
         32,
         0, 0, 0, 0, 0, 0,
         0,
         0,
         0,
         0, 0, 0, 0,
         24,
         8,
         0,
         PFD_MAIN_PLANE,
         0,
         0, 0, 0
      };

      HDC fakeDC = GetDC(fakeWindow);

      int fakePFChoice = ChoosePixelFormat(fakeDC, &fakePF);
      SetPixelFormat(fakeDC, fakePFChoice, &fakePF);

      HGLRC fakeGLC = wglCreateContext(fakeDC);
      if (wglMakeCurrent(fakeDC, fakeGLC)) {
         wglGetExtensionsStringEXT = (wglGetExtensionsStringEXT_t)wglGetProcAddress("wglGetExtensionsStringEXT");
         wglChoosePixelFormatARB = (wglChoosePixelFormatARB_t)wglGetProcAddress("wglChoosePixelFormatARB");
         wglCreateContextAttribsARB = (wglCreateContextAttribsARB_t)wglGetProcAddress("wglCreateContextAttribsARB");

         /*
         const char* wglExtensions = wglGetExtensionsStringEXT();
         log("WGL extensions: %s\n", wglExtensions);

         const unsigned char* glVersion = glGetString(GL_VERSION);
         GLint major, minor, numExtensions;

         glGetIntegerv(GL_MAJOR_VERSION, &major);
         glGetIntegerv(GL_MINOR_VERSION, &minor);
         glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

         glGetStringi = (PFNGLGETSTRINGIPROC)wglGetProcAddress("glGetStringi");
         log("Fake GL %s %i.%i\n", glVersion, major, minor);


         for (int i = 0; i < numExtensions; i++) {
            log("EXT: %s\n", glGetStringi(GL_EXTENSIONS, i));
         }
         */
         
         wglMakeCurrent(0, 0);
      }

      wglDeleteContext(fakeGLC);
      ReleaseDC(fakeWindow, fakeDC);
      DestroyWindow(fakeWindow);

   }
}

void Win32SetupPixelFormat(HDC dc)
{
   if (wglChoosePixelFormatARB) {
      int FormatChoice = 0;
      unsigned int PickChoice = 0;
      int AttribList[] = {
         WGL_DRAW_TO_WINDOW_ARB, 1,
         WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
         WGL_SUPPORT_OPENGL_ARB, 1,
         WGL_DOUBLE_BUFFER_ARB, 1,
         WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
         0
      };

      wglChoosePixelFormatARB(dc, AttribList, 0, 1, &FormatChoice, &PickChoice);

      if (PickChoice) {
         PIXELFORMATDESCRIPTOR PixelFormat;
         DescribePixelFormat(dc, FormatChoice, sizeof(PixelFormat), &PixelFormat);
         SetPixelFormat(dc, FormatChoice, &PixelFormat);
      } else {
         Platform.log("No pixel format was choosen!\n");
      }
   } else {
      Platform.log("wglChoosePixelFormatARB was not loaded!\n");
   }
}

void LoadGL()
{
   glGetStringi = (PFNGLGETSTRINGIPROC)wglGetProcAddress("glGetStringi");
   glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
   glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
   glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
   glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
   glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
   glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
   glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
   glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
   glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
   glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
   glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
   glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
   glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
   glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
   glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
   glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
   glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
   glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
   glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
   glDrawArrays = (PFNGLDRAWARRAYSPROC)wglGetProcAddress("glDrawArrays");
   glDrawElements = (PFNGLDRAWELEMENTSPROC)wglGetProcAddress("glDrawElements");
   glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
   glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)wglGetProcAddress("glDeleteVertexArrays");
   glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
}

#include "win32_opengl.h"
#include "renderer.h"

platform_api Platform;

// dll api for win32 renderer management
extern "C" {

// Setup a window for OpenGL and fill out a context
bool Win32SetupRenderContext(HWND window, platform_api* papi, win32_opengl_render_context* context)
{
   DoWin32OpenGLShenanigans();

   HDC dc = GetDC(window);

   Win32SetupPixelFormat(dc);

   int OpenGLAttribs[] = {
      WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
      WGL_CONTEXT_MINOR_VERSION_ARB, 3,
      WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB|WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
      WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
      0
   };

   HGLRC glContext = wglCreateContextAttribsARB(dc, 0, OpenGLAttribs);

   Platform = *papi;
   if (wglMakeCurrent(dc, glContext)) {
      Platform.log("wooooo, we have a modern snazzy OpenGL context!\n");

      context->window = window;
      context->dc = dc;
      context->context = glContext;

      context->Platform = *papi;

      LoadGL();

      // extract opengl info
      context->info.vendor = glGetString(GL_VENDOR);
      context->info.renderer = glGetString(GL_RENDERER);
      context->info.version = glGetString(GL_VERSION);
      context->info.shader_version = glGetString(GL_SHADING_LANGUAGE_VERSION);

      return true;
   } else {
      Platform.log("Failed to create context.\n");
      return false;
   }
}

bool Win32SelectRenderContext(win32_opengl_render_context* context)
{
   Platform = context->Platform;
   return wglMakeCurrent(context->dc, context->context);
}

// platform independent api

bool InitializeRenderer(renderer* r)
{
   // load opengl api on intialize in case of dll reload
   LoadGL();

	glClearColor(0.0, 0.0, 0.0, 1.0);
	
   if (r->VBO) {
      glDeleteBuffers(1, &r->VBO);
      glDeleteBuffers(1, &r->EBO);
      glDeleteVertexArrays(1, &r->VAO);
      glDeleteShader(r->fragmentShader);
      glDeleteShader(r->vertexShader);
      glDeleteProgram(r->shaderProgram);
   }

   float vertices[] = {
       0.5f,  0.5f, 0.0f,
       0.5f, -0.5f, 0.0f,
      -0.5f, -0.5f, 0.0f,
      -0.5f,  0.5f, 0.0f
   };
   unsigned int indices[] = {
      0, 1, 3, 1, 2, 3
   };
   const char* vertexShaderSource =
      "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;\n"
      "\n"
      "void main()\n"
      "{\n"
      "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
      "}\n\0";
   const char* fragmentShaderSource =
      "#version 330 core\n"
      "out vec4 FragColor;\n"
      "\n"
      "void main()\n"
      "{\n"
      "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
      "}\n\0";
   int success;
   char infoLog[512];
   
   unsigned int VBO;
   unsigned int VAO;
   unsigned int EBO;
   unsigned int vertexShader;
   unsigned int fragmentShader;
   unsigned int shaderProgram;

   // shader shit
   vertexShader = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
   glCompileShader(vertexShader);

   glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

   if (!success) {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      Platform.log("Failed to compile vertex shader: %s\n", infoLog);
   }

   fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
   glCompileShader(fragmentShader);

   glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

   if (!success) {
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      Platform.log("Failed to compile fragment shader: %s\n", infoLog);
   }

   shaderProgram = glCreateProgram();

   glAttachShader(shaderProgram, vertexShader);
   glAttachShader(shaderProgram, fragmentShader);
   glLinkProgram(shaderProgram);

   glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
   if (!success) {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      Platform.log("Failed to compile fragment shader: %s\n", infoLog);
   }

   // buffer shit
   glGenBuffers(1, &VBO);
   glGenBuffers(1, &EBO);
   glGenVertexArrays(1, &VAO);

   // this guy holds info on all bound buffers while it is bound
   glBindVertexArray(VAO);

   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
   glEnableVertexAttribArray(0);


	glClear(GL_COLOR_BUFFER_BIT);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   r->VBO = VBO;
   r->VAO = VAO;
   r->EBO = EBO;
   r->vertexShader = vertexShader;
   r->fragmentShader = fragmentShader;
   r->shaderProgram = shaderProgram;

   return true;
}

bool ResizeWindow(renderer* r, int width, int height)
{
   r->viewport.width = width;
   r->viewport.height = height;

   glViewport(0, 0, width, height);

   return true;
}

void RenderFrame(renderer* r)
{
   glFinish();

   glUseProgram(r->shaderProgram);
   glBindVertexArray(r->VAO);

  // glDrawArrays(GL_TRIANGLES, 0, 3);
   glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

   // this is unbinding the vertex array?
   glBindVertexArray(0);
}

}

