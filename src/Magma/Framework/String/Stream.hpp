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
			class HStream : public Memory::Handle
			{
			public:
				using Handle::Handle;
				using Handle::operator=;
				inline HStream(const Memory::Handle& object) : Memory::Handle(object) {}

				/// <summary>
				///		Seeks a position on the stream relative to its beginning.
				/// </summary>
				/// <param name="position">New position</param>
				void SeekBegin(mfmU64 position);

				/// <summary>
				///		Seeks a position on the stream relative to its end (new position = stream size - position - 1).
				/// </summary>
				/// <param name="position">New position</param>
				void SeekEnd(mfmU64 position);

				/// <summary>
				///		Seeks a position on the stream relative to the current position.
				/// </summary>
				/// <param name="offset">Offset relative to the current position</param>
				void SeekHead(mfmI64 offset);

				/// <summary>
				///		Gets the current position in the stream.
				/// </summary>
				/// <returns>The current position in the stream</returns>
				mfmU64 Tell();

				/// <summary>
				///		Checks if the stream has reached EOF.
				/// </summary>
				/// <returns>True if EOF, otherwise false</returns>
				bool IsEOF();

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
				/// <param name="data">Output string pointer (optional, can be NULL)</param>
				/// <param name="maxSize">Max string size in bytes (including null terminator)</param>
				/// <param name="terminator">Terminator string (if NULL, reads until a whitespace is found)</param>
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

				/// <summary>
				///		Prints a value.
				/// </summary>
				/// <param name="value">Value</param>
				template <typename T>
				void Print(T value);

				/// <summary>
				///		Prints a value.
				/// </summary>
				/// <param name="value">Value</param>
				template <typename T>
				inline HStream& operator<<(T value)
				{
					this->Print<T>(value);
					return *this;
				}

				/// <summary>
				///		Parses a value from the stream.
				/// </summary>
				/// <param name="value">Out value</param>
				template <typename T>
				void Parse(T& value);

				/// <summary>
				///		Parses a value from the stream.
				/// </summary>
				/// <param name="value">Value</param>
				template <typename T>
				inline HStream& operator>>(T& value)
				{
					this->Parse<T>(value);
					return *this;
				}

				/// <summary>
				///		Prints an unsigned 8 bit integer.
				/// </summary>
				/// <param name="value">Value</param>
				/// <param name="base">Base (2-16)</param>
				void PrintU8(mfmU8 value, mfmU64 base = 10);

				/// <summary>
				///		Prints an unsigned 16 bit integer.
				/// </summary>
				/// <param name="value">Value</param>
				/// <param name="base">Base (2-16)</param>
				void PrintU16(mfmU16 value, mfmU64 base = 10);

				/// <summary>
				///		Prints an unsigned 32 bit integer.
				/// </summary>
				/// <param name="value">Value</param>
				/// <param name="base">Base (2-16)</param>
				void PrintU32(mfmU32 value, mfmU64 base = 10);

				/// <summary>
				///		Prints an unsigned 64 bit integer.
				/// </summary>
				/// <param name="value">Value</param>
				/// <param name="base">Base (2-16)</param>
				void PrintU64(mfmU64 value, mfmU64 base = 10);

				/// <summary>
				///		Prints a signed 8 bit integer.
				/// </summary>
				/// <param name="value">Value</param>
				/// <param name="base">Base (2-16)</param>
				void PrintI8(mfmI8 value, mfmU64 base = 10);

				/// <summary>
				///		Prints a signed 16 bit integer.
				/// </summary>
				/// <param name="value">Value</param>
				/// <param name="base">Base (2-16)</param>
				void PrintI16(mfmI16 value, mfmU64 base = 10);

				/// <summary>
				///		Prints a signed 32 bit integer.
				/// </summary>
				/// <param name="value">Value</param>
				/// <param name="base">Base (2-16)</param>
				void PrintI32(mfmI32 value, mfmU64 base = 10);

				/// <summary>
				///		Prints a signed 64 bit integer.
				/// </summary>
				/// <param name="value">Value</param>
				/// <param name="base">Base (2-16)</param>
				void PrintI64(mfmI64 value, mfmU64 base = 10);

				/// <summary>
				///		Prints a 32 bit floating point.
				/// </summary>
				/// <param name="value">Value</param>
				/// <param name="decimalPlaces">Number of decimal places to show</param>
				/// <param name="base">Base (2-16)</param>
				void PrintF32(mfmF32 value, mfmU64 decimalPlaces = 4, mfmU64 base = 10);

				/// <summary>
				///		Prints a 64 bit floating point.
				/// </summary>
				/// <param name="value">Value</param>
				/// <param name="decimalPlaces">Number of decimal places to show</param>
				/// <param name="base">Base (2-16)</param>
				void PrintF64(mfmF64 value, mfmU64 decimalPlaces = 4, mfmU64 base = 10);

				/// <summary>
				///		Prints a pointer.
				/// </summary>
				/// <param name="value">Value</param>
				/// <param name="base">Base (2-16)</param>
				void PrintPointer(void* value, mfmU64 base = 10);

				/// <summary>
				///		Parses an unsigned 8 bit integer.
				/// </summary>
				/// <param name="terminator">Terminator string</param>
				/// <param name="base">Base (2-16)</param>
				mfmU8 ParseU8(const mfsUTF8CodeUnit* terminator, mfmU64 base = 10);

				/// <summary>
				///		Parses an unsigned 16 bit integer.
				/// </summary>
				/// <param name="terminator">Terminator string</param>
				/// <param name="base">Base (2-16)</param>
				mfmU16 ParseU16(const mfsUTF8CodeUnit* terminator, mfmU64 base = 10);

				/// <summary>
				///		Parses an unsigned 32 bit integer.
				/// </summary>
				/// <param name="terminator">Terminator string</param>
				/// <param name="base">Base (2-16)</param>
				mfmU32 ParseU32(const mfsUTF8CodeUnit* terminator, mfmU64 base = 10);

				/// <summary>
				///		Parses an unsigned 64 bit integer.
				/// </summary>
				/// <param name="terminator">Terminator string</param>
				/// <param name="base">Base (2-16)</param>
				mfmU64 ParseU64(const mfsUTF8CodeUnit* terminator, mfmU64 base = 10);

				/// <summary>
				///		Parses a signed 8 bit integer.
				/// </summary>
				/// <param name="terminator">Terminator string</param>
				/// <param name="base">Base (2-16)</param>
				mfmI8 ParseI8(const mfsUTF8CodeUnit* terminator, mfmU64 base = 10);

				/// <summary>
				///		Parses a signed 16 bit integer.
				/// </summary>
				/// <param name="terminator">Terminator string</param>
				/// <param name="base">Base (2-16)</param>
				mfmI16 ParseI16(const mfsUTF8CodeUnit* terminator, mfmU64 base = 10);

				/// <summary>
				///		Parses a signed 32 bit integer.
				/// </summary>
				/// <param name="terminator">Terminator string</param>
				/// <param name="base">Base (2-16)</param>
				mfmI32 ParseI32(const mfsUTF8CodeUnit* terminator, mfmU64 base = 10);

				/// <summary>
				///		Parses a signed 64 bit integer.
				/// </summary>
				/// <param name="terminator">Terminator string</param>
				/// <param name="base">Base (2-16)</param>
				mfmI64 ParseI64(const mfsUTF8CodeUnit* terminator, mfmU64 base = 10);

				/// <summary>
				///		Parses a 32 bit floating point.
				/// </summary>
				/// <param name="terminator">Terminator string</param>
				/// <param name="base">Base (2-16)</param>
				mfmF32 ParseF32(const mfsUTF8CodeUnit* terminator, mfmU64 base = 10);

				/// <summary>
				///		Parses a 64 bit floating point.
				/// </summary>
				/// <param name="terminator">Terminator string</param>
				/// <param name="base">Base (2-16)</param>
				mfmF64 ParseF64(const mfsUTF8CodeUnit* terminator, mfmU64 base = 10);
			};

			extern HStream OutStream;
			extern HStream ErrStream;
			extern HStream InStream;

			void InitStreams();
			void TerminateStreams();

			template<typename T>
			inline void HStream::Print(T) { static_assert(false); /* UNSUPPORTED TYPE */};

			template<>
			inline void HStream::Print<mfmU8>(mfmU8 value) { this->PrintU8(value); }
			template<>
			inline void HStream::Print<mfmU16>(mfmU16 value) { this->PrintU16(value); }
			template<>
			inline void HStream::Print<mfmU32>(mfmU32 value) { this->PrintU32(value); }
			template<>
			inline void HStream::Print<mfmU64>(mfmU64 value) { this->PrintU64(value); }
			template<>
			inline void HStream::Print<mfmI8>(mfmI8 value) { this->PrintI8(value); }
			template<>
			inline void HStream::Print<mfmI16>(mfmI16 value) { this->PrintI16(value); }
			template<>
			inline void HStream::Print<mfmI32>(mfmI32 value) { this->PrintI32(value); }
			template<>
			inline void HStream::Print<mfmI64>(mfmI64 value) { this->PrintI64(value); }
			template<>
			inline void HStream::Print<mfmF32>(mfmF32 value) { this->PrintF32(value); }
			template<>
			inline void HStream::Print<mfmF64>(mfmF64 value) { this->PrintF64(value); }
			template<>
			inline void HStream::Print<mfsUTF8CodeUnit>(mfsUTF8CodeUnit value) { this->PutByte(value); }
			template<>
			inline void HStream::Print<const mfsUTF8CodeUnit*>(const mfsUTF8CodeUnit* value) { this->PutString(value); }

			template<typename T>
			inline void HStream::Parse(T & value) { static_assert(false); /* UNSUPPORTED TYPE */ };

			template<>
			inline void HStream::Parse<mfmU8>(mfmU8& value) { value = this->ParseU8(NULL); }
			template<>
			inline void HStream::Parse<mfmU16>(mfmU16& value) { value = this->ParseU16(NULL); }
			template<>
			inline void HStream::Parse<mfmU32>(mfmU32& value) { value = this->ParseU32(NULL); }
			template<>
			inline void HStream::Parse<mfmU64>(mfmU64& value) { value = this->ParseU64(NULL); }
			template<>
			inline void HStream::Parse<mfmI8>(mfmI8& value) { value = this->ParseI8(NULL); }
			template<>
			inline void HStream::Parse<mfmI16>(mfmI16& value) { value = this->ParseI16(NULL); }
			template<>
			inline void HStream::Parse<mfmI32>(mfmI32& value) { value = this->ParseI32(NULL); }
			template<>
			inline void HStream::Parse<mfmI64>(mfmI64& value) { value = this->ParseI64(NULL); }
			template<>
			inline void HStream::Parse<mfmF32>(mfmF32& value) { value = this->ParseF32(NULL); }
			template<>
			inline void HStream::Parse<mfmF64>(mfmF64& value) { value = this->ParseF64(NULL); }
		}
	}
}
