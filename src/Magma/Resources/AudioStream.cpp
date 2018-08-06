#include "AudioStream.hpp"
#include "Manager.hpp"

void Magma::Resources::AudioStream::Claim()
{
	if (!m_claimMutex.try_lock())
	{
		std::stringstream ss;
		ss << "Failed to claim audio stream resource data:" << std::endl;
		ss << "Resource was already claimed";
		throw ResourceError(ss.str());
	}
}

void Magma::Resources::AudioStream::Unclaim()
{
	m_claimMutex.unlock();
}

void Magma::Resources::AudioStream::SeekPositionBytes(size_t position)
{
	std::lock_guard<std::mutex> lk(m_mutex);
}

void Magma::Resources::AudioStream::SeekPositionSamples(size_t position)
{
	std::lock_guard<std::mutex> lk(m_mutex);
}

void Magma::Resources::AudioStream::SeekPositionSeconds(float position)
{
	std::lock_guard<std::mutex> lk(m_mutex);
}

bool Magma::Resources::AudioStream::FillBuffer(Framework::Audio::Buffer* buffer)
{
	// Lock mutex
	std::unique_lock<std::mutex> lk(m_mutex);

	// Go back to the start if the stream had already ended before
	if (currentPosition >= dataPosition + dataHeader.chunkSize)
		currentPosition = dataPosition;

	// Wait for the data to be loaded
	m_dataLoadedCV.wait(lk, [this] { return m_dataLoaded; });

	// Update audio buffer
	buffer->Update(m_bufferData, nextBufferSize, formatChunk.format, formatChunk.sampleRate);

	// Increment current position
	currentPosition += nextBufferSize;

	// Unlock and notify loader thread
	m_dataLoaded = false;
	lk.unlock();

	// Check if we got to the end of the stream
	return currentPosition >= dataPosition + dataHeader.chunkSize;
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
	// Lock importer
	std::lock_guard<std::mutex> lk(this->m_mutex);
	// Get file system
	auto fs = this->GetManager()->GetFileSystem();

	// Update every stream
	for (auto& s : m_streams)
	{
		std::lock_guard<std::mutex> lk(s->m_mutex);
		
		// Data still loaded, skip this stream
		if (s->m_dataLoaded)
			continue;

		// Read buffer
		{	
			// Get buffer size
			s->nextBufferSize = s->BufferSize;
			if (s->currentPosition + s->nextBufferSize >= s->dataPosition + s->dataHeader.chunkSize)
				s->nextBufferSize = s->dataPosition + s->dataHeader.chunkSize - s->currentPosition;

			// Seek buffer position and read it
			fs->Seek(s->file, s->currentPosition);
			fs->Read(s->file, s->m_bufferData, s->nextBufferSize);

			// Set data as loaded
			s->m_dataLoaded = true;
		}
		s->m_dataLoadedCV.notify_one();

		// Wait for buffer to be used
		/*{
			std::unique_lock<std::mutex> lk(s->mutex);
			s->m_dataLoadedCV.wait(lk, [s] { return !s->m_dataLoaded; });
		}*/
	}
}

void Magma::Resources::AudioStreamImporter::Import(Resource * resource)
{
	std::lock_guard<std::mutex> lk(this->m_mutex);

	auto data = new (m_pool.Allocate(sizeof(AudioStream))) AudioStream(resource);
	auto path = resource->GetDataPath();

	// WAV extension
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
				fs->Skip(data->file, header.chunkSize);
		}

		// Setup read position
		data->currentPosition = data->dataPosition;
	}
	// Unsupported extension
	else
	{
		data->~AudioStream();
		m_pool.Deallocate(data);

		std::stringstream ss;
		ss << "Failed to import audio stream resource on path '" << path.ToString() << "':" << std::endl;
		ss << "Unsupported extension '" << path.GetExtension() << "':" << std::endl;
		ss << "Supported extensions: 'wav'";
		throw ImporterError(ss.str());
	}

	// Add the stream to the streams list
	m_streams.push_back(data);
	resource->SetData(data);
}

void Magma::Resources::AudioStreamImporter::Destroy(Resource * resource)
{
	// Get data
	auto data = resource->GetData<AudioStream>();
	// Get file system
	auto fs = this->GetManager()->GetFileSystem();

	// Remove it from the streams list
	for (auto it = m_streams.begin(); it != m_streams.end(); ++it)
		if (*it == data)
		{
			m_streams.erase(it);
			break;
		}

	// Close the stream file
	fs->CloseFile(data->file);

	// Deconstruct the stream and deallocate it
	data->~AudioStream();
	m_pool.Deallocate(data);
	resource->SetData(nullptr);
}

Magma::Resources::ResourceMode Magma::Resources::AudioStreamImporter::GetMode(Resource * resources)
{
	return ResourceMode::CPUStream;
}
