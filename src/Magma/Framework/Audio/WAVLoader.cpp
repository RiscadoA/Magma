#include "WAVLoader.hpp"
#include "Exception.hpp"

#include "../Memory/Endianness.hpp"

#include <sstream>

void Magma::Framework::Audio::LoadWAV(const void * data, size_t size, WAVData & out)
{
	size_t dataHead = 0;

	// Load WAV header
	dataHead += ParseWAVHeader((char*)data + dataHead, 12, out.fileHeader);

	// Check WAV file type
	if (out.fileHeader.type[0] != 'W' ||
		out.fileHeader.type[1] != 'A' ||
		out.fileHeader.type[2] != 'V' ||
		out.fileHeader.type[3] != 'E')
	{
		std::stringstream ss;
		ss << "Failed to load WAV:" << std::endl;
		ss << "Invalid file:" << std::endl;
		ss << "Invalid file type (only WAVE is supported for now)";
		throw LoaderError(ss.str());
	}
	
	// Get chunks
	while (dataHead < size)
	{
		auto oldHead = dataHead;

		// Get chunk header
		WAVChunkHeader header;
		dataHead += ParseWAVChunkHeader((char*)data + dataHead, size - dataHead, header);

		// Load format chunk
		if (header.chunkName[0] == 'f' &&
			header.chunkName[1] == 'm' &&
			header.chunkName[2] == 't' &&
			(header.chunkName[3] == ' ' || header.chunkName[3] == '\0'))
		{
			dataHead = oldHead;
			dataHead += ParseWAVFormatChunk((char*)data + dataHead, size - dataHead, out.formatChunk);
		}
		// Load data chunk
		else if (header.chunkName[0] == 'd' &&
				 header.chunkName[1] == 'a' &&
				 header.chunkName[2] == 't' &&
				 header.chunkName[3] == 'a')
		{
			dataHead = oldHead;
			dataHead += ParseWAVDataChunk((char*)data + dataHead, size - dataHead, out.dataChunk);
		}
		// Skip unknown chunk
		else dataHead += header.chunkSize;
	}
}

void Magma::Framework::Audio::UnloadWAV(WAVData & data)
{
	UnloadWAVDataChunk(data.dataChunk);
}

size_t Magma::Framework::Audio::ParseWAVHeader(const void * data, size_t size, WAVFileHeader & out)
{
	if (size < 12)
	{
		std::stringstream ss;
		ss << "Failed to parse WAV header:" << std::endl;
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
		ss << "Failed to parse WAV header:" << std::endl;
		ss << "Invalid file:" << std::endl;
		ss << "Invalid RIFF marker";
		throw LoaderError(ss.str());
	}

	// Get file size (4-7)
	{
		out.fileSize = *(int32_t*)((char*)data + 4);
		Memory::FromLittleEndian4(&out.fileSize);
	}

	// Get file type (8-11)
	out.type[0] = ((char*)data)[8];
	out.type[1] = ((char*)data)[9];
	out.type[2] = ((char*)data)[10];
	out.type[3] = ((char*)data)[11];

	return 12;
}

size_t Magma::Framework::Audio::ParseWAVFormatChunk(const void * data, size_t size, WAVFormatChunk & out)
{
	if (size < 18)
	{
		std::stringstream ss;
		ss << "Failed to parse WAV format chunk:" << std::endl;
		ss << "EOF encountered unexpectedly";
		throw LoaderError(ss.str());
	}

	// Get chunk name
	out.header.chunkName[0] = ((char*)data)[0];
	out.header.chunkName[1] = ((char*)data)[1];
	out.header.chunkName[2] = ((char*)data)[2];
	out.header.chunkName[3] = ((char*)data)[3];

	// Check format chunk name (0-3)
	if (out.header.chunkName[0] != 'f' ||
		out.header.chunkName[1] != 'm' ||
		out.header.chunkName[2] != 't' ||
		(out.header.chunkName[3] != ' ' && out.header.chunkName[3] != '\0'))
	{
		std::stringstream ss;
		ss << "Failed to parse WAV format chunk:" << std::endl;
		ss << "Invalid format chunk marker";
		throw LoaderError(ss.str());
	}

	// Length of format data (4-7)
	out.header.chunkSize = *(int32_t*)((char*)data + 4);

	// Type of format (8-9)
	{
		auto formatType = *(int16_t*)((char*)data + 8);
		Memory::FromLittleEndian2(&formatType);

		if (formatType != 1)
		{
			std::stringstream ss;
			ss << "Failed to parse WAV format chunk:" << std::endl;
			ss << "Unsupported format type '" << formatType << "' (only supports '1' for now, which corresponds to raw PCM)";
			throw LoaderError(ss.str());
		}
	}

	// Number of channels (10-11)
	{
		out.channelCount = *(int16_t*)((char*)data + 10);
		Memory::FromLittleEndian2(&out.channelCount);
	}

	// Sample rate (12-15)
	{
		out.sampleRate = *(int32_t*)((char*)data + 12);
		Memory::FromLittleEndian4(&out.sampleRate);
	}

	// Bits per sample (22-23)
	{
		out.bitsPerSample = *(int16_t*)((char*)data + 22);
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
			ss << "Failed to parse WAV format chunk:" << std::endl;
			ss << "Unsupported format (bits per sample: " << out.bitsPerSample << "; channel count: " << out.channelCount << ")";
			throw LoaderError(ss.str());
		}
	}

	return out.header.chunkSize + 8;
}

size_t Magma::Framework::Audio::ParseWAVDataChunk(const void * data, size_t size, WAVDataChunk & out)
{
	if (size < 8)
	{
		std::stringstream ss;
		ss << "Failed to parse WAV data chunk:" << std::endl;
		ss << "EOF encountered unexpectedly";
		throw LoaderError(ss.str());
	}

	// Get chunk name
	out.header.chunkName[0] = ((char*)data)[0];
	out.header.chunkName[1] = ((char*)data)[1];
	out.header.chunkName[2] = ((char*)data)[2];
	out.header.chunkName[3] = ((char*)data)[3];

	// Check data chunk name (0-3)
	if (out.header.chunkName[0] != 'd' ||
		out.header.chunkName[1] != 'a' ||
		out.header.chunkName[2] != 't' ||
		out.header.chunkName[3] != 'a')
	{
		std::stringstream ss;
		ss << "Failed to parse WAV format chunk:" << std::endl;
		ss << "Invalid format chunk marker";
		throw LoaderError(ss.str());
	}

	// Length of data data (4-7)
	out.header.chunkSize = *(int32_t*)((char*)data + 4);
	Memory::FromLittleEndian4(&out.header.chunkSize);

	// Store PCM data
	out.pcmData = malloc(out.header.chunkSize);
	memcpy(out.pcmData, (char*)data + 8, out.header.chunkSize);

	return out.header.chunkSize + 8;
}

size_t Magma::Framework::Audio::GetWAVDataChunk(const void* data, size_t dataSize, size_t chunkSize, void * out)
{
	if (dataSize < chunkSize)
		chunkSize = dataSize;
	memcpy(out, data, chunkSize);
	return chunkSize;
}

void Magma::Framework::Audio::UnloadWAVDataChunk(WAVDataChunk & data)
{
	free(data.pcmData);
}

size_t Magma::Framework::Audio::ParseWAVChunkHeader(const void * data, size_t size, WAVChunkHeader & out)
{
	if (size < 8)
	{
		std::stringstream ss;
		ss << "Failed to parse WAV chunk header:" << std::endl;
		ss << "EOF encountered unexpectedly";
		throw LoaderError(ss.str());
	}

	// Get chunk name
	out.chunkName[0] = ((char*)data)[0];
	out.chunkName[1] = ((char*)data)[1];
	out.chunkName[2] = ((char*)data)[2];
	out.chunkName[3] = ((char*)data)[3];

	// Length of format data (not needed) (4-7)
	out.chunkSize = *(int32_t*)((char*)data + 4);
	Memory::FromLittleEndian4(&out.chunkSize);

	return 8;
}