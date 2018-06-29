#include "UTF32.hpp"

size_t Magma::Framework::String::CopyU32(const U32Char* src, U32Char* dst, size_t size)
{
	auto it = src;
	for (size_t i = 0; i < size; ++i, ++it)
	{
		*dst = *it;
		if (*it == U'\0')
			return i + 1;
	}
	return size;
}
