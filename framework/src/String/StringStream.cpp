#include <Magma/Framework/String/StringStream.hpp>
#include <Magma/Framework/String/Exception.hpp>
#include <Magma/Framework/ErrorString.h>

Magma::Framework::String::StringStream::StringStream(void* buffer, mfmU64 size)
{
	mfError err = mfsCreateLocalStringStream(&m_ss, static_cast<mfmU8*>(buffer), size);
	if (err != MF_ERROR_OKAY)
		throw StreamError(mfErrorToString(err));
	err = mfmAcquireObject(&m_ss.base.object);
	if (err != MF_ERROR_OKAY)
		throw StreamError(mfErrorToString(err));
}

Magma::Framework::String::StringStream::~StringStream()
{
	mfmReleaseObject(&m_ss.base.object);
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
	throw StreamError(mfErrorToString(err));
}

Magma::Framework::String::HStringStream Magma::Framework::String::CreateStringStream(void * buffer, mfmU64 size, Memory::HAllocator allocator)
{
	mfsStream* stream;
	mfError err = mfsCreateStringStream(&stream, static_cast<mfmU8*>(buffer), size, allocator.GetNoChecks());
	if (err != MF_ERROR_OKAY)
		throw StreamError(mfErrorToString(err));
	return stream;
}