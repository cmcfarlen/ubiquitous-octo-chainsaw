#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>
#include <assert.h>

#include <stdio.h>

#include <strsafe.h>

//#include "types.h"
//#include "game.h"
//

#include "win32_opengl.h"
#include "renderer.h"
#include "platform.h"

void log(const char* fmt, ...)
{
	char buff[512];
	va_list args;

	va_start(args, fmt);

	StringCbVPrintfA(buff, sizeof(buff), fmt, args);
	va_end(args);

	OutputDebugStringA(buff);
}

u8* slurp(const char* resource, u32* size)
{
   char path[512];

   StringCbPrintfA(path, sizeof(path), "..\\data\\%s", resource);

   HANDLE h = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
   LARGE_INTEGER fs = {};

   GetFileSizeEx(h, &fs);

   *size = fs.LowPart;
   u32 toRead = *size;
   u32 bc = 0;
   u8* data = (u8*)malloc(*size);

   ReadFile(h, data, toRead, &bc, 0);

   assert(toRead == bc);

   CloseHandle(h);

   return data;
}

unsigned long long getModifyTime(const char* f)
{
	FILETIME mtime;
	HANDLE h = CreateFileA(f,
		GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, 0, NULL);

	GetFileTime(h, NULL, NULL, &mtime);

	CloseHandle(h);

	ULARGE_INTEGER i;
	i.HighPart = mtime.dwHighDateTime;
	i.LowPart = mtime.dwLowDateTime;
	
	return i.QuadPart;
}

typedef struct dll_watch
{
   const char* path;
   const char* pathCopy;
   const char* pathLock;
   unsigned long long lastModifyTime;
   HMODULE handle;
} dll_watch;

// Maybe load a watched dll, return true if it was reloaded
bool loadDll(dll_watch* watch)
{
	unsigned long long now = getModifyTime(watch->path);

	if (now != watch->lastModifyTime) {
		if (watch->handle) {
			log("Unloading library.\n");
			if (FreeLibrary(watch->handle) == 0) {
				log("Error freeing library: %i\n", GetLastError());
			}
			watch->handle = NULL;
		}

		log("loading dll: %s\n", watch->path);
		for (int tries = 0; tries < 100; tries++) {
         WIN32_FILE_ATTRIBUTE_DATA Data;

         if (GetFileAttributesEx(watch->pathLock, GetFileExInfoStandard, &Data) == 0) {
            CopyFileA(watch->path, watch->pathCopy, FALSE);
				HMODULE dll = LoadLibraryA(watch->pathCopy);

				if (dll == NULL) {
					DWORD err = GetLastError();
					log("Got error %i\n", err);
				}

            watch->lastModifyTime = getModifyTime(watch->path);
            watch->handle = dll;
				break;
			}
			else {
				Sleep(100);
			}
		}

      assert(watch->handle);
		
      return true;
	}
	
   return false;
}

#if 0
screen_font* checkFontStuff(HDC dcIn, int Width, int Height, const char* fontName)
{
   HDC dc = CreateCompatibleDC(dcIn);
   HBITMAP bitmap = CreateCompatibleBitmap(dc, Width, Height);
   BITMAP data;

   SelectObject(dc, bitmap);
   int fontSizes[] = {12, 18, 24, 36, 0};
   char firstChar = 0x21;
   char lastChar = 0x7e;
   int X = 0;
   int Y = 0;

   // TODO(dad): don't use malloc
   screen_font* result = (screen_font*)malloc(sizeof(screen_font));

   result->fontName = _strdup(fontName);
   result->firstChar = firstChar;
   result->lastChar = lastChar;
   result->numSizes = 4;
   result->sizes = (screen_font_size*)malloc(sizeof(screen_font_size) * 4);

   //SetBkMode(dc, TRANSPARENT);
   SetBkMode(dc, RGB(0, 0, 0));
   SetTextColor(dc, RGB(0xff, 0xff, 0xff));

   for (int i = 0; fontSizes[i]; i++) {
      HFONT font = CreateFont(fontSizes[i], 0, 0, 0, 
            FW_BOLD, 0, 0, 0,
            DEFAULT_CHARSET,
            OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS,
            PROOF_QUALITY,
            FF_MODERN,
            fontName);

      SelectObject(dc, font);

      screen_font_size* ps = result->sizes + i;

      ps->size = fontSizes[i];
      ps->entries = (screen_font_entry*)malloc(sizeof(screen_font_entry) * (lastChar - firstChar + 1));

      TEXTMETRIC metrics;
      GetTextMetrics(dc, &metrics);

      // for every character, get the size and write to bitmap
      for (char c = firstChar; c < lastChar; c++) {
         SIZE charSize;
         GetTextExtentPoint32(dc, &c, 1, &charSize);
         // when we lookup a char in entries, subtract firstChar to get the index into the entries array
         int idx = c - firstChar;

         if ((X + charSize.cy) > Width) {
            X = 0;
            Y += metrics.tmHeight;
         }
         f32 u = X / (f32)Width;
         f32 v = (Height - Y - charSize.cy) / (f32)Height; // convert to opengl coords (origin in lower left)

         int advance = charSize.cx + 2;

         ps->entries[idx].codePoint = c;
         ps->entries[idx].u1 = u;
         ps->entries[idx].v1 = v;
         ps->entries[idx].u2 = u + charSize.cx / (f32)Width;
         ps->entries[idx].v2 = v + charSize.cy / (f32)Height;
         ps->entries[idx].w = charSize.cx;
         ps->entries[idx].h = charSize.cy;

         TextOut(dc, X, Y, &c, 1);

         X += advance;
      }

      X = 0;
      Y += metrics.tmHeight;
   }

   // write bitmap to file
   GetObject(bitmap, sizeof(BITMAP), &data);

   BITMAPFILEHEADER header = {};
   BITMAPINFOHEADER bi = {};

   bi.biSize = sizeof(BITMAPINFOHEADER);
   bi.biWidth = data.bmWidth;
   bi.biHeight = data.bmHeight;
   bi.biPlanes = 1;
   bi.biBitCount = 32;
   bi.biCompression = BI_RGB;

   DWORD bmpSize = ((data.bmWidth * bi.biBitCount + 31) / 32) * 4 * data.bmHeight;
   char* imageData = (char*)malloc(bmpSize);

   GetDIBits(dc, bitmap, 0, Width, imageData,(BITMAPINFO*)&bi, DIB_RGB_COLORS);

   HANDLE f = CreateFile("test.bmp", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

   DWORD totalSize = bmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

   header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
   header.bfSize = totalSize;
   header.bfType = 0x4D42;

   DWORD byteCount = 0;
   WriteFile(f, (char*)&header, sizeof(BITMAPFILEHEADER), &byteCount, NULL);
   WriteFile(f, (char*)&bi, sizeof(BITMAPINFOHEADER), &byteCount, NULL);
   WriteFile(f, imageData, bmpSize, &byteCount, NULL);

   CloseHandle(f);

   DeleteObject(bitmap);
   DeleteObject(dc);

   result->textureData = imageData;
   return result;
}

void free_screen_font(screen_font* f)
{
   for (int s = 0; s < f->numSizes; s++) {
      free(f->sizes[s].entries);
   }
   free(f->sizes);
   free(f->textureData);
   free(f);
}
#endif

Win32SetupRenderContext_t Win32SetupRenderContext = 0;
Win32SelectRenderContext_t Win32SelectRenderContext = 0;

platform_api Platform = { log, slurp };

// global renderer
static renderer_api RenderAPI = {};
static win32_opengl_render_context OpenGLContext = {};
static renderer Renderer = {};

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_CREATE:
	{

      Win32SetupRenderContext(hWnd, &Platform, &OpenGLContext);
      Win32SelectRenderContext(&OpenGLContext);

      RenderAPI.InitializeRenderer(&Renderer);

#if 0
      GlobalState.TheFont = checkFontStuff(dc, 512, 512, "Droid Sans Mono Slashed"); //"Small Fonts Regular");
#endif

		RECT rect;
		GetClientRect(hWnd, &rect);

      RenderAPI.ResizeWindow(&Renderer, rect.right, rect.bottom);

      break;
	}
	case WM_SIZE:
	{
		RECT rect;
		GetClientRect(hWnd, &rect);

		log("size window to: [%i %i]\n", rect.right, rect.bottom);

      RenderAPI.ResizeWindow(&Renderer, rect.right, rect.bottom);
      break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
      break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

BOOL WindowsSetup(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEXW wcex = {};

	LPCWSTR szTitle = L"GAME";
	LPCWSTR szWindowClass = L"GAME";

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL; // LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAME));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = NULL; // LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassExW(&wcex);

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

void loadRenderAPI(renderer_api* api, HMODULE dll)
{
   Win32SetupRenderContext = (Win32SetupRenderContext_t)GetProcAddress(dll, "Win32SetupRenderContext");
   Win32SelectRenderContext = (Win32SelectRenderContext_t)GetProcAddress(dll, "Win32SelectRenderContext");

   api->InitializeRenderer = (InitializeRenderer_t)GetProcAddress(dll, "InitializeRenderer");
   api->ResizeWindow = (ResizeWindow_t)GetProcAddress(dll, "ResizeWindow");
   api->RenderFrame = (RenderFrame_t)GetProcAddress(dll, "RenderFrame");
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE,
	_In_ LPWSTR,
	_In_ int       nCmdShow)
{

   dll_watch renderWatch = {"win32_opengl.dll", "win32_opengl_copy.dll", "render_lock.tmp", 0, 0};

	// load game code first so the window can be initialized
   loadDll(&renderWatch);
   loadRenderAPI(&RenderAPI, renderWatch.handle);

	WindowsSetup(hInstance, nCmdShow);

	MSG msg;

	LARGE_INTEGER perffreq;
	LARGE_INTEGER now;
	LARGE_INTEGER prev;
	QueryPerformanceFrequency(&perffreq);
	QueryPerformanceCounter(&prev);
	
	int running = 1;
	long frame = 0;
	while (running)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				running = 0;
			}
			DispatchMessage(&msg);
		}
		// maybe load the game code
      if (loadDll(&renderWatch)) {
         log("Reloading Renderer\n");
         loadRenderAPI(&RenderAPI, renderWatch.handle);
         Win32SelectRenderContext(&OpenGLContext);
         RenderAPI.InitializeRenderer(&Renderer);
      }

		QueryPerformanceCounter(&now);
		LONGLONG diff = now.QuadPart - prev.QuadPart;
		float dt = (float)diff / (float)perffreq.QuadPart;

      RenderAPI.RenderFrame(&Renderer);
		SwapBuffers(OpenGLContext.dc);

		frame++;
		if ((frame % 100) == 0) {
			log("frame %i (%2.2f fps)\n", frame, frame / dt);
		}
	}

   return (int) msg.wParam;
}

