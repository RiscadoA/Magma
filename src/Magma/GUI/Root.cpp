#include "Root.hpp"
#include "Exception.hpp"

#include <sstream>

Magma::GUI::Root::Root(size_t poolSize, size_t maxElementSize)
	: Element(Elements::Type::Root)
{
	m_poolSize = poolSize;
	m_maxElementSize = maxElementSize;

	m_pool = (char*)malloc(m_poolSize * m_maxElementSize + m_poolSize);
	m_free = (bool*)(m_pool + (m_poolSize * m_maxElementSize));

	for (size_t i = 0; i < m_poolSize; ++i)
		m_free[i] = true;
}

Magma::GUI::Root::~Root()
{
	this->Destroy();
	this->Clean();
	free(m_pool);
}

size_t Magma::GUI::Root::Clean()
{
	size_t count = 0;

	for (size_t i = 0; i < m_poolSize; ++i)
	{
		auto element = (Element*)(m_pool + i * m_maxElementSize);
		if (!m_free[i] && !element->IsValid())
		{
			element->Destroy();
			element->~Element();
			m_free[i] = true;
			++count;
		}
	}

	if (count != 0)
		count += this->Clean();

	return count;
}

Magma::GUI::Element * Magma::GUI::Root::Allocate(size_t size)
{
	if (size > m_maxElementSize)
	{
		std::stringstream ss;
		ss << "Failed to allocate Element on Magma::GUI::Root:" << std::endl;
		ss << "Element size is bigger than the maximum element size set on root creation (" << size << " > " << m_maxElementSize << ")";
		throw ElementError(ss.str());
	}

	for (size_t i = 0; i < m_poolSize; ++i)
		if (m_free[i])
		{
			m_free[i] = false;
			return (Element*)(m_pool + m_maxElementSize * i);
		}

	std::stringstream ss;
	ss << "Failed to allocate Element on Magma::GUI::Root:" << std::endl;
	ss << "Element pool is already full. Try increasing the pool size (" << m_poolSize << ")";
	throw ElementError(ss.str());
}
