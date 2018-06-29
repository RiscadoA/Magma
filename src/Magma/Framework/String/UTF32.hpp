#pragma once

#include <cstdint>

#include "Exception.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace String
		{
			typedef char32_t U32Char;

			/// <summary>
			///		Copies a string into another string
			/// </summary>
			/// <param name="src">Source string</param>
			/// <param name="dst">Destination string</param>
			/// <param name="size">Maximum number of bytes to copy</param>
			/// <returns>The number of bytes that was copied</returns>
			size_t CopyU32(const U32Char* src, U32Char* dst, size_t size);
		}
	}
}