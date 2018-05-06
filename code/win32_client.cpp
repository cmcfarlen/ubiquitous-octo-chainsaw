#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>

#include <stdio.h>

#include <gl/GL.h>
#include <strsafe.h>

#include "types.h"

HDC gHdc;

void log(const char* fmt, ...)
{
	char buff[512];
	va_list args;

	va_start(args, fmt);

	StringCbVPrintfA(buff, sizeof(buff), fmt, args);
	va_end(args);

	OutputDebugStringA(buff);
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

typedef void(*render_t)(float);
typedef void(*initialize_t)(int, int); // width, height of the window

static render_t render = NULL;
static initialize_t initialize = NULL;
static unsigned long long last_mode_time = 0;
static HMODULE render_dll = NULL;

static const char* full_path_to_dll = "game.dll";
static const char* full_path_to_dll_copy = "game_copy.dll";

// load the game code dll
void loadGameCode(void)
{
	unsigned long long now = getModifyTime(full_path_to_dll);

	if (now != last_mode_time) {
		if (render_dll) {
			log("Unloading library.\n");
			if (FreeLibrary(render_dll) == 0) {
				log("Error freeing library: %i\n", GetLastError());
			}
			render_dll = NULL;
		}

		log("loading game code\n");
		for (int tries = 0; tries < 100; tries++) {
			if (CopyFileA(full_path_to_dll, full_path_to_dll_copy, FALSE)) {
				log("loading renderer from %s\n", full_path_to_dll_copy);
				HMODULE dll = LoadLibraryA(full_path_to_dll_copy);

				if (dll == NULL) {
					DWORD err = GetLastError();
					log("Got error %i\n", err);
				}

				render = (render_t)GetProcAddress(dll, "render");
				initialize = (initialize_t)GetProcAddress(dll, "initialize");

				if (!render) {
					log("Failed to get proc address for render");
				}

				if (!initialize) {
					log("Failed to get proc address for initialize");
				}

				last_mode_time = now;
				render_dll = dll;
				break;
			}
			else {
				Sleep(100);
			}
		}

		if (render_dll == NULL) {
			log("Failed to copy dll %i\n", GetLastError());
		}
		
	}
	
}

BOOL bSetupPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd, *ppfd;
	int pixelformat;

	ppfd = &pfd;

	ppfd->nSize = sizeof(PIXELFORMATDESCRIPTOR);
	ppfd->nVersion = 1;
	ppfd->dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	ppfd->dwLayerMask = PFD_MAIN_PLANE;
	ppfd->iPixelType = PFD_TYPE_COLORINDEX;
	ppfd->cColorBits = 8;
	ppfd->cDepthBits = 16;
	ppfd->cAccumBits = 0;
	ppfd->cStencilBits = 0;

	pixelformat = ChoosePixelFormat(hdc, ppfd);

	if ((pixelformat = ChoosePixelFormat(hdc, ppfd)) == 0)
	{
		MessageBoxW(NULL, L"ChoosePixelFormat failed", L"Error", MB_OK);
		return FALSE;
	}

	if (SetPixelFormat(hdc, pixelformat, ppfd) == FALSE)
	{
		MessageBoxW(NULL, L"SetPixelFormat failed", L"Error", MB_OK);
		return FALSE;
	}

	return TRUE;
}

void initializeGL()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	

	glClear(GL_COLOR_BUFFER_BIT);
}

screen_font* checkFontStuff(HDC dcIn)
{
   HDC dc = CreateCompatibleDC(dcIn);
   HBITMAP bitmap = CreateCompatibleBitmap(dc, 512, 512);
   BITMAP data;
   const char* fontName = "Small Fonts Regular"; //   "Droid Sans Mono Slashed"

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

      /*
      int cnt = GetTextFace(dc, 0, NULL);
      char* textFace = (char*)malloc(cnt);
      GetTextFace(dc, cnt, textFace);
      log("text face %s\n", textFace);
      */

      TEXTMETRIC metrics;
      GetTextMetrics(dc, &metrics);

      // for every character, get the size and write to bitmap
      for (char c = firstChar; c < lastChar; c++) {
         SIZE charSize;
         GetTextExtentPoint32(dc, &c, 1, &charSize);
         // when we lookup a char in entries, subtract firstChar to get the index into the entries array
         int idx = c - firstChar;

         if ((X + charSize.cy) > 512) {
            X = 0;
            Y += metrics.tmHeight;
         }

         ps->entries[idx].codePoint = c;
         ps->entries[idx].u = X;
         ps->entries[idx].v = 512 - Y - charSize.cy; // convert to opengl coords (origin in lower left)
         ps->entries[idx].w = charSize.cx;
         ps->entries[idx].h = charSize.cy;

         TextOut(dc, X, Y, &c, 1);

         X += charSize.cx;
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

   GetDIBits(dc, bitmap, 0, 512, imageData,(BITMAPINFO*)&bi, DIB_RGB_COLORS);

   HANDLE f = CreateFile("test.bmp", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

   DWORD totalSize = bmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

   header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
   header.bfSize = totalSize;
   header.bfType = 0x4D42;

   DWORD byteCount = 0;
   WriteFile(f, (char*)&header, sizeof(BITMAPFILEHEADER), &byteCount, NULL);
   WriteFile(f, (char*)&bi, sizeof(BITMAPINFOHEADER), &byteCount, NULL);
   WriteFile(f, imageData, bmpSize, &byteCount, NULL);

   free(imageData);
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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_CREATE:
	{
		HDC dc = gHdc = GetDC(hWnd);

		bSetupPixelFormat(dc);

		HGLRC glContext = wglCreateContext(dc);

		wglMakeCurrent(dc, glContext);

		initializeGL();

		RECT rect;
		GetClientRect(hWnd, &rect);

      if (initialize) {
		   initialize(rect.right, rect.bottom);
      }

      checkFontStuff(dc);
      break;
	}
	case WM_SIZE:
	{
		RECT rect;
		GetClientRect(hWnd, &rect);

		log("size window to: [%i %i]\n", rect.right, rect.bottom);

      if (initialize) {
		   initialize(rect.right, rect.bottom);
      }
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

	wcex.style = CS_HREDRAW | CS_VREDRAW;
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

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE,
	_In_ LPWSTR,
	_In_ int       nCmdShow)
{

	// load game code first so the window can be initialized
	loadGameCode();

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
		loadGameCode();
		QueryPerformanceCounter(&now);
		LONGLONG diff = now.QuadPart - prev.QuadPart;

		float dt = (float)diff / (float)perffreq.QuadPart;
      if (render) {
		   render(dt);
      }

		SwapBuffers(gHdc);
		glFinish();

		frame++;
		if ((frame % 100) == 0) {
			log("frame %i (%2.2f fps)\n", frame, frame / dt);
		}
	}

   return (int) msg.wParam;
}



