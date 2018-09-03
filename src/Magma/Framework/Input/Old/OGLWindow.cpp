#include "OGLWindow.hpp"
#include "../Exception.hpp"

#include <Config.h>

#if defined(MAGMA_FRAMEWORK_USE_OPENGL)

#include <sstream>

static void mfiWindowCloseCallback(void* window)
{
	auto win = (mfiWindow*)window;
	reinterpret_cast<Magma::Framework::Input::OGLWindow*>(win->userAttribute)->OnClose.Fire();
}

static void mfiWindowKeyDownCallback(void* window, mfiKeyCode code, mfiKeyMods mods)
{
	auto win = (mfiWindow*)window;
	reinterpret_cast<Magma::Framework::Input::OGLWindow*>(win->userAttribute)->OnKeyDown.Fire((Magma::Framework::Input::Keyboard)code, (Magma::Framework::Input::KeyModifiers)mods);
}

static void mfiWindowKeyUpCallback(void* window, mfiKeyCode code, mfiKeyMods mods)
{
	auto win = (mfiWindow*)window;
	reinterpret_cast<Magma::Framework::Input::OGLWindow*>(win->userAttribute)->OnKeyUp.Fire((Magma::Framework::Input::Keyboard)code, (Magma::Framework::Input::KeyModifiers)mods);
}

static void mfiWindowMouseMoveCallback(void* window, mfmF32 x, mfmF32 y)
{
	auto win = (mfiWindow*)window;
	reinterpret_cast<Magma::Framework::Input::OGLWindow*>(win->userAttribute)->OnMouseMove.Fire(x, y);
}

static void mfiWindowMouseDownCallback(void* window, mfiMouseButton button)
{
	auto win = (mfiWindow*)window;
	reinterpret_cast<Magma::Framework::Input::OGLWindow*>(win->userAttribute)->OnMouseDown.Fire((Magma::Framework::Input::Mouse)button);
}

static void mfiWindowMouseUpCallback(void* window, mfiMouseButton button)
{
	auto win = (mfiWindow*)window;
	reinterpret_cast<Magma::Framework::Input::OGLWindow*>(win->userAttribute)->OnMouseUp.Fire((Magma::Framework::Input::Mouse)button);
}

static void mfiWindowMouseScrollCallback(void* window, mfmF32 delta)
{
	auto win = (mfiWindow*)window;
	reinterpret_cast<Magma::Framework::Input::OGLWindow*>(win->userAttribute)->OnMouseScroll.Fire(delta);
}

static void mfiWindowMouseEnterCallback(void* window)
{
	auto win = (mfiWindow*)window;
	reinterpret_cast<Magma::Framework::Input::OGLWindow*>(win->userAttribute)->OnMouseEnter.Fire();
}

static void mfiWindowMouseLeaveCallback(void* window)
{
	auto win = (mfiWindow*)window;
	reinterpret_cast<Magma::Framework::Input::OGLWindow*>(win->userAttribute)->OnMouseLeave.Fire();
}

Magma::Framework::Input::OGLWindow::OGLWindow(mfmU32 width, mfmU32 height, const String::UTF8CodeUnit* title, Mode mode)
{
	m_window = nullptr;
	auto err = mfiCreateOGLWindow(&m_window, width, height, (mfiEnum)mode, title);
	if (err != MF_ERROR_OKAY)
	{
		std::stringstream ss;
		ss << "Failed to open OGLWindow:" << std::endl;
		ss << "mfiCreateGLWindow returned '" << err << "'";
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

Magma::Framework::Input::OGLWindow::~OGLWindow()
{
	if (m_window != nullptr)
		mfiDestroyGLWindow(m_window);
}

void Magma::Framework::Input::OGLWindow::PollEvents()
{
	m_window->pollEvents(m_window);
}

void Magma::Framework::Input::OGLWindow::WaitForEvents()
{
	m_window->waitForEvents(m_window);
}

#else
Magma::Framework::Input::OGLWindow::OGLWindow(unsigned int width, unsigned int height, const std::string& title, Mode mode)
{
	throw WindowError("Failed to construct OGLWindow: the project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}

Magma::Framework::Input::OGLWindow::~OGLWindow()
{
	
}

void Magma::Framework::Input::OGLWindow::MakeCurrent()
{
	throw WindowError("Failed to construct OGLWindow: the project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}

void Magma::Framework::Input::OGLWindow::PollEvents()
{
	throw WindowError("Failed to poll events on OGLWindow: the project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}

void Magma::Framework::Input::OGLWindow::WaitForEvents()
{
	throw WindowError("Failed to wait for events on OGLWindow: the project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}
#endif

