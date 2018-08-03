#include "Shader.hpp"
#include "Manager.hpp"

Magma::Resources::ShaderImporter::ShaderImporter(Manager* manager)
	: Importer("shader", manager), m_pool(sizeof(Shader), 256)
{

}

Magma::Resources::ShaderImporter::~ShaderImporter()
{

}

void Magma::Resources::ShaderImporter::Update()
{
	// Do nothing (not necessary here)
}

void Magma::Resources::ShaderImporter::Import(Resource * resource)
{
	auto data = new (m_pool.Allocate(sizeof(Shader))) Shader(resource);

	auto fs = this->GetManager()->GetFileSystem();

	fs->OpenFile(Framework::Files::FileMode::Read, resource->GetData());


	resource->SetData(data);
}

void Magma::Resources::ShaderImporter::Destroy(Resource * resource)
{
	m_pool.Deallocate(resource->GetData<Shader>());
	resource->SetData(nullptr);
}
