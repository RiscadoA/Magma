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

	typedef mfError(*mfsStreamReadFunction)(void*, mfmU8*, mfmU64, mfmU64*);
	typedef mfError(*mfsStreamWriteFunction)(void*, const mfmU8*, mfmU64, mfmU64*);
	typedef mfError(*mfsStreamFlushFunction)(void*);
	typedef mfError(*mfsStreamSetBufferFunction)(void*, mfmU8*, mfmU64);
	typedef mfError(*mfsStreamSeekBeginFunction)(void*, mfmU64 offset);
	typedef mfError(*mfsStreamSeekEndFunction)(void*, mfmU64 offset);
	typedef mfError(*mfsStreamSeekHeadFunction)(void*, mfmI64 offset);
	typedef mfError(*mfsStreamTellFunction)(void*, mfmU64* position);
	typedef mfError(*mfsStreamEOFFunction)(void*, mfmBool* eof);
	
	typedef struct
	{
		mfmObject object;
		mfsStreamReadFunction read;
		mfsStreamWriteFunction write;
		mfsStreamFlushFunction flush;
		mfsStreamSetBufferFunction setBuffer;
		mfsStreamSeekBeginFunction seekBegin;
		mfsStreamSeekEndFunction seekEnd;
		mfsStreamSeekHeadFunction seekHead;
		mfsStreamTellFunction tell;
		mfsStreamEOFFunction eof;

		mfmU8* buffer;
		mfmU64 bufferSize;
	} mfsStream;

	extern mfsStream* mfsInStream;
	extern mfsStream* mfsOutStream;
	extern mfsStream* mfsErrStream;

	/// <summary>
	///		Initializes the stream library.
	/// </summary>
	mfError mfsInitStream();

	/// <summary>
	///		Terminates the stream library.
	/// </summary>
	mfError mfsTerminateStream();

	/// <summary>
	///		Writes raw data into a stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="data">Pointer to data</param>
	/// <param name="dataSize">Data size</param>
	/// <param name="outSize">Size of the data that was written (optional, can be NULL)</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream is NULL or data is NULL.
	///		MFS_ERROR_UNSUPPORTED_FUNCTION if the stream doesn't support this function.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfError mfsWrite(mfsStream* stream, const mfmU8* data, mfmU64 dataSize, mfmU64* outSize);

	/// <summary>
	///		Reads raw data from a stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="data">Pointer to data</param>
	/// <param name="dataSize">Max data size to read</param>
	/// <param name="outSize">Size of the data that was read (optional, can be NULL)</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream is NULL or data is NULL.
	///		MFS_ERROR_UNSUPPORTED_FUNCTION if the stream doesn't support this function.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfError mfsRead(mfsStream* stream, mfmU8* data, mfmU64 dataSize, mfmU64* outSize);

	/// <summary>
	///		Reads raw data from a stream until a certain character sequence is found or the buffer is filled.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="data">Pointer to data</param>
	/// <param name="dataSize">Max data size to read</param>
	/// <param name="outSize">Size of the data that was read (optional, can be NULL)</param>
	/// <param name="terminator">Terminating character sequence</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream is NULL or data is NULL.
	///		MFS_ERROR_UNSUPPORTED_FUNCTION if the stream doesn't support this function.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfError mfsReadUntil(mfsStream* stream, mfmU8* data, mfmU64 dataSize, mfmU64* outSize, const mfsUTF8CodeUnit * terminator);

	/// <summary>
	///		Flushes a stream if the stream is buffered.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream is NULL.
	///		MFS_ERROR_UNSUPPORTED_FUNCTION if the stream doesn't support this function.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfError mfsFlush(mfsStream* stream);

	/// <summary>
	///		Sets the buffer that is used for internal stream operations.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="buffer">New buffer (if NULL, the stream becomes unbuffered)</param>
	/// <param name="bufferSize">New buffer size</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream is NULL.
	///		MFS_ERROR_UNSUPPORTED_FUNCTION if the stream doesn't support this function.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfError mfsSetBuffer(mfsStream* stream, mfmU8* buffer, mfmU64 bufferSize);

	/// <summary>
	///		Reads a byte from a stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="byte">Pointer to byte( (set to NULL to ignore)</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream is NULL.
	///		MFS_ERROR_EOF if the stream got to EOF.
	///		MFS_ERROR_FAILED_TO_READ_ALL if the stream couldn't read the byte.
	///		MFS_ERROR_UNSUPPORTED_FUNCTION if the stream doesn't support this function.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfError mfsGetByte(mfsStream* stream, mfmU8* byte);

	/// <summary>
	///		Writes a byte to a stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="byte">Byte to write</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream is NULL.
	///		MFS_ERROR_FAILED_TO_WRITE_ALL if the stream couldn't write the byte.
	///		MFS_ERROR_UNSUPPORTED_FUNCTION if the stream doesn't support this function.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfError mfsPutByte(mfsStream* stream, mfmU8 byte);

	/// <summary>
	///		Prints a UTF-8 string to a stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="str">Format UTF-8 string</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream or str are NULL.
	///		MFS_ERROR_UNSUPPORTED_FUNCTION if the stream doesn't support this function.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfError mfsPutString(mfsStream* stream, const mfsUTF8CodeUnit* str);

	/// <summary>
	///		Prints a formatted UTF-8 string to a stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="format">Format UTF-8 string</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream or format are NULL.
	///		MFS_ERROR_UNSUPPORTED_FUNCTION if the stream doesn't support this function.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfError mfsPrintFormat(mfsStream* stream, const mfsUTF8CodeUnit* format, ...);

	/// <summary>
	///		Seeks a position on a stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="offset">Position offset relative to begin</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream is NULL.
	///		MFS_ERROR_UNSUPPORTED_FUNCTION if the stream doesn't support this function.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfError mfsSeekBegin(mfsStream* stream, mfmU64 offset);

	/// <summary>
	///		Seeks a position on a stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="offset">Position offset relative to end</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream is NULL.
	///		MFS_ERROR_UNSUPPORTED_FUNCTION if the stream doesn't support this function.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfError mfsSeekEnd(mfsStream* stream, mfmU64 offset);

	/// <summary>
	///		Seeks a position on a stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="offset">Position offset relative to head</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream is NULL.
	///		MFS_ERROR_UNSUPPORTED_FUNCTION if the stream doesn't support this function.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfError mfsSeekHead(mfsStream* stream, mfmI64 offset);

	/// <summary>
	///		Gets the current position on a stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="outPosition">Out head position</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream is NULL.
	///		MFS_ERROR_UNSUPPORTED_FUNCTION if the stream doesn't support this function.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfError mfsTell(mfsStream* stream, mfmU64* outPosition);

	/// <summary>
	///		Checks if a stream has reached EOF.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="eof">Out EOF (MFM_TRUE is has reached EOF, otherwise MFM_FALSE)</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream is NULL.
	///		MFS_ERROR_UNSUPPORTED_FUNCTION if the stream doesn't support this function.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfError mfsEOF(mfsStream* stream, mfmBool* eof);

#ifdef __cplusplus
}
#endif
