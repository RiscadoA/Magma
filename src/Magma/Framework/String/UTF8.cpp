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

mfmU64 Magma::Framework::String::GetU8CharSize(UnicodePoint up)
{
	mfmU64 size;
	auto err = mfsGetSizeAsUTF8(up, &size);
	switch (err)
	{
		case MFS_ERROR_OKAY:
			break;
		case MFS_ERROR_INVALID_UNICODE:
		{
			std::stringstream ss;
			ss << "Failed to get unicode point value size as UTF-8:" << std::endl;
			ss << "Invalid unicode point value:" << std::endl;
			ss << "mfsGetSizeAsUTF8 returned MFS_ERROR_INVALID_UNICODE";
			throw StringError(ss.str());
		}
		default:
		{
			std::stringstream ss;
			ss << "Failed to get unicode point value size as UTF-8:" << std::endl;
			ss << "mfsGetSizeAsUTF8 returned '" << err << "'";
			throw StringError(ss.str());
		}
	}

	return size;
}

UnicodePoint Magma::Framework::String::GetU8Char(const U8CodePoint * src)
{
	mfmU64 size;
	auto err = mfsGetSizeAsUTF8(up, &size);
	switch (err)
	{
		case MFS_ERROR_OKAY:
			break;
		case MFS_ERROR_INVALID_UNICODE:
		{
			std::stringstream ss;
			ss << "Failed to get unicode point value size as UTF-8:" << std::endl;
			ss << "Invalid unicode point value:" << std::endl;
			ss << "mfsGetSizeAsUTF8 returned MFS_ERROR_INVALID_UNICODE";
			throw StringError(ss.str());
		}
		default:
		{
			std::stringstream ss;
			ss << "Failed to get unicode point value size as UTF-8:" << std::endl;
			ss << "mfsGetSizeAsUTF8 returned '" << err << "'";
			throw StringError(ss.str());
		}
	}

	return size;
}

mfmU64 Magma::Framework::String::SetU8Char(U8CodePoint * dst, UnicodePoint up, mfmU64 maxSize)
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
