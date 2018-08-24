#include "LinearAllocator.hpp"

#include <sstream>

Magma::Framework::Memory::LinearAllocator::LinearAllocator(mfmU64 size)
{
	m_linear = nullptr;
	auto err = ::mfmCreateLinearAllocator(&m_linear, size);
	switch (err)
	{
		case MF_ERROR_OKAY:
			break;
		case MFM_ERROR_ALLOCATION_FAILED:
		{
			std::stringstream ss;
			ss << "Failed to create LinearAllocator:" << std::endl;
			ss << "mfmCreateLinearAllocator returned MFM_ERROR_ALLOCATION_FAILED";
			throw AllocatorError(ss.str());
		}
		case MFM_ERROR_INVALID_ARGUMENTS:
		{
			std::stringstream ss;
			ss << "Failed to create LinearAllocator:" << std::endl;
			ss << "mfmCreateLinearAllocator returned MFM_ERROR_INVALID_ARGUMENTS";
			throw AllocatorError(ss.str());
		}
		default:
		{
			std::stringstream ss;
			ss << "Failed to create LinearAllocator:" << std::endl;
			ss << "mfmCreateLinearAllocator returned '" << err << "'";
			throw AllocatorError(ss.str());
		}
	}
}

Magma::Framework::Memory::LinearAllocator::~LinearAllocator()
{
	if (m_linear != nullptr)
		::mfmDestroyLinearAllocator(m_linear);
}

void * Magma::Framework::Memory::LinearAllocator::Allocate(mfmU64 size)
{
	void* memory;
	auto err = ::mfmLinearAllocate(m_linear, &memory, size);
	switch (err)
	{
		case MF_ERROR_OKAY:
			break;
		case MFM_ERROR_ALLOCATOR_OVERFLOW:
		{
			std::stringstream ss;
			ss << "Failed to allocate on LinearAllocator:" << std::endl;
			ss << "mfmLinearAllocate returned MFM_ERROR_ALLOCATOR_OVERFLOW";
			throw AllocatorError(ss.str());
		}
		default:
		{
			std::stringstream ss;
			ss << "Failed to allocate on LinearAllocator:" << std::endl;
			ss << "mfmLinearAllocate returned '" << err << "'";
			throw AllocatorError(ss.str());
		}
	}
	return memory;
}

void Magma::Framework::Memory::LinearAllocator::Reset()
{
	::mfmLinearReset(m_linear);
}
