#include "GLWindow.hpp"
#include "Exception.hpp"

#include <Config.h>

#if defined(MAGMA_FRAMEWORK_USE_OPENGL)

#include <sstream>

static void mfiWindowCloseCallback(void* window)
{
	auto win = (mfiWindow*)window;
	reinterpret_cast<Magma::Framework::Input::GLWindow*>(win->userAttribute)->OnClose.Fire();
}

static void mfiWindowKeyDownCallback(void* window, mfiKeyCode code, mfiKeyMods mods)
{
	auto win = (mfiWindow*)window;
	reinterpret_cast<Magma::Framework::Input::GLWindow*>(win->userAttribute)->OnKeyDown.Fire((Magma::Framework::Input::Keyboard)code, (Magma::Framework::Input::KeyModifiers)mods);
}

static void mfiWindowKeyUpCallback(void* window, mfiKeyCode code, mfiKeyMods mods)
{
	auto win = (mfiWindow*)window;
	reinterpret_cast<Magma::Framework::Input::GLWindow*>(win->userAttribute)->OnKeyUp.Fire((Magma::Framework::Input::Keyboard)code, (Magma::Framework::Input::KeyModifiers)mods);
}

static void mfiWindowMouseMoveCallback(void* window, mfmF32 x, mfmF32 y)
{
	auto win = (mfiWindow*)window;
	reinterpret_cast<Magma::Framework::Input::GLWindow*>(win->userAttribute)->OnMouseMove.Fire(x, y);
}

static void mfiWindowMouseDownCallback(void* window, mfiMouseButton button)
{
	auto win = (mfiWindow*)window;
	reinterpret_cast<Magma::Framework::Input::GLWindow*>(win->userAttribute)->OnMouseDown.Fire((Magma::Framework::Input::Mouse)button);
}

static void mfiWindowMouseUpCallback(void* window, mfiMouseButton button)
{
	auto win = (mfiWindow*)window;
	reinterpret_cast<Magma::Framework::Input::GLWindow*>(win->userAttribute)->OnMouseUp.Fire((Magma::Framework::Input::Mouse)button);
}

static void mfiWindowMouseScrollCallback(void* window, mfmF32 delta)
{
	auto win = (mfiWindow*)window;
	reinterpret_cast<Magma::Framework::Input::GLWindow*>(win->userAttribute)->OnMouseScroll.Fire(delta);
}

static void mfiWindowMouseEnterCallback(void* window)
{
	auto win = (mfiWindow*)window;
	reinterpret_cast<Magma::Framework::Input::GLWindow*>(win->userAttribute)->OnMouseEnter.Fire();
}

static void mfiWindowMouseLeaveCallback(void* window)
{
	auto win = (mfiWindow*)window;
	reinterpret_cast<Magma::Framework::Input::GLWindow*>(win->userAttribute)->OnMouseLeave.Fire();
}

Magma::Framework::Input::GLWindow::GLWindow(mfmU32 width, mfmU32 height, const String::UTF8CodeUnit* title, Mode mode)
{
	m_window = nullptr;
	auto err = mfiCreateGLWindow(&m_window, width, height, (mfiWindowMode)mode, title);
	if (err != MFI_ERROR_OKAY)
	{
		std::stringstream ss;
		ss << "Failed to open GLWindow:" << std::endl;
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

Magma::Framework::Input::GLWindow::~GLWindow()
{
	if (m_window != nullptr)
		mfiDestroyGLWindow(m_window);
}

void Magma::Framework::Input::GLWindow::PollEvents()
{
	m_window->pollEvents(m_window);
}

void Magma::Framework::Input::GLWindow::WaitForEvents()
{
	m_window->waitForEvents(m_window);
}

#else
Magma::Framework::Input::GLWindow::GLWindow(unsigned int width, unsigned int height, const std::string& title, Mode mode)
{
	throw WindowError("Failed to construct GLWindow: the project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}

Magma::Framework::Input::GLWindow::~GLWindow()
{
	
}

void Magma::Framework::Input::GLWindow::MakeCurrent()
{
	throw WindowError("Failed to construct GLWindow: the project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}

void Magma::Framework::Input::GLWindow::PollEvents()
{
	throw WindowError("Failed to poll events on GLWindow: the project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}

void Magma::Framework::Input::GLWindow::WaitForEvents()
{
	throw WindowError("Failed to wait for events on GLWindow: the project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}
#endif

