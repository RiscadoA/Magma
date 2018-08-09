#include "UTF8.hpp"

#include <sstream>

using namespace Magma::Framework::String;

bool Magma::Framework::String::IsValidU8Char(const U8CodePoint * src)
{
	return mfsIsValidUTF8Char(src) != MFM_FALSE;
}

mfmU64 Magma::Framework::String::GetU8CharSize(const U8CodePoint * src)
{
	mfmU64 size;
	auto err = mfsGetUTF8CharSize(src, &size);
	switch (err)
	{
		case MFS_ERROR_OKAY:
			break;
		case MFS_ERROR_INVALID_ARGUMENTS:
		{
			std::stringstream ss;
			ss << "Failed to get UTF-8 character size:" << std::endl;
			ss << "Invalid arguments:" << std::endl;
			ss << "mfsGetUTF8CharSize returned MFS_ERROR_INVALID_ARGUMENTS";
			throw StringError(ss.str());
		}
		case MFS_ERROR_INVALID_UTF8:
		{
			std::stringstream ss;
			ss << "Failed to get UTF-8 character size:" << std::endl;
			ss << "Invalid UTF-8:" << std::endl;
			ss << "mfsGetUTF8CharSize returned MFS_ERROR_INVALID_UTF8";
			throw StringError(ss.str());
		}
		default:
		{
			std::stringstream ss;
			ss << "Failed to get UTF-8 character size:" << std::endl;
			ss << "mfsGetUTF8CharSize returned '" << err << "'";
			throw StringError(ss.str());
		}
	}

	return size;
}

mfmU64 Magma::Framework::String::GetU8CharSize(U32Char up)
{
	if (up <= 0x007F)
		return 1;
	else if (up <= 0x07FF)
		return 2;
	else if (up <= 0xFFFF)
		return 3;
	else if (up <= 0x10FFFF)
		return 4;
	throw StringError("Failed to get UTF-8 character size:\nInvalid unicode point value");
}

U32Char Magma::Framework::String::GetU8Char(const U8CodePoint * src)
{
	auto s = GetU8CharSize(src);
	U32Char up = 0;
	switch (s)
	{
		case 1:
			up |= (U32Char(*(src + 0) & 0b0111'1111) << 0);
			break;
		case 2:
			up |= (U32Char(*(src + 0) & 0b0001'1111) << 6);
			up |= (U32Char(*(src + 1) & 0b0011'1111) << 0);
			break;
		case 3:
			up |= (U32Char(*(src + 0) & 0b0000'1111) << 12);
			up |= (U32Char(*(src + 1) & 0b0011'1111) << 6);
			up |= (U32Char(*(src + 2) & 0b0011'1111) << 0);
			break;
		case 4:
			up |= (U32Char(*(src + 0) & 0b0000'0111) << 18);
			up |= (U32Char(*(src + 1) & 0b0011'1111) << 12);
			up |= (U32Char(*(src + 2) & 0b0011'1111) << 6);
			up |= (U32Char(*(src + 3) & 0b0011'1111) << 0);
			break;
	}
	return up;
}

mfmU64 Magma::Framework::String::SetU8Char(U8CodePoint * dst, U32Char up, mfmU64 maxSize)
{
	size_t sz = GetU8CharSize(up);
	if (sz > maxSize)
		return 0;
	switch (sz)
	{
		case 1:
			dst[0] = (0b0000'0000 | ((up >> 0) & 0b0111'1111));
			break;
		case 2:
			dst[0] = (0b1100'0000 | ((up >> 6) & 0b0001'1111));
			dst[1] = (0b1000'0000 | ((up >> 0) & 0b0011'1111));
			break;
		case 3:
			dst[0] = (0b1110'0000 | ((up >> 12) & 0b0000'1111));
			dst[1] = (0b1000'0000 | ((up >> 6) & 0b0011'1111));
			dst[2] = (0b1000'0000 | ((up >> 0) & 0b0011'1111));
			break;
		case 4:
			dst[0] = (0b1111'0000 | ((up >> 18) & 0b0000'0111));
			dst[1] = (0b1000'0000 | ((up >> 12) & 0b0011'1111));
			dst[2] = (0b1000'0000 | ((up >> 6) & 0b0011'1111));
			dst[3] = (0b1000'0000 | ((up >> 0) & 0b0011'1111));
			break;
	}
	return sz;
}

U8CodePoint * Magma::Framework::String::NextU8Char(U8CodePoint * chr)
{
	return chr + GetU8CharSize(chr);
}

const U8CodePoint * Magma::Framework::String::NextU8Char(const U8CodePoint * chr)
{
	return chr + GetU8CharSize(chr);
}

mfmU64 Magma::Framework::String::CopyU8(const U8CodePoint * src, U8CodePoint * dst, mfmU64 size)
{
	if (size == 0)
		return 0;
	size--;

	size_t s = 0;
	while (*src != 0)
	{
		size_t chrSize = GetU8CharSize(src);
		if (s + chrSize > size)
			break;
		for (size_t i = 0; i < chrSize; ++i)
			dst[i] = src[i];
		src += chrSize;
		dst += chrSize;
		s += chrSize;
		if (s == size)
			break;
	}
	*dst = 0;
	return s + 1;
}

mfmU64 Magma::Framework::String::GetU8StringSize(const U8CodePoint * str)
{
	size_t size = 1;
	while (*str != 0)
	{
		size_t chrSize = GetU8CharSize(str);
		str += chrSize;
		size += chrSize;
	}
	return size;
}

mfmU64 Magma::Framework::String::GetU8StringLength(const U8CodePoint * str)
{
	size_t length = 0;
	while (*str != 0)
	{
		str += GetU8CharSize(str);
		length++;
	}
	return length;
}
