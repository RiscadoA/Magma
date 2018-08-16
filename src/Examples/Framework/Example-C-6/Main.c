#include <Magma/Framework/Input/GLWindow.h>
#include <Magma/Framework/Graphics/OGL4RenderDevice.h>
#include <Magma/Framework/String/UTF8.h>
#include <stdio.h>
#include <stdlib.h>

mfmBool windowOpen = MFM_TRUE;
mfiWindow* window;
mfgRenderDevice* renderDevice;

void OnClose(void* window)
{
	windowOpen = MFM_FALSE;
}

void Main(int argc, char** argv)
{
	// Open window
	{
		mfiError err = mfiCreateGLWindow(&window, 800, 600, MFI_WINDOWED, u8"Example-C-2");
		if (err != MFI_ERROR_OKAY)
			abort();
	}

	// Create render device
	{
		mfgRenderDeviceDesc desc;
		desc.vsyncEnabled = MFM_TRUE;
		mfgError err = mfgCreateOGL4RenderDevice(&renderDevice, window, &desc, NULL);
		if (err != MFG_ERROR_OKAY)
			abort();
	}

	window->onClose = &OnClose;

	while (windowOpen == MFM_TRUE)
	{
		window->pollEvents(window);
	}

	mfiDestroyGLWindow(window);
	mfgDestroyOGL4RenderDevice(renderDevice);
}
