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

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE,
	_In_ LPWSTR,
	_In_ int       nCmdShow)
{

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
	
	int running = 1;
	long frame = 0;
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
         LONGLONG diff = now.QuadPart - prev.QuadPart;
         float dt = (float)diff / (float)perffreq.QuadPart;

         GameAPI.UpdateGameState(GameState, dt);

         RenderAPI.RenderFrame(&Renderer, GameState);

         frame++;
         if ((frame % 100) == 0) {
            log("frame %i (%2.2f fps)\n", frame, frame / dt);
         }
      }
      SwapBuffers(OpenGLContext.dc);

      swapDebugFrame(GlobalDebug);

	}

   return (int) msg.wParam;
}

