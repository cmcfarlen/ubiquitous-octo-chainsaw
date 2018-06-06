
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <assert.h>
#include <stdlib.h>

#include "stb_image.h"

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
GLAPI void APIENTRY glReadPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels);

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
PFNGLUNIFORM1IPROC glUniform1i = 0;


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
   glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
}

#include "win32_opengl.h"
#include "renderer.h"
#include "vector.h"
#include "matrix.h"
#include "font.h"

// renderer implementation
//

#pragma pack(push, 1)
struct vertex
{
   vec3 P;
   vec2 T;
   vec4 C;
};
#pragma pack(pop)

struct vertex_buffer
{
   u32 max;
   u32 vcnt;
   u32 icnt;
   vertex* vertices;
   u32* indices;
   unsigned int buffers[2];
   unsigned int vao;
};

struct height_map
{
   u8* heights;
   u32 width;
   u32 height;
};

struct renderer_implementation
{
   screen_font* TheFont;
   unsigned int fontProgram;
   unsigned int FontTexture;
   vertex_buffer* FontVertexBuffer;
   vertex_buffer* Colors;
   unsigned int ColorProgram;

   unsigned int WorldProgram;
   unsigned int PickProgram;

   mat4 worldProj;
   mat4 uiProj;

   height_map* heightmap;
   unsigned int heighttexture;
};

platform_api Platform;
debug_system* GlobalDebug;

void assertGL()
{
   GLenum err = glGetError();
   //TODO: why does reloading the render dll return 0x500 from glGetError?
   //assert(err == GL_NO_ERROR);
}

vertex_buffer* createVertexBuffer(u32 max)
{
   vertex_buffer* result = (vertex_buffer*)malloc(sizeof(vertex_buffer));
   vertex* vertices = (vertex*)malloc(max * sizeof(vertex));
   u32* indices = (u32*)malloc(max * 6 * sizeof(u32));

   result->max = max;
   result->vcnt = 0;
   result->icnt = 0;
   result->vertices = vertices;
   result->indices = indices;

   glGenBuffers(2, result->buffers);
   glGenVertexArrays(1, &result->vao);

   glBindVertexArray(result->vao);

   glBindBuffer(GL_ARRAY_BUFFER, result->buffers[0]);
   glBufferData(GL_ARRAY_BUFFER, max * sizeof(vertex), 0, GL_STREAM_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result->buffers[1]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, max * 6 * sizeof(u32), 0, GL_STREAM_DRAW);

   // position
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), NULL);
   glEnableVertexAttribArray(0);
   // texture
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), (void*)(3*sizeof(f32)));
   glEnableVertexAttribArray(1);
   // color
   glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), (void*)(5*sizeof(f32)));
   glEnableVertexAttribArray(2);

   glBindVertexArray(0);

   return result;
}

void freeVertexBuffer(vertex_buffer* tvb)
{
   glDeleteBuffers(2, tvb->buffers);
   glDeleteVertexArrays(1, &tvb->vao);
   free(tvb->indices);
   free(tvb->vertices);
   free(tvb);
}

void addColoredCube(vertex_buffer* bin, vec3 origin, vec3 dim, vec4 color[6])
{
   u32 v = bin->vcnt;
   u32 ioff = bin->icnt;
   vertex* vtx = bin->vertices;
   u32* idx = bin->indices;
   f32 hw = 0.5f * dim.x;
   f32 hh = 0.5f * dim.y;
   f32 hd = 0.5f * dim.z;
   f32 ox = origin.x;
   f32 oy = origin.y;
   f32 oz = origin.z;

   f32 l = ox - hw;
   f32 r = ox + hw;
   f32 f = oz - hd; // far
   f32 n = oz + hd; // near
   f32 b = oy - hh;
   f32 t = oy + hh;

   // 8 vertices
   // front face
   vtx[v+0].P = vec3(l, b, n);
   vtx[v+0].C = color[0];
   vtx[v+1].P = vec3(r, b, n);
   vtx[v+1].C = color[0];
   vtx[v+2].P = vec3(r, t, n);
   vtx[v+2].C = color[0];
   vtx[v+3].P = vec3(l, t, n);
   vtx[v+3].C = color[0];

   // back face
   vtx[v+4].P = vec3(r, b, f);
   vtx[v+4].C = color[1];
   vtx[v+5].P = vec3(l, b, f);
   vtx[v+5].C = color[1];
   vtx[v+6].P = vec3(l, t, f);
   vtx[v+6].C = color[1];
   vtx[v+7].P = vec3(r, t, f);
   vtx[v+7].C = color[1];

   // top face
   vtx[v+8].P = vec3(l, t, n);
   vtx[v+8].C = color[2];
   vtx[v+9].P = vec3(l, t, f);
   vtx[v+9].C = color[2];
   vtx[v+10].P = vec3(r, t, f);
   vtx[v+10].C = color[2];
   vtx[v+11].P = vec3(r, t, n);
   vtx[v+11].C = color[2];

   // bottom face
   vtx[v+12].P = vec3(l, b, n);
   vtx[v+12].C = color[3];
   vtx[v+13].P = vec3(r, b, n);
   vtx[v+13].C = color[3];
   vtx[v+14].P = vec3(r, b, f);
   vtx[v+14].C = color[3];
   vtx[v+15].P = vec3(l, b, f);
   vtx[v+15].C = color[3];

   // right face
   vtx[v+16].P = vec3(r, b, n);
   vtx[v+16].C = color[4];
   vtx[v+17].P = vec3(r, b, f);
   vtx[v+17].C = color[4];
   vtx[v+18].P = vec3(r, t, f);
   vtx[v+18].C = color[4];
   vtx[v+19].P = vec3(r, t, n);
   vtx[v+19].C = color[4];

   // left face
   vtx[v+20].P = vec3(l, b, n);
   vtx[v+20].C = color[5];
   vtx[v+21].P = vec3(l, b, f);
   vtx[v+21].C = color[5];
   vtx[v+22].P = vec3(l, t, f);
   vtx[v+22].C = color[5];
   vtx[v+23].P = vec3(l, t, n);
   vtx[v+23].C = color[5];

   static int cube_indices[] = {
      0, 1, 2, 0, 2, 3,
      4, 5, 6, 4, 6, 7,
      8, 9,10, 8,10,11,
     12,13,14,12,14,15,
     16,17,18,16,18,19,
     20,21,22,20,22,23
   };

   for (int i = 0; i < 36; i++) {
      idx[ioff + i] = v + cube_indices[i];
   }

   bin->vcnt += 24;
   bin->icnt += 36;
}

void addColored2DQuad(vertex_buffer* b, vec2 pll, vec2 pur, vec4 color)
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

void addColoredVertex(vertex_buffer* b, vec3 p, vec4 c)
{
   u32 i = b->vcnt++;
   b->vertices[i].P = p;
   b->vertices[i].C = c;
}

void drawBufferElements(vertex_buffer* tvb, GLenum prim)
{
   glBindVertexArray(tvb->vao);

   glBindBuffer(GL_ARRAY_BUFFER, tvb->buffers[0]);
   glBufferSubData(GL_ARRAY_BUFFER, 0, tvb->vcnt * sizeof(vertex), tvb->vertices);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tvb->buffers[1]);
   glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, tvb->icnt * sizeof(u32), tvb->indices);

   glDrawElements(prim, tvb->icnt, GL_UNSIGNED_INT, 0);

   tvb->vcnt = 0;
   tvb->icnt = 0;
}

void drawBuffer(vertex_buffer* tvb, GLenum prim)
{
   glBindVertexArray(tvb->vao);

   glBindBuffer(GL_ARRAY_BUFFER, tvb->buffers[0]);
   glBufferSubData(GL_ARRAY_BUFFER, 0, tvb->vcnt * sizeof(vertex), tvb->vertices);
   assertGL();

   glDrawArrays(prim, 0, tvb->vcnt);
   assertGL();

   tvb->vcnt = 0;
   tvb->icnt = 0;
}

#if 0
void drawColoredVertexTriangles(vertex_buffer* tvb)
{
   glBindVertexArray(tvb->vao);

   glBindBuffer(GL_ARRAY_BUFFER, tvb->buffers[0]);
   glBufferSubData(GL_ARRAY_BUFFER, 0, tvb->vcnt * sizeof(vertex), tvb->vertices);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tvb->buffers[1]);
   glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, tvb->icnt * sizeof(u32), tvb->indices);

   glDrawElements(GL_TRIANGLES, tvb->icnt, GL_UNSIGNED_INT, 0);

   tvb->vcnt = 0;
   tvb->icnt = 0;
}

void drawColoredVertexLineStrip(vertex_buffer* tvb)
{
   glBindVertexArray(tvb->vao);

   glBindBuffer(GL_ARRAY_BUFFER, tvb->buffers[0]);
   glBufferSubData(GL_ARRAY_BUFFER, 0, tvb->vcnt * sizeof(vertex), tvb->vertices);
   assertGL();

   glDrawArrays(GL_LINE_STRIP, 0, tvb->vcnt);
   assertGL();

   tvb->vcnt = 0;
   tvb->icnt = 0;
}

void drawTexturedVertexBuffer(vertex_buffer* tvb)
{
   glBindVertexArray(tvb->vao);

   glBindBuffer(GL_ARRAY_BUFFER, tvb->buffers[0]);
   glBufferData(GL_ARRAY_BUFFER, tvb->max * sizeof(vertex), 0, GL_STREAM_DRAW);
   glBufferSubData(GL_ARRAY_BUFFER, 0, tvb->vcnt * sizeof(vertex), tvb->vertices);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tvb->buffers[1]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, tvb->max * 2 * sizeof(u32), 0, GL_STREAM_DRAW);
   glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, tvb->icnt * sizeof(u32), tvb->indices);

   glDrawElements(GL_TRIANGLES, tvb->icnt, GL_UNSIGNED_INT, 0);

   tvb->vcnt = 0;
   tvb->icnt = 0;
}
#endif

void addTextured2DQuad(vertex_buffer* b, vec2 pll, vec2 pur, vec2 tll, vec2 tur)
{
   if ((b->vcnt + 6) > b->max) {
      drawBufferElements(b, GL_TRIANGLES);
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

vec4 RGBA(int r, int g, int b, int a = 255)
{
   return vec4(r/255.0f, g/255.0f, b/255.0f, a/255.0f);
}

height_map* loadHeightMap(const char* resource)
{
   u32 hmsize = 0;
   int hmwidth, hmheight, hmchannels;
   u8* heightmapfile = Platform.slurp(resource, &hmsize);

   hmchannels = 0;
   u8* heightmap = (u8*)stbi_load_from_memory((stbi_uc const*)heightmapfile, (int)hmsize, &hmwidth, &hmheight, &hmchannels, 1);

   height_map* result = (height_map*)malloc(sizeof(height_map));
   result->heights = heightmap;
   result->width = hmwidth;
   result->height = hmheight;

   return result;
}

void freeHeightMap(height_map* hm)
{
   stbi_image_free(hm->heights);
   free(hm);
}

void addGrid(vertex_buffer* b, height_map* hm, vec3 pt, f32 width, f32 height, int rows, int cols)
{
   f32 stepx = width / cols;
   f32 stepz = height / rows;
   vertex* v = b->vertices;
   u32* e = b->indices;
   vec4 color = RGBA(242, 236, 218);
   u8* heightmap = hm->heights;

   int vidx0 = b->vcnt;

   for (int r = 0; r < rows; r++) {
      for (int c = 0; c < cols; c++) {
         int idx = vidx0 + r * cols + c;
         u32 h = heightmap[r * cols + c];
         if (h != 0xff) {
            h += 0;
         }
         f32 y = 10 * (1.0f - h / 255.0f);
         v[idx].P = pt + vec3(c * stepx, y, r * stepz);
         v[idx].C = color;
      }
   }

   int i = b->icnt;
   for (int r = 0; r < rows-1; r++) {
      for (int c = 0; c < cols-1; c++) {
         e[i++] = vidx0 + (r+1)*cols + c;
         e[i++] = vidx0 + (r)*cols + c + 1;
         e[i++] = vidx0 + (r)*cols + c;
         e[i++] = vidx0 + (r+1)*cols + c;
         e[i++] = vidx0 + (r+1)*cols + c + 1;
         e[i++] = vidx0 + (r)*cols + c + 1;
      }
   }

   b->vcnt += rows * cols;
   b->icnt = i;
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

bool bindUniform(GLuint program, const char* location, int v)
{
   int p = glGetUniformLocation(program, location);
   glUseProgram(program);
   glUniform1i(p, v);
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

#include "font.cpp"

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
   return true; //wglMakeCurrent(context->dc, context->context);
}

// platform independent api

bool InitializeRenderer(renderer* rin)
{
   // load opengl api on intialize in case of dll reload
   LoadGL();

   if (rin->impl) {
      renderer_implementation* r = rin->impl;

      free_screen_font(r->TheFont);
      r->TheFont = 0;
      glDeleteTextures(1, &r->FontTexture);

      glDeleteProgram(r->fontProgram);
      glDeleteProgram(r->ColorProgram);
      freeVertexBuffer(r->FontVertexBuffer);
      freeVertexBuffer(r->Colors);

      glDeleteProgram(r->WorldProgram);
      glDeleteProgram(r->PickProgram);

      free(r->heightmap);

      free(r);
   }

   rin->impl = (renderer_implementation*)malloc(sizeof(renderer_implementation));
   renderer_implementation *r = rin->impl;
   screen_viewport* vp = &rin->viewport;



   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


   r->TheFont = createFont(512, 512, "monaco.ttf");
   r->FontTexture = createTexture((u8*)r->TheFont->textureData, 512, 512, 1, GL_RED);
   r->fontProgram = compileShader("font.vert", "font.frag");
   r->FontVertexBuffer = createVertexBuffer(512);

   r->ColorProgram = compileShader("solid");
   r->Colors = createVertexBuffer(2048);

   r->WorldProgram = compileShader("solid3d");
   r->worldProj = Perspective(45.0f, (f32)vp->width / (f32)vp->height, 0.1f, 500.0f);

   r->PickProgram = compileShader("pick");

   r->uiProj = Ortho2D(0, (f32)vp->width, 0, (f32)vp->height, -1, 1);
   bindUniform(r->fontProgram, "proj", r->uiProj);
   bindUniform(r->fontProgram, "color", vec4(1, 1, 1, 1));

   bindUniform(r->ColorProgram, "proj", Ortho2D(0, (f32)vp->width, (f32)vp->height, 0, -1, 1));
   //bindUniform(r->ColorProgram, "view", IdentityMatrix());
   bindUniform(r->ColorProgram, "view", PlotView(-10, 10, -1.5, 1.5, 502, 502, 200, 100));

   r->heightmap = loadHeightMap("heightmaptest.bmp");
   r->heighttexture = createTexture("heightmaptest.bmp");

   return true;
}

bool ResizeWindow(renderer* rin, int width, int height)
{
   renderer_implementation* r = rin->impl;
   screen_viewport* vp = &rin->viewport;

   vp->width = width;
   vp->height = height;

   r->worldProj = Perspective(45.0f, (f32)vp->width / (f32)vp->height, 0.1f, 500.0f);
   r->uiProj = Ortho2D(0, (f32)vp->width, 0, (f32)vp->height, -1, 1);
   bindUniform(r->fontProgram, "proj", r->uiProj);

   bindUniform(r->ColorProgram, "proj", Ortho2D(0, (f32)vp->width, (f32)vp->height, 0, -1, 1));

   glViewport(0, 0, width, height);

   return true;
}

struct start_stack
{
   int id;
   u64 mark;
};

const char* button_names[] = {
   "NOTAKEY",
   "FORWARD",
   "BACK",
   "LEFT",
   "RIGHT",
   "UP",
   "DOWN"
};

void PickWorld(renderer* rin, game_state* state)
{
   timed_function();

   renderer_implementation* r = rin->impl;
   game_world* world = &state->world;

   // world pass
   world_camera* camera = &world->camera;

   glEnable(GL_DEPTH_TEST);
   glDisable(GL_BLEND);

   vec3 dir = directionFromPitchYaw(camera->pitch, camera->yaw);

   mat4 view = LookAt(camera->p, camera->p + dir, vec3(0, 1, 0));

   //mat4 view = rotationY(camera->yaw) * rotationX(camera->pitch) * translationMatrix(camera->p);

   glUseProgram(r->PickProgram);
   bindUniform(r->PickProgram, "proj", r->worldProj);
   bindUniform(r->PickProgram, "view", view);

   vec4 colors[6] = {
      vec4(1, 0, 0, 1),
      vec4(0, 1, 0, 1),
      vec4(0, 0, 1, 1),
      vec4(1, 1, 0, 1),
      vec4(1, 0, 1, 1),
      vec4(1, 0.5, 0.5, 1),
   };
   for (u32 i = 0; i < arraycount(world->cubes); i++)
   {
      cube* c = world->cubes + i;
      addColoredCube(r->Colors, vec3(0, 0, 0), vec3(1, 1, 1), colors);
      bindUniform(r->PickProgram, "model", translationMatrix(c->p) * scaleMatrix(c->dim) * rotationY(c->angle));
      bindUniform(r->PickProgram, "code", i + 1);
      drawBufferElements(r->Colors, GL_TRIANGLES);
   }

   vec2 mp = state->current_input.mouse_p;
   u32 res = 0;
   glReadPixels((int)mp.x, rin->viewport.height - (int)mp.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &res);

   state->world.picked_cube = res & 0xff;
}

void RenderWorld(renderer* rin, game_state* state)
{
   timed_function();

   renderer_implementation* r = rin->impl;
   game_world* world = &state->world;

   // world pass
   world_camera* camera = &world->camera;

   glEnable(GL_DEPTH_TEST);
   glDisable(GL_BLEND);

   vec3 dir = directionFromPitchYaw(camera->pitch, camera->yaw);

   mat4 view = LookAt(camera->p, camera->p + dir, vec3(0, 1, 0));

   //mat4 view = rotationY(camera->yaw) * rotationX(camera->pitch) * translationMatrix(camera->p);

   glUseProgram(r->WorldProgram);
   bindUniform(r->WorldProgram, "proj", r->worldProj);
   bindUniform(r->WorldProgram, "view", view);
   bindUniform(r->WorldProgram, "model", rotationY(state->angle));

   vec4 colors[6] = {
      vec4(1, 0, 0, 1),
      vec4(0, 1, 0, 1),
      vec4(0, 0, 1, 1),
      vec4(1, 1, 0, 1),
      vec4(1, 0, 1, 1),
      vec4(1, 0.5, 0.5, 1),
   };

   vec4 pcolors[6] = {
      vec4(1, 0, 1, 1),
      vec4(1, 0, 0, 1),
      vec4(1, 0, 0, 1),
      vec4(1, 0, 0, 1),
      vec4(1, 0, 0, 1),
      vec4(1, 0, 0, 1),
   };

   for (u32 i = 0; i < arraycount(world->cubes); i++)
   {
      vec4* clrs = colors;
      if (i + 1 == state->world.picked_cube) {
         clrs = pcolors;
      }
      cube* c = world->cubes + i;
      addColoredCube(r->Colors, vec3(0, 0, 0), vec3(1, 1, 1), clrs);
      bindUniform(r->WorldProgram, "model", translationMatrix(c->p) * scaleMatrix(c->dim) * rotationY(c->angle));
      drawBufferElements(r->Colors, GL_TRIANGLES);
   }

   //addColored2DQuad(r->Colors, vec2(-10, -10), vec2(10, 10), vec4(0.8f, 0.2f, 0.2f, 1));

   glDeleteTextures(1, &r->heighttexture);
   freeHeightMap(r->heightmap);

   r->heightmap = loadHeightMap("heightmaptest.bmp");
   r->heighttexture = createTexture("heightmaptest.bmp");

   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   addGrid(r->Colors, r->heightmap, vec3(-20, -1, -20), 40.0f, 40.0f, 32, 32);

   bindUniform(r->WorldProgram, "model", IdentityMatrix());
   drawBufferElements(r->Colors, GL_TRIANGLES);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


   glDisable(GL_DEPTH);
}

void RenderUI(renderer* rin, game_state* state)
{
   timed_function();

   renderer_implementation* r = rin->impl;
   game_world* world = &state->world;
   world_camera* camera = &world->camera;

   // ui pass
   vertex_buffer* t = r->FontVertexBuffer;
   screen_font_size* f = findFontSize(r->TheFont, 9);

   f32 y = f->size;
   f32 x = 50;
   vec3 dir = directionFromPitchYaw(camera->pitch, camera->yaw);

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

   y += f->size;
   x = 5;
   x += drawString(t, f, x, y, "Camera P: ");
   x += drawVec3(t, f, x, y, camera->p);

   y += f->size;
   x = 5;
   x += drawString(t, f, x, y, "Camera V: ");
   x += drawVec3(t, f, x, y, camera->v);

   y += f->size;
   x = 5;
   x += drawString(t, f, x, y, "Camera Dir: ");
   x += drawVec3(t, f, x, y, dir);

   y += f->size;
   x = 5;
   x += drawString(t, f, x, y, "Camera pitch/yaw: ");
   x += drawFloat(t, f, x, y, camera->pitch, 2);
   x += drawString(t, f, x, y, "/");
   x += drawFloat(t, f, x, y, camera->yaw, 2);

   y += f->size;
   x = 5;
   x += drawString(t, f, x, y, "Mouse P: ");
   x += drawVec2(t, f, x, y, state->current_input.mouse_p);
   x += drawVec2(t, f, x, y, state->current_input.mouse_dp);

   y += f->size;
   x = 5;
   x += drawString(t, f, x, y, "Buttons: [");
   int downCount = 0;
   for (int i = 0; i < BUTTON_MAX_DEFINED; i++)
   {
      if (state->current_input.buttons_down[i]) {
         if (downCount) {
            x += drawString(t, f, x, y, ",");
         }
         x += drawString(t, f, x, y,  button_names[i]);
         downCount++;
      }
   }
   x += drawString(t, f, x, y, "]");
   drawBufferElements(r->FontVertexBuffer, GL_TRIANGLES);

   y += f->size;
   x = 5;
   x += drawString(t, f, x, y, "Letters: [");
   downCount = 0;
   for (int i = 0; i < 256; i++)
   {
      if (state->current_input.letters_down[i]) {
         if (downCount) {
            x += drawChar(t, f, ',', x, y);
         }
         x += drawChar(t, f, i, x, y);
         downCount++;
      }
   }
   x += drawString(t, f, x, y, "]");
   drawBufferElements(r->FontVertexBuffer, GL_TRIANGLES);

   y += f->size;
   x = 5;
   x += drawString(t, f, x, y, "Clicked: [");
   downCount = 0;
   for (int i = 0; i < MOUSE_BUTTON_MAX; i++)
   {
      if (state->current_input.mouse_buttons_click[i]) {
         if (downCount) {
            x += drawChar(t, f, ',', x, y);
         }
         x += drawChar(t, f, '0' + i, x, y);
         downCount++;
      }
   }
   x += drawString(t, f, x, y, "]");

   y += f->size;
   x = 5;
   x += drawString(t, f, x, y, "Picked: ");
   x += drawInt(t, f, x, y, world->picked_cube);

   drawBufferElements(r->FontVertexBuffer, GL_TRIANGLES);

   glBindTexture(GL_TEXTURE_2D, r->heighttexture);
   addTextured2DQuad(r->FontVertexBuffer, vec2(5, y+50), vec2(55, y+100), vec2(0, 0), vec2(1, 1));
   drawBufferElements(r->FontVertexBuffer, GL_TRIANGLES);
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

void RenderFrame(renderer* rin, game_state* state)
{
   GlobalDebug = state->DebugSystem;

   glFinish();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   PickWorld(rin, state);

   if (!state->draw_pick_buffer) {
      glClearColor(0.2f, 0.2f, 0.2f, 1.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      RenderWorld(rin, state);
   }

   RenderUI(rin, state);
}

}

