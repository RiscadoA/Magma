#include "Conversion.hpp"

#include <cctype>

size_t Magma::Framework::String::ConvertU8ToU32(const U8Char * src, U32Char * dst, size_t maxSize)
{
	if (maxSize == 0)
		return 0;

	size_t s = 0;
	auto it = src;
	while (*it != 0)
	{
		size_t chrSize = GetU8CharSize(it);
		if (s + 1 > maxSize - 1)
			break;

		dst[s] = GetU8Char(it);
		it = NextU8Char(it);
		++s;
	}
	dst[s] = 0;
	return s + 1;
}

size_t Magma::Framework::String::ConvertU32ToU8(const U32Char * src, U8Char * dst, size_t maxSize)
{
	if (maxSize == 0)
		return 0;

	size_t s = 0;
	auto it = src;
	while (*it != 0)
	{
		size_t chrSize = GetU8CharSize(*it);
		if (s + chrSize > maxSize - 1)
			break;
		SetU8Char(dst + s, *it);
		++it;
		s += chrSize;
	}
	dst[s] = 0;
	return s + 1;
}

void Magma::Framework::String::ToLower(std::string & str)
{
	for (auto& c : str)
		c = tolower(c);
}
