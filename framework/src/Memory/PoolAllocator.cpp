#include <Magma/Framework/Memory/PoolAllocator.hpp>
#include <Magma/Framework/ErrorString.h>

Magma::Framework::Memory::HAllocator Magma::Framework::Memory::CreatePoolAllocator(const mfmPoolAllocatorDesc & desc)
{
	mfmPoolAllocator* alloc;
	mfError err = mfmCreatePoolAllocator(&alloc, &desc);
	if (err != MF_ERROR_OKAY)
		throw AllocatorError(mfErrorToString(err));
	return alloc;
}


Magma::Framework::Memory::HAllocator Magma::Framework::Memory::CreatePoolAllocatorOnMemory(const mfmPoolAllocatorDesc & desc, void* memory, mfmU64 memorySize)
{
	mfmPoolAllocator* alloc;
	mfError err = mfmCreatePoolAllocatorOnMemory(&alloc, &desc, memory, memorySize);
	if (err != MF_ERROR_OKAY)
		throw AllocatorError(mfErrorToString(err));
	return alloc;
}
