#include "Allocator.hpp"
#include "Exception.hpp"

Magma::Framework::Memory::Allocator Magma::Framework::Memory::StandardAllocator = nullptr;

void * Magma::Framework::Memory::Allocator::Allocate(mfmU64 size)
{
	void* mem;
	mfmError err = mfmAllocate(this->GetNoChecks(), &mem, size);
	if (err != MFM_ERROR_OKAY)
		throw AllocationError(ErrorToString(err));
	return mem;
}

void Magma::Framework::Memory::Allocator::Deallocate(void * memory)
{
	mfmError err = mfmDeallocate(this->GetNoChecks(), memory);
	if (err != MFM_ERROR_OKAY)
		throw AllocationError(ErrorToString(err));
}
