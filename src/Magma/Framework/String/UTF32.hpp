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
			///		Used to check if the current system uses little endianness or big endianness
			/// </summary>
			/// <returns>True if little endian, otherwise false</returns>
			inline bool IsLittleEndian()
			{
				union { uint32_t w = 1; uint8_t b[4]; } check;
				return (check.b[0] == 1);
			}

			/// <summary>
			///		Converts a UTF-32 character from the native endianness to little endianness
			/// </summary>
			/// <param name="up">UTF-32 character in the native endianness</param>
			/// <returns>The UTF-32 character in little endianness</returns>
			U32Char U32ToLE(U32Char up);

			/// <summary>
			///		Converts a UTF-32 character from the native endianness to big endianness
			/// </summary>
			/// <param name="up">UTF-32 character in the native endianness</param>
			/// <returns>The UTF-32 character in big endianness</returns>
			U32Char U32ToBE(U32Char up);

			/// <summary>
			///		Converts a UTF-32 character from little endianness to the native endianness
			/// </summary>
			/// <param name="up">UTF-32 character in little endianness</param>
			/// <returns>The UTF-32 character in the native endianness</returns>
			U32Char U32FromLE(U32Char le);

			/// <summary>
			///		Converts a UTF-32 character from big endianness to the native endianness
			/// </summary>
			/// <param name="up">UTF-32 character in big endianness</param>
			/// <returns>The UTF-32 character in the native endianness</returns>
			U32Char U32FromBE(U32Char be);

			/// <summary>
			///		Converts a UTF-32 character string from the native endianness to little endianness
			/// </summary>
			/// <param name="src">UTF-32 character string in the native endianness</param>
			/// <param name="dst">The UTF-32 character string in little endianness</param>
			/// <param name="maxSize">Max string size in characters (including the null terminator)</param>
			/// <returns>The number of characters that were converted (including the null terminator)</returns>
			size_t U32ToLE(const U32Char* src, U32Char* dst, size_t maxSize);

			/// <summary>
			///		Converts a UTF-32 character string from the native endianness to big endianness
			/// </summary>
			/// <param name="src">UTF-32 character string in the native endianness</param>
			/// <param name="dst">The UTF-32 character string in big endianness</param>
			/// <param name="maxSize">Max string size in characters (including the null terminator)</param>
			/// <returns>The number of characters that were converted (including the null terminator)</returns>
			size_t U32ToBE(const U32Char* src, U32Char* dst, size_t maxSize);

			/// <summary>
			///		Converts a UTF-32 character string from little endianness to the native endianness
			/// </summary>
			/// <param name="src">UTF-32 character string in little endianness</param>
			/// <param name="dst">The UTF-32 character string in the native endianness</param>
			/// <param name="maxSize">Max string size in characters (including the null terminator)</param>
			/// <returns>The number of characters that were converted (including the null terminator)</returns>
			size_t U32FromLE(const U32Char* src, U32Char* dst, size_t maxSize);

			/// <summary>
			///		Converts a UTF-32 character string from big endianness to the native endianness
			/// </summary>
			/// <param name="src">UTF-32 character string in big endianness</param>
			/// <param name="dst">The UTF-32 character string in the native endianness</param>
			/// <param name="maxSize">Max string size in characters (including the null terminator)</param>
			/// <returns>The number of characters that were converted (including the null terminator)</returns>
			size_t U32FromBE(const U32Char* src, U32Char* dst, size_t maxSize);

			/// <summary>
			///		Checks if a unicode point value is valid unicode.
			/// </summary>
			/// <param name="up">Unicode point value</param>
			/// <returns>True if valid, otherwise false</returns>
			bool IsValidU32Char(U32Char up);

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