#include <Magma/Framework/Input/GLWindow.h>
#include <Magma/Framework/String/UTF8.hpp>
#include <cstdio>
#include <cstdlib>

bool windowOpen = true;
mfiWindow* window;

void OnClose(void* window)
{
	windowOpen = false;
}

void Main(int argc, char** argv)
{
	mfiError err = mfiCreateGLWindow(&window, 800, 600, MFI_WINDOWED, u8"Example-C-2"_ms);
	if (err != MFI_ERROR_OKAY)
		abort();

	window->onClose = &OnClose;

	while (windowOpen)
	{
		window->pollEvents(window);
	}

	mfiDestroyGLWindow(window);
}