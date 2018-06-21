#include "GLWindow.hpp"

#include <Config.hpp>

#if defined(MAGMA_FRAMEWORK_USE_OPENGL)
#include <GLFW/glfw3.h>
#include <sstream>
#include <map>

static std::map<GLFWwindow*, Magma::Framework::Input::GLWindow*> glfwWindows;

Magma::Framework::Input::Keyboard GLFWToMagmaKey(int key)
{
	switch (key)
	{
	case GLFW_KEY_Q: return Magma::Framework::Input::Keyboard::Q;
	case GLFW_KEY_W: return Magma::Framework::Input::Keyboard::W;
	case GLFW_KEY_E: return Magma::Framework::Input::Keyboard::E;
	case GLFW_KEY_R: return Magma::Framework::Input::Keyboard::R;
	case GLFW_KEY_T: return Magma::Framework::Input::Keyboard::T;
	case GLFW_KEY_Y: return Magma::Framework::Input::Keyboard::Y;
	case GLFW_KEY_U: return Magma::Framework::Input::Keyboard::U;
	case GLFW_KEY_I: return Magma::Framework::Input::Keyboard::I;
	case GLFW_KEY_O: return Magma::Framework::Input::Keyboard::O;
	case GLFW_KEY_P: return Magma::Framework::Input::Keyboard::P;
	case GLFW_KEY_A: return Magma::Framework::Input::Keyboard::A;
	case GLFW_KEY_S: return Magma::Framework::Input::Keyboard::S;
	case GLFW_KEY_D: return Magma::Framework::Input::Keyboard::D;
	case GLFW_KEY_F: return Magma::Framework::Input::Keyboard::F;
	case GLFW_KEY_G: return Magma::Framework::Input::Keyboard::G;
	case GLFW_KEY_H: return Magma::Framework::Input::Keyboard::H;
	case GLFW_KEY_J: return Magma::Framework::Input::Keyboard::J;
	case GLFW_KEY_K: return Magma::Framework::Input::Keyboard::K;
	case GLFW_KEY_L: return Magma::Framework::Input::Keyboard::L;
	case GLFW_KEY_Z: return Magma::Framework::Input::Keyboard::Z;
	case GLFW_KEY_X: return Magma::Framework::Input::Keyboard::X;
	case GLFW_KEY_C: return Magma::Framework::Input::Keyboard::C;
	case GLFW_KEY_V: return Magma::Framework::Input::Keyboard::V;
	case GLFW_KEY_B: return Magma::Framework::Input::Keyboard::B;
	case GLFW_KEY_N: return Magma::Framework::Input::Keyboard::N;
	case GLFW_KEY_M: return Magma::Framework::Input::Keyboard::M;

	case GLFW_KEY_1: return Magma::Framework::Input::Keyboard::Num1;
	case GLFW_KEY_2: return Magma::Framework::Input::Keyboard::Num2;
	case GLFW_KEY_3: return Magma::Framework::Input::Keyboard::Num3;
	case GLFW_KEY_4: return Magma::Framework::Input::Keyboard::Num4;
	case GLFW_KEY_5: return Magma::Framework::Input::Keyboard::Num5;
	case GLFW_KEY_6: return Magma::Framework::Input::Keyboard::Num6;
	case GLFW_KEY_7: return Magma::Framework::Input::Keyboard::Num7;
	case GLFW_KEY_8: return Magma::Framework::Input::Keyboard::Num8;
	case GLFW_KEY_9: return Magma::Framework::Input::Keyboard::Num9;
	case GLFW_KEY_0: return Magma::Framework::Input::Keyboard::Num0;

	case GLFW_KEY_F1: return Magma::Framework::Input::Keyboard::F1;
	case GLFW_KEY_F2: return Magma::Framework::Input::Keyboard::F2;
	case GLFW_KEY_F3: return Magma::Framework::Input::Keyboard::F3;
	case GLFW_KEY_F4: return Magma::Framework::Input::Keyboard::F4;
	case GLFW_KEY_F5: return Magma::Framework::Input::Keyboard::F5;
	case GLFW_KEY_F6: return Magma::Framework::Input::Keyboard::F6;
	case GLFW_KEY_F7: return Magma::Framework::Input::Keyboard::F7;
	case GLFW_KEY_F8: return Magma::Framework::Input::Keyboard::F8;
	case GLFW_KEY_F9: return Magma::Framework::Input::Keyboard::F9;
	case GLFW_KEY_F10: return Magma::Framework::Input::Keyboard::F10;
	case GLFW_KEY_F11: return Magma::Framework::Input::Keyboard::F11;
	case GLFW_KEY_F12: return Magma::Framework::Input::Keyboard::F12;

	case GLFW_KEY_ESCAPE: return Magma::Framework::Input::Keyboard::Escape;
	case GLFW_KEY_TAB: return Magma::Framework::Input::Keyboard::Tab;
	case GLFW_KEY_CAPS_LOCK: return Magma::Framework::Input::Keyboard::Caps;
	case GLFW_KEY_LEFT_SHIFT: return Magma::Framework::Input::Keyboard::LShift;
	case GLFW_KEY_RIGHT_SHIFT: return Magma::Framework::Input::Keyboard::RShift;
	case GLFW_KEY_LEFT_CONTROL: return Magma::Framework::Input::Keyboard::LControl;
	case GLFW_KEY_RIGHT_CONTROL: return Magma::Framework::Input::Keyboard::RControl;
	case GLFW_KEY_LEFT_ALT: return Magma::Framework::Input::Keyboard::Alt;
	case GLFW_KEY_RIGHT_ALT: return Magma::Framework::Input::Keyboard::AltGr;
	case GLFW_KEY_SPACE: return Magma::Framework::Input::Keyboard::Space;
	case GLFW_KEY_ENTER: return Magma::Framework::Input::Keyboard::Enter;
	case GLFW_KEY_BACKSPACE: return Magma::Framework::Input::Keyboard::Backspace;
	case GLFW_KEY_INSERT: return Magma::Framework::Input::Keyboard::Insert;
	case GLFW_KEY_DELETE: return Magma::Framework::Input::Keyboard::Delete;
	case GLFW_KEY_HOME: return Magma::Framework::Input::Keyboard::Home;
	case GLFW_KEY_END: return Magma::Framework::Input::Keyboard::End;
	case GLFW_KEY_PAGE_UP: return Magma::Framework::Input::Keyboard::PageUp;
	case GLFW_KEY_PAGE_DOWN: return Magma::Framework::Input::Keyboard::PageDown;

	default: return Magma::Framework::Input::Keyboard::Invalid;
	}
}

void GLFWWindowCloseCallback(GLFWwindow* window)
{
	glfwWindows[window]->OnClose.Fire();
}

void GLFWErrorCallback(int err, const char* errMsg)
{
	std::stringstream ss;
	ss << "GLFW error caught (" << err << "): " << errMsg;
	throw std::runtime_error(ss.str());
}

void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto k = GLFWToMagmaKey(key);

	switch (action)
	{
	case GLFW_PRESS:
		glfwWindows[window]->OnKeyDown.Fire(k, (Magma::Framework::Input::KeyModifiers)mods);
		break;
	case GLFW_RELEASE:
		glfwWindows[window]->OnKeyUp.Fire(k, (Magma::Framework::Input::KeyModifiers)mods);
		break;
	}
}

void GLFWMousePositionCallback(GLFWwindow* window, double x, double y)
{
	glfwWindows[window]->OnMouseMove.Fire((int)x, (int)y);
}

void GLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	auto mouseButton = Magma::Framework::Input::Mouse::Invalid;

	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		mouseButton = Magma::Framework::Input::Mouse::Left;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		mouseButton = Magma::Framework::Input::Mouse::Right;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		mouseButton = Magma::Framework::Input::Mouse::Middle;
		break;
	}

	switch (action)
	{
	case GLFW_PRESS:
		glfwWindows[window]->OnMouseDown.Fire(mouseButton);
		break;
	case GLFW_RELEASE:
		glfwWindows[window]->OnMouseUp.Fire(mouseButton);
		break;
	}
}

void GLFWMouseScroll(GLFWwindow* window, double x, double y)
{
	glfwWindows[window]->OnMouseScroll.Fire((int)y);
}

void GLFWCursorEnterCallback(GLFWwindow* window, int enter)
{
	if (enter == GLFW_TRUE)
		glfwWindows[window]->OnMouseEnter.Fire();
	else
		glfwWindows[window]->OnMouseLeave.Fire();
}

Magma::Framework::Input::GLWindow::GLWindow(unsigned int width, unsigned int height, const std::string& title, Mode mode)
{
	m_width = width;
	m_height = height;
	m_mode = mode;

	if (glfwWindows.size() == 0)
	{
		glfwSetErrorCallback(GLFWErrorCallback);

		auto err = glfwInit();
		if (err != GLFW_TRUE)
		{
			std::stringstream ss;
			ss << "Failed to open window, couldn't init GLFW";
			throw std::runtime_error(ss.str());
		}
	}

	GLFWwindow* win;

	glfwWindowHint(GLFW_RESIZABLE, 0);

	switch (mode)
	{
	case Mode::Windowed:
		win = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
		break;

	case Mode::Fullscreen:
		win = glfwCreateWindow(width, height, title.c_str(), glfwGetPrimaryMonitor(), NULL);
		break;
	}

	if (win == nullptr)
		throw std::runtime_error("Failed to open window, window is NULL");

	glfwMakeContextCurrent(win);

	glfwSetKeyCallback(win, GLFWKeyCallback);
	glfwSetCursorPosCallback(win, GLFWMousePositionCallback);
	glfwSetWindowCloseCallback(win, GLFWWindowCloseCallback);
	glfwSetScrollCallback(win, GLFWMouseScroll);
	glfwSetCursorEnterCallback(win, GLFWCursorEnterCallback);

	m_glfwWindow = win;
	glfwWindows[win] = this;
}

Magma::Framework::Input::GLWindow::~GLWindow()
{
	glfwWindows.erase((GLFWwindow*)m_glfwWindow);
	glfwDestroyWindow((GLFWwindow*)m_glfwWindow);

	if (glfwWindows.size() == 0)
		glfwTerminate();
}

void Magma::Framework::Input::GLWindow::MakeCurrent()
{
	glfwMakeContextCurrent((GLFWwindow*)m_glfwWindow);
}

void Magma::Framework::Input::GLWindow::PollEvents()
{
	glfwPollEvents();
}

void Magma::Framework::Input::GLWindow::WaitForEvents()
{
	glfwWaitEvents();
}

void Magma::Framework::Input::GLWindow::SwapBuffers()
{
	glfwSwapBuffers((GLFWwindow*)m_glfwWindow);
}

#if not defined(MAGMA_FRAMEWORK_WINDOWS_ENTRY_POINT)
int main(int argc, char** argv)
{
	Main(argc, argv);
}
#endif
#else
Magma::Framework::Input::GLWindow::GLWindow(unsigned int width, unsigned int height, const std::string& title, Mode mode)
{
	throw std::runtime_error("Failed to construct GLWindow: the project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}

Magma::Framework::Input::GLWindow::~GLWindow()
{
	
}

void Magma::Framework::Input::GLWindow::MakeCurrent()
{
	throw std::runtime_error("Failed to construct GLWindow: the project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}

void Magma::Framework::Input::GLWindow::PollEvents()
{
	throw std::runtime_error("Failed to poll events on GLWindow: the project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}

void Magma::Framework::Input::GLWindow::WaitForEvents()
{
	throw std::runtime_error("Failed to wait for events on GLWindow: the project wasn't built for OpenGL (MAGMA_FRAMEWORK_USE_OPENGL must be defined)");
}
#endif