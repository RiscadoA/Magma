// #pragma once

#include "../Memory/Type.h"
#include "UTF8.h"
#include "Error.h"

/*
Stream and function declarations.
*/

#ifdef __cplusplus
extern "C"
{
#endif

	/// <summary>
	///		Prints a unsigned 8 bit integer to a buffer.
	/// </summary>
	/// <param name="buffer">Buffer to write to</param>
	/// <param name="bufferSize">Buffer size</param>
	/// <param name="value">Value to print</param>
	/// <param name="base">Print base (2-16)</param>
	/// <param name="outSize">Out write size</param>
	mfError mfsPrintToBufferU8(mfsUTF8CodeUnit* buffer, mfmU64 bufferSize, mfmU8 value, mfmU64 base, mfmU64* outSize);

	/// <summary>
	///		Prints a signed 8 bit integer to a buffer.
	/// </summary>
	/// <param name="buffer">Buffer to write to</param>
	/// <param name="bufferSize">Buffer size</param>
	/// <param name="value">Value to print</param>
	/// <param name="base">Print base (2-16)</param>
	/// <param name="outSize">Out write size</param>
	mfError mfsPrintToBufferI8(mfsUTF8CodeUnit* buffer, mfmU64 bufferSize, mfmI8 value, mfmU64 base, mfmU64* outSize);

	/// <summary>
	///		Prints a unsigned 16 bit integer to a buffer.
	/// </summary>
	/// <param name="buffer">Buffer to write to</param>
	/// <param name="bufferSize">Buffer size</param>
	/// <param name="value">Value to print</param>
	/// <param name="base">Print base (2-16)</param>
	/// <param name="outSize">Out write size</param>
	mfError mfsPrintToBufferU16(mfsUTF8CodeUnit* buffer, mfmU64 bufferSize, mfmU16 value, mfmU64 base, mfmU64* outSize);

	/// <summary>
	///		Prints a signed 16 bit integer to a buffer.
	/// </summary>
	/// <param name="buffer">Buffer to write to</param>
	/// <param name="bufferSize">Buffer size</param>
	/// <param name="value">Value to print</param>
	/// <param name="base">Print base (2-16)</param>
	/// <param name="outSize">Out write size</param>
	mfError mfsPrintToBufferI16(mfsUTF8CodeUnit* buffer, mfmU64 bufferSize, mfmI16 value, mfmU64 base, mfmU64* outSize);

	/// <summary>
	///		Prints a unsigned 32 bit integer to a buffer.
	/// </summary>
	/// <param name="buffer">Buffer to write to</param>
	/// <param name="bufferSize">Buffer size</param>
	/// <param name="value">Value to print</param>
	/// <param name="base">Print base (2-16)</param>
	/// <param name="outSize">Out write size</param>
	mfError mfsPrintToBufferU32(mfsUTF8CodeUnit* buffer, mfmU64 bufferSize, mfmU32 value, mfmU64 base, mfmU64* outSize);

	/// <summary>
	///		Prints a signed 32 bit integer to a buffer.
	/// </summary>
	/// <param name="buffer">Buffer to write to</param>
	/// <param name="bufferSize">Buffer size</param>
	/// <param name="value">Value to print</param>
	/// <param name="base">Print base (2-16)</param>
	/// <param name="outSize">Out write size</param>
	mfError mfsPrintToBufferI32(mfsUTF8CodeUnit* buffer, mfmU64 bufferSize, mfmI32 value, mfmU64 base, mfmU64* outSize);

	/// <summary>
	///		Prints a unsigned 64 bit integer to a buffer.
	/// </summary>
	/// <param name="buffer">Buffer to write to</param>
	/// <param name="bufferSize">Buffer size</param>
	/// <param name="value">Value to print</param>
	/// <param name="base">Print base (2-16)</param>
	/// <param name="outSize">Out write size</param>
	mfError mfsPrintToBufferU64(mfsUTF8CodeUnit* buffer, mfmU64 bufferSize, mfmU64 value, mfmU64 base, mfmU64* outSize);

	/// <summary>
	///		Prints a signed 64 bit integer to a buffer.
	/// </summary>
	/// <param name="buffer">Buffer to write to</param>
	/// <param name="bufferSize">Buffer size</param>
	/// <param name="value">Value to print</param>
	/// <param name="base">Print base (2-16)</param>
	/// <param name="outSize">Out write size</param>
	mfError mfsPrintToBufferI64(mfsUTF8CodeUnit* buffer, mfmU64 bufferSize, mfmI64 value, mfmU64 base, mfmU64* outSize);

	/// <summary>
	///		Prints a 32 bit floating point to a buffer.
	/// </summary>
	/// <param name="buffer">Buffer to write to</param>
	/// <param name="bufferSize">Buffer size</param>
	/// <param name="value">Value to print</param>
	/// <param name="base">Print base (2-16)</param>
	/// <param name="decimalPlaces">Number of decimal places to show</param>
	/// <param name="outSize">Out write size</param>
	mfError mfsPrintToBufferF32(mfsUTF8CodeUnit* buffer, mfmU64 bufferSize, mfmF32 value, mfmU64 base, mfmU64 decimalPlaces, mfmU64* outSize);

	/// <summary>
	///		Prints a 64 bit floating point to a buffer.
	/// </summary>
	/// <param name="buffer">Buffer to write to</param>
	/// <param name="bufferSize">Buffer size</param>
	/// <param name="value">Value to print</param>
	/// <param name="base">Print base (2-16)</param>
	/// <param name="decimalPlaces">Number of decimal places to show</param>
	/// <param name="outSize">Out write size</param>
	mfError mfsPrintToBufferF64(mfsUTF8CodeUnit* buffer, mfmU64 bufferSize, mfmF64 value, mfmU64 base, mfmU64 decimalPlaces, mfmU64* outSize);

	/// <summary>
	///		Reads and parses a unsigned 8 bit integer from a buffer.
	/// </summary>
	/// <param name="buffer">Buffer to read from</param>
	/// <param name="bufferSize">Buffer size</param>
	/// <param name="value">Out value</param>
	/// <param name="base">Base (2-16)</param>
	/// <param name="outSize">Out read size</param>
	mfError mfsParseFromBufferU8(const mfsUTF8CodeUnit* buffer, mfmU64 bufferSize, mfmU8* value, mfmU64 base, mfmU64* outSize);

	/// <summary>
	///		Reads and parses a signed 8 bit integer from a buffer.
	/// </summary>
	/// <param name="buffer">Buffer to read from</param>
	/// <param name="bufferSize">Buffer size</param>
	/// <param name="value">Out value</param>
	/// <param name="base">Base (2-16)</param>
	/// <param name="outSize">Out read size</param>
	mfError mfsParseFromBufferI8(const mfsUTF8CodeUnit* buffer, mfmU64 bufferSize, mfmI8* value, mfmU64 base, mfmU64* outSize);

	/// <summary>
	///		Reads and parses a unsigned 16 bit integer from a buffer.
	/// </summary>
	/// <param name="buffer">Buffer to read from</param>
	/// <param name="bufferSize">Buffer size</param>
	/// <param name="value">Out value</param>
	/// <param name="base">Base (2-16)</param>
	/// <param name="outSize">Out read size</param>
	mfError mfsParseFromBufferU16(const mfsUTF8CodeUnit* buffer, mfmU64 bufferSize, mfmU16* value, mfmU64 base, mfmU64* outSize);

	/// <summary>
	///		Reads and parses a signed 16 bit integer from a buffer.
	/// </summary>
	/// <param name="buffer">Buffer to read from</param>
	/// <param name="bufferSize">Buffer size</param>
	/// <param name="value">Out value</param>
	/// <param name="base">Base (2-16)</param>
	/// <param name="outSize">Out read size</param>
	mfError mfsParseFromBufferI16(const mfsUTF8CodeUnit* buffer, mfmU64 bufferSize, mfmI16* value, mfmU64 base, mfmU64* outSize);

	/// <summary>
	///		Reads and parses a unsigned 32 bit integer from a buffer.
	/// </summary>
	/// <param name="buffer">Buffer to read from</param>
	/// <param name="bufferSize">Buffer size</param>
	/// <param name="value">Out value</param>
	/// <param name="base">Base (2-16)</param>
	/// <param name="outSize">Out read size</param>
	mfError mfsParseFromBufferU32(const mfsUTF8CodeUnit* buffer, mfmU64 bufferSize, mfmU32* value, mfmU64 base, mfmU64* outSize);

	/// <summary>
	///		Reads and parses a signed 32 bit integer from a buffer.
	/// </summary>
	/// <param name="buffer">Buffer to read from</param>
	/// <param name="bufferSize">Buffer size</param>
	/// <param name="value">Out value</param>
	/// <param name="base">Base (2-16)</param>
	/// <param name="outSize">Out read size</param>
	mfError mfsParseFromBufferI32(const mfsUTF8CodeUnit* buffer, mfmU64 bufferSize, mfmI32* value, mfmU64 base, mfmU64* outSize);

	/// <summary>
	///		Reads and parses a unsigned 64 bit integer from a buffer.
	/// </summary>
	/// <param name="buffer">Buffer to read from</param>
	/// <param name="bufferSize">Buffer size</param>
	/// <param name="value">Out value</param>
	/// <param name="base">Base (2-16)</param>
	/// <param name="outSize">Out read size</param>
	mfError mfsParseFromBufferU64(const mfsUTF8CodeUnit* buffer, mfmU64 bufferSize, mfmU64* value, mfmU64 base, mfmU64* outSize);

	/// <summary>
	///		Reads and parses a signed 64 bit integer from a buffer.
	/// </summary>
	/// <param name="buffer">Buffer to read from</param>
	/// <param name="bufferSize">Buffer size</param>
	/// <param name="value">Out value</param>
	/// <param name="base">Base (2-16)</param>
	/// <param name="outSize">Out read size</param>
	mfError mfsParseFromBufferI64(const mfsUTF8CodeUnit* buffer, mfmU64 bufferSize, mfmI64* value, mfmU64 base, mfmU64* outSize);

	/// <summary>
	///		Reads and parses a signed 32 bit floating point from a buffer.
	/// </summary>
	/// <param name="buffer">Buffer to read from</param>
	/// <param name="bufferSize">Buffer size</param>
	/// <param name="value">Out value</param>
	/// <param name="base">Base (2-16)</param>
	/// <param name="outSize">Out read size</param>
	mfError mfsParseFromBufferF32(const mfsUTF8CodeUnit * buffer, mfmU64 bufferSize, mfmF32 * value, mfmU64 base, mfmU64 * outSize);

	/// <summary>
	///		Reads and parses a signed 64 bit floating point from a buffer.
	/// </summary>
	/// <param name="buffer">Buffer to read from</param>
	/// <param name="bufferSize">Buffer size</param>
	/// <param name="value">Out value</param>
	/// <param name="base">Base (2-16)</param>
	/// <param name="outSize">Out read size</param>
	mfError mfsParseFromBufferF64(const mfsUTF8CodeUnit * buffer, mfmU64 bufferSize, mfmF64 * value, mfmU64 base, mfmU64 * outSize);

#ifdef __cplusplus
}
#endif
