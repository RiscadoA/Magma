#include "AudioStream.hpp"
#include "Manager.hpp"

Magma::Framework::Audio::Buffer * Magma::Resources::AudioStream::GetNextBuffer()
{
	// Get next buffer and mark previous as not loaded and not playing
	buffers[bufferIndex].loaded = false;
	buffers[bufferIndex].playing = false;
	++bufferIndex;

	if (bufferIndex == BufferCount)
		bufferIndex = 0;

	// Wait for the buffer to be loaded before playing it
	while (!buffers[bufferIndex].loaded)
		;

	// If the previous buffer was the last buffer return nullptr
	if (buffers[bufferIndex].end == true && !repeating)
		return nullptr;

	// Set it as playing
	buffers[bufferIndex].playing = true;
	return buffers[bufferIndex].buffer;
}

Magma::Resources::AudioStreamImporter::AudioStreamImporter(Manager * manager, Framework::Audio::RenderDevice * device)
	: Importer("audio-stream", manager), m_device(device), m_pool(sizeof(AudioStream), 32)
{

}

Magma::Resources::AudioStreamImporter::~AudioStreamImporter()
{

}

void Magma::Resources::AudioStreamImporter::Update()
{
	for (auto& s : m_streams)
	{
		// Load buffers
		for (size_t i = s->bufferIndex; i < s->BufferCount; ++i)
			if (s->buffers[i].loaded == false)
				LoadBuffer(&s->buffers[i], s);
		for (size_t i = 0; i < s->bufferIndex; ++i)
			if (s->buffers[i].loaded == false)
				LoadBuffer(&s->buffers[i], s);
	}
}

void Magma::Resources::AudioStreamImporter::LoadBuffer(AudioStreamBuffer * buffer, AudioStream* stream)
{
	auto fs = this->GetManager()->GetFileSystem();

	if (stream->currentPosition >= stream->dataPosition + stream->dataHeader.chunkSize)
	{
		stream->currentPosition = stream->dataPosition;
		buffer->end = true;
		buffer->loaded = true;
	}
	else
	{
		auto bufferSize = stream->BufferSize;
		if (stream->currentPosition + bufferSize >= stream->dataPosition + stream->dataHeader.chunkSize)
			bufferSize = stream->dataPosition + stream->dataHeader.chunkSize - stream->currentPosition;
		fs->Seek(stream->file, stream->currentPosition);
		fs->Read(stream->file, m_bufferData, bufferSize);
		buffer->buffer->Update(m_bufferData, bufferSize, stream->formatChunk.format, stream->formatChunk.sampleRate);
		buffer->loaded = true;
	}
}

void Magma::Resources::AudioStreamImporter::Import(Resource * resource)
{
	auto data = new (m_pool.Allocate(sizeof(AudioStream))) AudioStream(resource);
	data->repeating = false;

	auto path = resource->GetDataPath();

	// Check extension
	if (path.GetExtension() == "wav")
	{
		// Read WAV file
		auto fs = this->GetManager()->GetFileSystem();
		data->file = fs->OpenFile(Framework::Files::FileMode::Read, path);

		// Get header
		char* memData = new char[256];
		fs->Read(data->file, memData, 12);
		Framework::Audio::ParseWAVHeader(memData, 12, data->fileHeader);

		bool hasFormat = false;
		bool hasData = false;

		// Get chunks
		while (!hasFormat || !hasData)
		{
			// Parse header
			Framework::Audio::WAVChunkHeader header;
			fs->Read(data->file, memData, 8);
			Framework::Audio::ParseWAVChunkHeader(memData, 8, header);

			// If it is the format chunk, parse it
			if (header.chunkName[0] == 'f' &&
				header.chunkName[1] == 'm' &&
				header.chunkName[2] == 't' &&
				(header.chunkName[3] == ' ' || header.chunkName[3] == '\0'))
			{
				Framework::Audio::WAVFormatChunk chunk;
				fs->Read(data->file, (char*)memData + 8, header.chunkSize);
				Framework::Audio::ParseWAVFormatChunk(memData, 256, data->formatChunk);
				hasFormat = true;
			}
			// If it is the data chunk, store its header
			else if (header.chunkName[0] == 'd' &&
					 header.chunkName[1] == 'a' &&
					 header.chunkName[2] == 't' &&
					 header.chunkName[3] == 'a')
			{
				data->dataHeader = header;
				data->dataPosition = fs->GetPosition(data->file);
				hasData = true;
			}
			// Unnecessary chunks
			else
			{
				fs->Skip(data->file, header.chunkSize);
			}
		}

		// Setup first data buffers
		data->currentPosition = data->dataPosition;
		for (size_t i = 0; i < AudioStream::BufferCount; ++i)
		{
			data->buffers[i].buffer = m_device->CreateBuffer();
			data->buffers[i].loaded = false;
			data->buffers[i].playing = false;
			LoadBuffer(&data->buffers[i], data);
		}
	}
	else
	{
		std::stringstream ss;
		ss << "Failed to import audio stream resource on path '" << path.ToString() << "':" << std::endl;
		ss << "Unsupported extension '" << path.GetExtension() << "':" << std::endl;
		ss << "Supported extensions: 'wav'";
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

Magma::Resources::ResourceMode Magma::Resources::AudioStreamImporter::GetMode(Resource * resources)
{
	return ResourceMode::CPUStream;
}
