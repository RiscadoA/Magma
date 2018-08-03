#include "Endianness.hpp"

#include <cstdint>

void SwapBytes4(void* value)
{
	*((uint32_t*)value) =
		(((*((uint32_t*)value) & 0x000000FF) << 24) |
		((*((uint32_t*)value) & 0x0000FF00) << 8) |
		 ((*((uint32_t*)value) & 0x00FF0000) >> 8) |
		 ((*((uint32_t*)value) & 0xFF000000) >> 24));
}

void SwapBytes2(void* value)
{
	*((uint16_t*)value) =
		(((*((uint16_t*)value) & 0x00FF) << 8) |
		((*((uint16_t*)value) & 0xFF00) >> 8));
}

bool Magma::Framework::Memory::IsLittleEndian()
{
	union { uint32_t w = 1; uint8_t b[4]; } check;
	return (check.b[0] == 1);
}

void Magma::Framework::Memory::ToLittleEndian4(void * data)
{
	if (!IsLittleEndian())
		SwapBytes4(data);
}

void Magma::Framework::Memory::FromLittleEndian4(void * data)
{
	if (!IsLittleEndian())
		SwapBytes4(data);
}

void Magma::Framework::Memory::ToBigEndian4(void * data)
{
	if (IsLittleEndian())
		SwapBytes4(data);
}

void Magma::Framework::Memory::FromBigEndian4(void * data)
{
	if (IsLittleEndian())
		SwapBytes4(data);
}

void Magma::Framework::Memory::ToLittleEndian2(void * data)
{
	if (!IsLittleEndian())
		SwapBytes2(data);
}

void Magma::Framework::Memory::FromLittleEndian2(void * data)
{
	if (!IsLittleEndian())
		SwapBytes2(data);
}

void Magma::Framework::Memory::ToBigEndian2(void * data)
{
	if (IsLittleEndian())
		SwapBytes2(data);
}

void Magma::Framework::Memory::FromBigEndian2(void * data)
{
	if (IsLittleEndian())
		SwapBytes2(data);
}
