#include "Shader.hpp"
#include "Manager.hpp"
#include "Exception.hpp"

#include <Magma/Framework/Graphics/1.X/ShaderCompiler.hpp>

Magma::Resources::ShaderImporter::ShaderImporter(Manager* manager, Framework::Graphics::V1X::RenderDevice* device)
	: Importer("shader", manager), m_pool(sizeof(Shader), 256), m_device(device)
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
	auto path = resource->GetDataPath();

	// Check extension
	if (path.GetExtension() == "msl")
	{
		// Read shader source from file
		auto fs = this->GetManager()->GetFileSystem();
		auto file = fs->OpenFile(Framework::File::FileMode::Read, path);
		auto size = fs->GetSize(file);
		char* src = new char[size + 1];
		fs->Read(file, src, size);
		src[size] = '\0';
		fs->CloseFile(file);

		// Compile shader
		try
		{
			char bo[4096];
			auto boSize = Framework::Graphics::V1X::ShaderCompiler::Run(src, bo, sizeof(bo));
			delete[] src;

			Framework::Graphics::V1X::ShaderData shaderData(bo, boSize);
			data->type = shaderData.GetShaderType();
			switch (data->type)
			{
				case Framework::Graphics::V1X::ShaderType::Vertex:
					data->vertexShader = m_device->CreateVertexShader(shaderData);
					break;

				case Framework::Graphics::V1X::ShaderType::Pixel:
					data->pixelShader = m_device->CreatePixelShader(shaderData);
					break;

				case Framework::Graphics::V1X::ShaderType::Invalid:
				{
					std::stringstream ss;
					ss << "Failed to import shader resource on path '" << path.ToString() << "':" << std::endl;
					ss << "Invalid shader type" << std::endl;
					throw ImporterError(ss.str());
					break;
				}

				default:
				{
					std::stringstream ss;
					ss << "Failed to import shader resource on path '" << path.ToString() << "':" << std::endl;
					ss << "Unsupported shader type" << std::endl;
					throw ImporterError(ss.str());
					break;
				}
			}
			
		}
		catch (Framework::Graphics::ShaderError& err)
		{
			std::stringstream ss;
			ss << "Failed to import shader resource on path '" << path.ToString() << "':" << std::endl;
			ss << "MSL shader compilation failed:" << std::endl;
			ss << err.what();
			throw ImporterError(ss.str());
		}
	}
	else
	{
		std::stringstream ss;
		ss << "Failed to import shader resource on path '" << path.ToString() << "':" << std::endl;
		ss << "Unsupported extension '" << path.GetExtension() << "':" << std::endl;
		ss << "Supported extensions: 'msl'";
		throw ImporterError(ss.str());
	}

	resource->SetData(data);
}

void Magma::Resources::ShaderImporter::Destroy(Resource * resource)
{
	auto data = resource->GetData<Shader>();
	switch (data->type)
	{
		case Framework::Graphics::V1X::ShaderType::Vertex:
			m_device->DestroyVertexShader(data->vertexShader);
			break;

		case Framework::Graphics::V1X::ShaderType::Pixel:
			m_device->DestroyPixelShader(data->pixelShader);
			break;
	}

	data->~Shader();
	m_pool.Deallocate(data);
	resource->SetData(nullptr);
}

Magma::Resources::ResourceMode Magma::Resources::ShaderImporter::GetMode(Resource * resource)
{
	auto mode = resource->GetParam("storage");

	if (mode == "permanent")
		return ResourceMode::GPUPermanent;
	else if (mode == "temporary")
		return ResourceMode::GPUTemporary;
	else
	{
		std::stringstream ss;
		ss << "Failed to get shader resource on path '" << resource->GetDataPath().ToString() << "' storage mode:" << std::endl;
		ss << "Unsupported storage mode '" << mode << "':" << std::endl;
		ss << "Supported storage modes: 'permament', 'temporary'";
		throw ImporterError(ss.str());
	}
}
