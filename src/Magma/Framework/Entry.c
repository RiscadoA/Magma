#include "Entry.h"
#include "Config.h"

#include "String/Stream.h"
#include "Input/Window.h"
#include "Input/OGLWindow.h"
#include "Input/D3DWindow.h"

static mfmBool mfInitialized = MFM_FALSE;

mfError mfInit(int argc, const char** argv)
{
	if (mfInitialized == MFM_TRUE)
		return MF_ERROR_ALREADY_INITIALIZED;
	mfError err;

	// Init stream library
	err = mfsInitStream();
	if (err != MF_ERROR_OKAY)
		return err;

	// Init windows library
	err = mfiInitWindows();
	if (err != MF_ERROR_OKAY)
		return err;

	// Register windows types
#ifdef MAGMA_FRAMEWORK_USE_OPENGL
	err = mfiRegisterWindowCreator(MFI_OGLWINDOW_TYPE_NAME, &mfiCreateOGLWindow);
	if (err != MF_ERROR_OKAY)
		return err;
#endif

#ifdef MAGMA_FRAMEWORK_USE_DIRECTX
	err = mfiRegisterWindowCreator(MFI_D3DWINDOW_TYPE_NAME, &mfiCreateD3DWindow);
	if (err != MF_ERROR_OKAY)
		return err;
#endif

	mfInitialized = MFM_TRUE;

	return MF_ERROR_OKAY;
}

void mfTerminate()
{
	if (mfInitialized == MFM_FALSE)
		return;
	mfError err;

	// Terminate windows library
	mfiTerminateWindows();

	// Terminate stream library
	err = mfsTerminateStream();
	if (err != MF_ERROR_OKAY)
		abort();

	mfInitialized = MFM_FALSE;
}

mfmBool mfIsInitialized()
{
	return mfInitialized;
}