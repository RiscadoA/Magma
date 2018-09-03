#include "PoolAllocator.hpp"

#include <sstream>

Magma::Framework::Memory::PoolAllocator::PoolAllocator(mfmU64 slotSize, mfmU64 slotCount, bool expandable)
{
	m_pool = nullptr;

	mfmPoolAllocatorDesc desc;
	desc.slotSize = slotSize;
	desc.slotCount = slotCount;
	desc.expandable = expandable ? MFM_TRUE : MFM_FALSE;
	auto err = ::mfmCreatePoolAllocator(&m_pool, &desc);
	switch (err)
	{
		case MF_ERROR_OKAY:
			break;
		case MFM_ERROR_ALLOCATION_FAILED:
		{
			std::stringstream ss;
			ss << "Failed to create PoolAllocator:" << std::endl;
			ss << "mfmCreatePoolAllocator returned MFM_ERROR_ALLOCATION_FAILED";
			throw AllocatorError(ss.str());
		}
		case MFM_ERROR_INVALID_ARGUMENTS:
		{
			std::stringstream ss;
			ss << "Failed to create PoolAllocator:" << std::endl;
			ss << "mfmCreatePoolAllocator returned MFM_ERROR_INVALID_ARGUMENTS";
			throw AllocatorError(ss.str());
		}
		default:
		{
			std::stringstream ss;
			ss << "Failed to create PoolAllocator:" << std::endl;
			ss << "mfmCreatePoolAllocator returned '" << err << "'";
			throw AllocatorError(ss.str());
		}
	}
}

Magma::Framework::Memory::PoolAllocator::~PoolAllocator()
{
	if (m_pool != nullptr)
		::mfmDestroyPoolAllocator(m_pool);
}

void * Magma::Framework::Memory::PoolAllocator::Allocate(mfmU64 size)
{
	void* memory;
	auto err = ::mfmPoolAllocate(m_pool, &memory, size);
	switch (err)
	{
		case MF_ERROR_OKAY:
			break;
		case MFM_ERROR_ALLOCATOR_OVERFLOW:
		{
			std::stringstream ss;
			ss << "Failed to allocate on PoolAllocator:" << std::endl;
			ss << "The pool has no more free slots:" << std::endl;
			ss << "mfmPoolAllocate returned MFM_ERROR_ALLOCATOR_OVERFLOW";
			throw AllocatorError(ss.str());
		}
		case MFM_ERROR_ALLOCATION_TOO_BIG:
		{
			std::stringstream ss;
			ss << "Failed to allocate on PoolAllocator:" << std::endl;
			ss << "The requested allocation size is bigger than each slot size:" << std::endl;
			ss << "mfmPoolAllocate returned MFM_ERROR_ALLOCATION_TOO_BIG";
			throw AllocatorError(ss.str());
		}
		case MFM_ERROR_ALLOCATION_FAILED:
		{
			std::stringstream ss;
			ss << "Failed to allocate on PoolAllocator:" << std::endl;
			ss << "Pool expansion allocation failed:" << std::endl;
			ss << "mfmPoolAllocate returned MFM_ERROR_ALLOCATION_FAILED";
			throw AllocatorError(ss.str());
		}
		default:
		{
			std::stringstream ss;
			ss << "Failed to allocate on PoolAllocator:" << std::endl;
			ss << "mfmPoolAllocate returned '" << err << "'";
			throw AllocatorError(ss.str());
		}
	}
	return memory;
}

void Magma::Framework::Memory::PoolAllocator::Deallocate(void * ptr)
{
	auto err = ::mfmPoolDeallocate(m_pool, ptr);
	switch (err)
	{
		case MF_ERROR_OKAY:
			break;
		case MFM_ERROR_OUT_OF_BOUNDS:
		{
			std::stringstream ss;
			ss << "Failed to deallocate on PoolAllocator:" << std::endl;
			ss << "The passed memory pointer is out of the pool bounds:" << std::endl;
			ss << "mfmPoolDeallocate returned MFM_ERROR_OUT_OF_BOUNDS";
			throw AllocatorError(ss.str());
		}
		default:
		{
			std::stringstream ss;
			ss << "Failed to deallocate on PoolAllocator:" << std::endl;
			ss << "mfmPoolDeallocate returned '" << err << "'";
			throw AllocatorError(ss.str());
		}
	}
}
