#include <Magma/Framework/String/Stream.hpp>
#include <Magma/Framework/String/Exception.hpp>
#include <Magma/Framework/ErrorString.h>
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

void Magma::Framework::String::HStream::SetBuffer(void * buffer, mfmU64 size)
{
	mfError err = mfsSetBuffer(reinterpret_cast<mfsStream*>(&this->Get()), (mfmU8*)buffer, size);
	if (err == MF_ERROR_OKAY)
		return;
	throw StreamError(mfErrorToString(err));
}

void Magma::Framework::String::HStream::SeekBegin(mfmU64 position)
{
	mfError err = mfsSeekBegin(reinterpret_cast<mfsStream*>(&this->Get()), position);
	if (err == MF_ERROR_OKAY)
		return;
	throw StreamError(mfErrorToString(err));
}

void Magma::Framework::String::HStream::SeekEnd(mfmU64 position)
{
	mfError err = mfsSeekEnd(reinterpret_cast<mfsStream*>(&this->Get()), position);
	if (err == MF_ERROR_OKAY)
		return;
	throw StreamError(mfErrorToString(err));
}

void Magma::Framework::String::HStream::SeekHead(mfmI64 offset)
{
	mfError err = mfsSeekHead(reinterpret_cast<mfsStream*>(&this->Get()), offset);
	if (err == MF_ERROR_OKAY)
		return;
	throw StreamError(mfErrorToString(err));
}

mfmU64 Magma::Framework::String::HStream::Tell()
{
	mfmU64 pos = MFM_FALSE;
	mfError err = mfsTell(reinterpret_cast<mfsStream*>(&this->Get()), &pos);
	if (err != MF_ERROR_OKAY)
		throw StreamError(mfErrorToString(err));
	return pos;
}

bool Magma::Framework::String::HStream::IsEOF()
{
	mfmBool eof = MFM_FALSE;
	mfError err = mfsEOF(reinterpret_cast<mfsStream*>(&this->Get()), &eof);
	if (err != MF_ERROR_OKAY)	
		throw StreamError(mfErrorToString(err));
	return eof != MFM_FALSE;
}

mfmU64 Magma::Framework::String::HStream::Write(const void * data, mfmU64 size)
{
	mfmU64 writtenSize = 0;
	mfError err = mfsWrite(reinterpret_cast<mfsStream*>(&this->Get()), static_cast<const mfmU8*>(data), size, &writtenSize);
	if (err == MF_ERROR_OKAY || err == MFS_ERROR_FAILED_TO_WRITE_ALL)
		return writtenSize;
	throw StreamError(mfErrorToString(err));
}

mfmU64 Magma::Framework::String::HStream::ReadUntil(mfsUTF8CodeUnit * data, mfmU64 maxSize, const mfsUTF8CodeUnit * terminator)
{
	mfmU64 readSize = 0;
	mfError err = mfsReadUntil(reinterpret_cast<mfsStream*>(&this->Get()), (mfmU8*)data, maxSize, &readSize, terminator);
	if (err == MF_ERROR_OKAY || err == MFS_ERROR_FAILED_TO_READ_ALL)
		return readSize;
	throw StreamError(mfErrorToString(err));
}

bool Magma::Framework::String::HStream::GetByte(mfmU8 & byte)
{
	mfError err = mfsGetByte(reinterpret_cast<mfsStream*>(&this->Get()), &byte);
	if (err == MF_ERROR_OKAY)
		return true;
	else if (err == MFS_ERROR_FAILED_TO_READ_ALL)
		return false;
	throw StreamError(mfErrorToString(err));
}

void Magma::Framework::String::HStream::Flush()
{
	mfError err = mfsFlush(reinterpret_cast<mfsStream*>(&this->Get()));
	if (err == MF_ERROR_OKAY)
		return;
	throw StreamError(mfErrorToString(err));
}

bool Magma::Framework::String::HStream::PutByte(mfmU8 byte)
{
	mfError err = mfsPutByte(reinterpret_cast<mfsStream*>(&this->Get()), byte);
	if (err == MF_ERROR_OKAY)
		return true;
	else if (err == MFS_ERROR_FAILED_TO_WRITE_ALL)
		return false;
	throw StreamError(mfErrorToString(err));
}

void Magma::Framework::String::HStream::PutString(const mfsUTF8CodeUnit * str)
{
	mfError err = mfsPutString(reinterpret_cast<mfsStream*>(&this->Get()), str);
	if (err == MF_ERROR_OKAY)
		return;
	throw StreamError(mfErrorToString(err));
}

mfmU64 Magma::Framework::String::HStream::Read(void * data, mfmU64 size)
{
	mfmU64 readSize = 0;
	mfError err = mfsRead(reinterpret_cast<mfsStream*>(&this->Get()), static_cast<mfmU8*>(data), size, &readSize);
	if (err == MF_ERROR_OKAY || err == MFS_ERROR_FAILED_TO_READ_ALL)
		return readSize;
	throw StreamError(mfErrorToString(err));
}

void Magma::Framework::String::HStream::PrintU8(mfmU8 value, mfmU64 base)
{
	mfError err = mfsPrintU8(reinterpret_cast<mfsStream*>(&this->Get()), value, base);
	if (err == MF_ERROR_OKAY)
		return;
	throw StreamError(mfErrorToString(err));
}

void Magma::Framework::String::HStream::PrintU16(mfmU16 value, mfmU64 base)
{
	mfError err = mfsPrintU16(reinterpret_cast<mfsStream*>(&this->Get()), value, base);
	if (err == MF_ERROR_OKAY)
		return;
	throw StreamError(mfErrorToString(err));
}

void Magma::Framework::String::HStream::PrintU32(mfmU32 value, mfmU64 base)
{
	mfError err = mfsPrintU32(reinterpret_cast<mfsStream*>(&this->Get()), value, base);
	if (err == MF_ERROR_OKAY)
		return;
	throw StreamError(mfErrorToString(err));
}

void Magma::Framework::String::HStream::PrintU64(mfmU64 value, mfmU64 base)
{
	mfError err = mfsPrintU64(reinterpret_cast<mfsStream*>(&this->Get()), value, base);
	if (err == MF_ERROR_OKAY)
		return;
	throw StreamError(mfErrorToString(err));
}

void Magma::Framework::String::HStream::PrintI8(mfmI8 value, mfmU64 base)
{
	mfError err = mfsPrintI8(reinterpret_cast<mfsStream*>(&this->Get()), value, base);
	if (err == MF_ERROR_OKAY)
		return;
	throw StreamError(mfErrorToString(err));
}

void Magma::Framework::String::HStream::PrintI16(mfmI16 value, mfmU64 base)
{
	mfError err = mfsPrintI16(reinterpret_cast<mfsStream*>(&this->Get()), value, base);
	if (err == MF_ERROR_OKAY)
		return;
	throw StreamError(mfErrorToString(err));
}

void Magma::Framework::String::HStream::PrintI32(mfmI32 value, mfmU64 base)
{
	mfError err = mfsPrintI32(reinterpret_cast<mfsStream*>(&this->Get()), value, base);
	if (err == MF_ERROR_OKAY)
		return;
	throw StreamError(mfErrorToString(err));
}

void Magma::Framework::String::HStream::PrintI64(mfmI64 value, mfmU64 base)
{
	mfError err = mfsPrintI64(reinterpret_cast<mfsStream*>(&this->Get()), value, base);
	if (err == MF_ERROR_OKAY)
		return;
	throw StreamError(mfErrorToString(err));
}

void Magma::Framework::String::HStream::PrintF32(mfmF32 value, mfmU64 decimalPlaces, mfmU64 base)
{
	mfError err = mfsPrintF32(reinterpret_cast<mfsStream*>(&this->Get()), value, base, decimalPlaces);
	if (err == MF_ERROR_OKAY)
		return;
	throw StreamError(mfErrorToString(err));
}

void Magma::Framework::String::HStream::PrintF64(mfmF64 value, mfmU64 decimalPlaces, mfmU64 base)
{
	mfError err = mfsPrintF64(reinterpret_cast<mfsStream*>(&this->Get()), value, base, decimalPlaces);
	if (err == MF_ERROR_OKAY)
		return;
	throw StreamError(mfErrorToString(err));
}

void Magma::Framework::String::HStream::PrintPointer(void * value, mfmU64 base)
{
	if constexpr (sizeof(value) == sizeof(mfmU32))
	{
		mfError err = mfsPrintU32(reinterpret_cast<mfsStream*>(&this->Get()), (mfmU32)value, base);
		if (err == MF_ERROR_OKAY)
			return;
		throw StreamError(mfErrorToString(err));
	}
	else if constexpr (sizeof(value) == sizeof(mfmU64))
	{
		mfError err = mfsPrintU64(reinterpret_cast<mfsStream*>(&this->Get()), (mfmU64)value, base);
		if (err == MF_ERROR_OKAY)
			return;
		throw StreamError(mfErrorToString(err));
	}
	else
		abort(); // Invalid size
}

mfmU8 Magma::Framework::String::HStream::ParseU8(const mfsUTF8CodeUnit * terminator, mfmU64 base)
{
	mfmU8 out;
	mfError err = mfsParseU8(reinterpret_cast<mfsStream*>(&this->Get()), &out, base, terminator);
	if (err != MF_ERROR_OKAY)
		throw StreamError(mfErrorToString(err));
	return out;
}

mfmU16 Magma::Framework::String::HStream::ParseU16(const mfsUTF8CodeUnit * terminator, mfmU64 base)
{
	mfmU16 out;
	mfError err = mfsParseU16(reinterpret_cast<mfsStream*>(&this->Get()), &out, base, terminator);
	if (err != MF_ERROR_OKAY)
		throw StreamError(mfErrorToString(err));
	return out;
}

mfmU32 Magma::Framework::String::HStream::ParseU32(const mfsUTF8CodeUnit * terminator, mfmU64 base)
{
	mfmU32 out;
	mfError err = mfsParseU32(reinterpret_cast<mfsStream*>(&this->Get()), &out, base, terminator);
	if (err != MF_ERROR_OKAY)
		throw StreamError(mfErrorToString(err));
	return out;
}

mfmU64 Magma::Framework::String::HStream::ParseU64(const mfsUTF8CodeUnit * terminator, mfmU64 base)
{
	mfmU64 out;
	mfError err = mfsParseU64(reinterpret_cast<mfsStream*>(&this->Get()), &out, base, terminator);
	if (err != MF_ERROR_OKAY)
		throw StreamError(mfErrorToString(err));
	return out;
}

mfmI8 Magma::Framework::String::HStream::ParseI8(const mfsUTF8CodeUnit * terminator, mfmU64 base)
{
	mfmI8 out;
	mfError err = mfsParseI8(reinterpret_cast<mfsStream*>(&this->Get()), &out, base, terminator);
	if (err != MF_ERROR_OKAY)
		throw StreamError(mfErrorToString(err));
	return out;
}

mfmI16 Magma::Framework::String::HStream::ParseI16(const mfsUTF8CodeUnit * terminator, mfmU64 base)
{
	mfmI16 out;
	mfError err = mfsParseI16(reinterpret_cast<mfsStream*>(&this->Get()), &out, base, terminator);
	if (err != MF_ERROR_OKAY)
		throw StreamError(mfErrorToString(err));
	return out;
}

mfmI32 Magma::Framework::String::HStream::ParseI32(const mfsUTF8CodeUnit * terminator, mfmU64 base)
{
	mfmI32 out;
	mfError err = mfsParseI32(reinterpret_cast<mfsStream*>(&this->Get()), &out, base, terminator);
	if (err != MF_ERROR_OKAY)
		throw StreamError(mfErrorToString(err));
	return out;
}

mfmI64 Magma::Framework::String::HStream::ParseI64(const mfsUTF8CodeUnit * terminator, mfmU64 base)
{
	mfmI64 out;
	mfError err = mfsParseI64(reinterpret_cast<mfsStream*>(&this->Get()), &out, base, terminator);
	if (err != MF_ERROR_OKAY)
		throw StreamError(mfErrorToString(err));
	return out;
}

mfmF32 Magma::Framework::String::HStream::ParseF32(const mfsUTF8CodeUnit * terminator, mfmU64 base)
{
	mfmF32 out;
	mfError err = mfsParseF32(reinterpret_cast<mfsStream*>(&this->Get()), &out, base, terminator);
	if (err != MF_ERROR_OKAY)
		throw StreamError(mfErrorToString(err));
	return out;
}

mfmF64 Magma::Framework::String::HStream::ParseF64(const mfsUTF8CodeUnit * terminator, mfmU64 base)
{
	mfmF64 out;
	mfError err = mfsParseF64(reinterpret_cast<mfsStream*>(&this->Get()), &out, base, terminator);
	if (err != MF_ERROR_OKAY)
		throw StreamError(mfErrorToString(err));
	return out;
}