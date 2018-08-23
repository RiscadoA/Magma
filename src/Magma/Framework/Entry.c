#include "Entry.h"

#include "String/Stream.h"

static mfmBool mfInitialized = MFM_FALSE;

mfError mfInit(int argc, const char** argv)
{
	if (mfInitialized == MFM_TRUE)
		return MF_ERROR_ALREADY_INITIALIZED;
	mfError err;

	// Init stream library
	err = mfsInitStream();
	if (err != MFS_ERROR_OKAY)
		return err;

	mfInitialized = MFM_TRUE;

	return MF_ERROR_OKAY;
}

void mfTerminate()
{
	if (mfInitialized == MFM_FALSE)
		return;
	mfError err;

	// Terminate stream library
	err = mfsTerminateStream();
	if (err != MFS_ERROR_OKAY)
		abort();

	mfInitialized = MFM_FALSE;
}

mfmBool mfIsInitialized()
{
	return mfInitialized;
}