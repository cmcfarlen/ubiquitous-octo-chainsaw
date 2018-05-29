#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <tchar.h>
#include <math.h>
#include <assert.h>

#include <stdio.h>

#include <strsafe.h>

#include "win32_opengl.h"
#include "renderer.h"
#include "platform.h"
#include "memory.h"

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
   u8* data = (u8*)malloc(*size + 1);

   ReadFile(h, data, toRead, &bc, 0);
   data[*size] = 0;

   assert(toRead == bc);

   CloseHandle(h);

   return data;
}

u32 pageSize(void)
{
   SYSTEM_INFO info = {};
   GetSystemInfo(&info);
   return info.dwPageSize;
}

u32 processorCount(void)
{
   SYSTEM_INFO info = {};
   GetSystemInfo(&info);
   return info.dwNumberOfProcessors;
}

void* allocateMemory(size_t bytes)
{
   // TODO(dad): need to check to see if we are wasting a lot of memory
   void* p = VirtualAlloc(0, bytes, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
   assert(GetLastError() == 0);
   return p;
}

void freeMemory(void* p)
{
   VirtualFree(p, 0, MEM_RELEASE);
   assert(GetLastError() == 0);
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

struct dll_watch
{
   const char* path;
   const char* pathCopy;
   const char* pathLock;
   unsigned long long lastModifyTime;
   HMODULE handle;
};

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

Win32SetupRenderContext_t Win32SetupRenderContext = 0;
Win32SelectRenderContext_t Win32SelectRenderContext = 0;

platform_api Platform = { log, slurp, allocateMemory, freeMemory, pageSize, processorCount };
debug_system* GlobalDebug = 0;

// global renderer
static renderer_api RenderAPI = {};
static game_api GameAPI = {};
static win32_opengl_render_context OpenGLContext = {};
static renderer Renderer = {};
static game_state* GameState = 0;
static game_input GlobalInput = {};
static u8 keymappings[256];

void setupKeymappings(u8* map)
{
   map[0+'W'] = BUTTON_FORWARD;
   map[0+'A'] = BUTTON_LEFT;
   map[0+'S'] = BUTTON_BACK;
   map[0+'D'] = BUTTON_RIGHT;
   map[0+'Q'] = BUTTON_DOWN;
   map[0+'E'] = BUTTON_UP;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_CREATE:
	{
      Win32SetupRenderContext(hWnd, &Platform, &OpenGLContext);
      Win32SelectRenderContext(&OpenGLContext);

      RenderAPI.InitializeRenderer(&Renderer);

		RECT rect;
		GetClientRect(hWnd, &rect);

      RenderAPI.ResizeWindow(&Renderer, rect.right, rect.bottom);
      GameAPI.InitializeGame(GameState);

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
   case WM_KEYDOWN:
   {
      // wParam is the keycode, map it to the game button

	   u8 button = keymappings[wParam];

      if (button) {
         GlobalInput.buttons_down[button] = 1;
      }
      GlobalInput.letters_down[wParam] = 1;
      break;
   }
   case WM_KEYUP:
   {
      u8 button = keymappings[wParam];

      if (button) {
         GlobalInput.buttons_down[button] = 0;
      }
      GlobalInput.letters_down[wParam] = 0;
      break;
   }
   case WM_LBUTTONDOWN:
   {
      GlobalInput.mouse_buttons_down[MOUSE_BUTTON1] = 1;
      break;
   }
   case WM_LBUTTONUP:
   {
      GlobalInput.mouse_buttons_down[MOUSE_BUTTON1] = 0;
      break;
   }
   case WM_RBUTTONDOWN:
   {
      GlobalInput.mouse_buttons_down[MOUSE_BUTTON2] = 1;
      break;
   }
   case WM_RBUTTONUP:
   {
      GlobalInput.mouse_buttons_down[MOUSE_BUTTON2] = 0;
      break;
   }
   case WM_MBUTTONDOWN:
   {
      GlobalInput.mouse_buttons_down[MOUSE_BUTTON3] = 1;
      break;
   }
   case WM_MBUTTONUP:
   {
      GlobalInput.mouse_buttons_down[MOUSE_BUTTON3] = 0;
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

void loadGameAPI(game_api* api, HMODULE dll)
{
   api->UpdateGameState = (UpdateGameState_t)GetProcAddress(dll, "UpdateGameState");
   api->InitializeGame = (InitializeGame_t)GetProcAddress(dll, "InitializeGame");
}

double measure_debug_ticks()
{
   u64 clocks = 0;
   u64 time = 0;
   u64 c1, c2;
   LARGE_INTEGER freq;
   LARGE_INTEGER startTime;
   LARGE_INTEGER endTime;

   QueryPerformanceFrequency(&freq);


   QueryPerformanceCounter(&startTime);
   c1 = debug_clocks();
   Sleep(1);
   QueryPerformanceCounter(&endTime);
   c2 = debug_clocks();
   clocks = c2 - c1;

   time = endTime.QuadPart - startTime.QuadPart;

   double hpsecs = (double)time / (double)freq.QuadPart;

   return (double) clocks /  hpsecs;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE,
	_In_ LPWSTR,
	_In_ int       nCmdShow)
{


   memory_region* test_region = createMemoryRegion(32);

   for (int i = 0; i < 10; i++) {
      allocateFromRegion(test_region, 5);
   }

   truncateRegion(test_region);

   for (int i = 0; i < 10; i++) {
      allocateFromRegion(test_region, 5);
   }

   allocateFromRegion(test_region, 500);

   freeRegion(test_region);

   dll_watch renderWatch = {"win32_opengl.dll", "win32_opengl_copy.dll", "render_lock.tmp", 0, 0};
   dll_watch gameWatch = {"game.dll", "game_copy.dll", "lock.tmp", 0, 0};
   memory_region* permanentRegion = createMemoryRegion();

	// load game code first so the window can be initialized
   loadDll(&gameWatch);
   loadGameAPI(&GameAPI, gameWatch.handle);

   GameState = (game_state*)allocateFromRegion(permanentRegion, sizeof(game_state));
   GameState->Platform = Platform;

   GlobalDebug = (debug_system*)allocateFromRegion(permanentRegion, sizeof(debug_system));
   initializeDebugSystem(GlobalDebug);


   double dtps = 0;
   for (int i = 0; i < 10; i++) {
      dtps += measure_debug_ticks();
   }

   dtps /= 10.0;
   GlobalDebug->count_per_second = dtps;

   log("debug ticks: %llf\n", dtps);
   GameState->DebugSystem = GlobalDebug;

   loadDll(&renderWatch);
   loadRenderAPI(&RenderAPI, renderWatch.handle);

	WindowsSetup(hInstance, nCmdShow);

	MSG msg;

	LARGE_INTEGER perffreq;
	LARGE_INTEGER now;
	LARGE_INTEGER prev;
	QueryPerformanceFrequency(&perffreq);
	QueryPerformanceCounter(&prev);

   // read initial cursor position
   POINT cursorPt;

   GetCursorPos(&cursorPt);
   GlobalInput.mouse_p = vec2((f32)cursorPt.x, (f32)cursorPt.y);
   GlobalInput.mouse_dp = vec2(0, 0);
   setupKeymappings(keymappings);
	
	int running = 1;
	while (running)
	{
      {
         timed_block("winmessages");
         while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
         {
            if (msg.message == WM_QUIT)
            {
               running = 0;
            }
            DispatchMessage(&msg);
         }
      }

		// maybe load the game code
      if (loadDll(&renderWatch)) {
         log("Reloading Renderer\n");
         loadRenderAPI(&RenderAPI, renderWatch.handle);
         Win32SelectRenderContext(&OpenGLContext);
         RenderAPI.InitializeRenderer(&Renderer);
         RenderAPI.ResizeWindow(&Renderer, Renderer.viewport.width, Renderer.viewport.height);
      }

      if (loadDll(&gameWatch)) {
         log("Reloading Game code\n");
         loadGameAPI(&GameAPI, gameWatch.handle);
      }

      {
         timed_block("updateandrender");
         QueryPerformanceCounter(&now);
         GetCursorPos(&cursorPt);

         LONGLONG diff = now.QuadPart - prev.QuadPart;
         prev = now;
         float dt = (float)diff / (float)perffreq.QuadPart;
         vec2 mouse_p = vec2((f32)cursorPt.x, (f32)cursorPt.y);

         GlobalInput.dt = dt;

         GlobalInput.mouse_dp = mouse_p - GlobalInput.mouse_p;
         GlobalInput.mouse_p = mouse_p;

         GameAPI.UpdateGameState(GameState, &GlobalInput);

         RenderAPI.RenderFrame(&Renderer, GameState);

      }
      SwapBuffers(OpenGLContext.dc);

      swapDebugFrame(GlobalDebug);

	}

   return (int) msg.wParam;
}

