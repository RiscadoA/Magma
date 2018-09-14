#include "Endianness.h"

#include <stdlib.h>

mfmBool mfmIsLittleEndian(void)
{
	union { mfmU32 w; mfmU8 b[4]; } check;
	check.w = 1;
	return (check.b[0] == 1) ? MFM_TRUE : MFM_FALSE;
}

mfmBool mfmIsBigEndian(void)
{
	union { mfmU32 w; mfmU8 b[4]; } check;
	check.w = 1;
	return (check.b[0] == 1) ? MFM_FALSE : MFM_TRUE;
}

static void mfmSwapBytes8(const void* src, void* dst)
{
#if defined(_MSC_VER)
	*((mfmU64*)dst) = _byteswap_uint64(*((mfmU64*)src));
#elif defined(__GNUC__)
	*((mfmU64*)dst) = __builtin_bswap64(*((mfmU64*)src));
#else
	*((mfmU64*)dst) =
		((*((mfmU64*)src) & 0x00000000000000FFULL) << 56) |
		((*((mfmU64*)src) & 0x000000000000FF00ULL) << 40) |
		((*((mfmU64*)src) & 0x0000000000FF0000ULL) << 24) |
		((*((mfmU64*)src) & 0x00000000FF000000ULL) << 8) |
		((*((mfmU64*)src) & 0x000000FF00000000ULL) >> 8) |
		((*((mfmU64*)src) & 0x0000FF0000000000ULL) >> 24) |
		((*((mfmU64*)src) & 0x00FF000000000000ULL) >> 40) |
		((*((mfmU64*)src) & 0xFF00000000000000ULL) >> 56);
#endif
}

static void mfmSwapBytes4(const void* src, void* dst)
{
#if defined(_MSC_VER)
	*((mfmU32*)dst) = _byteswap_ulong(*((mfmU32*)src));
#elif defined(__GNUC__)
	*((mfmU32*)dst) = __builtin_bswap32(*((mfmU32*)src));
#else
	*((mfmU32*)dst) = (
		((*((mfmU32*)src) & 0x000000FF) << 24) |
		((*((mfmU32*)src) & 0x0000FF00) <<  8) |
		((*((mfmU32*)src) & 0x00FF0000) >>  8) |
		((*((mfmU32*)src) & 0xFF000000) >> 24));
#endif
}

static void mfmSwapBytes2(const void* src, void* dst)
{
#if defined(_MSC_VER)
	*((mfmU16*)dst) = _byteswap_ushort(*((mfmU16*)src));
#elif defined(__GNUC__)
	*((mfmU16*)dst) = __builtin_bswap16(*((mfmU16*)src));
#else
	*((mfmU16*)dst) = (
		((*((mfmU16*)src) & 0x00FF) << 8) |
		((*((mfmU16*)src) & 0xFF00) >> 8));
#endif
}

void mfmToLittleEndian8(const void * src, void * dst)
{
	if (mfmIsLittleEndian())
		*((mfmU64*)dst) = *((mfmU64*)src);
	else
		mfmSwapBytes8(src, dst);
}

void mfmToLittleEndian4(const void * src, void * dst)
{
	if (mfmIsLittleEndian())
		*((mfmU32*)dst) = *((mfmU32*)src);
	else
		mfmSwapBytes4(src, dst);
}

void mfmToLittleEndian2(const void * src, void * dst)
{
	if (mfmIsLittleEndian())
		*((mfmU16*)dst) = *((mfmU16*)src);
	else
		mfmSwapBytes2(src, dst);
}

void mfmFromLittleEndian8(const void * src, void * dst)
{
	if (mfmIsLittleEndian())
		*((mfmU64*)dst) = *((mfmU64*)src);
	else
		mfmSwapBytes8(src, dst);
}

void mfmFromLittleEndian4(const void * src, void * dst)
{
	if (mfmIsLittleEndian())
		*((mfmU32*)dst) = *((mfmU32*)src);
	else
		mfmSwapBytes4(src, dst);
}

void mfmFromLittleEndian2(const void * src, void * dst)
{
	if (mfmIsLittleEndian())
		*((mfmU16*)dst) = *((mfmU16*)src);
	else
		mfmSwapBytes2(src, dst);
}

void mfmToBigEndian8(const void * src, void * dst)
{
	if (mfmIsBigEndian())
		*((mfmU64*)dst) = *((mfmU64*)src);
	else
		mfmSwapBytes8(src, dst);
}

void mfmToBigEndian4(const void * src, void * dst)
{
	if (mfmIsBigEndian())
		*((mfmU32*)dst) = *((mfmU32*)src);
	else
		mfmSwapBytes4(src, dst);
}

void mfmToBigEndian2(const void * src, void * dst)
{
	if (mfmIsBigEndian())
		*((mfmU16*)dst) = *((mfmU16*)src);
	else
		mfmSwapBytes2(src, dst);
}

void mfmFromBigEndian8(const void * src, void * dst)
{
	if (mfmIsBigEndian())
		*((mfmU64*)dst) = *((mfmU64*)src);
	else
		mfmSwapBytes8(src, dst);
}

void mfmFromBigEndian4(const void * src, void * dst)
{
	if (mfmIsBigEndian())
		*((mfmU32*)dst) = *((mfmU32*)src);
	else
		mfmSwapBytes4(src, dst);
}

void mfmFromBigEndian2(const void * src, void * dst)
{
	if (mfmIsBigEndian())
		*((mfmU16*)dst) = *((mfmU16*)src);
	else
		mfmSwapBytes2(src, dst);
}
