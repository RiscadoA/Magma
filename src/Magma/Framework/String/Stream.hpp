#pragma once

#include "../Memory/Handle.hpp"
#include "Stream.h"

namespace Magma
{
	namespace Framework
	{
		namespace String
		{
			/// <summary>
			///		Used as a stream handle.
			///		Destroys the stream automatically when there are no more references to it.
			/// </summary>
			class StreamHandle : public Memory::Handle
			{
			public:
				using Handle::Handle;
				using Handle::operator=;
				inline StreamHandle(const Memory::Handle& object) : Memory::Handle(object) {}

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
				///		Reads raw data from a stream.
				/// </summary>
				/// <param name="data">Data pointer</param>
				/// <param name="size">Data size in bytes</param>
				/// <returns>The size of the data read in bytes</returns>
				mfmU64 Read(void* data, mfmU64 size);

				/// <summary>
				///		Reads a string from a stream until a terminator string is found.
				/// </summary>
				/// <param name="data">Output string pointer</param>
				/// <param name="maxSize">Max string size in bytes (including null terminator)</param>
				/// <param name="terminator">Terminator string</param>
				/// <returns>The size of the string read in bytes (including null terminator)</returns>
				mfmU64 ReadUntil(mfsUTF8CodeUnit* data, mfmU64 maxSize, const mfsUTF8CodeUnit* terminator);

				/// <summary>
				///		Gets a single byte from a stream.
				/// </summary>
				/// <param name="byte">Output byte</param>
				/// <returns>True if the byte was read, otherwise false (EOF reached)</returns>
				bool GetByte(mfmU8& byte);

				/// <summary>
				///		Flushes the stream.
				/// </summary>
				void Flush();
			};

			extern StreamHandle OutStream;
			extern StreamHandle ErrStream;
			extern StreamHandle InStream;

			void InitStreams();
			void TerminateStreams();
		}
	}
}
