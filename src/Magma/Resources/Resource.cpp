#include "Resource.hpp"
#include "Exception.hpp"

#include <sstream>

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

void Magma::Resources::ResourceView::Release()
{
	if (m_resource == nullptr)
	{
		std::stringstream ss;
		ss << "Failed to release resource view:" << std::endl;
		ss << "Resource view was already released";
		throw ResourceError(ss.str());
	}

	--m_resource->m_referenceCount;
	m_resource = nullptr;
}

Magma::Resources::Resource & Magma::Resources::ResourceView::Get()
{
	if (m_resource == nullptr)
	{
		std::stringstream ss;
		ss << "Failed to get resource from resource view:" << std::endl;
		ss << "Resource view was released and its resource cannot be accessed again";
		throw ResourceError(ss.str());
	}

	return *m_resource;
}

const Magma::Resources::Resource & Magma::Resources::ResourceView::Get() const
{
	if (m_resource == nullptr)
	{
		std::stringstream ss;
		ss << "Failed to get resource from resource view:" << std::endl;
		ss << "Resource view was released and its resource cannot be accessed again";
		throw ResourceError(ss.str());
	}

	return *m_resource;
}

Magma::Resources::Resource::Resource(const std::string& name, const Framework::Files::Path& dataPath, ResourceMode mode)
	: m_name(name), m_dataPath(dataPath), m_mode(mode)
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
