#pragma once

#include "../Memory/Object.hpp"
#include "Stream.h"

namespace Magma
{
	namespace Framework
	{
		namespace String
		{
			/// <summary>
			///		Used as an object handle.
			///		Destroys the object automatically when there are no more references to it.
			/// </summary>
			class Stream : public Memory::Object
			{
			public:
				using Object::Object;

				/// <summary>
				///		Writes raw data into a stream.
				/// </summary>
				/// <param name="data">Data pointer</param>
				/// <param name="size">Data size in bytes</param>
				/// <returns>The size of the data written in bytes</returns>
				mfmU64 Write(const void* data, mfmU64 size);

				/// <summary>
				///		Writes a single byte into a stream.
				/// </summary>
				/// <param name="byte">Byte to write</param>
				/// <returns>True if the byte was written, otherwise false</returns>
				bool PutByte(mfmU8 byte);

				/// <summary>
				///		Writes a UTF-8 null-terminated string into a stream.
				/// </summary>
				/// <param name="str">UTF-8 string pointer</param>
				/// <returns>The size of the data written in bytes</returns>
				void PutString(const mfsUTF8CodeUnit* str);

				/// <summary>
				///		Flushes the stream.
				/// </summary>
				void Flush();
			};

			extern Stream OutStream;
			extern Stream ErrStream;
			extern Stream InStream;

			void InitStreams();
			void TerminateStreams();
		}
	}
}
