#include <Magma/Framework/Entry.h>
#include <Magma/Framework/Thread/Thread.h>
#include <Magma/Framework/Thread/Mutex.h>
#include <Magma/Framework/String/Stream.h>

mftMutex* mutex;

void ThreadFunction(void* params)
{
	for (mfmU16 i = 0; i < 10000; ++i)
	{
		mftLockMutex(mutex, 0);
		mfsPutString(mfsOutStream, u8"Thread 1\n");
		mftUnlockMutex(mutex);
	}
}

int main(int argc, const char** argv)
{
	if (mfInit(argc, argv) != MF_ERROR_OKAY)
		abort();

	if (mftCreateMutex(&mutex, NULL) != MF_ERROR_OKAY)
		abort();

	mftThread* thread;
	if (mftCreateThread(&thread, &ThreadFunction, NULL, NULL) != MF_ERROR_OKAY)
		abort();

	for (mfmU16 i = 0; i < 10000; ++i)
	{
		mftLockMutex(mutex, 0);
		mfsPutString(mfsOutStream, u8"Thread 2\n");
		mftUnlockMutex(mutex);
	}

	if (mftWaitForThread(thread, 0) != MF_ERROR_OKAY)
		abort();

	if (mftDestroyThread(thread) != MF_ERROR_OKAY)
		abort();

	if (mftDestroyMutex(mutex) != MF_ERROR_OKAY)
		abort();

	mfTerminate();
	return 0;
}
