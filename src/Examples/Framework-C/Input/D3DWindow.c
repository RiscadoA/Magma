﻿#include <Magma/Framework/Input/D3DWindow.h>
#include <Magma/Framework/String/UTF8.h>
#include <stdio.h>
#include <stdlib.h>

mfmBool windowOpen = MFM_TRUE;
mfiWindow* window;

void OnClose(void* window)
{
	windowOpen = MFM_FALSE;
}

int main(int argc, const char** argv)
{
	if (mfInit(argc, argv) != MF_ERROR_OKAY)
		abort();

	mfError err = mfiCreateOGLWindow(&window, 800, 600, MFI_WINDOWED, u8"D3D-Window");
	if (err != MF_ERROR_OKAY)
		abort();

	window->onClose = &OnClose;

	while (windowOpen == MFM_TRUE)
	{
		window->pollEvents(window);
	}

	mfiDestroyGLWindow(window);

	mfTerminate();
	return 0;
}