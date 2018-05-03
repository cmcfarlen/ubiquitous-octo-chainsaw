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
typedef void(*initialize_t)(int, int);

static render_t render = NULL;
static initialize_t initialize = NULL;
static unsigned long long last_mode_time = 0;
static HMODULE render_dll = NULL;

static char full_path_to_dll[MAX_PATH];
static char full_path_to_dll_copy[MAX_PATH];

void buildGamePaths(void)
{
	snprintf(full_path_to_dll, sizeof(full_path_to_dll), "%s", "game.dll");
	snprintf(full_path_to_dll_copy, sizeof(full_path_to_dll), "%s", "game_copy.dll");

	log("game dll: %s\n", full_path_to_dll);
	log("game dll copy: %s\n", full_path_to_dll_copy);
}

void checkModuleHandle(const char* f)
{
	HMODULE h = GetModuleHandleA(f);
	if (h == NULL) {
		log("No module: %s\n", f);
	}
	else {
		log("Handle reference found for %s\n", f);
	}
}

void loadGameCode(void)
{
	unsigned long long now = getModifyTime(full_path_to_dll);

	if (now != last_mode_time) {
		checkModuleHandle(full_path_to_dll_copy);
		if (render_dll) {
			log("Unloading library.\n");
			if (FreeLibrary(render_dll) == 0) {
				log("Error freeing library: %i\n", GetLastError());
			}
			if (FreeLibrary(render_dll) == 0) {
				log("2 Error freeing library: %i\n", GetLastError());
			}
			if (FreeLibrary(render_dll) == 0) {
				log("3 Error freeing library: %i\n", GetLastError());
			}
			if (FreeLibrary(render_dll) == 0) {
				log("4 Error freeing library: %i\n", GetLastError());
			}
			render_dll = NULL;
		}

		checkModuleHandle(full_path_to_dll_copy);

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

		checkModuleHandle(full_path_to_dll_copy);

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

float PI = 3.14159;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// load game code first so the window can be initialized
	buildGamePaths();

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



