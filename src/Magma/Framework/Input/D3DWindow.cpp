#include "D3DWindow.hpp"

#include <Config.hpp>

#if defined(MAGMA_FRAMEWORK_WINDOWS_ENTRY_POINT)
#include <Windows.h>
#include <windowsx.h>
#include <map>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>

std::map<HWND, Magma::Framework::Input::D3DWindow*> winWindows = {};

HINSTANCE ghInstance = nullptr;
int gnCmdShow = 0;

LRESULT CALLBACK WindowProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam);

Magma::Framework::Input::Keyboard WindowsToMagmaKey(int key)
{
	switch (key)
	{
	case 'Q': return Magma::Framework::Input::Keyboard::Q;
	case 'W': return Magma::Framework::Input::Keyboard::W;
	case 'E': return Magma::Framework::Input::Keyboard::E;
	case 'R': return Magma::Framework::Input::Keyboard::R;
	case 'T': return Magma::Framework::Input::Keyboard::T;
	case 'Y': return Magma::Framework::Input::Keyboard::Y;
	case 'U': return Magma::Framework::Input::Keyboard::U;
	case 'I': return Magma::Framework::Input::Keyboard::I;
	case 'O': return Magma::Framework::Input::Keyboard::O;
	case 'P': return Magma::Framework::Input::Keyboard::P;
	case 'A': return Magma::Framework::Input::Keyboard::A;
	case 'S': return Magma::Framework::Input::Keyboard::S;
	case 'D': return Magma::Framework::Input::Keyboard::D;
	case 'F': return Magma::Framework::Input::Keyboard::F;
	case 'G': return Magma::Framework::Input::Keyboard::G;
	case 'H': return Magma::Framework::Input::Keyboard::H;
	case 'J': return Magma::Framework::Input::Keyboard::J;
	case 'K': return Magma::Framework::Input::Keyboard::K;
	case 'L': return Magma::Framework::Input::Keyboard::L;
	case 'Z': return Magma::Framework::Input::Keyboard::Z;
	case 'X': return Magma::Framework::Input::Keyboard::X;
	case 'C': return Magma::Framework::Input::Keyboard::C;
	case 'V': return Magma::Framework::Input::Keyboard::V;
	case 'B': return Magma::Framework::Input::Keyboard::B;
	case 'N': return Magma::Framework::Input::Keyboard::N;
	case 'M': return Magma::Framework::Input::Keyboard::M;

	case '1': return Magma::Framework::Input::Keyboard::Num1;
	case '2': return Magma::Framework::Input::Keyboard::Num2;
	case '3': return Magma::Framework::Input::Keyboard::Num3;
	case '4': return Magma::Framework::Input::Keyboard::Num4;
	case '5': return Magma::Framework::Input::Keyboard::Num5;
	case '6': return Magma::Framework::Input::Keyboard::Num6;
	case '7': return Magma::Framework::Input::Keyboard::Num7;
	case '8': return Magma::Framework::Input::Keyboard::Num8;
	case '9': return Magma::Framework::Input::Keyboard::Num9;
	case '0': return Magma::Framework::Input::Keyboard::Num0;

	case VK_F1: return Magma::Framework::Input::Keyboard::F1;
	case VK_F2: return Magma::Framework::Input::Keyboard::F2;
	case VK_F3: return Magma::Framework::Input::Keyboard::F3;
	case VK_F4: return Magma::Framework::Input::Keyboard::F4;
	case VK_F5: return Magma::Framework::Input::Keyboard::F5;
	case VK_F6: return Magma::Framework::Input::Keyboard::F6;
	case VK_F7: return Magma::Framework::Input::Keyboard::F7;
	case VK_F8: return Magma::Framework::Input::Keyboard::F8;
	case VK_F9: return Magma::Framework::Input::Keyboard::F9;
	case VK_F10: return Magma::Framework::Input::Keyboard::F10;
	case VK_F11: return Magma::Framework::Input::Keyboard::F11;
	case VK_F12: return Magma::Framework::Input::Keyboard::F12;

	case VK_ESCAPE: return Magma::Framework::Input::Keyboard::Escape;
	case VK_TAB: return Magma::Framework::Input::Keyboard::Tab;
	case VK_CAPITAL: return Magma::Framework::Input::Keyboard::Caps;
	case VK_LSHIFT: return Magma::Framework::Input::Keyboard::LShift;
	case VK_RSHIFT: return Magma::Framework::Input::Keyboard::RShift;
	case VK_LCONTROL: return Magma::Framework::Input::Keyboard::LControl;
	case VK_RCONTROL: return Magma::Framework::Input::Keyboard::RControl;
	case VK_LMENU: return Magma::Framework::Input::Keyboard::Alt;
	case VK_RMENU: return Magma::Framework::Input::Keyboard::AltGr;
	case VK_SPACE: return Magma::Framework::Input::Keyboard::Space;
	case VK_RETURN: return Magma::Framework::Input::Keyboard::Enter;
	case VK_BACK: return Magma::Framework::Input::Keyboard::Backspace;
	case VK_INSERT: return Magma::Framework::Input::Keyboard::Insert;
	case VK_DELETE: return Magma::Framework::Input::Keyboard::Delete;
	case VK_HOME: return Magma::Framework::Input::Keyboard::Home;
	case VK_END: return Magma::Framework::Input::Keyboard::End;
	case VK_PRIOR: return Magma::Framework::Input::Keyboard::PageUp;
	case VK_NEXT: return Magma::Framework::Input::Keyboard::PageDown;

	default: return Magma::Framework::Input::Keyboard::Invalid;
	}
}

Magma::Framework::Input::D3DWindow::D3DWindow(unsigned int width, unsigned int height, const std::string & title, Window::Mode mode)
{
	m_width = width;
	m_height = height;
	m_mode = mode;

	HWND hWnd;

	{
		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof(WNDCLASSEX));
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WindowProc;
		wc.hInstance = ghInstance;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wc.lpszClassName = "Magma::Framework::Input::D3DWindow::Windowed";
		RegisterClassEx(&wc);
	}

	{
		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof(WNDCLASSEX));
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WindowProc;
		wc.hInstance = ghInstance;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		// wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wc.lpszClassName = "Magma::Framework::Input::D3DWindow::Fullscreen";
		RegisterClassEx(&wc);
	}
	
	switch (mode)
	{
		case Window::Mode::Windowed:
		{
			RECT windowRect;
			windowRect.left = 0;
			windowRect.top = 0;
			windowRect.right = width;
			windowRect.bottom = height;
			AdjustWindowRect(&windowRect, WS_OVERLAPPED | WS_MINIMIZE | WS_SYSMENU | WS_CAPTION, FALSE);
			hWnd = CreateWindowEx(
				NULL,
				"Magma::Framework::Input::D3DWindow::Windowed",
				title.c_str(),
				WS_OVERLAPPED | WS_MINIMIZE | WS_SYSMENU | WS_CAPTION,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				windowRect.right - windowRect.left,
				windowRect.bottom - windowRect.top,
				NULL,
				NULL,
				ghInstance,
				NULL);
		} break;

		case Window::Mode::Fullscreen:
		{
			RECT windowRect;
			windowRect.left = 0;
			windowRect.top = 0;
			windowRect.right = width;
			windowRect.bottom = height;
			AdjustWindowRect(&windowRect, WS_EX_TOPMOST | WS_POPUP, FALSE);
			hWnd = CreateWindowEx(
				NULL,
				"Magma::Framework::Input::D3DWindow::Fullscreen",
				title.c_str(),
				WS_EX_TOPMOST | WS_POPUP,
				0,
				0,
				windowRect.right - windowRect.left,
				windowRect.bottom - windowRect.top,
				NULL,
				NULL,
				ghInstance,
				NULL);
		} break;

		default:
		{
			throw std::runtime_error("Failed to open D3D window, invalid window mode");
		} break;
	}

	ShowWindow(hWnd, gnCmdShow);

	m_hwnd = hWnd;
	winWindows[hWnd] = this;
}

Magma::Framework::Input::D3DWindow::~D3DWindow()
{
	DestroyWindow((HWND)m_hwnd);
	winWindows.erase((HWND)m_hwnd);
}

void Magma::Framework::Input::D3DWindow::MakeCurrent()
{
	ShowWindow((HWND)m_hwnd, 0);
}

void Magma::Framework::Input::D3DWindow::PollEvents()
{
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Magma::Framework::Input::D3DWindow::WaitForEvents()
{
	MSG msg;
	GetMessage(&msg, NULL, 0, 0);
	TranslateMessage(&msg);
	DispatchMessage(&msg);
}

unsigned int Magma::Framework::Input::D3DWindow::GetWidth()
{
	return m_width;
}

unsigned int Magma::Framework::Input::D3DWindow::GetHeight()
{
	return m_height;
}

Magma::Framework::Input::Window::Mode Magma::Framework::Input::D3DWindow::GetMode()
{
	return m_mode;
}

LRESULT CALLBACK WindowProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	using namespace Magma::Framework::Input;

	static bool tracking = false;

	static bool alt = false;
	static bool control = false;
	static bool system = false;
	static bool shift = false;

	auto window = winWindows.find(hWnd);
	if (window != winWindows.end())
		switch (message)
		{
			case WM_DESTROY:
			{
				(window->second)->OnClose.Fire();
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
					tracking = true;
				}
				(window->second)->OnMouseMove.Fire(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			} break;
			case WM_MOUSEHOVER:
			{
				(window->second)->OnMouseEnter.Fire();
			} break;
			case WM_MOUSELEAVE:
			{
				tracking = false;
				(window->second)->OnMouseLeave.Fire();
			} break;
			case WM_MOUSEWHEEL:
			{
				(window->second)->OnMouseScroll.Fire(GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? 1 : -1);
			} break;
			case WM_KEYDOWN:
			{
				if (WindowsToMagmaKey(wParam) == Keyboard::LShift || WindowsToMagmaKey(wParam) == Keyboard::RShift)
					shift = true;
				else if (WindowsToMagmaKey(wParam) == Keyboard::Alt)
					alt = true;
				else if (WindowsToMagmaKey(wParam) == Keyboard::LControl || WindowsToMagmaKey(wParam) == Keyboard::RControl)
					control = true;
				else if (wParam == VK_LWIN || wParam == VK_RWIN)
					system = true;

				KeyModifiers mods = KeyModifiers::None;
				if (alt) mods |= KeyModifiers::Alt;
				if (control) mods |= KeyModifiers::Control;
				if (system) mods |= KeyModifiers::System;
				if (shift) mods |= KeyModifiers::Shift;

				(window->second)->OnKeyDown.Fire(WindowsToMagmaKey(wParam), mods);
			} break;
			case WM_KEYUP:
			{
				if (WindowsToMagmaKey(wParam) == Keyboard::LShift || WindowsToMagmaKey(wParam) == Keyboard::RShift)
					shift = false;
				else if (WindowsToMagmaKey(wParam) == Keyboard::Alt)
					alt = false;
				else if (WindowsToMagmaKey(wParam) == Keyboard::LControl || WindowsToMagmaKey(wParam) == Keyboard::RControl)
					control = false;
				else if (wParam == VK_LWIN || wParam == VK_RWIN)
					system = false;

				KeyModifiers mods = KeyModifiers::None;
				if (alt) mods |= KeyModifiers::Alt;
				if (control) mods |= KeyModifiers::Control;
				if (system) mods |= KeyModifiers::System;
				if (shift) mods |= KeyModifiers::Shift;

				(window->second)->OnKeyUp.Fire(WindowsToMagmaKey(wParam), mods);
			} break;
			case WM_LBUTTONDOWN:
			{
				(window->second)->OnMouseDown.Fire(Mouse::Left);
			} break;
			case WM_MBUTTONDOWN:
			{
				(window->second)->OnMouseDown.Fire(Mouse::Middle);
			} break;
			case WM_RBUTTONDOWN:
			{
				(window->second)->OnMouseDown.Fire(Mouse::Right);
			} break;
			default:
			{
				return DefWindowProc(hWnd, message, wParam, lParam);
			} break;
		}
	else return DefWindowProc(hWnd, message, wParam, lParam);

	return 0;
}

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hpPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	// Open console
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);

	ghInstance = hInstance;
	gnCmdShow = nCmdShow;

	// Run program
	Main(0, nullptr);

	return 0;
}

#else

Magma::Framework::Input::D3DWindow::D3DWindow(unsigned int width, unsigned int height, const std::string & title, Window::Mode mode)
{
	throw std::runtime_error("Failed to construct D3DWindow: the project wasn't built for Windows (MAGMA_FRAMEWORK_WINDOWS_ENTRY_POINT must be defined)");
}

Magma::Framework::Input::D3DWindow::~D3DWindow()
{
	
}

void Magma::Framework::Input::D3DWindow::MakeCurrent()
{
	throw std::runtime_error("Failed to make D3DWindow current: the project wasn't built for Windows (MAGMA_FRAMEWORK_WINDOWS_ENTRY_POINT must be defined)");
}

void Magma::Framework::Input::D3DWindow::PollEvents()
{
	throw std::runtime_error("Failed to poll events on D3DWindow: the project wasn't built for Windows (MAGMA_FRAMEWORK_WINDOWS_ENTRY_POINT must be defined)");
}

void Magma::Framework::Input::D3DWindow::WaitForEvents()
{
	throw std::runtime_error("Failed to wait for events on D3DWindow: the project wasn't built for Windows (MAGMA_FRAMEWORK_WINDOWS_ENTRY_POINT must be defined)");
}

unsigned int Magma::Framework::Input::D3DWindow::GetWidth()
{
	throw std::runtime_error("Failed to get D3DWindow width: the project wasn't built for Windows (MAGMA_FRAMEWORK_WINDOWS_ENTRY_POINT must be defined)");
}

unsigned int Magma::Framework::Input::D3DWindow::GetHeight()
{
	throw std::runtime_error("Failed to get D3DWindow height: the project wasn't built for Windows (MAGMA_FRAMEWORK_WINDOWS_ENTRY_POINT must be defined)");
}

Magma::Framework::Input::Window::Mode Magma::Framework::Input::D3DWindow::GetMode()
{
	throw std::runtime_error("Failed to get D3DWindow mode: the project wasn't built for Windows (MAGMA_FRAMEWORK_WINDOWS_ENTRY_POINT must be defined)");
}
#endif