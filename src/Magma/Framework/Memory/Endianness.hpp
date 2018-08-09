#pragma once

#include "Endianness.h"

namespace Magma
{
	namespace Framework
	{
		namespace Memory
		{
			/// <summary>
			///		Converts a 8 byte value from the native endianness to little endianness
			/// </summary>
			/// <param name="src">Pointer to the 8 byte value in the native endianness</param>
			/// <param name="dst">Pointer to the output 8 byte value</param>
			inline void ToLittleEndian8(void* src, void* dst) { mfmToLittleEndian8(src, dst); }

			/// <summary>
			///		Converts a 4 byte value from the native endianness to little endianness
			/// </summary>
			/// <param name="src">Pointer to the 4 byte value in the native endianness</param>
			/// <param name="dst">Pointer to the output 4 byte value</param>
			inline void ToLittleEndian4(void* src, void* dst) { mfmToLittleEndian4(src, dst); }

			/// <summary>
			///		Converts a 2 byte value from the native endianness to little endianness
			/// </summary>
			/// <param name="src">Pointer to the 2 byte value in the native endianness</param>
			/// <param name="dst">Pointer to the output 2 byte value</param>
			inline void ToLittleEndian2(void* src, void* dst) { mfmToLittleEndian2(src, dst); }

			/// <summary>
			///		Converts a 8 byte value from little endianness to the native endianness
			/// </summary>
			/// <param name="src">Pointer to the 8 byte value in little endianness</param>
			/// <param name="dst">Pointer to the output 8 byte value</param>
			inline void FromLittleEndian8(void* src, void* dst) { mfmFromLittleEndian8(src, dst); }

			/// <summary>
			///		Converts a 4 byte value from little endianness to the native endianness
			/// </summary>
			/// <param name="src">Pointer to the 4 byte value in little endianness</param>
			/// <param name="dst">Pointer to the output 4 byte value</param>
			inline void FromLittleEndian4(void* src, void* dst) { mfmFromLittleEndian4(src, dst); }

			/// <summary>
			///		Converts a 2 byte value from little endianness to the native endianness
			/// </summary>
			/// <param name="src">Pointer to the 2 byte value in little endianness</param>
			/// <param name="dst">Pointer to the output 2 byte value</param>
			inline void FromLittleEndian2(void* src, void* dst) { mfmFromLittleEndian2(src, dst); }

			/// <summary>
			///		Converts a 8 byte value from the native endianness to big endianness
			/// </summary>
			/// <param name="src">Pointer to the 8 byte value in the native endianness</param>
			/// <param name="dst">Pointer to the output 8 byte value</param>
			inline void ToBigEndian8(void* src, void* dst) { mfmToBigEndian8(src, dst); }

			/// <summary>
			///		Converts a 4 byte value from the native endianness to big endianness
			/// </summary>
			/// <param name="src">Pointer to the 4 byte value in the native endianness</param>
			/// <param name="dst">Pointer to the output 4 byte value</param>
			inline void ToBigEndian4(void* src, void* dst) { mfmToBigEndian4(src, dst); }

			/// <summary>
			///		Converts a 2 byte value from the native endianness to big endianness
			/// </summary>
			///  <param name="src">Pointer to the 2 byte value in the native endianness</param>
			/// <param name="dst">Pointer to the output 2 byte value</param>
			inline void ToBigEndian2(void* src, void* dst) { mfmToBigEndian2(src, dst); }

			/// <summary>
			///		Converts a 8 byte value from big endianness to the native endianness
			/// </summary>
			/// <param name="src">Pointer to the 8 byte value in big endianness</param>
			/// <param name="dst">Pointer to the output 8 byte value</param>
			inline void FromBigEndian8(void* src, void* dst) { mfmFromBigEndian8(src, dst); }

			/// <summary>
			///		Converts a 4 byte value from big endianness to the native endianness
			/// </summary>
			/// <param name="src">Pointer to the 4 byte value in big endianness</param>
			/// <param name="dst">Pointer to the output 4 byte value</param>
			inline void FromBigEndian4(void* src, void* dst) { mfmFromBigEndian4(src, dst); }

			/// <summary>
			///		Converts a 2 byte value from big endianness to the native endianness
			/// </summary>
			/// <param name="src">Pointer to the 2 byte value in big endianness</param>
			/// <param name="dst">Pointer to the output 2 byte value</param>
			inline void FromBigEndian2(void* src, void* dst) { mfmFromBigEndian2(src, dst); }
		}
	}
}