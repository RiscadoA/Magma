#include "StringStream.hpp"
#include "Exception.hpp"

Magma::Framework::String::StringStream::StringStream(void* buffer, mfmU64 size)
{
	mfError err = mfsCreateLocalStringStream(&m_ss, static_cast<mfmU8*>(buffer), size);
	if (err != MF_ERROR_OKAY)
		throw StreamError(ErrorToString(err));
	err = mfmIncObjectRef(&m_ss.base.object);
	if (err != MF_ERROR_OKAY)
		throw StreamError(ErrorToString(err));
}

Magma::Framework::String::StringStream::~StringStream()
{
	mfmDecObjectRef(&m_ss.base.object);
	mfsDestroyLocalStringStream(&m_ss);
}

Magma::Framework::String::HStringStream Magma::Framework::String::StringStream::Get()
{
	return &m_ss;
}

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