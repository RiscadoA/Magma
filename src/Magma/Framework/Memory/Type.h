#pragma once

/*
	Variable data typedefs.
*/

#ifdef _MSC_VER
#define _Static_assert static_assert
#endif

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include <limits.h>
#include <float.h>
#include <math.h>

#ifdef __cplusplus
extern "C"
{
#endif

	_Static_assert(CHAR_BIT == 8, u8"A byte must be 8 bits long");

	/// <summary>
	///		Guaranteed to be a 8 bit unsigned integer.
	/// </summary>
	typedef uint8_t mfmU8;
	_Static_assert(sizeof(mfmU8) == 1, u8"mfmU8 must be 1 byte long");

	/// <summary>
	///		Guaranteed to be a 16 bit unsigned integer.
	/// </summary>
	typedef uint16_t mfmU16;
	_Static_assert(sizeof(mfmU16) == 2, u8"mfmU16 must be 2 bytes long");

	/// <summary>
	///		Guaranteed to be a 32 bit unsigned integer.
	/// </summary>
	typedef uint32_t mfmU32;
	_Static_assert(sizeof(mfmU32) == 4, u8"mfmU32 must be 4 bytes long");

	/// <summary>
	///		Guaranteed to be a 64 bit unsigned integer.
	/// </summary>
	typedef uint64_t mfmU64;
	_Static_assert(sizeof(mfmU64) == 8, u8"mfmU64 must be 8 bytes long");

	/// <summary>
	///		Guaranteed to be a 8 bit signed integer.
	/// </summary>
	typedef int8_t mfmI8;
	_Static_assert(sizeof(mfmI8) == 1, u8"mfmI8 must be 1 byte long");

	/// <summary>
	///		Guaranteed to be a 16 bit signed integer.
	/// </summary>
	typedef int16_t mfmI16;
	_Static_assert(sizeof(mfmI16) == 2, u8"mfmI16 must be 2 bytes long");

	/// <summary>
	///		Guaranteed to be a 32 bit signed integer.
	/// </summary>
	typedef int32_t mfmI32;
	_Static_assert(sizeof(mfmI32) == 4, u8"mfmI32 must be 4 bytes long");

	/// <summary>
	///		Guaranteed to be a 64 bit signed integer.
	/// </summary>
	typedef int64_t mfmI64;
	_Static_assert(sizeof(mfmI64) == 8, u8"mfmI64 must be 8 bytes long");

	/// <summary>
	///		Guaranteed to be a 32 bit floating point number.
	/// </summary>
	typedef float mfmF32;
	_Static_assert(sizeof(mfmF32) == 4, u8"mfmF32 must be 4 bytes long");

	/// <summary>
	///		Guaranteed to be a 64 bit floating point number.
	/// </summary>
	typedef double mfmF64;
	_Static_assert(sizeof(mfmF64) == 8, u8"mfmF64 must be 8 bytes long");

	/// <summary>
	///		Used to store a boolean value (can be either MFM_TRUE or MFM_FALSE).
	/// </summary>
	typedef mfmU8 mfmBool;

	/// <summary>
	///		Used to store an enum value.
	/// </summary>
	typedef mfmU32 mfmEnum;

#define MFM_U8_MAX  UINT8_MAX
#define MFM_U16_MAX UINT16_MAX
#define MFM_U32_MAX UINT32_MAX
#define MFM_U64_MAX UINT64_MAX

#define MFM_I8_MIN 	INT8_MIN
#define MFM_I16_MIN INT16_MIN
#define MFM_I32_MIN INT32_MIN
#define MFM_I64_MIN INT64_MIN

#define MFM_I8_MAX 	INT8_MAX
#define MFM_I16_MAX INT16_MAX
#define MFM_I32_MAX INT32_MAX
#define MFM_I64_MAX INT64_MAX

#define MFM_F32_MIN	FLT_MIN
#define MFM_F32_MAX	FLT_MAX
#define MFM_F64_MIN	DBL_MIN
#define MFM_F64_MAX	DBL_MAX

#define MFM_F32_INFINITY INFINITY
#define MFM_F32_NAN NAN
#define MFM_F64_INFINITY INFINITY
#define MFM_F64_NAN NAN

#define MFM_FALSE 0
#define MFM_TRUE 1

#ifdef __cplusplus
}
#endif
