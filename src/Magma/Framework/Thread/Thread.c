#include "Thread.h"
#include "Atomic.h"
#include "Config.h"

#include "../Memory/Object.h"
#include "../Memory/Allocator.h"

#if defined(MAGMA_FRAMEWORK_USE_WINDOWS_THREADS)

#include <Windows.h>
#include <process.h>

struct mftThread
{
	mfmObject object;
	HANDLE* handle;
	void* allocator;
	void* args;
	void(*function)(void*);
	unsigned int id;

	volatile mfmBool running;
};

static unsigned int __stdcall mftThreadFunc(void* args)
{
	mftThread* thread = (mftThread*)args;
	if (mftAtomic8Store(&thread->running, MFM_TRUE) != MF_ERROR_OKAY)
		return 1;
	thread->function(thread->args);
	if (mftAtomic8Store(&thread->running, MFM_FALSE) != MF_ERROR_OKAY)
		return 2;
	return 0;
}

static void mftDestroyThreadNoErrors(void* thread)
{
	if (thread == NULL)
		abort();
	mfError err = mftDestroyThread((mftThread*)thread);
	if (err != MF_ERROR_OKAY)
		abort();
}

mfError mftCreateThread(mftThread ** thread, void(*function)(void*), void* args, void* allocator)
{
	if (thread == NULL || function == NULL)
		return MFT_ERROR_INVALID_ARGUMENTS;
	mfError err = mfmAllocate(allocator, thread, sizeof(mftThread));
	if (err != MF_ERROR_OKAY)
		return err;
	err = mfmInitObject(&(*thread)->object);
	if (err != MF_ERROR_OKAY)
		return err;

	(*thread)->object.destructorFunc = &mftDestroyThreadNoErrors;
	(*thread)->allocator = allocator;
	(*thread)->args = args;
	(*thread)->function = function;
	(*thread)->running = MFM_FALSE;
	(*thread)->handle = _beginthreadex(NULL, 0, &mftThreadFunc, *thread, 0, &(*thread)->id);
	if ((*thread)->handle == NULL)
	{
		mfmDeallocate(allocator, *thread);
		return MFT_ERROR_INTERNAL;
	}
	return MF_ERROR_OKAY;
}

mfError mftDestroyThread(mftThread * thread)
{
	if (thread == NULL)
		return MFT_ERROR_INVALID_ARGUMENTS;
	mfError err;

	// Check if the thread is running
	mfmBool running = MFM_FALSE;
	err = mftAtomic8Load(&thread->running, &running);
	if (err != MF_ERROR_OKAY)
		return err;
	if (running != MFM_FALSE)
		return MFT_ERROR_STILL_RUNNING;

	// Close thread handle
	if (CloseHandle(thread->handle) == FALSE)
		return MFT_ERROR_INTERNAL;

	// Destroy and deallocate thread
	err = mfmDestroyObject(&thread->object);
	if (err != MF_ERROR_OKAY)
		return err;

	err = mfmDeallocate(thread->allocator, thread);
	if (err != MF_ERROR_OKAY)
		return err;
	return MF_ERROR_OKAY;
}

mfError mftWaitForThread(mftThread * thread, mfmU32 timeOut)
{
	DWORD ret;
	if (timeOut == 0)
		ret = WaitForSingleObject(thread->handle, INFINITE);
	else
		ret = WaitForSingleObject(thread->handle, timeOut);
	
	if (ret == WAIT_TIMEOUT)
		return MFT_ERROR_TIMEOUT;
	else if (ret == WAIT_FAILED)
		return MFT_ERROR_INTERNAL;
	else
		return MF_ERROR_OKAY;
}

#else
#error No magma framework thread library support
#endif