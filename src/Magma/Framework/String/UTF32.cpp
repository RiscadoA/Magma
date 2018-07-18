#include "UTF32.hpp"

Magma::Framework::String::U32Char SwapBytes(Magma::Framework::String::U32Char chr)
{
	return
		(((chr & 0x000000FF) << 24) |
		((chr & 0x0000FF00) << 8) |
		((chr & 0x00FF0000) >> 8) |
		((chr & 0xFF000000) >> 24));
}

Magma::Framework::String::U32Char Magma::Framework::String::U32ToLE(U32Char up)
{
	if (IsLittleEndian())
		return up;
	else return SwapBytes(up);
}

Magma::Framework::String::U32Char Magma::Framework::String::U32ToBE(U32Char up)
{
	if (!IsLittleEndian())
		return up;
	else return SwapBytes(up);
}

Magma::Framework::String::U32Char Magma::Framework::String::U32FromLE(U32Char le)
{
	if (IsLittleEndian())
		return le;
	else return SwapBytes(le);
}

Magma::Framework::String::U32Char Magma::Framework::String::U32FromBE(U32Char be)
{
	if (!IsLittleEndian())
		return be;
	else return SwapBytes(be);
}

size_t Magma::Framework::String::U32ToLE(const U32Char * src, U32Char * dst, size_t maxSize)
{
	if (maxSize == 0)
		return 0;

	if (IsLittleEndian())
	{
		size_t s = 0;
		while (src[s] != 0)
		{
			if (s == maxSize - 1)
				break;
			dst[s] = src[s];
			++s;
		}
		dst[s] = 0x0000'0000;
		return s + 1;
	}
	else
	{
		size_t s = 0;
		while (src[s] != 0)
		{
			if (s == maxSize - 1)
				break;
			dst[s] = SwapBytes(src[s]);
			++s;
		}
		dst[s] = 0x0000'0000;
		return s + 1;
	}
}

size_t Magma::Framework::String::U32ToBE(const U32Char * src, U32Char * dst, size_t maxSize)
{
	if (maxSize == 0)
		return 0;

	if (!IsLittleEndian())
	{
		size_t s = 0;
		while (src[s] != 0)
		{
			if (s == maxSize - 1)
				break;
			dst[s] = src[s];
			++s;
		}
		dst[s] = 0x0000'0000;
		return s + 1;
	}
	else
	{
		size_t s = 0;
		while (src[s] != 0)
		{
			if (s == maxSize - 1)
				break;
			dst[s] = SwapBytes(src[s]);
			++s;
		}
		dst[s] = 0x0000'0000;
		return s + 1;
	}
}

size_t Magma::Framework::String::U32FromLE(const U32Char * src, U32Char * dst, size_t maxSize)
{
	if (maxSize == 0)
		return 0;

	if (IsLittleEndian())
	{
		size_t s = 0;
		while (src[s] != 0)
		{
			if (s == maxSize - 1)
				break;
			dst[s] = src[s];
			++s;
		}
		dst[s] = 0x0000'0000;
		return s + 1;
	}
	else
	{
		size_t s = 0;
		while (src[s] != 0)
		{
			if (s == maxSize - 1)
				break;
			dst[s] = SwapBytes(src[s]);
			++s;
		}
		dst[s] = 0x0000'0000;
		return s + 1;
	}
}

size_t Magma::Framework::String::U32FromBE(const U32Char * src, U32Char * dst, size_t maxSize)
{
	if (maxSize == 0)
		return 0;

	if (!IsLittleEndian())
	{
		size_t s = 0;
		while (src[s] != 0)
		{
			if (s == maxSize - 1)
				break;
			dst[s] = src[s];
			++s;
		}
		dst[s] = 0x0000'0000;
		return s + 1;
	}
	else
	{
		size_t s = 0;
		while (src[s] != 0)
		{
			if (s == maxSize - 1)
				break;
			dst[s] = SwapBytes(src[s]);
			++s;
		}
		dst[s] = 0x0000'0000;
		return s + 1;
	}
}

bool Magma::Framework::String::IsValidU32Char(U32Char up)
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
	for (size_t i = 0; i < size - 1; ++i, ++it, ++dst)
	{
		*dst = *it;
		if (*it == U'\0')
			return i + 1;  
	}
	*dst = U'\0';
	return size;
}
