#include "Allocator.hpp"
#include "Exception.hpp"
#include "../ErrorString.h"

Magma::Framework::Memory::HAllocator Magma::Framework::Memory::StandardAllocator = nullptr;

void * Magma::Framework::Memory::HAllocator::Allocate(mfmU64 size)
{
	void* mem;
	mfError err = mfmAllocate(this->GetNoChecks(), &mem, size);
	if (err != MF_ERROR_OKAY)
		throw AllocatorError(mfErrorToString(err));
	return mem;
}

void Magma::Framework::Memory::HAllocator::Deallocate(void * memory)
{
	mfError err = mfmDeallocate(this->GetNoChecks(), memory);
	if (err != MF_ERROR_OKAY)
		throw AllocatorError(mfErrorToString(err));
}
