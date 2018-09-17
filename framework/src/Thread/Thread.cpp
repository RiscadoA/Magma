#include <Magma/Framework/Thread/Thread.hpp>
#include <Magma/Framework/Thread/Atomic.hpp>
#include <Magma/Framework/Thread/Exception.hpp>
#include <Magma/Framework/ErrorString.h>

using namespace Magma::Framework;
using namespace Magma::Framework::Thread;

struct ThreadData
{
	Memory::Functor<void(void*)> func;
	void* param;
	volatile mfmI32 done;
};

static void ThreadFunction(void* param)
{
	auto data = (ThreadData*)param;
	Memory::Functor<void(void*)> f = data->func;
	void* p = data->param;
	mfError err = mftAtomic32Store(&data->done, MFM_TRUE);
	if (err != MF_ERROR_OKAY)
		throw ThreadError(mfErrorToString(err));
	f(p);
}

bool Magma::Framework::Thread::HThread::Wait(mfmU64 timeOut)
{
	mfError err = mftWaitForThread((mftThread*)&this->Get(), timeOut);
	if (err == MF_ERROR_OKAY)
		return true;
	else if (err == MFT_ERROR_TIMEOUT)
		return false;
	throw ThreadError(mfErrorToString(err));
}

Magma::Framework::Thread::HThread Magma::Framework::Thread::CreateThread(Memory::Functor<void(void*)> func, void * param, Memory::HAllocator allocator)
{
	ThreadData data = { func, param, MFM_FALSE };
	mftThread* thread = NULL;
	mfError err = mftCreateThread(&thread, &ThreadFunction, &data, allocator.GetNoChecks());
	if (err != MF_ERROR_OKAY)
		throw ThreadError(mfErrorToString(err));
	mfmI32 done = MFM_FALSE;
	while (done == MFM_FALSE)
	{
		err = mftAtomic32Load(&data.done, &done);
		if (err != MF_ERROR_OKAY)
			throw ThreadError(mfErrorToString(err));
	}
	return thread;
}