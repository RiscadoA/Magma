#include "Stream.hpp"
#include "Exception.hpp"
#include "Config.h"

Magma::Framework::String::HStream Magma::Framework::String::OutStream = NULL;
Magma::Framework::String::HStream Magma::Framework::String::ErrStream = NULL;
Magma::Framework::String::HStream Magma::Framework::String::InStream = NULL;

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

void Magma::Framework::String::HStream::SeekBegin(mfmU64 position)
{
	mfError err = mfsSeekBegin(reinterpret_cast<mfsStream*>(&this->Get()), position);
	if (err == MF_ERROR_OKAY)
		return;
	throw StreamError(ErrorToString(err));
}

void Magma::Framework::String::HStream::SeekEnd(mfmU64 position)
{
	mfError err = mfsSeekEnd(reinterpret_cast<mfsStream*>(&this->Get()), position);
	if (err == MF_ERROR_OKAY)
		return;
	throw StreamError(ErrorToString(err));
}

void Magma::Framework::String::HStream::SeekHead(mfmI64 offset)
{
	mfError err = mfsSeekHead(reinterpret_cast<mfsStream*>(&this->Get()), offset);
	if (err == MF_ERROR_OKAY)
		return;
	throw StreamError(ErrorToString(err));
}

mfmU64 Magma::Framework::String::HStream::Tell()
{
	mfmU64 pos = MFM_FALSE;
	mfError err = mfsTell(reinterpret_cast<mfsStream*>(&this->Get()), &pos);
	if (err != MF_ERROR_OKAY)
		throw StreamError(ErrorToString(err));
	return pos;
}

bool Magma::Framework::String::HStream::IsEOF()
{
	mfmBool eof = MFM_FALSE;
	mfError err = mfsEOF(reinterpret_cast<mfsStream*>(&this->Get()), &eof);
	if (err != MF_ERROR_OKAY)	
		throw StreamError(ErrorToString(err));
	return eof != MFM_FALSE;
}

mfmU64 Magma::Framework::String::HStream::Write(const void * data, mfmU64 size)
{
	mfmU64 writtenSize = 0;
	mfError err = mfsWrite(reinterpret_cast<mfsStream*>(&this->Get()), static_cast<const mfmU8*>(data), size, &writtenSize);
	if (err == MF_ERROR_OKAY || err == MFS_ERROR_FAILED_TO_WRITE_ALL)
		return writtenSize;
	throw StreamError(ErrorToString(err));
}

mfmU64 Magma::Framework::String::HStream::ReadUntil(mfsUTF8CodeUnit * data, mfmU64 maxSize, const mfsUTF8CodeUnit * terminator)
{
	mfmU64 readSize = 0;
	mfError err = mfsReadUntil(reinterpret_cast<mfsStream*>(&this->Get()), (mfmU8*)data, maxSize, &readSize, terminator);
	if (err == MF_ERROR_OKAY || err == MFS_ERROR_FAILED_TO_READ_ALL)
		return readSize;
	throw StreamError(ErrorToString(err));
}

bool Magma::Framework::String::HStream::GetByte(mfmU8 & byte)
{
	mfError err = mfsGetByte(reinterpret_cast<mfsStream*>(&this->Get()), &byte);
	if (err == MF_ERROR_OKAY)
		return true;
	else if (err == MFS_ERROR_FAILED_TO_READ_ALL)
		return false;
	throw StreamError(ErrorToString(err));
}

void Magma::Framework::String::HStream::Flush()
{
	mfError err = mfsFlush(reinterpret_cast<mfsStream*>(&this->Get()));
	if (err == MF_ERROR_OKAY)
		return;
	throw StreamError(ErrorToString(err));
}

bool Magma::Framework::String::HStream::PutByte(mfmU8 byte)
{
	mfError err = mfsPutByte(reinterpret_cast<mfsStream*>(&this->Get()), byte);
	if (err == MF_ERROR_OKAY)
		return true;
	else if (err == MFS_ERROR_FAILED_TO_WRITE_ALL)
		return false;
	throw StreamError(ErrorToString(err));
}

void Magma::Framework::String::HStream::PutString(const mfsUTF8CodeUnit * str)
{
	mfError err = mfsPutString(reinterpret_cast<mfsStream*>(&this->Get()), str);
	if (err == MF_ERROR_OKAY)
		return;
	throw StreamError(ErrorToString(err));
}

mfmU64 Magma::Framework::String::HStream::Read(void * data, mfmU64 size)
{
	mfmU64 readSize = 0;
	mfError err = mfsRead(reinterpret_cast<mfsStream*>(&this->Get()), static_cast<mfmU8*>(data), size, &readSize);
	if (err == MF_ERROR_OKAY || err == MFS_ERROR_FAILED_TO_READ_ALL)
		return readSize;
	throw StreamError(ErrorToString(err));
}
