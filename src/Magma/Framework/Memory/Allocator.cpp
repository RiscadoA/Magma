#include "Allocator.hpp"
#include "Exception.hpp"

Magma::Framework::Memory::Allocator Magma::Framework::Memory::StandardAllocator = nullptr;

void * Magma::Framework::Memory::Allocator::Allocate(mfmU64 size)
{
	void* mem;
	mfError err = mfmAllocate(this->GetNoChecks(), &mem, size);
	if (err != MF_ERROR_OKAY)
		throw AllocatorError(ErrorToString(err));
	return mem;
}

void Magma::Framework::Memory::Allocator::Deallocate(void * memory)
{
	mfError err = mfmDeallocate(this->GetNoChecks(), memory);
	if (err != MF_ERROR_OKAY)
		throw AllocatorError(ErrorToString(err));
}
