#pragma once

#include "Error.h"

/*
	Multi-threading functions.
*/

#ifdef __cplusplus
extern "C"
{
#endif

	// Is an mfmObject
	typedef struct mftThread mftThread;

	/// <summary>
	///		Creates a new thread.
	/// </summary>
	/// <param name="thread">Out thread handle</param>
	/// <param name="function">Thread function</param>
	/// <param name="args">Thread arguments</param>
	/// <param name="allocator">AllocatorHandle where the thread will be allocated</param>
	/// <returns>
	///		Returns MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mftCreateThread(mftThread** thread, void (*function)(void*), void* args, void* allocator);

	/// <summary>
	///		Destroys a previously created thread (the thread must have finished before calling this function).
	/// </summary>
	/// <param name="thread">Thread handle</param>
	/// <returns>
	///		Returns MF_ERROR_OKAY if there were no errors.
	///		Returns MFT_ERROR_STILL_RUNNING if the thread is still running.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mftDestroyThread(mftThread* thread);

	/// <summary>
	///		Waits for a thread to finish.
	/// </summary>
	/// <param name="thread">Thread handle</param>
	/// <param name="timeOut">Wait timeout in milliseconds (set to 0 to be infinite)</param>
	/// <returns>
	///		Returns MF_ERROR_OKAY if there were no errors.
	///		Returns MFT_ERROR_TIMEOUT if the time specified in <paramref name="timeOut">timeOut</paramref> has already passed and the thread still hasn't finished.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mftWaitForThread(mftThread* thread, mfmU32 timeOut);

#ifdef __cplusplus
}
#endif
