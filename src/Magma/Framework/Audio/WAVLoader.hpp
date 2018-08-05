#pragma once

#include <cstdint>

#include "RenderDevice.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace Audio
		{
			/// <summary>
			///		Holds data about the file header in a WAV file
			/// </summary>
			struct WAVFileHeader
			{
				char type[4];
				int32_t fileSize;
			};

			/// <summary>
			///		Holds data about a chunk header in a WAV file
			/// </summary>
			struct WAVChunkHeader
			{
				char chunkName[4];
				int32_t chunkSize;
			};

			/// <summary>
			///		Holds data about the format chunk in a WAV file
			/// </summary>
			struct WAVFormatChunk
			{
				WAVChunkHeader header;

				Audio::Format format;
				int32_t sampleRate;
				int16_t channelCount;
				int16_t bitsPerSample;
			};

			/// <summary>
			///		Holds data of a single chunk in a WAV file
			/// </summary>
			struct WAVDataChunk
			{
				WAVChunkHeader header;

				/// <summary>
				///		PCM data pointer
				/// </summary>
				void* pcmData;
			};

			/// <summary>
			///		Holds data of the whole WAV file
			/// </summary>
			struct WAVData
			{
				WAVFileHeader fileHeader;
				WAVFormatChunk formatChunk;
				WAVDataChunk dataChunk;
			};

			void LoadWAV(void* data, size_t size, WAVData& out);

			void UnloadWAV(WAVData& data);

			size_t ParseWAVHeader(void* data, size_t size, WAVFileHeader& out);

			size_t ParseWAVFormatChunk(void* data, size_t size, WAVFormatChunk& out);

			size_t ParseWAVDataChunk(void* data, size_t size, WAVDataChunk& out);

			void UnloadWAVDataChunk(WAVDataChunk& data);

			size_t ParseWAVChunkHeader(void* data, size_t size, WAVChunkHeader& out);
		}
	}
}