#include <Magma/Framework/Memory/LinearAllocator.hpp>
#include <Magma/Framework/ErrorString.h>

void Magma::Framework::Memory::HLinearAllocator::Reset()
{
	mfError err = mfmLinearReset((mfmLinearAllocator*)&this->Get());
	if (err != MF_ERROR_OKAY)
		throw AllocatorError(mfErrorToString(err));
}

Magma::Framework::Memory::HLinearAllocator Magma::Framework::Memory::CreateLinearAllocator(mfmU64 size)
{
	mfmLinearAllocator* alloc;
	mfError err = mfmCreateLinearAllocator(&alloc, size);
	if (err != MF_ERROR_OKAY)
		throw AllocatorError(mfErrorToString(err));
	return alloc;
}

Magma::Framework::Memory::HLinearAllocator Magma::Framework::Memory::CreateLinearAllocatorOnMemory(mfmU64 size, void * memory, mfmU64 memorySize)
{
	mfmLinearAllocator* alloc;
	mfError err = mfmCreateLinearAllocatorOnMemory(&alloc, size, memory, memorySize);
	if (err != MF_ERROR_OKAY)
		throw AllocatorError(mfErrorToString(err));
	return alloc;
}
