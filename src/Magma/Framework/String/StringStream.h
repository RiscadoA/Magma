#pragma once

#include "Stream.h"

/*
	String stream implementation.
*/

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct
	{
		mfsStream base;
		void* allocator;
		mfmU64 writeHead;
		mfmU64 readHead;
	} mfsStringStream;

	/// <summary>
	///		Creates a new string stream.
	/// </summary>
	/// <param name="stream">Pointer to stream handle</param>
	/// <param name="buffer">Buffer where the data will be written to</param>
	/// <param name="size">Buffer size</param>
	/// <param name="allocator">The allocator the stream is going to use</param>
	/// <returns>The error code (MF_ERROR_OKAY if there no errors).</returns>
	mfError mfsCreateStringStream(mfsStream** stream, mfmU8* buffer, mfmU64 size, void* allocator);

	/// <summary>
	///		Destroys a string stream created by mfsCreateStringStream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	void mfsDestroyStringStream(mfsStream* stream);

	/// <summary>
	///		Creates a new string stream without allocating space for it.
	/// </summary>
	/// <param name="stream">Pointer to stream</param>
	/// <param name="buffer">Buffer where the data will be written to</param>
	/// <param name="size">Buffer size</param>
	/// <returns>The error code (MF_ERROR_OKAY if there no errors).</returns>
	mfError mfsCreateLocalStringStream(mfsStringStream* stream, mfmU8* buffer, mfmU64 size);

	/// <summary>
	///		Destroys a string stream created by mfsCreateLocalStringStream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	void mfsDestroyLocalStringStream(mfsStringStream* stream);

	/// <summary>
	///		Clears all data in a string stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <returns>The error code (MF_ERROR_OKAY if there no errors).</returns>
	mfError mfsClearStringStream(mfsStream* stream);

#ifdef __cplusplus
}
#endif
