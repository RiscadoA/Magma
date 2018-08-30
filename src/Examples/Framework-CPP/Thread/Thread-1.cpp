#include <Magma/Framework/Entry.hpp>
#include <Magma/Framework/Thread/Thread.h>
#include <Magma/Framework/Thread/Mutex.h>
#include <Magma/Framework/String/Stream.hpp>

using namespace Magma::Framework;

mftMutex* mutex;

void ThreadFunction(void* params)
{
	for (mfmU16 i = 0; i < 10000; ++i)
	{
		mftLockMutex(mutex, 0);
		String::OutStream.PutString(u8"Test string 1\n");
		mftUnlockMutex(mutex);
	}
}

int main(int argc, const char** argv)
{
	Magma::Framework::Init(argc, argv);

	if (mftCreateMutex(&mutex, NULL) != MF_ERROR_OKAY)
		abort();

	mftThread* thread;
	if (mftCreateThread(&thread, &ThreadFunction, NULL, NULL) != MF_ERROR_OKAY)
		abort();

	for (mfmU16 i = 0; i < 10000; ++i)
	{
		mftLockMutex(mutex, 0);
		String::OutStream.PutString(u8"Test string 2\n");
		mftUnlockMutex(mutex);
	}

	if (mftWaitForThread(thread, 0) != MF_ERROR_OKAY)
		abort();

	if (mftDestroyThread(thread) != MF_ERROR_OKAY)
		abort();

	if (mftDestroyMutex(mutex) != MF_ERROR_OKAY)
		abort();

	Magma::Framework::Terminate();
	return 0;
}
