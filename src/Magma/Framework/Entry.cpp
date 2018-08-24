#include "Entry.hpp"
#include "String/Stream.hpp"
#include "Input/Window.hpp"
#include "Input/OGLWindow.h"
#include "Input/D3DWindow.h"
#include "Config.h"

void Magma::Framework::Init(int argc, const char ** argv)
{
	mfError err = mfInit(argc, argv);
	if (err == MF_ERROR_ALREADY_INITIALIZED)
		throw CoreError(u8"Failed to init Magma Framework:\nFramework already initialized", err);
	else if (err != MF_ERROR_OKAY)
		throw CoreError(u8"Failed to init Magma Framework:\nmfInit returned an error code", err);

	String::InitStreams();
}

void Magma::Framework::Terminate()
{
	if (mfIsInitialized() == MFM_FALSE)
		return;

	String::TerminateStreams();

	mfTerminate();
}