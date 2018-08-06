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
		///		Audio stream resource data.
		/// </summary>
		class AudioStream : public ResourceData
		{
		public:
			using ResourceData::ResourceData;

			/// <summary>
			///		Audio stream buffer size.
			/// </summary>
			static const size_t BufferSize = 65536;

			/// <summary>
			///		Claims this audio stream (this prevents it from being used in multiple places at the same time).
			/// </summary>
			void Claim();

			/// <summary>
			///		Unclaims this audio stream.
			/// </summary>
			void Unclaim();

			/// <summary>
			///		Sets the position of the stream in bytes.
			/// </summary>
			/// <param name="position">Position in bytes</param>
			void SeekPositionBytes(size_t position);

			/// <summary>
			///		Sets the position of the stream in samples.
			/// </summary>
			/// <param name="position">Position in samples</param>
			void SeekPositionSamples(size_t position);

			/// <summary>
			///		Sets the position of the stream in seconds.
			/// </summary>
			/// <param name="position">Position in seconds</param>
			void SeekPositionSeconds(float position);

			/// <summary>
			///		Fills an audio buffer with next BufferSize bytes in the stream.
			/// </summary>
			/// <param name="buffer">Handle to an audio buffer.</param>
			/// <returns>If this buffer was the last buffer in the stream, returns true, otherwise returns false.</returns>
			bool FillBuffer(Framework::Audio::Buffer* buffer);

			/// <summary>
			///		WAV file header.
			/// </summary>
			Framework::Audio::WAVFileHeader fileHeader;

			/// <summary>
			///		WAV data header.
			/// </summary>
			Framework::Audio::WAVChunkHeader dataHeader;

			/// <summary>
			///		WAV format chunk.
			/// </summary>
			Framework::Audio::WAVFormatChunk formatChunk;

		private:
			friend class AudioStreamImporter;
			
			size_t dataPosition;
			size_t currentPosition;
			size_t nextBufferSize;

			std::mutex m_mutex;
			std::mutex m_claimMutex;

			void* file;

			char m_bufferData[BufferSize];
			bool m_dataLoaded = false;
			std::condition_variable m_dataLoadedCV;
		};

		/// <summary>
		///		Audio stream claiming and unclaiming utility.
		/// </summary>
		class AudioStreamClaimGuard final
		{
		public:
			/// <summary>
			///		Claims an audio stream.
			/// </summary>
			inline AudioStreamClaimGuard(AudioStream* stream) : m_claimed(false), m_stream(stream) { m_stream->Claim(); m_claimed = true; }

			/// <summary>
			///		Unclaims an audio stream (if it was claimed).
			/// </summary>
			inline ~AudioStreamClaimGuard() { this->Unclaim(); }

			/// <summary>
			///		Unclaims the audio stream (if it was claimed).
			/// </summary>
			inline void Unclaim() { if (m_claimed) { m_stream->Unclaim(); m_claimed = false; } }

		private:
			AudioStream * m_stream;
			bool m_claimed;
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
			Framework::Audio::RenderDevice* m_device;
			Framework::Memory::PoolAllocator m_pool;

			std::vector<AudioStream*> m_streams;

			std::mutex m_mutex;
		};
	}
}
