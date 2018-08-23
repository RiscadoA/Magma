#include "Entry.h"

#include "String/Stream.h"

static mfmBool mfInitialized = MFM_FALSE;

mfError mfInit(int argc, const char** argv)
{
	if (mfInitialized == MFM_TRUE)
		return MF_ERROR_ALREADY_INITIALIZED;

	// Init stream library
	mfsInitStream();

	mfInitialized = MFM_TRUE;

	return MF_ERROR_OKAY;
}

void mfTerminate()
{
	if (mfInitialized == MFM_FALSE)
		return;

	// Terminate stream library
	mfsTerminateStream();

	mfInitialized = MFM_FALSE;
}

mfmBool mfIsInitialized()
{
	return mfInitialized;
}