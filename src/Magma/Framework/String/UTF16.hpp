#pragma once

#include "UTF32.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace String
		{
			typedef char16_t U16Char;

			/// <summary>
			///		Gets the UTF-16 byte order marker byte order
			/// </summary>
			/// <param name="bom">UTF-16 byte order marker pointer</param>
			/// <returns>The byte order stored in the byte order marker</returns>
			ByteOrder GetU16ByteOrderMarker(U16Char* bom);

			/// <summary>
			///		Checks if a UTF-16 character is valid or not
			/// </summary>
			/// <param name="src">UTF-16 char pointer</param>
			/// <returns>True if valid, otherwise false</returns>
			bool IsValidU16Char(const U16Char* src);

			size_t GetU16CharSize(const U16Char* src);
		}
	}
}