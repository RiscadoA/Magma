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
			///		Checks if a UTF-8 character is valid or not
			/// </summary>
			/// <param name="src">UTF-8 char pointer</param>
			/// <returns>True if valid, otherwise false</returns>
			bool IsValidU8Char(const U8Char* src);

			/// <summary>
			///		Gets the size in bytes of a UTF-8 character
			/// </summary>
			/// <param name="src">UTF-8 char pointer</param>
			/// <returns>The UTF-8 character size in bytes</returns>
			/// <exception cref="Magma::Framework::String::StringError">Thrown if the character has an unknown header</exception>
			size_t GetU8CharSize(const U8Char* src);

			/// <summary>
			///		Gets the unicode point of a UTF-8 character
			/// </summary>
			/// <param name="src">UTF-8 char pointer</param>
			/// <returns>UTF-8 character unicode point value</returns>
			/// <exception cref="Magma::Framework::String::StringError">Thrown if the character is invalid UTF-8</exception>
			U32Char GetU8Char(const U8Char* src);

			/// <summary>
			///		Gets the next UTF-8 character pointer in a string
			/// </summary>
			/// <param name="chr">UTF-8 character pointer</param>
			/// <returns>Next UTF-8 character pointer</returns>
			U8Char* NextU8Char(U8Char* chr);

			/// <summary>
			///		Gets the next UTF-8 character pointer in a string
			/// </summary>
			/// <param name="chr">UTF-8 character pointer</param>
			/// <returns>Next UTF-8 character pointer</returns>
			const U8Char* NextU8Char(const U8Char* chr);

			/// <summary>
			///		Copies a UTF-8 string into another UTF-8 string
			/// </summary>
			/// <param name="src">Source string</param>
			/// <param name="dst">Destination string</param>
			/// <param name="size">Maximum number of bytes to copy (the 0 string terminator is included)</param>
			/// <returns>The number of bytes that was copied</returns>
			size_t CopyU8(const U8Char* src, U8Char* dst, size_t size);

			/// <summary>
			///		Gets a UTF-8 string size in bytes.
			/// </summary>
			/// <returns>The string size in bytes</returns>
			size_t GetU8StringSize(const U8Char* str);

			/// <summary>
			///		Gets a UTF-8 string length in characters.
			/// </summary>
			/// <returns>The string length in characters</returns>
			size_t GetU8StringLength(const U8Char* str);
		}
	}
}