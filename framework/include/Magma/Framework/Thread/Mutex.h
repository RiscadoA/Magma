#pragma once

#include "Error.h"

/*
	Mutual exclusive locks (mutex) functions.
*/

#ifdef __cplusplus
extern "C"
{
#endif

	// Is an mfmObject
	typedef struct mftMutex mftMutex;

	/// <summary>
	///		Creates a new mutex lock.
	/// </summary>
	/// <param name="mutex">Out mutex handle</param>
	/// <param name="allocator">AllocatorHandle where the mutex lock will be allocated</param>
	/// <returns>
	///		Returns MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mftCreateMutex(mftMutex** mutex, void* allocator);

	/// <summary>
	///		Destroys a previously created mutex lock (the thread must have been unlocked before calling this function).
	/// </summary>
	/// <param name="mutex">Mutex handle</param>
	/// <returns>
	///		Returns MF_ERROR_OKAY if there were no errors.
	///		Returns MFT_ERROR_STILL_RUNNING if the mutex is still locked.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mftDestroyMutex(mftMutex* mutex);

	/// <summary>
	///		Locks a mutex (if it is already locked by another thread, waits until it is unlocked).
	/// </summary>
	/// <param name="mutex">Mutex handle</param>
	/// <param name="timeOut">Wait timeout in milliseconds (set to 0 to be infinite)</param>
	/// <returns>
	///		Returns MF_ERROR_OKAY if there were no errors.
	///		Returns MFT_ERROR_TIMEOUT if the time specified in <paramref name="timeOut">timeOut</paramref> has already passed and the mutex still hasn't been unlocked.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mftLockMutex(mftMutex* mutex, mfmU32 timeOut);

	/// <summary>
	///		Tries to lock a mutex.
	/// </summary>
	/// <param name="mutex">Mutex handle</param>
	/// <returns>
	///		Returns MF_ERROR_OKAY if there were no errors and the mutex was successfully locked.
	///		Returns MFT_ERROR_MUTEX_LOCKED if the mutex is locked.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mftTryLockMutex(mftMutex* mutex);

	/// <summary>
	///		Unlocks a mutex.
	/// </summary>
	/// <param name="mutex">Mutex handle</param>
	/// <returns>
	///		Returns MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mftUnlockMutex(mftMutex* mutex);

#ifdef __cplusplus
}
#endif
