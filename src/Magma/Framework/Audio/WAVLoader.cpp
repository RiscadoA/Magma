#include "WAVLoader.hpp"
#include "Exception.hpp"

#include "../Memory/Endianness.hpp"

#include <sstream>

void Magma::Framework::Audio::LoadWAV(void * data, size_t size, WAVData & out)
{
	if (size < 8)
	{
		std::stringstream ss;
		ss << "Failed to load WAV:" << std::endl;
		ss << "EOF encountered unexpectedly";
		throw LoaderError(ss.str());
	}

	// Check file marker (0-3)
	if (((char*)data)[0] != 'R' ||
		((char*)data)[1] != 'I' ||
		((char*)data)[2] != 'F' ||
		((char*)data)[3] != 'F')
	{
		std::stringstream ss;
		ss << "Failed to load WAV:" << std::endl;
		ss << "Invalid file:" << std::endl;
		ss << "Invalid RIFF marker";
		throw LoaderError(ss.str());
	}

	// Check file size (4-7)
	{
		auto fileSize = *(int32_t*)((char*)data + 4);
		Memory::FromLittleEndian4(&fileSize);
		if (size < fileSize)
		{
			std::stringstream ss;
			ss << "Failed to load WAV:" << std::endl;
			ss << "WAV header file size doesn't match the size sent to the function";
			throw LoaderError(ss.str());
		}
	}

	// Check file type header (8-11)
	if (((char*)data)[8] != 'W' ||
		((char*)data)[9] != 'A' ||
		((char*)data)[10] != 'V' ||
		((char*)data)[11] != 'E')
	{
		std::stringstream ss;
		ss << "Failed to load WAV:" << std::endl;
		ss << "Invalid file:" << std::endl;
		ss << "Invalid file type header (should be WAVE)";
		throw LoaderError(ss.str());
	}

	// Check format chunk marker (12-15)
	if (((char*)data)[12] != 'f' ||
		((char*)data)[13] != 'm' ||
		((char*)data)[14] != 't' ||
		(((char*)data)[15] != ' ' && ((char*)data)[15] != '\0'))
	{
		std::stringstream ss;
		ss << "Failed to load WAV:" << std::endl;
		ss << "Invalid file:" << std::endl;
		ss << "Invalid format chunk marker";
		throw LoaderError(ss.str());
	}

	// Length of format data (not needed) (16-19)

	// Type of format (20-21)
	{
		auto formatType = *(int16_t*)((char*)data + 20);
		Memory::FromLittleEndian2(&formatType);

		if (formatType != 1)
		{
			std::stringstream ss;
			ss << "Failed to load WAV:" << std::endl;
			ss << "Invalid file:" << std::endl;
			ss << "Unsupported format type '" << formatType << "' (only supports '1' for now, which corresponds to raw PCM)";
			throw LoaderError(ss.str());
		}
	}

	// Number of channels (22-23)
	{
		out.channelCount = *(int16_t*)((char*)data + 22);
		Memory::FromLittleEndian2(&out.channelCount);
	}

	// Sample rate (24-27)
	{
		out.sampleRate = *(int32_t*)((char*)data + 24);
		Memory::FromLittleEndian4(&out.sampleRate);
	}

	// Bits per sample (34-35)
	{
		out.bitsPerSample = *(int16_t*)((char*)data + 34);
		Memory::FromLittleEndian2(&out.bitsPerSample);
	}

	// Get format
	{
		if (out.bitsPerSample == 8 && out.channelCount == 1)
			out.format = Format::Mono8;
		else if (out.bitsPerSample == 16 && out.channelCount == 1)
			out.format = Format::Mono16;
		else if (out.bitsPerSample == 8 && out.channelCount > 1)
			out.format = Format::Stereo8;
		else if (out.bitsPerSample == 16 && out.channelCount > 1)
			out.format = Format::Stereo16;
		else
		{
			std::stringstream ss;
			ss << "Failed to load WAV:" << std::endl;
			ss << "Invalid file:" << std::endl;
			ss << "Unsupported format (bits per sample: " << out.bitsPerSample << "; channel count: " << out.channelCount << ")";
			throw LoaderError(ss.str());
		}
	}

	// Check data chunk marker (36-39)
	size_t dataLoc = 36;

	// Ignore other chunks
	while (((char*)data)[dataLoc] != 'd' ||
		((char*)data)[dataLoc + 1] != 'a' ||
		((char*)data)[dataLoc + 2] != 't' ||
		((char*)data)[dataLoc + 3] != 'a')
	{
		auto size = *(int32_t*)((char*)data + dataLoc + 4);
		Memory::FromLittleEndian4(&size);
		dataLoc += size + 8;
	}

	// Data size (40-43)
	{
		out.pcmSize = *(int32_t*)((char*)data + dataLoc + 4);
		Memory::FromLittleEndian4(&out.pcmSize);
	}

	// Data (44-x)
	out.pcmData = new char[out.pcmSize];
	memcpy(out.pcmData, (char*)data + dataLoc + 8, out.pcmSize);

	for (size_t i = 0; i < out.pcmSize / 4; ++i)
		Memory::FromLittleEndian4((int32_t*)out.pcmData + i);
}

void Magma::Framework::Audio::UnloadWAV(WAVData & data)
{
	delete[] data.pcmData;
}
