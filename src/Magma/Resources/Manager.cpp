#include "Manager.hpp"

#include <Magma/Framework/Files/STDFileSystem.hpp>
#include <Magma/Framework/String/MNIDocument.hpp>

Magma::Resources::Manager* Magma::Resources::Manager::s_manager = nullptr;

void Magma::Resources::Manager::Init(const ManagerSettings& settings)
{
	if (s_manager != nullptr)
	{
		std::stringstream ss;
		ss << "Failed to init Magma::Resources::Manager:" << std::endl;
		ss << "The manager was already initialized";
		throw ManagerError(ss.str());
	}

	s_manager = new Manager(settings);
}

void Magma::Resources::Manager::Load()
{
	for (auto& r : s_manager->m_resources)
		if (r->GetMode() == ResourceMode::CPUPermanent ||
			r->GetMode() == ResourceMode::GPUPermanent)
			s_manager->Import(r);
}

void Magma::Resources::Manager::Terminate()
{
	if (s_manager == nullptr)
	{
		std::stringstream ss;
		ss << "Failed to init Magma::Resources::Manager:" << std::endl;
		ss << "The manager is not initialized or was already terminated";
		throw ManagerError(ss.str());
	}

	delete s_manager;
	s_manager = nullptr;
}

Magma::Resources::ResourceView Magma::Resources::Manager::GetResource(const std::string & name)
{
	// Search for resource
	for (auto& r : s_manager->m_resources)
	{
		// If it wasn't imported yet, import it
		if (!r->HasData())
			s_manager->Import(r);
		return ResourceView(*r);
	}

	// Not found
	std::stringstream ss;
	ss << "Failed to get resource on Magma::Resources::Manager:" << std::endl;
	ss << "No resource with name '" << name << "' found";
	throw ManagerError(ss.str());
}

void Magma::Resources::Manager::DestroyResource(const std::string & name)
{
	// Search for resource
	for (auto& r : s_manager->m_resources)
	{
		// If it isn't already destroyed, destroy it
		if (r->HasData())
			s_manager->Destroy(r);
		return;
	}

	// Not found
	std::stringstream ss;
	ss << "Failed to destroy resource on Magma::Resources::Manager:" << std::endl;
	ss << "No resource with name '" << name << "' found";
	throw ManagerError(ss.str());
}

Magma::Resources::Manager::Manager(const ManagerSettings& settings)
	: m_settings(settings)
{
	m_fileSystem = new Framework::Files::STDFileSystem(settings.rootPath);

	this->LoadMetaData();
}

Magma::Resources::Manager::~Manager()
{
	for (auto& r : m_resources)
		delete r;
	m_resources.clear();

	delete m_fileSystem;
}

void Magma::Resources::Manager::LoadMetaData(const Framework::Files::Path& directory)
{	
	std::vector<Framework::Files::Path> contents;
	m_fileSystem->GetDirectoryContents(directory, contents);

	for (auto& p : contents)
	{
		if (p.IsDirectory())
			LoadMetaData(p);
		else if (p.GetExtension() == "mni")
		{
			auto file = m_fileSystem->OpenFile(Framework::Files::FileMode::Read, p);
			auto srcSize = m_fileSystem->GetSize(file);
			std::string src;
			src.resize(srcSize);
			m_fileSystem->Read(file, src.data(), srcSize);
			m_fileSystem->CloseFile(file);

			CreateResources(src);
		}
	}
}

void Magma::Resources::Manager::CreateResources(const std::string& metaData)
{
	Framework::String::MNIDocument document;
	document.Read(metaData);

	auto regions = document.GetRegions();
	for (auto& r : regions)
	{
		std::string name = r.name;
		std::string type = document.GetEntry(r.name, "type").value;
		std::string path = document.GetEntry(r.name, "path").value;

		m_resources.emplace_back(new Resource(name, type, path, ResourceMode::CPUPermanent));
	}
}

void Magma::Resources::Manager::Import(Resource * resource)
{
	// Destroy resource if it was already imported before
	if (resource->HasData())
		this->Destroy(resource);

	// Search for importer
	for (auto& i : this->m_importers)
	{
		// If the importer type matches the resource
		if (i->GetName() == resource->GetType())
		{
			i->Import(resource);
			return;
		}
	}

	// Not found
	std::stringstream ss;
	ss << "Failed to import resource on Magma::Resources::Manager:" << std::endl;
	ss << "No importer with name '" << resource->GetType() << "' found";
	throw ManagerError(ss.str());
}

void Magma::Resources::Manager::Destroy(Resource * resource)
{
	// If resource was already destroyed, do nothing
	if (resource->HasData())
		return;

	// Search for importer
	for (auto& i : s_manager->m_importers)
	{
		// If the importer type matches the resource
		if (i->GetName() == resource->GetType())
		{
			i->Destroy(resource);
			return;
		}
	}

	// Not found
	std::stringstream ss;
	ss << "Failed to destroy resource on Magma::Resources::Manager:" << std::endl;
	ss << "No importer with name '" << resource->GetType() << "' found";
	throw ManagerError(ss.str());
}
