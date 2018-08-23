#include "Stream.hpp"
#include "Exception.hpp"
#include "Config.h"

Magma::Framework::String::Stream Magma::Framework::String::OutStream = NULL;
Magma::Framework::String::Stream Magma::Framework::String::ErrStream = NULL;
Magma::Framework::String::Stream Magma::Framework::String::InStream = NULL;

void Magma::Framework::String::InitStreams()
{
	OutStream.Set(*reinterpret_cast<mfmObject*>(mfsOutStream));
	ErrStream.Set(*reinterpret_cast<mfmObject*>(mfsErrStream));
	InStream.Set(*reinterpret_cast<mfmObject*>(mfsInStream));
}

void Magma::Framework::String::TerminateStreams()
{
	InStream.Release();
	ErrStream.Release();
	OutStream.Release();
}

mfmU64 Magma::Framework::String::Stream::Write(const void * data, mfmU64 size)
{
	mfmU64 writtenSize = 0;
	mfsError err = mfsWrite(reinterpret_cast<mfsStream*>(&this->Get()), static_cast<const mfmU8*>(data), size, &writtenSize);
	if (err == MFS_ERROR_OKAY || err == MFS_ERROR_FAILED_TO_WRITE_ALL)
		return writtenSize;
	
	switch (err)
	{
		case MFS_ERROR_INVALID_ARGUMENTS:
			throw StreamError("Failed to write to stream, invalid arguments");
			break;
		case MFS_ERROR_INVALID_UTF8:
			throw StreamError("Failed to write to stream, invalid UTF-8");
			break;
		case MFS_ERROR_INVALID_UTF16:
			throw StreamError("Failed to write to stream, invalid UTF-16");
			break;
		case MFS_ERROR_INVALID_UNICODE:
			throw StreamError("Failed to write to stream, invalid unicode");
			break;
		case MFS_ERROR_CHARACTER_TOO_BIG:
			throw StreamError("Failed to write to stream, character too big");
			break;
		case MFS_ERROR_EOF:
			throw StreamError("Failed to write to stream, EOF reached");
			break;
		case MFS_ERROR_INTERNAL:
			throw StreamError("Failed to write to stream, internal stream error");
			break;
		case MFS_ERROR_FAILED_TO_READ_ALL:
			throw StreamError("Failed to write to stream, failed to read all error");
			break;
		default:
			throw StreamError("Failed to write to stream, unknown error");
			break;
	}
}

mfmU64 Magma::Framework::String::Stream::ReadUntil(mfsUTF8CodeUnit * data, mfmU64 maxSize, const mfsUTF8CodeUnit * terminator)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (data == nullptr || terminator == nullptr)
		throw StreamError("Failed to read until terminator from stream, NULL arguments");
#endif

	if (maxSize == 0)
		return 0;
	else if (maxSize == 1)
	{
		data[0] = '\0';
		return 1;
	}

	mfsError err = MFS_ERROR_OKAY;

	mfmU64 size = 0;
	mfmU64 terminatorIndex = 0;
	while (size < maxSize - 1)
	{
		if (terminator[terminatorIndex] == '\0')
			break;

		mfsUTF8CodeUnit chr;
		err = mfsGetByte(reinterpret_cast<mfsStream*>(&this->Get()), reinterpret_cast<mfmU8*>(&chr));
		if (err == MFS_ERROR_OKAY)
		{		
			if (chr == terminator[terminatorIndex])
				++terminatorIndex;
			else
			{
				for (mfmU64 i = 0; i < terminatorIndex; ++i)
				{
					data[size++] = terminator[i];
					if (size >= maxSize - 1)
						break;
				}

				terminatorIndex = 0;

				if (size >= maxSize - 1)
					break;
				else
					data[size++] = chr;
			}
		}
		else if (err == MFS_ERROR_EOF)
			break;
		else
		{
			switch (err)
			{
				case MFS_ERROR_INVALID_ARGUMENTS:
					throw StreamError("Failed to read until terminator from stream, invalid arguments");
					break;
				case MFS_ERROR_INVALID_UTF8:
					throw StreamError("Failed to read until terminator from stream, invalid UTF-8");
					break;
				case MFS_ERROR_INVALID_UTF16:
					throw StreamError("Failed to read until terminator from stream, invalid UTF-16");
					break;
				case MFS_ERROR_INVALID_UNICODE:
					throw StreamError("Failed to read until terminator from stream, invalid unicode");
					break;
				case MFS_ERROR_CHARACTER_TOO_BIG:
					throw StreamError("Failed to read until terminator from stream, character too big");
					break;
				case MFS_ERROR_INTERNAL:
					throw StreamError("Failed to read until terminator from stream, internal stream error");
					break;
				case MFS_ERROR_FAILED_TO_WRITE_ALL:
					throw StreamError("Failed to read until terminator from stream, failed to write all error");
					break;
				case MFS_ERROR_FAILED_TO_READ_ALL:
					throw StreamError("Failed to read until terminator from stream, failed to read all error");
					break;
				default:
					throw StreamError("Failed to read until terminator from stream, unknown error");
					break;
			}
		}
	}

	data[size] = '\0';

	return size;
}

bool Magma::Framework::String::Stream::GetByte(mfmU8 & byte)
{
	return false;
}

void Magma::Framework::String::Stream::Flush()
{
	mfsError err = mfsFlush(reinterpret_cast<mfsStream*>(&this->Get()));
	if (err == MFS_ERROR_OKAY)
		return;
	
	switch (err)
	{
		case MFS_ERROR_INVALID_ARGUMENTS:
			throw StreamError("Failed to flush stream, invalid arguments");
			break;
		case MFS_ERROR_INVALID_UTF8:
			throw StreamError("Failed to flush stream, invalid UTF-8");
			break;
		case MFS_ERROR_INVALID_UTF16:
			throw StreamError("Failed to flush stream, invalid UTF-16");
			break;
		case MFS_ERROR_INVALID_UNICODE:
			throw StreamError("Failed to flush stream, invalid unicode");
			break;
		case MFS_ERROR_CHARACTER_TOO_BIG:
			throw StreamError("Failed to flush stream, character too big");
			break;
		case MFS_ERROR_EOF:
			throw StreamError("Failed to flush stream, EOF reached");
			break;
		case MFS_ERROR_INTERNAL:
			throw StreamError("Failed to flush stream, internal stream error");
			break;
		case MFS_ERROR_FAILED_TO_WRITE_ALL:
			throw StreamError("Failed to flush stream, failed to write all error");
			break;
		case MFS_ERROR_FAILED_TO_READ_ALL:
			throw StreamError("Failed to flush stream, failed to read all error");
			break;
		default:
			throw StreamError("Failed to flush stream, unknown error");
			break;
	}
}

bool Magma::Framework::String::Stream::PutByte(mfmU8 byte)
{
	mfsError err = mfsPutByte(reinterpret_cast<mfsStream*>(&this->Get()), byte);
	if (err == MFS_ERROR_OKAY)
		return true;
	else if (err == MFS_ERROR_FAILED_TO_WRITE_ALL)
		return false;

	switch (err)
	{
		case MFS_ERROR_INVALID_ARGUMENTS:
			throw StreamError("Failed to put byte in stream, invalid arguments");
			break;
		case MFS_ERROR_INVALID_UTF8:
			throw StreamError("Failed to put byte in stream, invalid UTF-8");
			break;
		case MFS_ERROR_INVALID_UTF16:
			throw StreamError("Failed to put byte in stream, invalid UTF-16");
			break;
		case MFS_ERROR_INVALID_UNICODE:
			throw StreamError("Failed to put byte in stream, invalid unicode");
			break;
		case MFS_ERROR_CHARACTER_TOO_BIG:
			throw StreamError("Failed to put byte in stream, character too big");
			break;
		case MFS_ERROR_EOF:
			throw StreamError("Failed to put byte in stream, EOF reached");
			break;
		case MFS_ERROR_INTERNAL:
			throw StreamError("Failed to put byte in stream, internal stream error");
			break;
		case MFS_ERROR_FAILED_TO_READ_ALL:
			throw StreamError("Failed to put byte in stream, failed to read all error");
			break;
		default:
			throw StreamError("Failed to put byte in stream, unknown error");
			break;
	}
}

void Magma::Framework::String::Stream::PutString(const mfsUTF8CodeUnit * str)
{
	mfsError err = mfsPutString(reinterpret_cast<mfsStream*>(&this->Get()), str);
	if (err == MFS_ERROR_OKAY)
		return;

	switch (err)
	{
		case MFS_ERROR_INVALID_ARGUMENTS:
			throw StreamError("Failed to put string in stream, invalid arguments");
			break;
		case MFS_ERROR_INVALID_UTF8:
			throw StreamError("Failed to put string in stream, invalid UTF-8");
			break;
		case MFS_ERROR_INVALID_UTF16:
			throw StreamError("Failed to put string in stream, invalid UTF-16");
			break;
		case MFS_ERROR_INVALID_UNICODE:
			throw StreamError("Failed to put string in stream, invalid unicode");
			break;
		case MFS_ERROR_CHARACTER_TOO_BIG:
			throw StreamError("Failed to put string in stream, character too big");
			break;
		case MFS_ERROR_EOF:
			throw StreamError("Failed to put string in stream, EOF reached");
			break;
		case MFS_ERROR_INTERNAL:
			throw StreamError("Failed to put string in stream, internal stream error");
			break;
		case MFS_ERROR_FAILED_TO_WRITE_ALL:
			throw StreamError("Failed to put string in stream, failed to write all error");
			break;
		case MFS_ERROR_FAILED_TO_READ_ALL:
			throw StreamError("Failed to put string in stream, failed to read all error");
			break;
		default:
			throw StreamError("Failed to put string in stream, unknown error");
			break;
	}
}

mfmU64 Magma::Framework::String::Stream::Read(void * data, mfmU64 size)
{
	mfmU64 readSize = 0;
	mfsError err = mfsRead(reinterpret_cast<mfsStream*>(&this->Get()), static_cast<mfmU8*>(data), size, &readSize);
	if (err == MFS_ERROR_OKAY || err == MFS_ERROR_FAILED_TO_READ_ALL)
		return readSize;

	switch (err)
	{
		case MFS_ERROR_INVALID_ARGUMENTS:
			throw StreamError("Failed to read from stream, invalid arguments");
			break;
		case MFS_ERROR_INVALID_UTF8:
			throw StreamError("Failed to read from stream, invalid UTF-8");
			break;
		case MFS_ERROR_INVALID_UTF16:
			throw StreamError("Failed to read from stream, invalid UTF-16");
			break;
		case MFS_ERROR_INVALID_UNICODE:
			throw StreamError("Failed to read from stream, invalid unicode");
			break;
		case MFS_ERROR_CHARACTER_TOO_BIG:
			throw StreamError("Failed to read from stream, character too big");
			break;
		case MFS_ERROR_EOF:
			throw StreamError("Failed to read from stream, EOF reached");
			break;
		case MFS_ERROR_INTERNAL:
			throw StreamError("Failed to read from stream, internal stream error");
			break;
		case MFS_ERROR_FAILED_TO_WRITE_ALL:
			throw StreamError("Failed to read from stream, failed to write all error");
			break;
		default:
			throw StreamError("Failed to read from stream, unknown error");
			break;
	}
}

Magma::Framework::String::Stream Magma::Framework::String::OpenFile(const mfsUTF8CodeUnit * path, FileMode mode)
{
	mfsStream* stream;
	mfsError err = mfsOpenFile(&stream, static_cast<mfmU32>(mode), path);
	if (err != MFS_ERROR_OKAY)
	{
		// Handle error
		switch (err)
		{
			case MFS_ERROR_INVALID_ARGUMENTS:
				throw StreamError("Failed to open file stream, invalid arguments");
				break;
			case MFS_ERROR_INVALID_UTF8:
				throw StreamError("Failed to open file stream, invalid UTF-8");
				break;
			case MFS_ERROR_INVALID_UTF16:
				throw StreamError("Failed to open file stream, invalid UTF-16");
				break;
			case MFS_ERROR_INVALID_UNICODE:
				throw StreamError("Failed to open file stream, invalid unicode");
				break;
			case MFS_ERROR_CHARACTER_TOO_BIG:
				throw StreamError("Failed to open file stream, character too big");
				break;
			case MFS_ERROR_EOF:
				throw StreamError("Failed to open file stream, EOF reached");
				break;
			case MFS_ERROR_INTERNAL:
				throw StreamError("Failed to open file stream, internal error");
				break;
			case MFS_ERROR_FAILED_TO_WRITE_ALL:
				throw StreamError("Failed to open file stream, failed to write all error");
				break;
			default:
				throw StreamError("Failed to open file stream, unknown error");
				break;
		}
	}

	return stream;
}