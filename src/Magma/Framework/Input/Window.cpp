#include "Window.hpp"
#include "Exception.hpp"
#include "Error.h"

typedef struct
{
	bool active = false;
	mfsUTF8CodeUnit type[16];
	Magma::Framework::Input::WindowCreatorFunction func;
} WindowCreatorRegisterEntry;

WindowCreatorRegisterEntry windowCreatorRegisterEntries[16];

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

void Magma::Framework::Input::RegisterWindowCreator(mfsUTF8CodeUnit type[16], Magma::Framework::Input::WindowCreatorFunction func)
{
	for (mfmU32 i = 0; i < 16; ++i)
	{
		if (windowCreatorRegisterEntries[i].active == false)
		{
			for (mfmU32 j = 0; j < 16; ++j)
			{
				windowCreatorRegisterEntries[i].type[j] = type[j];
				if (type[j] == '\0')
					break;
			}
			windowCreatorRegisterEntries[i].func = func;
			windowCreatorRegisterEntries[i].active = true;
			return;
		}
	}

	throw RegisterError(u8"Failed to register window creator function, no more slots available (there is a maximum of 16 window creator functions)");
}

Magma::Framework::Input::Window Magma::Framework::Input::CreateWindow(const mfsUTF8CodeUnit * type, mfmU32 width, mfmU32 height, WindowMode mode, const mfsUTF8CodeUnit * title)
{
	for (mfmU32 i = 0; i < 16; ++i)
	{
		if (windowCreatorRegisterEntries[i].active == true &&
			strcmp(type, windowCreatorRegisterEntries[i].type) == 0)
		{
			mfiWindow* win = NULL;
			mfError err = windowCreatorRegisterEntries[i].func(&win, width, height, static_cast<mfiWindowMode>(mode), title);
			if (err != MFI_ERROR_OKAY)
				throw WindowError("Failed to create window");
			return win;
		}
	}

	throw RegisterError(u8"Failed to create window, no window creator function was registered with the requested type name");
}
