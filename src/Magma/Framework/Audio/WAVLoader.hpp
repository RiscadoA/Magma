#pragma once

#include <cstdint>

#include "RenderDevice.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace Audio
		{
			struct WAVData
			{
				Audio::Format format;
				int32_t sampleRate;
				int16_t channelCount;
				int16_t bitsPerSample;
				int32_t pcmSize;
				void* pcmData;
			};

			void LoadWAV(void* data, size_t size, WAVData& out);

			void UnloadWAV(WAVData& data);
		}
	}
}