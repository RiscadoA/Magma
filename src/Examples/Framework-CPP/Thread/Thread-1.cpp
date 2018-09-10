#include <Magma/Framework/Entry.hpp>
#include <Magma/Framework/Thread/Thread.h>
#include <Magma/Framework/Thread/Mutex.hpp>
#include <Magma/Framework/String/Stream.hpp>

using namespace Magma::Framework;

void ThreadFunction(void* param)
{
	Thread::HMutex mutex = param;

	for (mfmU16 i = 0; i < 10000; ++i)
	{
		mutex.Lock();
		String::OutStream.PutString(u8"Thread 1\n");
		mutex.Unlock();
	}
}

int main(int argc, const char** argv)
{
	Magma::Framework::Init(argc, argv);

	auto mutex = Thread::CreateMutex();

	mftThread* thread;
	if (mftCreateThread(&thread, &ThreadFunction, mutex.GetNoChecks(), NULL) != MF_ERROR_OKAY)
		abort();

	for (mfmU16 i = 0; i < 10000; ++i)
	{
		mutex.Lock();
		String::OutStream.PutString(u8"Thread 2\n");
		mutex.Unlock();
	}

	if (mftWaitForThread(thread, 0) != MF_ERROR_OKAY)
		abort();

	if (mftDestroyThread(thread) != MF_ERROR_OKAY)
		abort();

	mutex.Release();
	
	Magma::Framework::Terminate();
	return 0;
}
