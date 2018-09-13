#include "Mutex.hpp"
#include "Exception.hpp"
#include "../ErrorString.h"

bool Magma::Framework::Thread::HMutex::Lock(mfmU64 timeOut)
{
	mfError err = mftLockMutex((mftMutex*)&this->Get(), timeOut);
	if (err == MF_ERROR_OKAY)
		return true;
	else if (err == MFT_ERROR_TIMEOUT)
		return false;
	throw MutexError(mfErrorToString(err));
}

void Magma::Framework::Thread::HMutex::Unlock()
{
	mfError err = mftUnlockMutex((mftMutex*)&this->Get());
	if (err != MF_ERROR_OKAY)
		throw MutexError(mfErrorToString(err));
}

bool Magma::Framework::Thread::HMutex::TryLock()
{
	mfError err = mftTryLockMutex((mftMutex*)&this->Get());
	if (err == MF_ERROR_OKAY)
		return true;
	else if (err == MFT_ERROR_MUTEX_LOCKED)
		return false;
	throw MutexError(mfErrorToString(err));
}

Magma::Framework::Thread::HMutex Magma::Framework::Thread::CreateMutex(Memory::HAllocator allocator)
{
	mftMutex* mtx;
	mfError err = mftCreateMutex(&mtx, allocator.GetNoChecks());
	if (err != MF_ERROR_OKAY)
		throw MutexError(mfErrorToString(err));
	return mtx;
}