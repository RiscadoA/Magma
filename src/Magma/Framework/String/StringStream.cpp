#include "StringStream.hpp"
#include "Exception.hpp"

void Magma::Framework::String::HStringStream::Clear()
{
	mfError err = mfsClearStringStream(reinterpret_cast<mfsStream*>(&this->Get()));
	if (err == MF_ERROR_OKAY)
		return;
	throw StreamError(ErrorToString(err));
}

Magma::Framework::String::HStringStream Magma::Framework::String::CreateStringStream(void * buffer, mfmU64 size, Memory::HAllocator allocator)
{
	mfsStream* stream;
	mfError err = mfsCreateStringStream(&stream, static_cast<mfmU8*>(buffer), size, allocator.GetNoChecks());
	if (err != MF_ERROR_OKAY)
		throw StreamError(ErrorToString(err));
	return stream;
}