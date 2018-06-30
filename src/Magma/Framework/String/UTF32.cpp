#include "UTF32.hpp"

bool Magma::Framework::String::IsValidU32(U32Char up)
{
	if (up > 0x10FFFF)
		return false;
	if (up >= 0xD800 && up <= 0xDFFF)
		return false;
	return true;
}

size_t Magma::Framework::String::CopyU32(const U32Char* src, U32Char* dst, size_t size)
{
	auto it = src;
	for (size_t i = 0; i < size - 1; ++i, ++it)
	{
		*dst = *it;
		if (*it == U'\0')
			return i + 1;  
	}
	dst[size - 1]== U'\0';
	return size;
}
