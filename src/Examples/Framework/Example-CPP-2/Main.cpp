#include <Magma/Framework/Entry.hpp>
#include <Magma/Framework/Thread/Thread.h>
#include <Magma/Framework/String/Stream.hpp>

using namespace Magma::Framework;

void ThreadFunction(void* params)
{
	for (mfmU16 i = 0; i < 10000; ++i)
		String::OutStream.PutString(u8"Test string\n");
}

int main(int argc, const char** argv)
{
	Magma::Framework::Init(argc, argv);

	mftThread* thread;
	if (mftCreateThread(&thread, &ThreadFunction, NULL, NULL) != MFT_ERROR_OKAY)
		abort();

	if (mftWaitForThread(thread, 0) != MFT_ERROR_OKAY)
		abort();

	if (mftDestroyThread(thread) != MFT_ERROR_OKAY)
		abort();

	Magma::Framework::Terminate();
	return 0;
}
