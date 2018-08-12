#pragma once

#include "../Memory/Type.h"
#include "../Memory/Object.h"
#include "UTF8.h"
#include "Error.h"

/*
	Stream and function declarations.
*/

#ifdef __cplusplus
extern "C"
{
#endif

#define MFS_FILE_READ	0x01
#define MFS_FILE_WRITE	0x02

	typedef mfsError(*mfsStreamReadFunction)(void*, mfmU8*, mfmU64, mfmU64*);
	typedef mfsError(*mfsStreamWriteFunction)(void*, const mfmU8*, mfmU64, mfmU64*);
	typedef mfsError(*mfsStreamFlushFunction)(void*);
	typedef mfsError(*mfsStreamSetBufferFunction)(void*, mfmU8*, mfmU64);
	
	typedef struct
	{
		mfmObject object;
		mfsStreamReadFunction read;
		mfsStreamWriteFunction write;
		mfsStreamFlushFunction flush;
		mfsStreamSetBufferFunction setBuffer;

		mfmU8* buffer;
		mfmU64 bufferSize;
		mfmU64 bufferHead;
	} mfsStream;

	extern mfsStream* mfsIn;
	extern mfsStream* mfsOut;

	/// <summary>
	///		Initializes the stream library.
	/// </summary>
	void mfsInitStream();

	/// <summary>
	///		Terminates the stream library.
	/// </summary>
	void mfsTerminateStream();

	/// <summary>
	///		Writes raw data into a stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="data">Pointer to data</param>
	/// <param name="dataSize">Data size</param>
	/// <param name="outSize">Size of the data that was written (optional, can be NULL)</param>
	/// <returns>
	///		MFS_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream is NULL or data is NULL.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfsError mfsWrite(mfsStream* stream, const mfmU8* data, mfmU64 dataSize, mfmU64* outSize);

	/// <summary>
	///		Reads raw data from a stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="data">Pointer to data</param>
	/// <param name="dataSize">Max data size to read</param>
	/// <param name="outSize">Size of the data that was read (optional, can be NULL)</param>
	/// <returns>
	///		MFS_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream is NULL or data is NULL.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfsError mfsRead(mfsStream* stream, mfmU8* data, mfmU64 dataSize, mfmU64* outSize);

	/// <summary>
	///		Flushes a stream if the stream is buffered.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <returns>
	///		MFS_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream is NULL.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfsError mfsFlush(mfsStream* stream);

	/// <summary>
	///		Sets the buffer that is used for internal stream operations.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="buffer">New buffer (if NULL, the stream becomes unbuffered)</param>
	/// <param name="bufferSize">New buffer size</param>
	/// <returns>
	///		MFS_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream is NULL.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfsError mfsSetBuffer(mfsStream* stream, mfmU8* buffer, mfmU64 bufferSize);

	/// <summary>
	///		Opens a file stream using the stdio.h library.
	/// </summary>
	/// <param name="stream">Pointer to stream pointer</param>
	/// <param name="mode">Open mode (MFS_FILE_READ or MFS_FILE_WRITE)</param>
	/// <param name="path">File path</param>
	/// <returns>
	///		MFS_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream or path are NULL or if the <paramref name="mode">file mode</paramref> is invalid.
	///		MFS_ERROR_INTERNAL if the system couldn't open the file on the specified <paramref name="path">path</paramref>.
	/// </returns>
	mfsError mfsOpenFile(mfsStream** stream, mfmU32 mode, const mfsUTF8CodeUnit* path);

	/// <summary>
	///		Closes a file stream opened by mfsOpenFile.
	/// </summary>
	/// <param name="stream">File stream</param>
	void mfsCloseFile(mfsStream* stream);

#ifdef __cplusplus
}
#endif