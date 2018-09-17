#include <Magma/Framework/String/UTF8.hpp>
#include <Magma/Framework/String/StringStream.hpp>
#include <Magma/Framework/ErrorString.h>

using namespace Magma::Framework::String;

bool Magma::Framework::String::IsValidU8Char(const UTF8CodeUnit * src)
{
	return mfsIsValidUTF8Char(src) != MFM_FALSE;
}

mfmU64 Magma::Framework::String::GetU8CharSize(const UTF8CodeUnit * src)
{
	mfmU64 size;
	auto err = mfsGetUTF8CharSize(src, &size);
	if (err == MF_ERROR_OKAY)
		return size;

	mfsUTF8CodeUnit msg[512];
	auto ss = String::StringStream(msg, sizeof(msg));
	ss.Get() << "Failed to get UTF-8 character size:\n";
	ss.Get() << "mfsGetUTF8CharSize returned '" << mfErrorToString(err) << "'";
	throw StringError(msg);
}

mfmU64 Magma::Framework::String::GetU8CharSize(UnicodePoint up)
{
	mfmU64 size;
	auto err = mfsGetSizeAsUTF8(up, &size);
	if (err == MF_ERROR_OKAY)
		return size;

	mfsUTF8CodeUnit msg[512];
	auto ss = String::StringStream(msg, sizeof(msg));
	ss.Get() << "Failed to get unicode point value size as UTF-8:\n";
	ss.Get() << "mfsGetSizeAsUTF8 returned '" << mfErrorToString(err) << "'";
	throw StringError(msg);
}

UnicodePoint Magma::Framework::String::GetU8Char(const UTF8CodeUnit * src)
{
	mfsUnicodePoint up;
	auto err = mfsGetUTF8Char(src, &up);
	if (err == MF_ERROR_OKAY)
		return up;

	mfsUTF8CodeUnit msg[512];
	auto ss = String::StringStream(msg, sizeof(msg));
	ss.Get() << "Failed to get unicode point value from UTF-8 character:\n";
	ss.Get() << "mfsGetUTF8Char returned '" << mfErrorToString(err) << "'";
	throw StringError(msg);
}

mfmU64 Magma::Framework::String::SetU8Char(UTF8CodeUnit * dst, UnicodePoint up, mfmU64 maxSize)
{
	mfmU64 size;
	auto err = mfsSetUTF8Char(up, dst, &size, maxSize);
	if (err == MF_ERROR_OKAY)
		return size;

	mfsUTF8CodeUnit msg[512];
	auto ss = String::StringStream(msg, sizeof(msg));
	ss.Get() << "Failed to set UTF-8 character to unicode point value:\n";
	ss.Get() << "mfsSetUTF8Char returned '" << mfErrorToString(err) << "'";
	throw StringError(msg);
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
