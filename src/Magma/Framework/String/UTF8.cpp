#include "UTF8.hpp"

#include <sstream>

using namespace Magma::Framework::String;

bool Magma::Framework::String::IsValidU8Char(const UTF8CodeUnit * src)
{
	return mfsIsValidUTF8Char(src) != MFM_FALSE;
}

mfmU64 Magma::Framework::String::GetU8CharSize(const UTF8CodeUnit * src)
{
	mfmU64 size;
	auto err = mfsGetUTF8CharSize(src, &size);
	switch (err)
	{
		case MF_ERROR_OKAY:
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
		case MF_ERROR_OKAY:
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

UnicodePoint Magma::Framework::String::GetU8Char(const UTF8CodeUnit * src)
{
	mfsUnicodePoint up;
	auto err = mfsGetUTF8Char(src, &up);
	switch (err)
	{
		case MF_ERROR_OKAY:
			break;
		case MFS_ERROR_INVALID_ARGUMENTS:
		{
			std::stringstream ss;
			ss << "Failed to get unicode point value from UTF-8 character:" << std::endl;
			ss << "Invalid arguments:" << std::endl;
			ss << "mfsGetUTF8Char returned MFS_ERROR_INVALID_ARGUMENTS";
			throw StringError(ss.str());
		}
		case MFS_ERROR_INVALID_UTF8:
		{
			std::stringstream ss;
			ss << "Failed to get unicode point value from UTF-8 character:" << std::endl;
			ss << "Invalid unicode point value:" << std::endl;
			ss << "mfsGetUTF8Char returned MFS_ERROR_INVALID_UTF8";
			throw StringError(ss.str());
		}
		default:
		{
			std::stringstream ss;
			ss << "Failed to get unicode point value from UTF-8 character:" << std::endl;
			ss << "mfsGetUTF8Char returned '" << err << "'";
			throw StringError(ss.str());
		}
	}
	return up;
}

mfmU64 Magma::Framework::String::SetU8Char(UTF8CodeUnit * dst, UnicodePoint up, mfmU64 maxSize)
{
	mfmU64 size;
	auto err = mfsSetUTF8Char(up, dst, &size, maxSize);
	switch (err)
	{
		case MF_ERROR_OKAY:
			break;
		case MFS_ERROR_INVALID_ARGUMENTS:
		{
			std::stringstream ss;
			ss << "Failed to set UTF-8 character to unicode point value:" << std::endl;
			ss << "Invalid arguments:" << std::endl;
			ss << "mfsSetUTF8Char returned MFS_ERROR_INVALID_ARGUMENTS";
			throw StringError(ss.str());
		}
		case MFS_ERROR_INVALID_UNICODE:
		{
			std::stringstream ss;
			ss << "Failed to set UTF-8 character to unicode point value:" << std::endl;
			ss << "Invalid unicode point value:" << std::endl;
			ss << "mfsSetUTF8Char returned MFS_ERROR_INVALID_UNICODE";
			throw StringError(ss.str());
		}
		case MFS_ERROR_CHARACTER_TOO_BIG:
		{
			std::stringstream ss;
			ss << "Failed to set UTF-8 character to unicode point value:" << std::endl;
			ss << "The character is bigger than the passed max size:" << std::endl;
			ss << "mfsSetUTF8Char returned MFS_ERROR_CHARACTER_TOO_BIG";
			throw StringError(ss.str());
		}
		default:
		{
			std::stringstream ss;
			ss << "Failed to set UTF-8 character to unicode point value:" << std::endl;
			ss << "mfsSetUTF8Char returned '" << err << "'";
			throw StringError(ss.str());
		}
	}
	return size;
}

UTF8CodeUnit * Magma::Framework::String::NextU8Char(UTF8CodeUnit * chr)
{
	return chr + GetU8CharSize(chr);
}

const UTF8CodeUnit * Magma::Framework::String::NextU8Char(const UTF8CodeUnit * chr)
{
	return chr + GetU8CharSize(chr);
}

mfmU64 Magma::Framework::String::CopyU8(const UTF8CodeUnit * src, UTF8CodeUnit * dst, mfmU64 size)
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

mfmU64 Magma::Framework::String::GetU8StringSize(const UTF8CodeUnit * str)
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

mfmU64 Magma::Framework::String::GetU8StringLength(const UTF8CodeUnit * str)
{
	size_t length = 0;
	while (*str != 0)
	{
		str += GetU8CharSize(str);
		length++;
	}
	return length;
}
