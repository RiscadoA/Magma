#pragma once

namespace Magma
{
	namespace Framework
	{
		namespace Memory
		{
			/// <summary>
			///		Used to check if the current system uses little endianness or big endianness
			/// </summary>
			/// <returns>True if little endian, otherwise false</returns>
			bool IsLittleEndian();

			/// <summary>
			///		Converts a 4 byte value from the native endianness to little endianness
			/// </summary>
			/// <param name="data">Pointer to the 4 byte value in the native endianness</param>
			void ToLittleEndian4(void* data);

			/// <summary>
			///		Converts a 4 byte value from little endianness to the native endianness
			/// </summary>
			/// <param name="data">Pointer to the 4 byte value in little endianness</param>
			void FromLittleEndian4(void* data);

			/// <summary>
			///		Converts a 4 byte value from the native endianness to big endianness
			/// </summary>
			/// <param name="data">Pointer to the 4 byte value in the native endianness</param>
			void ToBigEndian4(void* data);

			/// <summary>
			///		Converts a 4 byte value from big endianness to the native endianness
			/// </summary>
			/// <param name="data">Pointer to the 4 byte value in big endianness</param>
			void FromBigEndian4(void* data);

			/// <summary>
			///		Converts a 4 byte value from the native endianness to little endianness
			/// </summary>
			/// <param name="data">Pointer to the 2 byte value in the native endianness</param>
			void ToLittleEndian2(void* data);

			/// <summary>
			///		Converts a 2 byte value from little endianness to the native endianness
			/// </summary>
			/// <param name="data">Pointer to the 2 byte value in little endianness</param>
			void FromLittleEndian2(void* data);

			/// <summary>
			///		Converts a 2 byte value from the native endianness to big endianness
			/// </summary>
			/// <param name="data">Pointer to the 2 byte value in the native endianness</param>
			void ToBigEndian2(void* data);

			/// <summary>
			///		Converts a 2 byte value from big endianness to the native endianness
			/// </summary>
			/// <param name="data">Pointer to the 2 byte value in big endianness</param>
			void FromBigEndian2(void* data);
		}
	}
}