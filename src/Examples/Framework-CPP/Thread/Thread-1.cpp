#include <Magma/Framework/Entry.hpp>
#include <Magma/Framework/Thread/Thread.h>
#include <Magma/Framework/Thread/Mutex.hpp>
#include <Magma/Framework/String/Stream.hpp>
#include <Magma/Framework/Memory/Functor.hpp>

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

class Test
{
public:
	void test1(mfmI32 x, mfmF32) { mfsPutString(mfsOutStream, u8"test1\n"); }
};

void test2(mfmI32 x, mfmF32)
{
	mfsPutString(mfsOutStream, u8"test2\n");
}

int main(int argc, const char** argv)
{
	Magma::Framework::Init(argc, argv);

	Test test;

	auto func = Memory::GetMethod(test, &Test::test1, Memory::StandardAllocator);

	func(2, 0.0f);

	func = &test2;

	func(2, 0.0f);

	func = [](mfmI32, mfmF32) { mfsPutString(mfsOutStream, u8"test3\n"); };

	func(2, 0.0f);
	
	auto mutex = Thread::CreateMutex();

	mftThread* thread;
	if (mftCreateThread(&thread, &ThreadFunction, &mutex.Get(), NULL) != MF_ERROR_OKAY)
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
