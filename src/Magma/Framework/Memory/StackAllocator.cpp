#include "StackAllocator.hpp"

#include <cstdlib>

Magma::Framework::Memory::StackAllocator::StackAllocator(size_t size)
{
	m_size = size;
	m_head = 0;
	m_memory = malloc(m_size);
}

Magma::Framework::Memory::StackAllocator::~StackAllocator()
{
	free(m_memory);
}

void * Magma::Framework::Memory::StackAllocator::Allocate(size_t size)
{
	if (m_size < m_head + size)
		throw AllocationError("Failed to allocate on StackAllocator:\nAllocator overflow");
	m_head += size;
	return (char*)m_memory + m_head - size;
}

size_t Magma::Framework::Memory::StackAllocator::GetHead()
{
	return m_head;
}

void Magma::Framework::Memory::StackAllocator::SetHead(size_t head)
{
	if (head > m_size)
		throw AllocationError("Failed to set head on StackAllocator:\nNew head out of bounds");
	m_head = head;
}
