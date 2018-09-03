#include "StringStream.hpp"
#include "Exception.hpp"

void Magma::Framework::String::StringStream::Clear()
{
	mfError err = mfsClearStringStream(reinterpret_cast<mfsStream*>(&this->Get()));
	if (err == MF_ERROR_OKAY)
		return;
	throw StreamError(ErrorToString(err));
}

Magma::Framework::String::StringStream Magma::Framework::String::CreateStringStream(void * buffer, mfmU64 size, Memory::AllocatorHandle allocator)
{
	mfsStream* stream;
	mfError err = mfsCreateStringStream(&stream, static_cast<mfmU8*>(buffer), size, allocator.GetNoChecks());
	if (err != MF_ERROR_OKAY)
		throw StreamError(ErrorToString(err));
	return stream;
}