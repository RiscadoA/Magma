#pragma once

#include "Importer.hpp"

#include <Magma/Framework/Audio/RenderDevice.hpp>
#include <Magma/Framework/Memory/PoolAllocator.hpp>

#include <vector>

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
			std::atomic<bool> playing = false;
			std::atomic<bool> loaded = false;
		};

		/// <summary>
		///		Audio stream resource data.
		/// </summary>
		class AudioStream : public ResourceData
		{
		public:
			using ResourceData::ResourceData;

			inline const static size_t BufferCount = 4;

			Framework::Audio::Buffer* GetNextBuffer();

			Framework::Audio::Format format;
			std::atomic<size_t> bufferIndex;
			AudioStreamBuffer buffers[BufferCount];
			bool repeating;
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
			void LoadBuffer(AudioStreamBuffer* buffer);

			char m_bufferData[65536];

			Framework::Audio::RenderDevice* m_device;
			Framework::Memory::PoolAllocator m_pool;

			std::vector<AudioStream*> m_streams;
		};
	}
}
