#include "PoolAllocator.hpp"

#include <sstream>

Magma::Framework::Memory::PoolAllocator::PoolAllocator(mfmU64 slotSize, mfmU64 slotCount, bool expandable)
{
	mfmPoolAllocatorDesc desc;
	desc.slotSize = slotSize;
	desc.slotCount = slotCount;
	desc.expandable = expandable ? MFM_TRUE : MFM_FALSE;
	auto err = mfmCreatePoolAllocator(&m_pool, &desc);
	switch (err)
	{
		case MFM_ERROR_OKAY:
			break;
		case MFM_ERROR_ALLOCATION_FAILED:
		{
			std::stringstream ss;
			ss << "Failed to create PoolAllocator:" << std::endl;
			ss << "mfmCreatePoolAllocator returned MFM_ERROR_ALLOCATION_FAILED";
			throw AllocationError(ss.str());
		}
		case MFM_ERROR_INVALID_ARGUMENTS:
		{
			std::stringstream ss;
			ss << "Failed to create PoolAllocator:" << std::endl;
			ss << "mfmCreatePoolAllocator returned MFM_ERROR_INVALID_ARGUMENTS";
			throw AllocationError(ss.str());
		}
		default:
		{
			std::stringstream ss;
			ss << "Failed to create PoolAllocator:" << std::endl;
			ss << "mfmCreatePoolAllocator returned '" << err << "'";
			throw AllocationError(ss.str());
		}
	}
}

Magma::Framework::Memory::PoolAllocator::~PoolAllocator()
{
	mfmDestroyPoolAllocator(m_pool);
}

void * Magma::Framework::Memory::PoolAllocator::Allocate(mfmU64 size)
{
	void* memory;
	auto err = mfmPoolAllocate(&memory, m_pool, size);
	switch (err)
	{
		case MFM_ERROR_OKAY:
			break;
		case MFM_ERROR_ALLOCATOR_OVERFLOW:
		{
			std::stringstream ss;
			ss << "Failed to allocate on PoolAllocator:" << std::endl;
			ss << "The pool has no more free slots:" << std::endl;
			ss << "mfmPoolAllocate returned MFM_ERROR_ALLOCATOR_OVERFLOW";
			throw AllocationError(ss.str());
		}
		case MFM_ERROR_ALLOCATION_TOO_BIG:
		{
			std::stringstream ss;
			ss << "Failed to allocate on PoolAllocator:" << std::endl;
			ss << "The requested allocation size is bigger than each slot size:" << std::endl;
			ss << "mfmPoolAllocate returned MFM_ERROR_ALLOCATION_TOO_BIG";
			throw AllocationError(ss.str());
		}
		case MFM_ERROR_ALLOCATION_FAILED:
		{
			std::stringstream ss;
			ss << "Failed to allocate on PoolAllocator:" << std::endl;
			ss << "Pool expansion allocation failed:" << std::endl;
			ss << "mfmPoolAllocate returned MFM_ERROR_ALLOCATION_FAILED";
			throw AllocationError(ss.str());
		}
		default:
		{
			std::stringstream ss;
			ss << "Failed to allocate on PoolAllocator:" << std::endl;
			ss << "mfmPoolAllocate returned '" << err << "'";
			throw AllocationError(ss.str());
		}
	}
	return memory;
}

void Magma::Framework::Memory::PoolAllocator::Deallocate(void * ptr)
{
	auto err = mfmPoolDeallocate(ptr, m_pool);
	switch (err)
	{
		case MFM_ERROR_OKAY:
			break;
		case MFM_ERROR_OUT_OF_BOUNDS:
		{
			std::stringstream ss;
			ss << "Failed to deallocate on PoolAllocator:" << std::endl;
			ss << "The passed memory pointer is out of the pool bounds:" << std::endl;
			ss << "mfmPoolDeallocate returned MFM_ERROR_OUT_OF_BOUNDS";
			throw AllocationError(ss.str());
		}
		default:
		{
			std::stringstream ss;
			ss << "Failed to deallocate on PoolAllocator:" << std::endl;
			ss << "mfmPoolDeallocate returned '" << err << "'";
			throw AllocationError(ss.str());
		}
	}
}
