#include <Magma/Framework/Entry.hpp>
#include <Magma/Framework/String/Stream.hpp>
#include "Config.h"

void Magma::Framework::Init(int argc, char ** argv)
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