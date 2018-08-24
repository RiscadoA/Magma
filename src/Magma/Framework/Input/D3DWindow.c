#include "D3DWindow.h"
#include "Config.h"

#include "../Memory/Allocator.h"

#ifdef MAGMA_FRAMEWORK_USE_DIRECTX
#include <Windows.h>
#include <windowsx.h>
#include <stdio.h>

typedef struct
{
	// Base window data
	mfiWindow base;

	// Properties
	mfmU32 width;
	mfmU32 height;
	mfiEnum mode;

	// GLFW window handle
	HWND hwnd;
} mfiD3DWindow;

static mfiD3DWindow* currentWindow = NULL;

mfiKeyCode mfiWindowsToKeyCode(int key)
{
	switch (key)
	{
		case 'Q': return MFI_KEYBOARD_Q;
		case 'W': return MFI_KEYBOARD_W;
		case 'E': return MFI_KEYBOARD_E;
		case 'R': return MFI_KEYBOARD_R;
		case 'T': return MFI_KEYBOARD_T;
		case 'Y': return MFI_KEYBOARD_Y;
		case 'U': return MFI_KEYBOARD_U;
		case 'I': return MFI_KEYBOARD_I;
		case 'O': return MFI_KEYBOARD_O;
		case 'P': return MFI_KEYBOARD_P;
		case 'A': return MFI_KEYBOARD_A;
		case 'S': return MFI_KEYBOARD_S;
		case 'D': return MFI_KEYBOARD_D;
		case 'F': return MFI_KEYBOARD_F;
		case 'G': return MFI_KEYBOARD_G;
		case 'H': return MFI_KEYBOARD_H;
		case 'J': return MFI_KEYBOARD_J;
		case 'K': return MFI_KEYBOARD_K;
		case 'L': return MFI_KEYBOARD_L;
		case 'Z': return MFI_KEYBOARD_Z;
		case 'X': return MFI_KEYBOARD_X;
		case 'C': return MFI_KEYBOARD_C;
		case 'V': return MFI_KEYBOARD_V;
		case 'B': return MFI_KEYBOARD_B;
		case 'N': return MFI_KEYBOARD_N;
		case 'M': return MFI_KEYBOARD_M;

		case '1': return MFI_KEYBOARD_NUM1;
		case '2': return MFI_KEYBOARD_NUM2;
		case '3': return MFI_KEYBOARD_NUM3;
		case '4': return MFI_KEYBOARD_NUM4;
		case '5': return MFI_KEYBOARD_NUM5;
		case '6': return MFI_KEYBOARD_NUM6;
		case '7': return MFI_KEYBOARD_NUM7;
		case '8': return MFI_KEYBOARD_NUM8;
		case '9': return MFI_KEYBOARD_NUM9;
		case '0': return MFI_KEYBOARD_NUM0;

		case VK_F1: return MFI_KEYBOARD_F1;
		case VK_F2: return MFI_KEYBOARD_F2;
		case VK_F3: return MFI_KEYBOARD_F3;
		case VK_F4: return MFI_KEYBOARD_F4;
		case VK_F5: return MFI_KEYBOARD_F5;
		case VK_F6: return MFI_KEYBOARD_F6;
		case VK_F7: return MFI_KEYBOARD_F7;
		case VK_F8: return MFI_KEYBOARD_F8;
		case VK_F9: return MFI_KEYBOARD_F9;
		case VK_F10: return MFI_KEYBOARD_F10;
		case VK_F11: return MFI_KEYBOARD_F11;
		case VK_F12: return MFI_KEYBOARD_F12;

		case VK_ESCAPE: return MFI_KEYBOARD_ESCAPE;
		case VK_TAB: return MFI_KEYBOARD_TAB;
		case VK_CAPITAL: return MFI_KEYBOARD_CAPS;
		case VK_LSHIFT: return MFI_KEYBOARD_LSHIFT;
		case VK_RSHIFT: return MFI_KEYBOARD_RSHIFT;
		case VK_LCONTROL: return MFI_KEYBOARD_LCONTROL;
		case VK_RCONTROL: return MFI_KEYBOARD_RCONTROL;
		case VK_LMENU: return MFI_KEYBOARD_ALT;
		case VK_RMENU: return MFI_KEYBOARD_ALTGR;
		case VK_SPACE: return MFI_KEYBOARD_SPACE;
		case VK_RETURN: return MFI_KEYBOARD_ENTER;
		case VK_BACK: return MFI_KEYBOARD_BACKSPACE;
		case VK_INSERT: return MFI_KEYBOARD_INSERT;
		case VK_DELETE: return MFI_KEYBOARD_DELETE;
		case VK_HOME: return MFI_KEYBOARD_HOME;
		case VK_END: return MFI_KEYBOARD_END;
		case VK_PRIOR: return MFI_KEYBOARD_PAGEUP;
		case VK_NEXT: return MFI_KEYBOARD_PAGEDOWN;

		case VK_LEFT: return MFI_KEYBOARD_LEFT;
		case VK_RIGHT: return MFI_KEYBOARD_RIGHT;
		case VK_UP: return MFI_KEYBOARD_UP;
		case VK_DOWN: return MFI_KEYBOARD_DOWN;

		default: return MFI_KEYBOARD_INVALID;
	}
}

LRESULT CALLBACK WindowProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam);

void mfiD3DWindowPollEvents(void* window)
{
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void mfiD3DWindowWaitForEvents(void* window)
{
	MSG msg;
	GetMessage(&msg, NULL, 0, 0);
	TranslateMessage(&msg);
	DispatchMessage(&msg);
}

mfmU32 mfiGetD3DWindowWidth(void* window)
{
	mfiD3DWindow* d3dWindow = (mfiD3DWindow*)window;
	return d3dWindow->width;
}

mfmU32 mfiGetD3DWindowHeight(void* window)
{
	mfiD3DWindow* d3dWindow = (mfiD3DWindow*)window;
	return d3dWindow->height;
}

mfiEnum mfiGetD3DWindowMode(void* window)
{
	mfiD3DWindow* d3dWindow = (mfiD3DWindow*)window;
	return d3dWindow->mode;
}

#endif

mfError mfiCreateD3DWindow(mfiWindow ** window, mfmU32 width, mfmU32 height, mfiEnum mode, const mfsUTF8CodeUnit * title)
{
#ifdef MAGMA_FRAMEWORK_USE_DIRECTX
	if (currentWindow != NULL)
		return MFI_ERROR_ALREADY_INITIALIZED;

	mfiD3DWindow* d3dWindow = NULL;
	if (mfmAllocate(NULL, &d3dWindow, sizeof(mfiD3DWindow)) != MF_ERROR_OKAY)
		return MFI_ERROR_ALLOCATION_FAILED;

	// Set properties
	d3dWindow->base.type = MFI_D3DWINDOW_TYPE_NAME;

	d3dWindow->width = width;
	d3dWindow->height = height;
	d3dWindow->mode = mode;

	// Set destructor
	{
		mfError err = mfmInitObject(&d3dWindow->base.object);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	d3dWindow->base.object.destructorFunc = &mfiDestroyD3DWindow;

	// Set functions
	d3dWindow->base.pollEvents = &mfiD3DWindowPollEvents;
	d3dWindow->base.waitForEvents = &mfiD3DWindowWaitForEvents;

	// Set getters
	d3dWindow->base.getWidth = &mfiGetD3DWindowWidth;
	d3dWindow->base.getHeight = &mfiGetD3DWindowHeight;
	d3dWindow->base.getMode = &mfiGetD3DWindowMode;

	// Set callbacks to NULL
	d3dWindow->base.onClose = NULL;
	d3dWindow->base.onMouseEnter = NULL;
	d3dWindow->base.onMouseLeave = NULL;
	d3dWindow->base.onMouseMove = NULL;
	d3dWindow->base.onMouseScroll = NULL;
	d3dWindow->base.onKeyUp = NULL;
	d3dWindow->base.onKeyDown = NULL;
	d3dWindow->base.onMouseUp = NULL;
	d3dWindow->base.onMouseDown = NULL;

	// Get instance
	HINSTANCE instance = GetModuleHandle(NULL);

	// Open window
	{
		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof(WNDCLASSEX));
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WindowProc;
		wc.hInstance = instance;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wc.lpszClassName = "mfiD3DWindowWindowed";
		RegisterClassEx(&wc);
	}

	{
		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof(WNDCLASSEX));
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WindowProc;
		wc.hInstance = instance;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		// wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wc.lpszClassName = "mfiD3DWindowFullscreen";
		RegisterClassEx(&wc);
	}

	switch (mode)
	{
		case MFI_WINDOWED:
		{
			RECT windowRect;
			windowRect.left = 0;
			windowRect.top = 0;
			windowRect.right = width;
			windowRect.bottom = height;
			AdjustWindowRect(&windowRect, WS_OVERLAPPED | WS_MINIMIZE | WS_SYSMENU | WS_CAPTION, FALSE);
			d3dWindow->hwnd = CreateWindowEx(
				NULL,
				"mfiD3DWindowWindowed",
				title,
				WS_OVERLAPPED | WS_MINIMIZE | WS_SYSMENU | WS_CAPTION,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				windowRect.right - windowRect.left,
				windowRect.bottom - windowRect.top,
				NULL,
				NULL,
				instance,
				NULL);
		} break;

		case MFI_FULLSCREEN:
		{
			RECT windowRect;
			windowRect.left = 0;
			windowRect.top = 0;
			windowRect.right = width;
			windowRect.bottom = height;
			AdjustWindowRect(&windowRect, WS_EX_TOPMOST | WS_POPUP, FALSE);
			d3dWindow->hwnd = CreateWindowEx(
				NULL,
				"mfiD3DWindowFullscreen",
				title,
				WS_EX_TOPMOST | WS_POPUP,
				0,
				0,
				windowRect.right - windowRect.left,
				windowRect.bottom - windowRect.top,
				NULL,
				NULL,
				instance,
				NULL);
		} break;

		default:
		{
			if (mfmDeallocate(NULL, d3dWindow) != MF_ERROR_OKAY)
				abort();
			return MFI_ERROR_INVALID_ARGUMENTS;
		}
	}

	ShowWindow(d3dWindow->hwnd, SW_SHOWNORMAL);
	currentWindow = d3dWindow;
	*window = d3dWindow;

	return MF_ERROR_OKAY;
#else
	return MFI_ERROR_NOT_SUPPORTED;
#endif
}

void mfiDestroyD3DWindow(void * window)
{
#ifdef MAGMA_FRAMEWORK_USE_DIRECTX
	// Destroy window
	mfiD3DWindow* d3dWindow = (mfiD3DWindow*)window;
	DestroyWindow(d3dWindow->hwnd);
	if (mfmDestroyObject(&d3dWindow->base.object) != MF_ERROR_OKAY)
		abort();
	if (mfmDeallocate(NULL, d3dWindow) != MF_ERROR_OKAY)
		abort();
	currentWindow = NULL;
#endif
}

void * mfiGetD3DWindowHandle(void * window)
{
#ifdef MAGMA_FRAMEWORK_USE_DIRECTX
	mfiD3DWindow* d3dWindow = (mfiD3DWindow*)window;
	return d3dWindow->hwnd;
#else
	return NULL;
#endif
}
 
#ifdef MAGMA_FRAMEWORK_USE_DIRECTX

LRESULT CALLBACK WindowProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	static mfmBool tracking = MFM_FALSE;

	static mfmBool alt = MFM_FALSE;
	static mfmBool control = MFM_FALSE;
	static mfmBool system = MFM_FALSE;
	static mfmBool shift = MFM_FALSE;

	switch (message)
	{
		case WM_DESTROY:
		{
			if (currentWindow->base.onClose != NULL)
				currentWindow->base.onClose(currentWindow);
			return 0;
		} break;
		case WM_MOUSEMOVE:
		{
			if (!tracking)
			{
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof(TRACKMOUSEEVENT);
				tme.dwFlags = TME_HOVER | TME_LEAVE;
				tme.dwHoverTime = 1;
				tme.hwndTrack = hWnd;
				TrackMouseEvent(&tme);
				tracking = TRUE;
			}

			if (currentWindow->base.onMouseMove != NULL)
				currentWindow->base.onMouseMove(currentWindow, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		} break;
		case WM_MOUSEHOVER:
		{
			if (currentWindow->base.onMouseEnter != NULL)
				currentWindow->base.onMouseEnter(currentWindow);
		} break;
		case WM_MOUSELEAVE:
		{
			tracking = MFM_FALSE;
			if (currentWindow->base.onMouseLeave != NULL)
				currentWindow->base.onMouseLeave(currentWindow);
		} break;
		case WM_MOUSEWHEEL:
		{
			if (currentWindow->base.onMouseScroll != NULL)
				currentWindow->base.onMouseScroll(currentWindow, GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? 1.0f : -1.0f);
		} break;
		case WM_KEYDOWN:
		{
			if (mfiWindowsToKeyCode(wParam) == MFI_KEYBOARD_LSHIFT || mfiWindowsToKeyCode(wParam) == MFI_KEYBOARD_RSHIFT)
				shift = MFM_TRUE;
			else if (mfiWindowsToKeyCode(wParam) == MFI_KEYBOARD_ALT)
				alt = MFM_TRUE;
			else if (mfiWindowsToKeyCode(wParam) == MFI_KEYBOARD_LCONTROL || mfiWindowsToKeyCode(wParam) == MFI_KEYBOARD_RCONTROL)
				control = MFM_TRUE;
			else if (wParam == VK_LWIN || wParam == VK_RWIN)
				system = MFM_TRUE;

			mfiKeyMods mods = MFI_KEY_MOD_NONE;
			if (alt) mods |= MFI_KEY_MOD_ALT;
			if (control) mods |= MFI_KEY_MOD_CONTROL;
			if (system) mods |= MFI_KEY_MOD_SYSTEM;
			if (shift) mods |= MFI_KEY_MOD_SHIFT;

			if (currentWindow->base.onKeyDown != NULL)
				currentWindow->base.onKeyDown(currentWindow, mfiWindowsToKeyCode(wParam), mods);
		} break;
		case WM_KEYUP:
		{
			if (mfiWindowsToKeyCode(wParam) == MFI_KEYBOARD_LSHIFT || mfiWindowsToKeyCode(wParam) == MFI_KEYBOARD_RSHIFT)
				shift = MFM_FALSE;
			else if (mfiWindowsToKeyCode(wParam) == MFI_KEYBOARD_ALT)
				alt = MFM_FALSE;
			else if (mfiWindowsToKeyCode(wParam) == MFI_KEYBOARD_LCONTROL || mfiWindowsToKeyCode(wParam) == MFI_KEYBOARD_RCONTROL)
				control = MFM_FALSE;
			else if (wParam == VK_LWIN || wParam == VK_RWIN)
				system = MFM_FALSE;

			mfiKeyMods mods = MFI_KEY_MOD_NONE;
			if (alt) mods |= MFI_KEY_MOD_ALT;
			if (control) mods |= MFI_KEY_MOD_CONTROL;
			if (system) mods |= MFI_KEY_MOD_SYSTEM;
			if (shift) mods |= MFI_KEY_MOD_SHIFT;

			if (currentWindow->base.onKeyUp != NULL)
				currentWindow->base.onKeyUp(currentWindow, mfiWindowsToKeyCode(wParam), mods);
		} break;
		case WM_LBUTTONDOWN:
		{
			if (currentWindow->base.onMouseDown != NULL)
				currentWindow->base.onMouseDown(currentWindow, MFI_MOUSE_LEFT);
		} break;
		case WM_MBUTTONDOWN:
		{
			if (currentWindow->base.onMouseDown != NULL)
				currentWindow->base.onMouseDown(currentWindow, MFI_MOUSE_MIDDLE);
		} break;
		case WM_RBUTTONDOWN:
		{
			if (currentWindow->base.onMouseDown != NULL)
				currentWindow->base.onMouseDown(currentWindow, MFI_MOUSE_RIGHT);
		} break;
		case WM_LBUTTONUP:
		{
			if (currentWindow->base.onMouseUp != NULL)
				currentWindow->base.onMouseUp(currentWindow, MFI_MOUSE_LEFT);
		} break;
		case WM_MBUTTONUP:
		{
			if (currentWindow->base.onMouseUp != NULL)
				currentWindow->base.onMouseUp(currentWindow, MFI_MOUSE_MIDDLE);
		} break;
		case WM_RBUTTONUP:
		{
			if (currentWindow->base.onMouseUp != NULL)
				currentWindow->base.onMouseUp(currentWindow, MFI_MOUSE_RIGHT);
		} break;
		default:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		} break;
	}

	return 0;
}

#endif