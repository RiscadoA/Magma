#pragma once

#include "Importer.hpp"

#include <Magma/Framework/Audio/WAVLoader.hpp>
#include <Magma/Framework/Audio/RenderDevice.hpp>
#include <Magma/Framework/Memory/PoolAllocator.hpp>

#include <vector>
#include <mutex>

namespace Magma
{
	namespace Resources
	{
		/// <summary>
		///		Represents a buffer in the audio stream.
		/// </summary>
		struct AudioStreamBuffer
		{
			Framework::Audio::Buffer* buffer;
			std::atomic<bool> loaded = false;
			std::atomic<bool> end = false;
		};

		/// <summary>
		///		Audio stream resource data.
		/// </summary>
		class AudioStream : public ResourceData
		{
		public:
			using ResourceData::ResourceData;

			static const size_t BufferCount = 4;
			static const size_t BufferSize = 65536;

			Framework::Audio::Buffer* GetNextBuffer();

			void FillBuffer(Framework::Audio::Buffer* buffer);

			std::atomic<size_t> bufferIndex;
			AudioStreamBuffer buffers[AudioStream::BufferCount];
			bool repeating;

			Framework::Audio::WAVFileHeader fileHeader;
			Framework::Audio::WAVChunkHeader dataHeader;
			Framework::Audio::WAVFormatChunk formatChunk;

			size_t dataPosition;
			size_t currentPosition;

			std::mutex mutex;

			void* file;
		};

		/// <summary>
		///		Used to import audio stream resources.
		/// </summary>
		class AudioStreamImporter final : public Importer
		{
		public:
			AudioStreamImporter(Manager* manager, Framework::Audio::RenderDevice* device);
			virtual ~AudioStreamImporter() final;

			// Inherited via Importer
			virtual void Update() final;
			virtual void Import(Resource * resource) final;
			virtual void Destroy(Resource * resource) final;
			virtual ResourceMode GetMode(Resource* resources) final;

		private:
			void LoadBuffer(AudioStreamBuffer* buffer, AudioStream* stream);

			char m_bufferData[AudioStream::BufferSize];

			Framework::Audio::RenderDevice* m_device;
			Framework::Memory::PoolAllocator m_pool;

			std::vector<AudioStream*> m_streams;

			std::mutex m_mutex;
		};
	}
}
