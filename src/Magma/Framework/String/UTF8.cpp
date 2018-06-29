#include "UTF8.hpp"

#include <sstream>

using namespace Magma::Framework::String;

size_t Magma::Framework::String::GetU8CharSize(const U8Char * src)
{
	if ((*src & 0b1000'0000) == 0b0000'0000)
		return 1;
	else if ((*src & 0b1110'0000) == 0b1100'0000)
	{
		if ((*(src + 1) & 0b1100'0000) != 0b1000'0000)
			throw StringError("Failed to get UTF-8 character size:\nInvalid 2 byte character second byte");
		return 2;
	}
	else if ((*src & 0b1111'0000) == 0b1110'0000)
	{
		if ((*(src + 1) & 0b1100'0000) != 0b1000'0000)
			throw StringError("Failed to get UTF-8 character size:\nInvalid 3 byte character second byte");
		if ((*(src + 2) & 0b1100'0000) != 0b1000'0000)
			throw StringError("Failed to get UTF-8 character size:\nInvalid 3 byte character third byte");
		return 3;
	}
	else if ((*src & 0b1111'1000) == 0b1111'0000)
	{
		if ((*(src + 1) & 0b1100'0000) != 0b1000'0000)
			throw StringError("Failed to get UTF-8 character size:\nInvalid 4 byte character second byte");
		if ((*(src + 2) & 0b1100'0000) != 0b1000'0000)
			throw StringError("Failed to get UTF-8 character size:\nInvalid 4 byte character third byte");
		if ((*(src + 3) & 0b1100'0000) != 0b1000'0000)
			throw StringError("Failed to get UTF-8 character size:\nInvalid 4 byte character fourth byte");
		return 4;
	}
	else throw StringError("Failed to get UTF-8 character size:\nInvalid character first byte");
}

U32Char Magma::Framework::String::GetU8Char(const U8Char * src)
{
	auto s = GetU8CharSize(src);
	U32Char up = 0;
	switch (s)
	{
		case 1:

			break;
		case 2:

			break;
		case 3:

			break;
		case 4:

			break;
	}
	return up;
}

size_t Magma::Framework::String::CopyU8(const U8Char * src, U8Char * dst, size_t size)
{
	return size_t();
}
