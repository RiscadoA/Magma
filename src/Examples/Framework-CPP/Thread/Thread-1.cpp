#include <Magma/Framework/Entry.hpp>
#include <Magma/Framework/Thread/Thread.hpp>
#include <Magma/Framework/Thread/Atomic.hpp>
#include <Magma/Framework/Thread/Mutex.hpp>
#include <Magma/Framework/String/Stream.hpp>
#include <Magma/Framework/Memory/Functor.hpp>

using namespace Magma::Framework;

Thread::Atomic<mfmI32> count;

class Test
{
public:
	void test1(void* param)
	{
		Thread::HMutex mutex = param;
		for (mfmU16 i = 0; i < 1000; ++i)
		{
			auto localCount = count.Load();
			count.Add(1);
			mutex.Lock();
			String::OutStream.PutString(u8"Thread 1\n");
			mutex.Unlock();
		}
	}
};

void test2(mfmI32 x, mfmF32)
{
	mfsPutString(mfsOutStream, u8"test2\n");
}

int main(int argc, const char** argv)
{
	Magma::Framework::Init(argc, argv);

	Test test;

	count.Store(0);

	auto mutex = Thread::CreateMutex();
	auto thread = Thread::CreateThread(Memory::GetMethod(test, &Test::test1), mutex.GetNoChecks(), Memory::StandardAllocator);

	for (mfmU16 i = 0; i < 1000; ++i)
	{
		auto localCount = count.Load();
		count.Add(1);
		mutex.Lock();
		String::OutStream.PutString(u8"Thread 2\n");
		mutex.Unlock();
	}

	thread.Wait();
	thread.Release();
	mutex.Release();
	
	Magma::Framework::Terminate();
	return 0;
}
