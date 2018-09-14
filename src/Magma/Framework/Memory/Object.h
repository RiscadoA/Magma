#pragma once

#include "Error.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef void (*mfmDestructor)(void*);

	typedef struct
	{
		mfmDestructor destructorFunc;
		volatile mfmI32 m_referenceCount;
	} mfmObject;

	/// <summary>
	///		Inits an object.
	/// </summary>
	/// <param name="obj">Object</param>
	/// <returns>
	///		Returns MF_ERROR_OKAY if there was no error.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mfmInitObject(mfmObject* obj);

	/// <summary>
	///		Deinits an object (doesn't call destructor).
	/// </summary>
	/// <param name="obj">Object pointer</param>
	/// <returns>
	///		Returns MF_ERROR_OKAY if there was no error.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mfmDeinitObject(mfmObject* obj);

	/// <summary>
	///		Increases an object's reference count.
	/// </summary>
	/// <param name="obj">Object pointer</param>
	/// <returns>
	///		Returns MF_ERROR_OKAY if there was no error.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mfmAcquireObject(mfmObject* obj);

	/// <summary>
	///		Decreases an object's reference count.
	/// </summary>
	/// <param name="obj">Object pointer</param>
	/// <returns>
	///		Returns MF_ERROR_OKAY if there was no error.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mfmReleaseObject(mfmObject* obj);

	/// <summary>
	///		Gets an object's reference count.
	/// </summary>
	/// <param name="obj">Object pointer</param>
	/// <param name="refCount">Out reference count</param>
	/// <returns>
	///		Returns MF_ERROR_OKAY if there was no error.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mfmGetObjectRefCount(mfmObject* obj, mfmI32* refCount);

#ifdef __cplusplus
}
#endif
