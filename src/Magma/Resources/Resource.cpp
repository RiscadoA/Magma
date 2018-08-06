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

Magma::Resources::ResourceView::ResourceView()
{
	m_resource = nullptr;
}


void Magma::Resources::ResourceView::Set(Resource & resource)
{
	if (m_resource != nullptr)
	{
		--m_resource->m_referenceCount;
		m_resource = nullptr;
	}

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

Magma::Resources::Resource & Magma::Resources::ResourceView::Get() const
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

Magma::Resources::Resource::Resource(const std::string& name, const std::string& type, const Framework::Files::Path& dataPath)
	: m_name(name), m_type(type), m_dataPath(dataPath)
{
	m_mode = ResourceMode::Invalid;
	m_importer = nullptr;
	m_data = nullptr;
	m_referenceCount = 0;
}

Magma::Resources::Resource::~Resource()
{

}

void Magma::Resources::Resource::RemoveParam(const std::string & name)
{
	m_params.erase(name);
}

void Magma::Resources::Resource::SetParam(const std::string & name, const std::string & value)
{
	m_params[name] = value;
}

const std::string & Magma::Resources::Resource::GetParam(const std::string & name)
{
	return m_params.at(name);
}

Magma::Resources::ResourceData::ResourceData(Resource * resource)
	: m_resource(resource)
{

}
