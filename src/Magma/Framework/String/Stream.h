#pragma once

#include "../Memory/Type.h"
#include "../Memory/Object.h"
#include "UTF8.h"
#include "Error.h"

#include <stdarg.h>

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
	///		Prints an unsigned 8 bit integer to a stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="value">Value</param>
	/// <param name="base">Base (2-16)</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream is NULL.
	///		MFS_ERROR_UNSUPPORTED_FUNCTION if the stream doesn't support this function.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfError mfsPrintU8(mfsStream* stream, mfmU8 value, mfmU64 base);

	/// <summary>
	///		Prints an unsigned 16 bit integer to a stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="value">Value</param>
	/// <param name="base">Base (2-16)</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream is NULL.
	///		MFS_ERROR_UNSUPPORTED_FUNCTION if the stream doesn't support this function.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfError mfsPrintU16(mfsStream* stream, mfmU16 value, mfmU64 base);

	/// <summary>
	///		Prints an unsigned 32 bit integer to a stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="value">Value</param>
	/// <param name="base">Base (2-16)</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream is NULL.
	///		MFS_ERROR_UNSUPPORTED_FUNCTION if the stream doesn't support this function.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfError mfsPrintU32(mfsStream* stream, mfmU32 value, mfmU64 base);

	/// <summary>
	///		Prints an unsigned 64 bit integer to a stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="value">Value</param>
	/// <param name="base">Base (2-16)</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream is NULL.
	///		MFS_ERROR_UNSUPPORTED_FUNCTION if the stream doesn't support this function.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfError mfsPrintU64(mfsStream* stream, mfmU64 value, mfmU64 base);

	/// <summary>
	///		Prints an signed 8 bit integer to a stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="value">Value</param>
	/// <param name="base">Base (2-16)</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream is NULL.
	///		MFS_ERROR_UNSUPPORTED_FUNCTION if the stream doesn't support this function.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfError mfsPrintI8(mfsStream* stream, mfmI8 value, mfmU64 base);

	/// <summary>
	///		Prints an signed 16 bit integer to a stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="value">Value</param>
	/// <param name="base">Base (2-16)</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream is NULL.
	///		MFS_ERROR_UNSUPPORTED_FUNCTION if the stream doesn't support this function.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfError mfsPrintI16(mfsStream* stream, mfmI16 value, mfmU64 base);

	/// <summary>
	///		Prints an signed 32 bit integer to a stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="value">Value</param>
	/// <param name="base">Base (2-16)</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream is NULL.
	///		MFS_ERROR_UNSUPPORTED_FUNCTION if the stream doesn't support this function.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfError mfsPrintI32(mfsStream* stream, mfmI32 value, mfmU64 base);

	/// <summary>
	///		Prints an signed 64 bit integer to a stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="value">Value</param>
	/// <param name="base">Base (2-16)</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream is NULL.
	///		MFS_ERROR_UNSUPPORTED_FUNCTION if the stream doesn't support this function.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfError mfsPrintI64(mfsStream* stream, mfmI64 value, mfmU64 base);

	/// <summary>
	///		Prints a 32 bit floating point to a stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="value">Value</param>
	/// <param name="base">Base (2-16)</param>
	/// <param name="decimalPlaces">How many decimal places should be shown</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream is NULL.
	///		MFS_ERROR_UNSUPPORTED_FUNCTION if the stream doesn't support this function.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfError mfsPrintF32(mfsStream* stream, mfmF32 value, mfmU64 base, mfmU64 decimalPlaces);

	/// <summary>
	///		Prints a 64 bit floating point to a stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="value">Value</param>
	/// <param name="base">Base (2-16)</param>
	/// <param name="decimalPlaces">How many decimal places should be shown</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream is NULL.
	///		MFS_ERROR_UNSUPPORTED_FUNCTION if the stream doesn't support this function.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfError mfsPrintF64(mfsStream* stream, mfmF64 value, mfmU64 base, mfmU64 decimalPlaces);

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
	///		Prints a formatted UTF-8 string to a stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="format">Format UTF-8 string</param>
	/// <param name="list">Param list</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream or format are NULL.
	///		MFS_ERROR_UNSUPPORTED_FUNCTION if the stream doesn't support this function.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfError mfsPrintFormatList(mfsStream* stream, const mfsUTF8CodeUnit* format, va_list list);

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

	/// <summary>
	///		Parses an unsigned 8 bit integer from a stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="value">Out value</param>
	/// <param name="base">Base (2-16)</param>
	/// <param name="sep">String that separates input numbers</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream is NULL.
	///		MFS_ERROR_UNSUPPORTED_FUNCTION if the stream doesn't support this function.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfError mfsParseU8(mfsStream* stream, mfmU8* value, mfmU64 base, const mfsUTF8CodeUnit* sep);

	/// <summary>
	///		Parses an unsigned 16 bit integer from a stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="value">Out value</param>
	/// <param name="base">Base (2-16)</param>
	/// <param name="sep">String that separates input numbers</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream is NULL.

	///		MFS_ERROR_UNSUPPORTED_FUNCTION if the stream doesn't support this function.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfError mfsParseU16(mfsStream* stream, mfmU16* value, mfmU64 base, const mfsUTF8CodeUnit* sep);

	/// <summary>
	///		Parses an unsigned 32 bit integer from a stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="value">Out value</param>
	/// <param name="base">Base (2-16)</param>
	/// <param name="sep">String that separates input numbers</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream is NULL.
	///		MFS_ERROR_UNSUPPORTED_FUNCTION if the stream doesn't support this function.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfError mfsParseU32(mfsStream* stream, mfmU32* value, mfmU64 base, const mfsUTF8CodeUnit* sep);

	/// <summary>
	///		Parses an unsigned 64 bit integer from a stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="value">Out value</param>
	/// <param name="base">Base (2-16)</param>
	/// <param name="sep">String that separates input numbers</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream is NULL.
	///		MFS_ERROR_UNSUPPORTED_FUNCTION if the stream doesn't support this function.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfError mfsParseU64(mfsStream* stream, mfmU64* value, mfmU64 base, const mfsUTF8CodeUnit* sep);

	/// <summary>
	///		Parses an signed 8 bit integer from a stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="value">Out value</param>
	/// <param name="sep">String that separates input numbers</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream is NULL.
	///		MFS_ERROR_UNSUPPORTED_FUNCTION if the stream doesn't support this function.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfError mfsParseI8(mfsStream* stream, mfmI8* value, mfmU64 base, const mfsUTF8CodeUnit* sep);

	/// <summary>
	///		Parses an signed 16 bit integer from a stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="value">Out value</param>
	/// <param name="base">Base (2-16)</param>
	/// <param name="sep">String that separates input numbers</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream is NULL.
	///		MFS_ERROR_UNSUPPORTED_FUNCTION if the stream doesn't support this function.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfError mfsParseI16(mfsStream* stream, mfmI16* value, mfmU64 base, const mfsUTF8CodeUnit* sep);

	/// <summary>
	///		Parses an signed 32 bit integer from a stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="value">Out value</param>
	/// <param name="base">Base (2-16)</param>
	/// <param name="sep">String that separates input numbers</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream is NULL.
	///		MFS_ERROR_UNSUPPORTED_FUNCTION if the stream doesn't support this function.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfError mfsParseI32(mfsStream* stream, mfmI32* value, mfmU64 base, const mfsUTF8CodeUnit* sep);

	/// <summary>
	///		Parses an signed 64 bit integer from a stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="value">Out value</param>
	/// <param name="base">Base (2-16)</param>
	/// <param name="sep">String that separates input numbers</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream is NULL.
	///		MFS_ERROR_UNSUPPORTED_FUNCTION if the stream doesn't support this function.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfError mfsParseI64(mfsStream* stream, mfmI64* value, mfmU64 base, const mfsUTF8CodeUnit* sep);

	/// <summary>
	///		Parses an 32 bit floating point from a stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="value">Out value</param>
	/// <param name="base">Base (2-16)</param>
	/// <param name="sep">String that separates input numbers</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream is NULL.
	///		MFS_ERROR_UNSUPPORTED_FUNCTION if the stream doesn't support this function.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfError mfsParseF32(mfsStream* stream, mfmF32* value, mfmU64 base, const mfsUTF8CodeUnit* sep);

	/// <summary>
	///		Parses an 64 bit floating point from a stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <param name="value">Out value</param>
	/// <param name="base">Base (2-16)</param>
	/// <param name="sep">String that separates input numbers</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFS_ERROR_INVALID_ARGUMENTS if stream is NULL.
	///		MFS_ERROR_UNSUPPORTED_FUNCTION if the stream doesn't support this function.
	///		Other error codes are returned by specific stream types.
	/// </returns>
	mfError mfsParseF64(mfsStream* stream, mfmF64* value, mfmU64 base, const mfsUTF8CodeUnit* sep);

#ifdef __cplusplus
}
#endif
