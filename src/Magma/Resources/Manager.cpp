#include "Manager.hpp"

#include <Magma/Framework/Files/STDFileSystem.hpp>

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

}

void Magma::Resources::Manager::DestroyResource(const std::string & name)
{
}

Magma::Resources::Manager::Manager(const ManagerSettings& settings)
	: m_settings(settings)
{
	m_fileSystem = new Framework::Files::STDFileSystem(settings.rootPath);
}

Magma::Resources::Manager::~Manager()
{
	delete m_fileSystem;
}
