#pragma once

/*
	Variable data typedefs.
*/

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

	/// <summary>
	///		Guaranteed to be a 8 bit unsigned integer.
	/// </summary>
	typedef uint8_t mfmU8;

	/// <summary>
	///		Guaranteed to be a 16 bit unsigned integer.
	/// </summary>
	typedef uint16_t mfmU16;

	/// <summary>
	///		Guaranteed to be a 32 bit unsigned integer.
	/// </summary>
	typedef uint32_t mfmU32;

	/// <summary>
	///		Guaranteed to be a 64 bit unsigned integer.
	/// </summary>
	typedef uint64_t mfmU64;

	/// <summary>
	///		Guaranteed to be a 8 bit signed integer.
	/// </summary>
	typedef int8_t mfmI8;

	/// <summary>
	///		Guaranteed to be a 16 bit signed integer.
	/// </summary>
	typedef int16_t mfmI16;

	/// <summary>
	///		Guaranteed to be a 32 bit signed integer.
	/// </summary>
	typedef int32_t mfmI32;

	/// <summary>
	///		Guaranteed to be a 64 bit signed integer.
	/// </summary>
	typedef int64_t mfmI64;

	/// <summary>
	///		Guaranteed to be a 32 bit floating point number.
	/// </summary>
	typedef float mfmF32;
	
	/// <summary>
	///		Guaranteed to be a 64 bit floating point number.
	/// </summary>
	typedef double mfmF64;

	/// <summary>
	///		Guaranteed to be an unsigned integer with the same size as a pointer.
	/// </summary>
	typedef uintptr_t mfmUPtr;

	/// <summary>
	///		Guaranteed to be a signed integer with the same size as a pointer.
	/// </summary>
	typedef intptr_t mfmIPtr;

	/// <summary>
	///		Used to store a boolean value (can be either MFM_TRUE or MFM_FALSE).
	/// </summary>
	typedef mfmU8 mfmBool;

	typedef mfmU32 mfmEnum;

#define MFM_FALSE 0
#define MFM_TRUE 1

#ifdef __cplusplus
}
#endif
