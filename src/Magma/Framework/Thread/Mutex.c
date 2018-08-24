#include "Mutex.h"
#include "Config.h"

#include "../Memory/Object.h"
#include "../Memory/Allocator.h"

#if defined(MAGMA_FRAMEWORK_USE_WINDOWS_THREADS)

#include <Windows.h>

struct mftMutex
{
	mfmObject object;
	void* allocator;
	HANDLE handle;
};

static void mftDestroyMutexNoErrors(void* mutex)
{
	if (mutex == NULL)
		abort();
	mfError err = mftDestroyMutex((mftMutex*)mutex);
	if (err != MF_ERROR_OKAY)
		abort();
}

mfError mftCreateMutex(mftMutex ** mutex, void * allocator)
{
	if (mutex == NULL)
		return MFT_ERROR_INVALID_ARGUMENTS;
	mfError err = mfmAllocate(allocator, mutex, sizeof(mftMutex));
	if (err != MF_ERROR_OKAY)
		return err;
	err = mfmInitObject(&(*mutex)->object);
	if (err != MF_ERROR_OKAY)
		return err;

	(*mutex)->object.destructorFunc = &mftDestroyMutexNoErrors;
	(*mutex)->allocator = allocator;
	(*mutex)->handle = CreateMutex(NULL, FALSE, NULL);
	if ((*mutex)->handle == NULL)
	{
		mfmDeallocate(allocator, *mutex);
		return MFT_ERROR_INTERNAL;
	}
	return MF_ERROR_OKAY;
}

mfError mftDestroyMutex(mftMutex * mutex)
{
	if (mutex == NULL)
		return MFT_ERROR_INVALID_ARGUMENTS;
	mfError err;

	// Close mutex handle
	if (CloseHandle(mutex->handle) == FALSE)
		return MFT_ERROR_INTERNAL;
	
	// Destroy and deallocate mutex
	err = mfmDestroyObject(&mutex->object);
	if (err != MF_ERROR_OKAY)
		return err;

	err = mfmDeallocate(mutex->allocator, mutex);
	if (err != MF_ERROR_OKAY)
		return err;
	return MF_ERROR_OKAY;
}

mfError mftLockMutex(mftMutex * mutex, mfmU32 timeOut)
{
	if (mutex == NULL)
		return MFT_ERROR_INVALID_ARGUMENTS;

	DWORD ret;
	if (timeOut == 0)
		ret = WaitForSingleObject(mutex->handle, INFINITE);
	else
		ret = WaitForSingleObject(mutex->handle, timeOut);

	switch (ret)
	{
		case WAIT_OBJECT_0:
			return MF_ERROR_OKAY;
		case WAIT_TIMEOUT:
			return MFT_ERROR_TIMEOUT;
		default:
			return MFT_ERROR_INTERNAL;
	}
}

mfError mftTryLockMutex(mftMutex * mutex)
{
	if (mutex == NULL)
		return MFT_ERROR_INVALID_ARGUMENTS;

	DWORD ret = WaitForSingleObject(mutex->handle, 0);

	switch (ret)
	{
		case WAIT_OBJECT_0:
			return MF_ERROR_OKAY;
		case WAIT_TIMEOUT:
			return MFT_ERROR_MUTEX_LOCKED;
		default:
			return MFT_ERROR_INTERNAL;
	}
}

mfError mftUnlockMutex(mftMutex * mutex)
{
	if (mutex == NULL)
		return MFT_ERROR_INVALID_ARGUMENTS;

	if (ReleaseMutex(mutex->handle) == FALSE)
		return MFT_ERROR_INTERNAL;
	return MF_ERROR_OKAY;
}

#else
#error No magma framework thread library support
#endif