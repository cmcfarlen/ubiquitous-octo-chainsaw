
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#include "stb_image_write.h"

// win32 has to manage its own gl function loading
#include "glcorearb.h"

// direct linked gl functions
extern "C" {
GLAPI void APIENTRY glClearColor (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
GLAPI void APIENTRY glClear (GLbitfield mask);
GLAPI void APIENTRY glPolygonMode (GLint, GLint);
GLAPI void APIENTRY glFinish (void);
GLAPI void APIENTRY glEnable (GLenum);
GLAPI void APIENTRY glFlush (void);
GLAPI void APIENTRY glDisable (GLenum);
GLAPI unsigned char* APIENTRY glGetString (GLint);
GLAPI void APIENTRY glViewport (GLint x, GLint y, GLsizei width, GLsizei height);
GLAPI void APIENTRY glGenTextures (GLsizei n, GLuint* textures);
GLAPI void APIENTRY glDeleteTextures(GLsizei n, const GLuint * textures); 
GLAPI void APIENTRY glBindTexture (GLenum target, GLuint texture);
GLAPI void APIENTRY glTexParameteri(GLenum target, GLenum name, GLint param);
GLAPI void APIENTRY glTexParameterf(GLenum target, GLenum name, GLfloat param);
GLAPI void APIENTRY glTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
GLAPI void APIENTRY glBlendFunc(	GLenum  	sfactor, GLenum  	dfactor);
GLAPI GLenum APIENTRY glGetError(void);
GLAPI void APIENTRY glScissor (GLint, GLint, GLint, GLint);

}

// loaded functions
PFNGLGETSTRINGIPROC glGetStringi = 0;
PFNGLGENBUFFERSPROC glGenBuffers = 0;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = 0;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = 0;
PFNGLBUFFERDATAPROC glBufferData = 0;
PFNGLBUFFERSUBDATAPROC glBufferSubData = 0;
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
PFNGLGENERATEMIPMAPPROC glGenerateMipmap = 0;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = 0;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = 0;
PFNGLUNIFORM4FVPROC glUniform4fv = 0;


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
#include "debug.h"

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
   glBufferSubData = (PFNGLBUFFERSUBDATAPROC)wglGetProcAddress("glBufferSubData");
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
   glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmap");
   glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
   glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
   glUniform4fv = (PFNGLUNIFORM4FVPROC)wglGetProcAddress("glUniform4fv");
}

#include "win32_opengl.h"
#include "renderer.h"

// renderer implementation
//

void assertGL()
{
   GLenum err = glGetError();
   assert(err == GL_NO_ERROR);
}

platform_api Platform;
debug_system* GlobalDebug;

mat4 IdentityMatrix()
{
   mat4 r;

   r.m[0] = 1; r.m[4] = 0; r.m[ 8] = 0; r.m[12] = 0;
   r.m[1] = 0; r.m[5] = 1; r.m[ 9] = 0; r.m[13] = 0;
   r.m[2] = 0; r.m[6] = 0; r.m[10] = 1; r.m[14] = 0;
   r.m[3] = 0; r.m[7] = 0; r.m[11] = 0; r.m[15] = 1;

   return r;
}

mat4 Ortho2D(f32 left, f32 right, f32 top, f32 bottom, f32 n, f32 f)
{
   f32 rml = right - left;
   f32 tmb = top - bottom;
   f32 fmn = f - n;
   mat4 r;

   r.m[0] = 2.0f/rml; r.m[4] =        0; r.m[ 8] =      0; r.m[12] = -(right + left)/rml;
   r.m[1] =        0; r.m[5] = 2.0f/tmb; r.m[ 9] =      0; r.m[13] = -(top + bottom)/tmb;
   r.m[2] =        0; r.m[6] =        0; r.m[10] = -2/fmn; r.m[14] =          -(f+n)/fmn;
   r.m[3] =        0; r.m[7] =        0; r.m[11] =      0; r.m[15] =                   1;

   return r;
}

mat4 PlotView(f32 DomainFrom, f32 DomainTo, f32 RangeFrom, f32 RangeTo, f32 X, f32 Y, f32 W, f32 H)
{
   mat4 r;
   f32 xs = W / (DomainTo - DomainFrom);
   f32 ys = H / (RangeTo - RangeFrom);

   r.m[0] = xs; r.m[4] =  0; r.m[ 8] = 0; r.m[12] = X - (xs * DomainFrom);
   r.m[1] =  0; r.m[5] = ys; r.m[ 9] = 0; r.m[13] = Y - (ys * RangeFrom);
   r.m[2] =  0; r.m[6] =  0; r.m[10] = 1; r.m[14] = 0;
   r.m[3] =  0; r.m[7] =  0; r.m[11] = 0; r.m[15] = 1;

   return r;
}

colored_vertex_buffer* createColoredVertexBuffer(u32 max)
{
   colored_vertex_buffer* result = (colored_vertex_buffer*)malloc(sizeof(colored_vertex_buffer));
   colored_vertex* vertices = (colored_vertex*)malloc(max * sizeof(colored_vertex));
   u32* indices = (u32*)malloc(max * 2 * sizeof(u32));

   result->max = max;
   result->vcnt = 0;
   result->icnt = 0;
   result->vertices = vertices;
   result->indices = indices;

   glGenBuffers(2, result->buffers);
   glGenVertexArrays(1, &result->vao);

   glBindVertexArray(result->vao);

   glBindBuffer(GL_ARRAY_BUFFER, result->buffers[0]);
   glBufferData(GL_ARRAY_BUFFER, max * sizeof(colored_vertex), 0, GL_STREAM_DRAW);
   assertGL();

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result->buffers[1]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, max * 2 * sizeof(u32), 0, GL_STREAM_DRAW);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(f32), NULL);
   assertGL();
   glEnableVertexAttribArray(0);
   assertGL();
   glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(f32), (void*)(3*sizeof(f32)));
   assertGL();
   glEnableVertexAttribArray(1);
   assertGL();

   glBindVertexArray(0);

   return result;
}

void addColored2DQuad(colored_vertex_buffer* b, vec2 pll, vec2 pur, vec4 color)
{
   u32 v = b->vcnt;
   u32 i = b->icnt;

   b->vertices[v+0].P = vec3(pur); // top right
   b->vertices[v+0].C = color;
   b->vertices[v+1].P = vec3(pur.x, pll.y); // bottom right
   b->vertices[v+1].C = color;
   b->vertices[v+2].P = vec3(pll); // bottom left
   b->vertices[v+2].C = color;
   b->vertices[v+3].P = vec3(pll.x, pur.y); // top left
   b->vertices[v+3].C = color;

   b->indices[i+0] = v + 0;
   b->indices[i+1] = v + 1;
   b->indices[i+2] = v + 3;
   b->indices[i+3] = v + 1;
   b->indices[i+4] = v + 2;
   b->indices[i+5] = v + 3;


   b->vcnt += 4;
   b->icnt += 6;
}

void addColoredVertex(colored_vertex_buffer* b, vec3 p, vec4 c)
{
   u32 i = b->vcnt++;
   b->vertices[i].P = p;
   b->vertices[i].C = c;
}

void freeColoredVertexBuffer(colored_vertex_buffer* tvb)
{
   glDeleteBuffers(2, tvb->buffers);
   glDeleteVertexArrays(1, &tvb->vao);
   free(tvb->indices);
   free(tvb->vertices);
   free(tvb);
}

void drawColoredVertexTriangles(colored_vertex_buffer* tvb)
{
   glBindVertexArray(tvb->vao);

   glBindBuffer(GL_ARRAY_BUFFER, tvb->buffers[0]);
   glBufferSubData(GL_ARRAY_BUFFER, 0, tvb->vcnt * sizeof(colored_vertex), tvb->vertices);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tvb->buffers[1]);
   glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, tvb->icnt * sizeof(u32), tvb->indices);

   glDrawElements(GL_TRIANGLES, tvb->icnt, GL_UNSIGNED_INT, 0);

   tvb->vcnt = 0;
   tvb->icnt = 0;
}

void drawColoredVertexLineStrip(colored_vertex_buffer* tvb)
{
   glBindVertexArray(tvb->vao);

   glBindBuffer(GL_ARRAY_BUFFER, tvb->buffers[0]);
   glBufferSubData(GL_ARRAY_BUFFER, 0, tvb->vcnt * sizeof(colored_vertex), tvb->vertices);
   assertGL();

   glDrawArrays(GL_LINE_STRIP, 0, tvb->vcnt);
   assertGL();

   tvb->vcnt = 0;
   tvb->icnt = 0;
}

textured_vertex_buffer* createTexturedVertexBuffer(u32 max)
{
   textured_vertex_buffer* result = (textured_vertex_buffer*)malloc(sizeof(textured_vertex_buffer));
   textured_vertex* vertices = (textured_vertex*)malloc(max * sizeof(textured_vertex));
   u32* indices = (u32*)malloc(max * 2 * sizeof(u32));

   result->max = max;
   result->vcnt = 0;
   result->icnt = 0;
   result->vertices = vertices;
   result->indices = indices;

   glGenBuffers(2, result->buffers);
   glGenVertexArrays(1, &result->vao);

   glBindVertexArray(result->vao);

   glBindBuffer(GL_ARRAY_BUFFER, result->buffers[0]);
   glBufferData(GL_ARRAY_BUFFER, max * sizeof(textured_vertex), 0, GL_STREAM_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result->buffers[1]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, max * 2 * sizeof(u32), 0, GL_STREAM_DRAW);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), NULL);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)(3*sizeof(f32)));
   glEnableVertexAttribArray(1);

   glBindVertexArray(0);

   return result;
}

void freeTexturedVertexBuffer(textured_vertex_buffer* tvb)
{
   glDeleteBuffers(2, tvb->buffers);
   glDeleteVertexArrays(1, &tvb->vao);
   free(tvb->indices);
   free(tvb->vertices);
   free(tvb);
}

void drawTexturedVertexBuffer(textured_vertex_buffer* tvb)
{
   glBindVertexArray(tvb->vao);

   glBindBuffer(GL_ARRAY_BUFFER, tvb->buffers[0]);
   glBufferData(GL_ARRAY_BUFFER, tvb->max * sizeof(textured_vertex), 0, GL_STREAM_DRAW);
   glBufferSubData(GL_ARRAY_BUFFER, 0, tvb->vcnt * sizeof(textured_vertex), tvb->vertices);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tvb->buffers[1]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, tvb->max * 2 * sizeof(u32), 0, GL_STREAM_DRAW);
   glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, tvb->icnt * sizeof(u32), tvb->indices);

   glDrawElements(GL_TRIANGLES, tvb->icnt, GL_UNSIGNED_INT, 0);

   tvb->vcnt = 0;
   tvb->icnt = 0;
}

   //addTextured2DQuad(b, vec2(q.x0, q.y0), vec2(q.x1, q.y1), vec2(q.s0, q.t0), vec2(q.s1, q.t1));
void addTextured2DQuad(textured_vertex_buffer* b, vec2 pll, vec2 pur, vec2 tll, vec2 tur)
{
   if ((b->vcnt + 6) > b->max) {
      drawTexturedVertexBuffer(b);
   }

   u32 v = b->vcnt;
   u32 i = b->icnt;

   b->vertices[v+0].P = vec3(pll); // lower-left
   b->vertices[v+0].T = tll;
   b->vertices[v+1].P = vec3(pur.x, pll.y); // lower right
   b->vertices[v+1].T = vec2(tur.x, tll.y);
   b->vertices[v+2].P = vec3(pur); // upper-right
   b->vertices[v+2].T = tur;
   b->vertices[v+3].P = vec3(pll.x, pur.y); // top left
   b->vertices[v+3].T = vec2(tll.x, tur.y);

   b->indices[i+0] = v + 0;
   b->indices[i+1] = v + 1;
   b->indices[i+2] = v + 2;
   b->indices[i+3] = v + 0;
   b->indices[i+4] = v + 2;
   b->indices[i+5] = v + 3;


   b->vcnt += 4;
   b->icnt += 6;
}

struct screen_font_size
{
   f32 size;
   int firstChar;
   int lastChar;
   int width;
   int height;
   stbtt_bakedchar cdata[96];
};


struct screen_font
{
   const char* fontName;
   int numSizes;
   u8* textureData;
   screen_font_size sizes[5];

};

screen_font* createFont(int Width, int Height, const char* fontName)
{
   u32 ttf_size;
   u8*ttf = Platform.slurp(fontName, &ttf_size);
   u8* img = (u8*)malloc(Width*Height); //Platform.allocateMemory(Width*Height);

   screen_font* result = (screen_font*)malloc(sizeof(screen_font));

   f32 sizes[] = {12.0, 16.0, 18.0, 22.0,  32.0 };

   result->fontName = fontName;
   result->numSizes = sizeof(sizes)/sizeof(sizes[0]);
   result->textureData = img;


   u32 rows = 0;
   for (int i = 0; i < result->numSizes; i++) {
      screen_font_size* fs = result->sizes + i;
      f32 sz = sizes[i];

      fs->size = sz;
      fs->firstChar = 32;
      fs->lastChar = 96;
      fs->width = Width;
      fs->height = Height;

      u32 r = stbtt_BakeFontBitmap(ttf, 0, sz, img + Width * rows, Width, Height - rows, 32, 96, fs->cdata);

      for (int idx = 0; idx < 96; idx++) {
         fs->cdata[idx].y0 += (u16)rows;
         fs->cdata[idx].y1 += (u16)rows;
      }

      rows += r;
   }

   stbi_write_bmp("stbtt.bmp", Width, Height, 1, img);

   return result;
}

screen_font_size* findFontSize(screen_font* font, int size)
{
   for (int i = 0; i < font->numSizes; i++) {
      if (font->sizes[i].size > size) {
         return font->sizes + i;
      }
   }

   // return first if not found
   return font->sizes;
}

f32 drawChar(textured_vertex_buffer* b, screen_font_size* s, int c, f32 X, f32 Y)
{
   f32 x = X;
   f32 y = Y;
   stbtt_aligned_quad q;

   stbtt_GetBakedQuad(s->cdata, s->width, s->height, c - s->firstChar, &x, &y, &q, 1);

   addTextured2DQuad(b, vec2(q.x0, q.y0), vec2(q.x1, q.y1), vec2(q.s0, q.t0), vec2(q.s1, q.t1));

   return x - X;
}

f32 drawString(textured_vertex_buffer* b, screen_font_size* p, f32 XBegin, f32 Y, const char* text)
{
   f32 X = XBegin;
   for (const char* c = text; *c; c++) {
      if (*c == ' ') {
         X += 10;
      } else {
         X += drawChar(b, p, *c, X, Y);
      }
   }

   return X - XBegin;
}

f32 drawInt(textured_vertex_buffer* b, screen_font_size* p, f32 XBegin, f32 Y, s64 number, unsigned int padding = 1)
{
   f32 X = XBegin;

   char buffer[32] = "0000000000";
   if (padding > 10) {
      padding = 10;
   }

   buffer[padding] = 0;

   if (number < 0) {
      X += drawChar(b, p, '-', X, Y);
      number = -number;
   }

   unsigned int idx = 0;
   while (number > 0) {
      buffer[idx++] = (number % 10) + '0';
      number /= 10;
   }

   padding = padding < idx ? idx : padding;

   for (unsigned int i = 0; i < padding; i++) {
      X += drawChar(b, p, buffer[padding - i - 1], X, Y);
   }

   return X - XBegin;
}

f32 drawFloat(textured_vertex_buffer* b, screen_font_size* p, f32 XBegin, f32 Y, f32 number, int precision)
{
   f32 X = XBegin;
   int whole = (int)floorf(number);
   int fraction = (int)roundf((number - whole) * powf(10.0f, (f32)precision));

   X += drawInt(b, p, X, Y, whole);
   X += drawString(b, p, X, Y, ".");
   X += drawInt(b, p, X, Y, fraction, precision);

   return X - XBegin;
}

f32 drawDouble(textured_vertex_buffer* b, screen_font_size* p, f32 XBegin, f32 Y, f64 number, int precision)
{
   f32 X = XBegin;
   s64 whole = (s64)floor(number);
   s64 fraction = (s64)round((number - whole) * powf(10.0f, (f32)precision));

   X += drawInt(b, p, X, Y, whole);
   X += drawString(b, p, X, Y, ".");
   X += drawInt(b, p, X, Y, fraction, precision);

   return X - XBegin;
}

void free_screen_font(screen_font* f)
{
   free(f->textureData);
   free(f);
}

GLuint compileShader(const char* vertexName, const char* fragmentName)
{
   unsigned int vertexShader;
   unsigned int fragmentShader;
   unsigned int shaderProgram;
   int success;
   char infoLog[512];
   
   u32 vertexShaderLength = 0;
   char* vertexShaderSource = (char*)Platform.slurp(vertexName, &vertexShaderLength);
   u32 fragmentShaderLength = 0;
   char* fragmentShaderSource = (char*)Platform.slurp(fragmentName, &fragmentShaderLength);

   // shader shit
   vertexShader = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
   glCompileShader(vertexShader);

   glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

   if (!success) {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      Platform.log("Failed to compile vertex shader: %s\n", infoLog);
      Platform.log("source: %s\n", vertexShaderSource);
   }

   fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
   glCompileShader(fragmentShader);

   glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

   if (!success) {
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      Platform.log("Failed to compile fragment shader: %s\n", infoLog);
      Platform.log("source: %s\n", fragmentShaderSource);
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

   glDeleteShader(vertexShader);
   glDeleteShader(fragmentShader);
   free(vertexShaderSource);
   free(fragmentShaderSource);

   return shaderProgram;
}

const char* addExtension(char* buff, int max, const char* name, const char* ext)
{
   char* s = buff;
   --max;
   while (max-- > 0 && *name) {
      *buff++ = *name++;
   }
   if (max-- > 0) {
      *buff++ = '.';
   }
   while (max-- > 0 && *ext) {
      *buff++ = *ext++;
   }
   *buff = 0;

   return s;
}

GLuint compileShader(const char* name)
{
   char vertName[256];
   char fragName[256];

   return compileShader(addExtension(vertName, sizeof(vertName), name, "vert"),
                        addExtension(fragName, sizeof(fragName), name, "frag"));
}

bool bindUniform(GLuint program, const char* location, const mat4& matrix)
{
   int p = glGetUniformLocation(program, location);
   glUseProgram(program);
   glUniformMatrix4fv(p, 1, GL_FALSE, matrix.m);
   return true;
}

bool bindUniform(GLuint program, const char* location, const vec4& v)
{
   int p = glGetUniformLocation(program, location);
   glUseProgram(program);
   glUniform4fv(p, 1, v.v);
   return true;
}

GLuint createTexture(u8* imageData, int width, int height, int channels, GLenum type = 0)
{
   unsigned int texture;

   glGenTextures(1, &texture);
   glBindTexture(GL_TEXTURE_2D, texture);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   GLenum format = GL_RGBA;
   if (channels == 3) {
      format = GL_RGB;
   } else if (channels == 1) {
      format = GL_RED;
   }

   if (!type) {
      type = format;
   }

   glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
   glGenerateMipmap(GL_TEXTURE_2D);

   return texture;
}

GLuint createTexture(const char* resource)
{
   u32 size = 0;
   u8 *resourceData = Platform.slurp(resource, &size);
   int width, height, channels;
   stbi_uc* imageData = stbi_load_from_memory((stbi_uc const*)resourceData, (int)size, &width, &height, &channels, 0);

   unsigned int texture = createTexture(imageData, width, height, channels);

   stbi_image_free(imageData);
   free(resourceData);

   return texture;
}

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

	glClearColor(0.2f, 0.2f, 0.2f, 1.0);
	
   if (r->TheFont) {
      free_screen_font(r->TheFont);
      r->TheFont = 0;
      glDeleteTextures(1, &r->FontTexture);

      glDeleteProgram(r->fontProgram);
      glDeleteProgram(r->ColorProgram);
      freeTexturedVertexBuffer(r->FontVertexBuffer);
      freeColoredVertexBuffer(r->Colors);
   }

   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


   r->TheFont = createFont(512, 512, "monaco.ttf");
   r->FontTexture = createTexture((u8*)r->TheFont->textureData, 512, 512, 1, GL_RED);
   r->fontProgram = compileShader("font.vert", "font.frag");
   r->FontVertexBuffer = createTexturedVertexBuffer(512);

   r->ColorProgram = compileShader("solid");
   r->Colors = createColoredVertexBuffer(256);

   r->uiProj = Ortho2D(0, (f32)r->viewport.width, 0, (f32)r->viewport.height, -1, 1);
   bindUniform(r->fontProgram, "proj", r->uiProj);
   bindUniform(r->fontProgram, "color", vec4(1, 1, 1, 1));

   bindUniform(r->ColorProgram, "proj", Ortho2D(0, (f32)r->viewport.width, (f32)r->viewport.height, 0, -1, 1));
   //bindUniform(r->ColorProgram, "view", IdentityMatrix());
   bindUniform(r->ColorProgram, "view", PlotView(-10, 10, -1.5, 1.5, 502, 502, 200, 100));

   return true;
}

bool ResizeWindow(renderer* r, int width, int height)
{
   r->viewport.width = width;
   r->viewport.height = height;

   r->uiProj = Ortho2D(0, (f32)r->viewport.width, 0, (f32)r->viewport.height, -1, 1);
   bindUniform(r->fontProgram, "proj", r->uiProj);

   bindUniform(r->ColorProgram, "proj", Ortho2D(0, (f32)r->viewport.width, (f32)r->viewport.height, 0, -1, 1));

   glViewport(0, 0, width, height);

   return true;
}

struct start_stack
{
   int id;
   u64 mark;
};

void RenderFrame(renderer* r, game_state* state)
{
   GlobalDebug = state->DebugSystem;

   glFinish();

   timed_function();

   glClear(GL_COLOR_BUFFER_BIT);

   textured_vertex_buffer* t = r->FontVertexBuffer;
   screen_font_size* f = findFontSize(r->TheFont, 9);

   f32 y = f->size;
   f32 x = 50;

   // draw ui text
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   glUseProgram(r->fontProgram);
   glBindTexture(GL_TEXTURE_2D, r->FontTexture);
   
   x = 5;
   start_stack debug_stack[512];
   int stackp = 0;
   debug_frame* df = GlobalDebug->frame + (1 - GlobalDebug->frame_index);
   for (u32 i = 0; i < df->itemCount; i++) {
      debug_item* item = df->items + i;
      if (item->type == DEBUG_FUNCTION_ENTER) {
         debug_stack[stackp].id = item->id;
         debug_stack[stackp].mark = item->mark;
         ++stackp;
      } else if (item->type ==  DEBUG_FUNCTION_EXIT) {
         --stackp;
         assert(debug_stack[stackp].id == item->id);
         u64 cycles = item->mark - debug_stack[stackp].mark;

         float dt = (float)((double)cycles / GlobalDebug->count_per_second);

         x += drawString(t, f, x, y, item->file);
         x += drawString(t, f, x, y, ": ");
         x += drawString(t, f, x, y, item->name);
         x += drawString(t, f, x, y, "(");
         x += drawInt(t, f, x, y, item->line);
         x += drawString(t, f, x, y, ") ");
         x += drawFloat(t, f, x, y, dt * 1000.0f, 2);

         x = 5;
         y += f->size;
      }
   }

   x += drawString(t, f, x, y, "Debug cps: ");
   x += drawDouble(t, f, x, y, GlobalDebug->count_per_second, 1);

   drawTexturedVertexBuffer(r->FontVertexBuffer);


   /*
   // draw ui plots
   glUseProgram(r->ColorProgram);

   glEnable(GL_SCISSOR_TEST);
   glScissor(500, 500, 200, 200);

   addColored2DQuad(r->Colors, vec2(-10, -1.5), vec2(10, 1.5), vec4(0.1f, 0.1f, 0.1f, 1));
   drawColoredVertexTriangles(r->Colors);

   vec4 plotColor = vec4(1, 0, 0.57, 1);
   for (f32 d = -10; d < 10.5; d += 0.25) {
      addColoredVertex(r->Colors, vec3(d, sinf(d), 0), plotColor);
   }
   drawColoredVertexLineStrip(r->Colors);

   glDisable(GL_SCISSOR_TEST);
   */

   glBindVertexArray(0);
}

}

