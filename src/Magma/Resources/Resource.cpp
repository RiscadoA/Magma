#include "Resource.hpp"

Magma::Resources::ResourceView::ResourceView(const ResourceView & rhs)
{
	m_resource = rhs.m_resource;
	++m_resource->m_referenceCount;
}

Magma::Resources::ResourceView::ResourceView(ResourceView && rhs)
{
	m_resource = rhs.m_resource;
	rhs.m_resource = nullptr;
}

Magma::Resources::ResourceView::ResourceView(Resource & resource)
{
	m_resource = &resource;
	++m_resource->m_referenceCount;
}

Magma::Resources::ResourceView::~ResourceView()
{
	if (m_resource != nullptr)
		--m_resource->m_referenceCount;
}

Magma::Resources::Resource & Magma::Resources::ResourceView::Get()
{
	return *m_resource;
}

const Magma::Resources::Resource & Magma::Resources::ResourceView::Get() const
{
	return *m_resource;
}

Magma::Resources::Resource::Resource()
{
	m_data = nullptr;
	m_referenceCount = 0;
}

Magma::Resources::Resource::~Resource()
{

}

Magma::Resources::ResourceData::ResourceData(Resource * resource)
	: m_resource(resource)
{

}
