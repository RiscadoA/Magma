#include "AudioStream.hpp"

Magma::Framework::Audio::Buffer * Magma::Resources::AudioStream::GetNextBuffer()
{
	buffers[bufferIndex].loaded = false;
	buffers[bufferIndex].playing = false;
	++bufferIndex;

	if (bufferIndex == BufferCount)
		bufferIndex = 0;

	while (!buffers[bufferIndex].loaded)
		;

	buffers[bufferIndex].playing = true;
	return buffers[bufferIndex].buffer;
}

Magma::Resources::AudioStreamImporter::AudioStreamImporter(Manager * manager, Framework::Audio::RenderDevice * device)
	: Importer("audio-stream", manager), m_device(device), m_pool(sizeof(AudioStream), 32)
{

}

void Magma::Resources::AudioStreamImporter::Update()
{
	for (auto& s : m_streams)
	{
		// Load buffers
		for (size_t i = s->bufferIndex; i < s->BufferCount; ++i)
			if (s->buffers[i].loaded == false)
				LoadBuffer(&s->buffers[i]);
		for (size_t i = 0; i < s->bufferIndex; ++i)
			if (s->buffers[i].loaded == false)
				LoadBuffer(&s->buffers[i]);
	}
}

void Magma::Resources::AudioStreamImporter::LoadBuffer(AudioStreamBuffer * buffer)
{

}

void Magma::Resources::AudioStreamImporter::Import(Resource * resource)
{
	auto data = new (m_pool.Allocate(sizeof(AudioStream))) AudioStream(resource);
	auto path = resource->GetDataPath();

	// Check extension
	if (path.GetExtension() == "wav")
	{
		// Read shader source from file
		auto fs = this->GetManager()->GetFileSystem();
		auto file = fs->OpenFile(Framework::Files::FileMode::Read, path);
		auto size = fs->GetSize(file);
		char* src = new char[size + 1];
		fs->Read(file, src, size);
		src[size] = '\0';
		fs->CloseFile(file);

		// Compile shader
		try
		{
			char bo[4096];
			auto boSize = Framework::Graphics::ShaderCompiler::Run(src, bo, sizeof(bo));
			delete[] src;

			Framework::Graphics::ShaderData shaderData(bo, boSize);
			data->type = shaderData.GetShaderType();
			switch (data->type)
			{
				case Framework::Graphics::ShaderType::Vertex:
					data->vertexShader = m_device->CreateVertexShader(shaderData);
					break;

				case Framework::Graphics::ShaderType::Pixel:
					data->pixelShader = m_device->CreatePixelShader(shaderData);
					break;

				case Framework::Graphics::ShaderType::Invalid:
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

void Magma::Resources::AudioStreamImporter::Destroy(Resource * resource)
{
	auto data = resource->GetData<AudioStream>();

	data->~AudioStream();
	m_pool.Deallocate(data);
	resource->SetData(nullptr);
}

ResourceMode Magma::Resources::AudioStreamImporter::GetMode(Resource * resources)
{
	return ResourceMode();
}
