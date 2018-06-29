#pragma once

#include "UTF32.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace String
		{
			typedef char U8Char;

			/// <summary>
			///		Gets the size in bytes of a UTF-8 character
			/// </summary>
			/// <param name="src">UTF-8 char pointer</param>
			/// <returns>The UTF-8 character size in bytes</returns>
			size_t GetU8CharSize(const U8Char* src);

			/// <summary>
			///		Gets the unicode point of a UTF-8 character
			/// </summary>
			/// <param name="src">UTF-8 char pointer</param>
			/// <returns>UTF-8 character unicode point value</returns>
			U32Char GetU8Char(const U8Char* src);

			/// <summary>
			///		Copies a UTF-8 string into another UTF-8 string
			/// </summary>
			/// <param name="src">Source string</param>
			/// <param name="dst">Destination string</param>
			/// <param name="size">Maximum number of bytes to copy</param>
			/// <returns>The number of bytes that was copied</returns>
			size_t CopyU8(const U8Char* src, U8Char* dst, size_t size);
		}
	}
}