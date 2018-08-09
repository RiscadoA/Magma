#include "Endianness.h"

#include <stdlib.h>

mfmBool mfmIsLittleEndian()
{
	union { mfmU32 w; mfmU8 b[4]; } check;
	check.w = 1;
	return (check.b[0] == 1) ? MFM_TRUE : MFM_FALSE;
}

mfmBool mfmIsBigEndian()
{
	union { mfmU32 w; mfmU8 b[4]; } check;
	check.w = 1;
	return (check.b[0] == 1) ? MFM_FALSE : MFM_TRUE;
}

void mfmSwapBytes8(void* src, void* dst)
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

void mfmSwapBytes4(void* src, void* dst)
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

void mfmSwapBytes2(void* src, void* dst)
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

void mfmToLittleEndian8(void * src, void * dst)
{
	if (mfmIsLittleEndian())
		*((mfmU64*)dst) = *((mfmU64*)src);
	else
		mfmSwapBytes8(src, dst);
}

void mfmToLittleEndian4(void * src, void * dst)
{
	if (mfmIsLittleEndian())
		*((mfmU32*)dst) = *((mfmU32*)src);
	else
		mfmSwapBytes4(src, dst);
}

void mfmToLittleEndian2(void * src, void * dst)
{
	if (mfmIsLittleEndian())
		*((mfmU16*)dst) = *((mfmU16*)src);
	else
		mfmSwapBytes2(src, dst);
}

void mfmFromLittleEndian8(void * src, void * dst)
{
	if (mfmIsLittleEndian())
		*((mfmU64*)dst) = *((mfmU64*)src);
	else
		mfmSwapBytes8(src, dst);
}

void mfmFromLittleEndian4(void * src, void * dst)
{
	if (mfmIsLittleEndian())
		*((mfmU32*)dst) = *((mfmU32*)src);
	else
		mfmSwapBytes4(src, dst);
}

void mfmFromLittleEndian2(void * src, void * dst)
{
	if (mfmIsLittleEndian())
		*((mfmU16*)dst) = *((mfmU16*)src);
	else
		mfmSwapBytes2(src, dst);
}

void mfmToBigEndian8(void * src, void * dst)
{
	if (mfmIsBigEndian())
		*((mfmU64*)dst) = *((mfmU64*)src);
	else
		mfmSwapBytes8(src, dst);
}

void mfmToBigEndian4(void * src, void * dst)
{
	if (mfmIsBigEndian())
		*((mfmU32*)dst) = *((mfmU32*)src);
	else
		mfmSwapBytes4(src, dst);
}

void mfmToBigEndian2(void * src, void * dst)
{
	if (mfmIsBigEndian())
		*((mfmU16*)dst) = *((mfmU16*)src);
	else
		mfmSwapBytes2(src, dst);
}

void mfmFromBigEndian8(void * src, void * dst)
{
	if (mfmIsBigEndian())
		*((mfmU64*)dst) = *((mfmU64*)src);
	else
		mfmSwapBytes8(src, dst);
}

void mfmFromBigEndian4(void * src, void * dst)
{
	if (mfmIsBigEndian())
		*((mfmU32*)dst) = *((mfmU32*)src);
	else
		mfmSwapBytes4(src, dst);
}

void mfmFromBigEndian2(void * src, void * dst)
{
	if (mfmIsBigEndian())
		*((mfmU16*)dst) = *((mfmU16*)src);
	else
		mfmSwapBytes2(src, dst);
}
