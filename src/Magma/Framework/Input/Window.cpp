#include "Window.hpp"
#include "Exception.hpp"
#include "ErrorString.h"
#include "Config.h"

void Magma::Framework::Input::HWindow::PollEvents()
{
	reinterpret_cast<mfiWindow*>(&this->Get())->pollEvents(&this->Get());
}

void Magma::Framework::Input::HWindow::WaitForEvents()
{
	reinterpret_cast<mfiWindow*>(&this->Get())->waitForEvents(&this->Get());
}

mfmU32 Magma::Framework::Input::HWindow::GetWidth()
{
	return reinterpret_cast<mfiWindow*>(&this->Get())->getWidth(&this->Get());
}

mfmU32 Magma::Framework::Input::HWindow::GetHeight()
{
	return reinterpret_cast<mfiWindow*>(&this->Get())->getHeight(&this->Get());
}

Magma::Framework::Input::WindowMode Magma::Framework::Input::HWindow::GetMode()
{
	return static_cast<WindowMode>(reinterpret_cast<mfiWindow*>(&this->Get())->getMode(&this->Get()));
}

void Magma::Framework::Input::HWindow::SetOnCloseCallback(mfiOnCloseCallback callback)
{
	reinterpret_cast<mfiWindow*>(&this->Get())->onClose = callback;
}

void Magma::Framework::Input::HWindow::SetOnMouseEnterCallback(mfiOnMouseEnterCallback callback)
{
	reinterpret_cast<mfiWindow*>(&this->Get())->onMouseEnter = callback;
}

void Magma::Framework::Input::HWindow::SetOnMouseLeaveCallback(mfiOnMouseLeaveCallback callback)
{
	reinterpret_cast<mfiWindow*>(&this->Get())->onMouseLeave = callback;
}

void Magma::Framework::Input::HWindow::SetOnMouseMoveCallback(mfiOnMouseMoveCallback callback)
{
	reinterpret_cast<mfiWindow*>(&this->Get())->onMouseMove = callback;
}

void Magma::Framework::Input::HWindow::SetOnMouseScrollCallback(mfiOnMouseScrollCallback callback)
{
	reinterpret_cast<mfiWindow*>(&this->Get())->onMouseScroll = callback;
}

void Magma::Framework::Input::HWindow::SetOnMouseUpCallback(mfiOnMouseUpCallback callback)
{
	reinterpret_cast<mfiWindow*>(&this->Get())->onMouseUp = callback;
}

void Magma::Framework::Input::HWindow::SetOnMouseDownCallback(mfiOnMouseDownCallback callback)
{
	reinterpret_cast<mfiWindow*>(&this->Get())->onMouseDown = callback;
}

void Magma::Framework::Input::HWindow::SetOnKeyUpCallback(mfiOnKeyUpCallback callback)
{
	reinterpret_cast<mfiWindow*>(&this->Get())->onKeyUp = callback;
}

void Magma::Framework::Input::HWindow::SetOnKeyDownCallback(mfiOnKeyDownCallback callback)
{
	reinterpret_cast<mfiWindow*>(&this->Get())->onKeyDown = callback;
}

Magma::Framework::Input::HWindow Magma::Framework::Input::CreateWindow(const mfsUTF8CodeUnit * type, mfmU32 width, mfmU32 height, WindowMode mode, const mfsUTF8CodeUnit * title)
{
	if (type == NULL)
	{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
		type = u8"d3d";
#elif defined(MAGMA_FRAMEWORK_USE_OPENGL)
		type = u8"ogl";
#endif
	}

	mfiWindow* win = NULL;
	mfError err = mfiCreateWindow(type, &win, width, height, static_cast<mfiEnum>(mode), title);
	if (err != MF_ERROR_OKAY)
		throw WindowError(mfiErrorToString(err));
	return win;
}
