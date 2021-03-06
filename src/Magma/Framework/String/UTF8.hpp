#pragma once

#include "UTF32.hpp"
#include "UTF8.h"

namespace Magma
{
	namespace Framework
	{
		namespace String
		{
			typedef mfsUTF8CodeUnit UTF8CodeUnit;

			/// <summary>
			///		Checks if a UTF-8 character is valid or not
			/// </summary>
			/// <param name="src">UTF-8 char pointer</param>
			/// <returns>True if valid, otherwise false</returns>
			bool IsValidU8Char(const UTF8CodeUnit* src);

			/// <summary>
			///		Gets the size in bytes of a UTF-8 character
			/// </summary>
			/// <param name="src">UTF-8 char pointer</param>
			/// <returns>The UTF-8 character size in bytes</returns>
			/// <exception cref="Magma::Framework::String::StringError">Thrown if the character has an unknown header</exception>
			mfmU64 GetU8CharSize(const UTF8CodeUnit* src);

			/// <summary>
			///		Gets the size in bytes of a unicode point value stored as a UTF-8 character.
			/// </summary>
			/// <param name="up">Unicode point value</param>
			/// <returns>The UTF-8 character size in bytes</returns>
			mfmU64 GetU8CharSize(UnicodePoint up);

			/// <summary>
			///		Gets the unicode point of a UTF-8 character
			/// </summary>
			/// <param name="src">UTF-8 char pointer</param>
			/// <returns>UTF-8 character unicode point value</returns>
			/// <exception cref="Magma::Framework::String::StringError">Thrown if the character is invalid UTF-8</exception>
			UnicodePoint GetU8Char(const UTF8CodeUnit* src);

			/// <summary>
			///		Sets a UTF-8 character value
			/// </summary>
			/// <param name="dst">UTF-8 character destination pointer</param>
			/// <param name="up">Unicode point value</param>
			/// <param name="maxSize">Maximum UTF-8 character size</param>
			/// <returns>The size of the character</returns>
			mfmU64 SetU8Char(UTF8CodeUnit* dst, UnicodePoint up, mfmU64 maxSize = 4);

			/// <summary>
			///		Gets the next UTF-8 character pointer in a string
			/// </summary>
			/// <param name="chr">UTF-8 character pointer</param>
			/// <returns>Next UTF-8 character pointer</returns>
			UTF8CodeUnit* NextU8Char(UTF8CodeUnit* chr);

			/// <summary>
			///		Gets the next UTF-8 character pointer in a string
			/// </summary>
			/// <param name="chr">UTF-8 character pointer</param>
			/// <returns>Next UTF-8 character pointer</returns>
			const UTF8CodeUnit* NextU8Char(const UTF8CodeUnit* chr);

			/// <summary>
			///		Copies a UTF-8 string into another UTF-8 string
			/// </summary>
			/// <param name="src">Source string</param>
			/// <param name="dst">Destination string</param>
			/// <param name="size">Maximum number of bytes to copy (the 0 string terminator is included)</param>
			/// <returns>The number of bytes that was copied</returns>
			mfmU64 CopyU8(const UTF8CodeUnit* src, UTF8CodeUnit* dst, mfmU64 size);

			/// <summary>
			///		Gets a UTF-8 string size in bytes.
			/// </summary>
			/// <returns>The string size in bytes</returns>
			mfmU64 GetU8StringSize(const UTF8CodeUnit* str);

			/// <summary>
			///		Gets a UTF-8 string length in characters.
			/// </summary>
			/// <returns>The string length in characters</returns>
			mfmU64 GetU8StringLength(const UTF8CodeUnit* str);
		}
	}
}
