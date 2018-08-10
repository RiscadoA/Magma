#pragma once

#include "UTF32.h"

#include <cstdint>

#include "Exception.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace String
		{
			typedef mfsUTF32CodeUnit UTF32CodeUnit;
			typedef mfsUnicodePoint UnicodePoint;

			/// <summary>
			///		Checks if a unicode point value is valid unicode.
			/// </summary>
			/// <param name="up">Unicode point value</param>
			/// <returns>True if valid, otherwise false</returns>
			bool IsValidUnicode(UnicodePoint up);

			/// <summary>
			///		Copies a UTF-32 string into another string.
			/// </summary>
			/// <param name="src">Source string</param>
			/// <param name="dst">Destination string</param>
			/// <param name="size">Maximum number of bytes to copy</param>
			/// <returns>The number of bytes that was copied</returns>
			mfmU64 CopyUTF32(const UTF32CodeUnit* src, UTF32CodeUnit* dst, mfmU64 size);
		}
	}
}

inline const Magma::Framework::String::UTF32CodeUnit* operator "" _ms(const char32_t* str, size_t) { return reinterpret_cast<const  Magma::Framework::String::UTF32CodeUnit*>(str); }
