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
			///		Used to represent the different byte orders
			/// </summary>
			enum class ByteOrder
			{
				Invalid = -1,

				BigEndian,
				LittleEndian,

				Count,
			};

			/// <summary>
			///		Checks if a unicode point value is valid unicode.
			/// </summary>
			/// <param name="up">Unicode point value</param>
			/// <returns>True if valid, otherwise false</returns>
			bool IsValidU32(U32Char up);

			/// <summary>
			///		Copies a string into another string.
			/// </summary>
			/// <param name="src">Source string</param>
			/// <param name="dst">Destination string</param>
			/// <param name="size">Maximum number of bytes to copy</param>
			/// <returns>The number of bytes that was copied</returns>
			size_t CopyU32(const U32Char* src, U32Char* dst, size_t size);
		}
	}
}