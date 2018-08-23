#include "StringStream.hpp"
#include "Exception.hpp"

void Magma::Framework::String::StringStream::Clear()
{
	mfsError err = mfsClearStringStream(reinterpret_cast<mfsStream*>(&this->Get()));
	if (err == MFS_ERROR_OKAY)
		return;
	throw StreamError(ErrorToString(err));
}

Magma::Framework::String::StringStream Magma::Framework::String::CreateStringStream(void * buffer, mfmU64 size, Memory::Allocator allocator)
{
	mfsStream* stream;
	mfsError err = mfsCreateStringStream(&stream, static_cast<mfmU8*>(buffer), size, allocator.GetNoChecks());
	if (err != MFS_ERROR_OKAY)
		throw StreamError(ErrorToString(err));
	return stream;
}