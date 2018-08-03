#include "Importer.hpp"

Magma::Resources::Importer::Importer(const std::string & name, Manager* manager)
{
	m_name = name;
	m_manager = manager;
}

Magma::Resources::Importer::~Importer()
{

}
