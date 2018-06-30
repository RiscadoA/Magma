#include "UTF32.hpp"
#include "UTF8.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace String
		{
			/// <summary>
			///		Converts a UTF-8 string to a UTF-32 string.
			/// </summary>
			/// <param name="src">UTF-8 source string</param>
			/// <param name="dst">UTF-32 destination string</param>
			/// <param name="maxSize">Max UTF-32 string size (in characters, including the null character)</param>
			/// <returns>UTF-32 string size in characters, including the null character</returns>
			size_t ConvertU8ToU32(const U8Char* src, U32Char* dst, size_t maxSize);

			/// <summary>
			///		Converts a UTF-32 string to a UTF-8 string.
			/// </summary>
			/// <param name="src">UTF-32 source string</param>
			/// <param name="dst">UTF-8 destination string</paramref>
			/// <param name="maxSize">Max UTF-8 string size (in bytes, including the null character)</param>
			/// <returns>UTF-8 string size in characters, including the null character</returns>
			size_t ConvertU32ToU8(const U32Char* src, U8Char* dst, size_t maxSize);
		}
	}
}