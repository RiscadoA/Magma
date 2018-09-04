#pragma once

#include "../Error.h"
#include "../String/Stream.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/// <summary>
	///		Creates a stream that decompresses data using DEFLATE.
	/// </summary>
	/// <param name="stream">Out stream handle</param>
	/// <param name="buffer">Stream buffer</param>
	/// <param name="bufferSize">Stream buffer size</param>
	/// <param name="allocator">Stream internal allocator</param>
	mfError mfmCreateInflateStream(mfsStream** stream, mfmU8* buffer, mfmU64 bufferSize, void* allocator);

	/// <summary>
	///		Destroys a DEFLATE stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	void mfmDestroyInflateStream(mfsStream* stream);

#ifdef __cplusplus
}
#endif
