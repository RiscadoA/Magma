#include "UTF8.h"

mfmBool mfsIsValidUTF8Char(mfsUTF8CodeUnit * chr)
{
	if ((*chr & 0b1000'0000) == 0b0000'0000)
	{
		mfsUnicodePoint up;
		mfsGetUTF8Char(chr, &up);
		return mfsIsValidUTF32Char(up);
	}
	else if ((*chr & 0b1110'0000) == 0b1100'0000)
	{
		if ((*(chr + 1) & 0b1100'0000) != 0b1000'0000)
			return MFM_FALSE;
		mfsUnicodePoint up;
		mfsGetUTF8Char(chr, &up);
		return mfsIsValidUTF32Char(up);
	}
	else if ((*chr & 0b1111'0000) == 0b1110'0000)
	{
		if ((*(chr + 1) & 0b1100'0000) != 0b1000'0000)
			return MFM_FALSE;
		if ((*(chr + 2) & 0b1100'0000) != 0b1000'0000)
			return MFM_FALSE;
		mfsUnicodePoint up;
		mfsGetUTF8Char(chr, &up);
		return mfsIsValidUTF32Char(up);
	}
	else if ((*chr & 0b1111'1000) == 0b1111'0000)
	{
		if ((*(chr + 1) & 0b1100'0000) != 0b1000'0000)
			return MFM_FALSE;
		if ((*(chr + 2) & 0b1100'0000) != 0b1000'0000)
			return MFM_FALSE;
		if ((*(chr + 3) & 0b1100'0000) != 0b1000'0000)
			return MFM_FALSE;
		mfsUnicodePoint up;
		mfsGetUTF8Char(chr, &up);
		return mfsIsValidUTF32Char(up);
	}
	else return MFM_FALSE;
}

mfError mfsGetUTF8CharSize(const mfsUTF8CodeUnit * chr, mfmU64 * size)
{
	if (chr == NULL || size == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;

	if ((*chr & 0b1000'0000) == 0b0000'0000)
		*size = 1;
	else if ((*chr & 0b1110'0000) == 0b1100'0000)
		*size = 2;
	else if ((*chr & 0b1111'0000) == 0b1110'0000)
		*size = 3;
	else if ((*chr & 0b1111'1000) == 0b1111'0000)
		*size = 4;
	else
		return MFS_ERROR_INVALID_UTF8;
	return MF_ERROR_OKAY;
}

mfError mfsGetSizeAsUTF8(mfsUnicodePoint up, mfmU64 * size)
{
	if (size == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;

	if (up <= 0x007F)
		*size = 1;
	else if (up <= 0x07FF)
		*size = 2;
	else if (up <= 0xFFFF)
		*size = 3;
	else if (up <= 0x10FFFF)
		*size = 4;
	else
		return MFS_ERROR_INVALID_UNICODE;
	return MF_ERROR_OKAY;
}

mfError mfsGetUTF8Char(const mfsUTF8CodeUnit * chr, mfsUnicodePoint * up)
{
	if (chr == NULL || up == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;

	mfmU64 sz;
	mfError err = mfsGetUTF8CharSize(chr, &sz);
	if (err != MF_ERROR_OKAY)
		return err;

	*up = 0;
	switch (sz)
	{
		case 1:
			*up |= ((mfsUnicodePoint)(*(chr + 0) & 0b0111'1111) << 0);
			break;
		case 2:
			*up |= ((mfsUnicodePoint)(*(chr + 0) & 0b0001'1111) << 6);
			*up |= ((mfsUnicodePoint)(*(chr + 1) & 0b0011'1111) << 0);
			break;
		case 3:
			*up |= ((mfsUnicodePoint)(*(chr + 0) & 0b0000'1111) << 12);
			*up |= ((mfsUnicodePoint)(*(chr + 1) & 0b0011'1111) << 6);
			*up |= ((mfsUnicodePoint)(*(chr + 2) & 0b0011'1111) << 0);
			break;
		case 4:
			*up |= ((mfsUnicodePoint)(*(chr + 0) & 0b0000'0111) << 18);
			*up |= ((mfsUnicodePoint)(*(chr + 1) & 0b0011'1111) << 12);
			*up |= ((mfsUnicodePoint)(*(chr + 2) & 0b0011'1111) << 6);
			*up |= ((mfsUnicodePoint)(*(chr + 3) & 0b0011'1111) << 0);
			break;
		default:
			return MFS_ERROR_INVALID_UTF8;
	}

	return MF_ERROR_OKAY;
}

mfError mfsSetUTF8Char(mfsUnicodePoint up, mfsUTF8CodeUnit * chr, mfmU64 * chrSize, mfmU64 maxChrSize)
{
	if (chr == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;

	mfmU64 sz; 
	mfError err = mfsGetSizeAsUTF8(up, &sz);
	if (err != MF_ERROR_OKAY)
		return err;
	if (sz > maxChrSize)
		return MFS_ERROR_CHARACTER_TOO_BIG;

	switch (sz)
	{
		case 1:
			chr[0] = (0b0000'0000 | ((up >> 0) & 0b0111'1111));
			break;
		case 2:
			chr[0] = (0b1100'0000 | ((up >> 6) & 0b0001'1111));
			chr[1] = (0b1000'0000 | ((up >> 0) & 0b0011'1111));
			break;
		case 3:
			chr[0] = (0b1110'0000 | ((up >> 12) & 0b0000'1111));
			chr[1] = (0b1000'0000 | ((up >> 6) & 0b0011'1111));
			chr[2] = (0b1000'0000 | ((up >> 0) & 0b0011'1111));
			break;
		case 4:
			chr[0] = (0b1111'0000 | ((up >> 18) & 0b0000'0111));
			chr[1] = (0b1000'0000 | ((up >> 12) & 0b0011'1111));
			chr[2] = (0b1000'0000 | ((up >> 6) & 0b0011'1111));
			chr[3] = (0b1000'0000 | ((up >> 0) & 0b0011'1111));
			break;
		default:
			return MFS_ERROR_INVALID_UNICODE;
	}

	if (chrSize != NULL)
		*chrSize = sz;

	return MF_ERROR_OKAY;
}