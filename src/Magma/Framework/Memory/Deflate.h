#pragma once

#include "../Error.h"
#include "../String/Stream.h"

#include "Compression.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/// <summary>
	///		Creates a stream that compresses data using DEFLATE.
	/// </summary>
	/// <param name="stream">Out stream handle</param>
	/// <param name="buffer">Stream buffer</param>
	/// <param name="bufferSize">Stream buffer size</param>
	/// <param name="compression">Compression mode</param>
	/// <param name="allocator">Stream internal allocator</param>
	mfError mfmCreateDeflateStream(mfsStream** stream, mfmU8* buffer, mfmU64 bufferSize, mfmEnum compression, void* allocator);

	/// <summary>
	///		Destroys a DEFLATE stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	void mfmDestroyDeflateStream(mfsStream* stream);

#ifdef __cplusplus
}
#endif
