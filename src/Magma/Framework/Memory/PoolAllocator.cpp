#include "PoolAllocator.hpp"

#include <sstream>

Magma::Framework::Memory::PoolAllocator::PoolAllocator(size_t elementSize, size_t elementCount)
	: m_elementSize(elementSize), m_elementCount(elementCount)
{
	m_memory = (char*)malloc(m_elementSize * m_elementCount + m_elementCount);
	m_occupied = (bool*)(m_memory + m_elementSize * m_elementCount);
	for (size_t i = 0; i < m_elementCount; ++i)
		m_occupied[i] = false;
}

Magma::Framework::Memory::PoolAllocator::~PoolAllocator()
{
	free(m_memory);
}

void * Magma::Framework::Memory::PoolAllocator::Allocate(size_t size)
{
	if (size > m_elementSize)
	{
		std::stringstream ss;
		ss << "Failed to allocate on PoolAllocator:" << std::endl;
		ss << "Allocation size is bigger than element size (" << size << " > " << m_elementSize << ")";
		throw AllocationError(ss.str());
	}

	for (size_t i = 0; i < m_elementCount; ++i)
		if (m_occupied[i] == false)
		{
			m_occupied[i] = true;
			return m_memory + i * m_elementSize;
		}
	
	std::stringstream ss;
	ss << "Failed to allocate on PoolAllocator:" << std::endl;
	ss << "Pool is full";
	throw AllocationError(ss.str());
}

void Magma::Framework::Memory::PoolAllocator::Deallocate(void * ptr)
{
	for (size_t i = 0; i < m_elementCount; ++i)
		if (m_memory + i * m_elementSize == ptr)
		{
			m_occupied[i] = false;
			return;
		}

	std::stringstream ss;
	ss << "Failed to deallocate on PoolAllocator:" << std::endl;
	ss << "Pointer is out of bounds";
	throw AllocationError(ss.str());
}
