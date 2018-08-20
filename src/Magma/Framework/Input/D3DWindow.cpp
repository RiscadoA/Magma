#include "D3DWindow.hpp"
#include "Exception.hpp"

#include <Config.h>

#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)

#include <sstream>

void mfiWindowCloseCallback(void* window)
{
	auto win = (mfiWindow*)window;
	reinterpret_cast<Magma::Framework::Input::D3DWindow*>(win->userAttribute)->OnClose.Fire();
}

void mfiWindowKeyDownCallback(void* window, mfiKeyCode code, mfiKeyMods mods)
{
	auto win = (mfiWindow*)window;
	reinterpret_cast<Magma::Framework::Input::D3DWindow*>(win->userAttribute)->OnKeyDown.Fire((Magma::Framework::Input::Keyboard)code, (Magma::Framework::Input::KeyModifiers)mods);
}

void mfiWindowKeyUpCallback(void* window, mfiKeyCode code, mfiKeyMods mods)
{
	auto win = (mfiWindow*)window;
	reinterpret_cast<Magma::Framework::Input::D3DWindow*>(win->userAttribute)->OnKeyUp.Fire((Magma::Framework::Input::Keyboard)code, (Magma::Framework::Input::KeyModifiers)mods);
}

void mfiWindowMouseMoveCallback(void* window, mfmF32 x, mfmF32 y)
{
	auto win = (mfiWindow*)window;
	reinterpret_cast<Magma::Framework::Input::D3DWindow*>(win->userAttribute)->OnMouseMove.Fire(x, y);
}

void mfiWindowMouseDownCallback(void* window, mfiMouseButton button)
{
	auto win = (mfiWindow*)window;
	reinterpret_cast<Magma::Framework::Input::D3DWindow*>(win->userAttribute)->OnMouseDown.Fire((Magma::Framework::Input::Mouse)button);
}

void mfiWindowMouseUpCallback(void* window, mfiMouseButton button)
{
	auto win = (mfiWindow*)window;
	reinterpret_cast<Magma::Framework::Input::D3DWindow*>(win->userAttribute)->OnMouseUp.Fire((Magma::Framework::Input::Mouse)button);
}

void mfiWindowMouseScrollCallback(void* window, mfmF32 delta)
{
	auto win = (mfiWindow*)window;
	reinterpret_cast<Magma::Framework::Input::D3DWindow*>(win->userAttribute)->OnMouseScroll.Fire(delta);
}

void mfiWindowMouseEnterCallback(void* window)
{
	auto win = (mfiWindow*)window;
	reinterpret_cast<Magma::Framework::Input::D3DWindow*>(win->userAttribute)->OnMouseEnter.Fire();
}

void mfiWindowMouseLeaveCallback(void* window)
{
	auto win = (mfiWindow*)window;
	reinterpret_cast<Magma::Framework::Input::D3DWindow*>(win->userAttribute)->OnMouseLeave.Fire();
}

Magma::Framework::Input::D3DWindow::D3DWindow(mfmU32 width, mfmU32 height, const String::UTF8CodeUnit* title, Window::Mode mode)
{
	m_window = nullptr;
	auto err = mfiCreateD3DWindow(&m_window, width, height, (mfiWindowMode)mode, title);
	if (err != MFI_ERROR_OKAY)
	{
		std::stringstream ss;
		ss << "Failed to open D3DWindow:" << std::endl;
		ss << "mfiCreateD3DWindow returned '" << err << "'";
		throw WindowError(ss.str());
	}

	m_window->userAttribute = reinterpret_cast<mfmU64>(this);

	m_window->onClose = &mfiWindowCloseCallback;
	m_window->onKeyDown = &mfiWindowKeyDownCallback;
	m_window->onKeyUp = &mfiWindowKeyUpCallback;
	m_window->onMouseMove = &mfiWindowMouseMoveCallback;
	m_window->onMouseEnter = &mfiWindowMouseEnterCallback;
	m_window->onMouseLeave = &mfiWindowMouseLeaveCallback;
	m_window->onMouseDown = &mfiWindowMouseDownCallback;
	m_window->onMouseUp = &mfiWindowMouseUpCallback;
	m_window->onMouseScroll = &mfiWindowMouseScrollCallback;
}

Magma::Framework::Input::D3DWindow::~D3DWindow()
{
	if (m_window != nullptr)
		mfiDestroyD3DWindow(m_window);
}

void Magma::Framework::Input::D3DWindow::PollEvents()
{
	m_window->pollEvents(m_window);
}

void Magma::Framework::Input::D3DWindow::WaitForEvents()
{
	m_window->waitForEvents(m_window);
}

#else

Magma::Framework::Input::D3DWindow::D3DWindow(mfmU32 width, mfmU32 height, const String::UTF8CodeUnit* title, Window::Mode mode)
{
	throw WindowError("Failed to construct D3DWindow: the project wasn't built for Windows (MAGMA_FRAMEWORK_WINDOWS_ENTRY_POINT must be defined)");
}

Magma::Framework::Input::D3DWindow::~D3DWindow()
{
	
}

void Magma::Framework::Input::D3DWindow::PollEvents()
{
	throw WindowError("Failed to poll events on D3DWindow: the project wasn't built for Windows (MAGMA_FRAMEWORK_WINDOWS_ENTRY_POINT must be defined)");
}

void Magma::Framework::Input::D3DWindow::WaitForEvents()
{
	throw WindowError("Failed to wait for events on D3DWindow: the project wasn't built for Windows (MAGMA_FRAMEWORK_WINDOWS_ENTRY_POINT must be defined)");
}

#endif