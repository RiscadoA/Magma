#include "Window.hpp"
#include "Exception.hpp"
#include "ErrorString.h"

void Magma::Framework::Input::Window::PollEvents()
{
	reinterpret_cast<mfiWindow*>(&this->Get())->pollEvents(&this->Get());
}

void Magma::Framework::Input::Window::WaitForEvents()
{
	reinterpret_cast<mfiWindow*>(&this->Get())->waitForEvents(&this->Get());
}

mfmU32 Magma::Framework::Input::Window::GetWidth()
{
	return reinterpret_cast<mfiWindow*>(&this->Get())->getWidth(&this->Get());
}

mfmU32 Magma::Framework::Input::Window::GetHeight()
{
	return reinterpret_cast<mfiWindow*>(&this->Get())->getHeight(&this->Get());
}

Magma::Framework::Input::WindowMode Magma::Framework::Input::Window::GetMode()
{
	return static_cast<WindowMode>(reinterpret_cast<mfiWindow*>(&this->Get())->getMode(&this->Get()));
}

Magma::Framework::Input::Window Magma::Framework::Input::CreateWindow(const mfsUTF8CodeUnit * type, mfmU32 width, mfmU32 height, WindowMode mode, const mfsUTF8CodeUnit * title)
{
	mfiWindow* win = NULL;
	mfError err = mfiCreateWindow(type, &win, width, height, static_cast<mfiEnum>(mode), title);
	if (err != MF_ERROR_OKAY)
		throw WindowError(mfiErrorToString(err));
	return win;
}
