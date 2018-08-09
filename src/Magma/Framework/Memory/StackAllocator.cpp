#include "StackAllocator.hpp"

#include <sstream>

Magma::Framework::Memory::StackAllocator::StackAllocator(mfmU64 size)
{
	m_stack = nullptr;
	auto err = ::mfmCreateStackAllocator(&m_stack, size);
	switch (err)
	{
		case MFM_ERROR_OKAY:
			break;
		case MFM_ERROR_ALLOCATION_FAILED:
		{
			std::stringstream ss;
			ss << "Failed to create StackAllocator:" << std::endl;
			ss << "mfmCreateStackAllocator returned MFM_ERROR_ALLOCATION_FAILED";
			throw AllocationError(ss.str());
		}
		case MFM_ERROR_INVALID_ARGUMENTS:
		{
			std::stringstream ss;
			ss << "Failed to create StackAllocator:" << std::endl;
			ss << "mfmCreateStackAllocator returned MFM_ERROR_INVALID_ARGUMENTS";
			throw AllocationError(ss.str());
		}
		default:
		{
			std::stringstream ss;
			ss << "Failed to create StackAllocator:" << std::endl;
			ss << "mfmCreateStackAllocator returned '" << err << "'";
			throw AllocationError(ss.str());
		}
	}
}

Magma::Framework::Memory::StackAllocator::~StackAllocator()
{
	if (m_stack != nullptr)
		::mfmDestroyStackAllocator(m_stack);
}

void * Magma::Framework::Memory::StackAllocator::Allocate(mfmU64 size)
{
	void* memory;
	auto err = ::mfmStackAllocate(m_stack, &memory, size);
	switch (err)
	{
		case MFM_ERROR_OKAY:
			break;
		case MFM_ERROR_ALLOCATOR_OVERFLOW:
		{
			std::stringstream ss;
			ss << "Failed to allocate on StackAllocator:" << std::endl;
			ss << "mfmStackAllocate returned MFM_ERROR_ALLOCATOR_OVERFLOW";
			throw AllocationError(ss.str());
		}
		default:
		{
			std::stringstream ss;
			ss << "Failed to allocate on StackAllocator:" << std::endl;
			ss << "mfmStackAllocate returned '" << err << "'";
			throw AllocationError(ss.str());
		}
	}
	return memory;
}

void Magma::Framework::Memory::StackAllocator::SetHead(void* head)
{
	auto err = ::mfmStackSetHead(m_stack, head);
	switch (err)
	{
		case MFM_ERROR_OKAY:
			break;
		case MFM_ERROR_OUT_OF_BOUNDS:
		{
			std::stringstream ss;
			ss << "Failed to set stack head on StackAllocator:" << std::endl;
			ss << "mfmStackSetHead returned MFM_ERROR_OUT_OF_BOUNDS";
			throw AllocationError(ss.str());
		}
		default:
		{
			std::stringstream ss;
			ss << "Failed to set stack head on StackAllocator:" << std::endl;
			ss << "mfmStackSetHead returned '" << err << "'";
			throw AllocationError(ss.str());
		}
	}
}
