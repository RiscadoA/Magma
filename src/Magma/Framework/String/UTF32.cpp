#include "UTF32.hpp"

bool Magma::Framework::String::IsValidUnicode(UnicodePoint up)
{
	if (up > 0x10FFFF)
		return false;
	if (up >= 0xD800 && up <= 0xDFFF)
		return false;
	return true;
}

mfmU64 Magma::Framework::String::CopyUTF32(const UTF32CodeUnit* src, UTF32CodeUnit* dst, mfmU64 size)
{
	auto it = src;
	for (size_t i = 0; i < size - 1; ++i, ++it, ++dst)
	{
		*dst = *it;
		if (*it == U'\0')
			return i + 1;  
	}
	*dst = U'\0';
	return size;
}
