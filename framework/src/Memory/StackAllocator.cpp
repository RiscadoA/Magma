#include <Magma/Framework/Memory/StackAllocator.hpp>
#include <Magma/Framework/ErrorString.h>

Magma::Framework::Memory::StackAllocatorHandle Magma::Framework::Memory::CreateStackAllocator(mfmU64 size)
{
	mfmStackAllocator* alloc;
	mfError err = mfmCreateStackAllocator(&alloc, size);
	if (err != MF_ERROR_OKAY)
		throw AllocatorError(mfErrorToString(err));
	return alloc;
}

Magma::Framework::Memory::StackAllocatorHandle Magma::Framework::Memory::CreateStackAllocatorOnMemory(mfmU64 size, void * memory, mfmU64 memorySize)
{
	mfmStackAllocator* alloc;
	mfError err = mfmCreateStackAllocatorOnMemory(&alloc, size, memory, memorySize);
	if (err != MF_ERROR_OKAY)
		throw AllocatorError(mfErrorToString(err));
	return alloc;
}
