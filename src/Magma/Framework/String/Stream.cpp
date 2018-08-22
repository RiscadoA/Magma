#include "Stream.hpp"
#include "Exception.hpp"

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