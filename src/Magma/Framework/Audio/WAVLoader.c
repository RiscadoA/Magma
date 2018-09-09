#include "WAVLoader.h"
#include "../String/Stream.h"
#include "../Memory/Endianness.h"
#include "../Memory/Allocator.h"

mfError mfaLoadWAV(void * stream, mfaWAVData * data, void * allocator)
{
	if (stream == NULL || data == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;

	mfError err;

	// Load WAV file header
	err = mfaLoadWAVFileHeader(stream, &data->fileHeader);
	if (err != MF_ERROR_OKAY)
		return err;

	// Check WAV file type
	if (data->fileHeader.type[0] != 'W' ||
		data->fileHeader.type[1] != 'A' ||
		data->fileHeader.type[2] != 'V' ||
		data->fileHeader.type[3] != 'E')
		return MFA_ERROR_INVALID_DATA;

	// Get chunks
	mfmU64 currentPos = 0;
	do
	{
		// Get chunk header
		mfaWAVChunkHeader header;
		err = mfaLoadWAVChunkHeader(stream, &header);
		if (err != MF_ERROR_OKAY)
			return err;

		// Load chunk
		// "fmt " chunk
		if (header.name[0] == 'f' && header.name[1] == 'm' && header.name[2] == 't' && header.name[3] == ' ')
		{
			data->formatChunk.header = header;
			err = mfaLoadWAVFormatChunkNoHeader(stream, &data->formatChunk);
			if (err != MF_ERROR_OKAY)
				return err;
		}
		// "data" chunk
		else if (header.name[0] == 'd' && header.name[1] == 'a' && header.name[2] == 't' && header.name[3] == 'a')
		{
			data->dataChunk.header = header;
			err = mfaLoadWAVDataChunkNoHeader(stream, &data->dataChunk, &data->formatChunk, allocator);
			if (err != MF_ERROR_OKAY)
				return err;
		}
		// Skip unknown chunk
		else
		{
			err = mfsSeekHead(stream, header.size);
			if (err != MF_ERROR_OKAY)
				return err;
		}

		// Get position
		err = mfsTell(stream, &currentPos);
		if (err != MF_ERROR_OKAY)
			return err;
	} while (currentPos < data->fileHeader.size + 8);

	return MF_ERROR_OKAY;
}

mfError mfaLoadWAVFileHeader(void * stream, mfaWAVFileHeader * out)
{
	if (stream == NULL || out == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;

	mfError err;
	mfmU64 readSize = 0;

	// Check marker
	mfsUTF8CodeUnit marker[4];
	err = mfsRead(stream, marker, 4, &readSize);
	if (err != MF_ERROR_OKAY)
		return err;
	if (readSize < 4 ||
		marker[0] != 'R' ||
		marker[1] != 'I' ||
		marker[2] != 'F' ||
		marker[3] != 'F')
		return MFA_ERROR_INVALID_DATA;

	// Get file size
	err = mfsRead(stream, &out->size, 4, &readSize);
	if (err != MF_ERROR_OKAY)
		return err;
	if (readSize < 4)
		return MFA_ERROR_INVALID_DATA;
	mfmFromLittleEndian4(&out->size, &out->size);

	// Get file type
	err = mfsRead(stream, &out->type, 4, &readSize);
	if (err != MF_ERROR_OKAY)
		return err;
	if (readSize < 4)
		return MFA_ERROR_INVALID_DATA;

	return MF_ERROR_OKAY;
}

mfError mfaLoadWAVChunkHeader(void * stream, mfaWAVChunkHeader * out)
{
	if (stream == NULL || out == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;

	mfError err;
	mfmU64 readSize = 0;

	// Get chunk name
	err = mfsRead(stream, out->name, 4, &readSize);
	if (err != MF_ERROR_OKAY)
		return err;
	if (readSize < 4)
		return MFA_ERROR_INVALID_DATA;

	// Get chunk size
	err = mfsRead(stream, &out->size, 4, &readSize);
	if (err != MF_ERROR_OKAY)
		return err;
	if (readSize < 4)
		return MFA_ERROR_INVALID_DATA;
	mfmFromLittleEndian4(&out->size, &out->size);

	return MF_ERROR_OKAY;
}

mfError mfaLoadWAVFormatChunk(void * stream, mfaWAVFormatChunk * out)
{
	if (stream == NULL || out == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;

	mfError err;

	err = mfaLoadWAVChunkHeader(stream, &out->header);
	if (err != MF_ERROR_OKAY)
		return err;

	if (out->header.name[0] != 'f' ||
		out->header.name[1] != 'm' ||
		out->header.name[2] != 't' ||
		out->header.name[3] != ' ')
		return MFA_ERROR_INVALID_DATA;

	err = mfaLoadWAVFormatChunkNoHeader(stream, out);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

mfError mfaLoadWAVFormatChunkNoHeader(void * stream, mfaWAVFormatChunk * out)
{
	if (stream == NULL || out == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;

	mfError err;
	mfmU64 readSize = 0;

	mfmU64 oldPos = 0;
	err = mfsTell(stream, &oldPos);
	if (err != MF_ERROR_OKAY)
		return err;

	// Check format type
	{
		mfmI16 formatType;

		err = mfsRead(stream, &formatType, 2, &readSize);
		if (err != MF_ERROR_OKAY)
			return err;
		if (readSize < 2)
			return MFA_ERROR_INVALID_DATA;
		mfmFromLittleEndian2(&formatType, &formatType);

		if (formatType != 1)
			return MFA_ERROR_NOT_SUPPORTED;
	}

	// Get number of channels
	{
		err = mfsRead(stream, &out->channelCount, 2, &readSize);
		if (err != MF_ERROR_OKAY)
			return err;
		if (readSize < 2)
			return MFA_ERROR_INVALID_DATA;
		mfmFromLittleEndian2(&out->channelCount, &out->channelCount);
	}

	// Get sample rate
	{
		err = mfsRead(stream, &out->sampleRate, 4, &readSize);
		if (err != MF_ERROR_OKAY)
			return err;
		if (readSize < 4)
			return MFA_ERROR_INVALID_DATA;
		mfmFromLittleEndian4(&out->sampleRate, &out->sampleRate);
	}

	// Skip data
	err = mfsSeekHead(stream, 6);
	if (err != MF_ERROR_OKAY)
		return err;

	// Get bits per sample
	{
		err = mfsRead(stream, &out->bitsPerSample, 2, &readSize);
		if (err != MF_ERROR_OKAY)
			return err;
		if (readSize < 2)
			return MFA_ERROR_INVALID_DATA;
		mfmFromLittleEndian2(&out->bitsPerSample, &out->bitsPerSample);
	}

	// Get format
	if (out->bitsPerSample == 8 && out->channelCount == 1)
		out->format = MFA_MONO8;
	else if (out->bitsPerSample == 16 && out->channelCount == 1)
		out->format = MFA_MONO16;
	else if (out->bitsPerSample == 8 && out->channelCount > 1)
		out->format = MFA_STEREO8;
	else if (out->bitsPerSample == 16 && out->channelCount > 1)
		out->format = MFA_STEREO16;
	else
		return MFA_ERROR_NOT_SUPPORTED;

	// Skip to end of chunk
	err = mfsSeekBegin(stream, oldPos + out->header.size);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

mfError mfaLoadWAVDataChunk(void * stream, mfaWAVDataChunk * out, const mfaWAVFormatChunk* format, void * allocator)
{
	if (stream == NULL || out == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;

	mfError err;

	err = mfaLoadWAVChunkHeader(stream, &out->header);
	if (err != MF_ERROR_OKAY)
		return err;

	if (out->header.name[0] != 'd' ||
		out->header.name[1] != 'a' ||
		out->header.name[2] != 't' ||
		out->header.name[3] != 'a')
		return MFA_ERROR_INVALID_DATA;

	err = mfaLoadWAVDataChunkNoHeader(stream, out, format, allocator);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

mfError mfaLoadWAVDataChunkNoHeader(void * stream, mfaWAVDataChunk * out, const mfaWAVFormatChunk* format, void * allocator)
{
	if (stream == NULL || out == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;

	mfError err;
	mfmU64 readSize = 0;

	err = mfmAllocate(allocator, &out->pcmData, out->header.size);
	if (err != MF_ERROR_OKAY)
		return err;

	err = mfsRead(stream, out->pcmData, out->header.size, &readSize);
	if (err != MF_ERROR_OKAY)
		return err;
	if (readSize < out->header.size)
		return MFA_ERROR_INVALID_DATA;

	// Convert to little endianness, if needed
	if (format->bitsPerSample == 16)
		for (mfmU64 i = 0; i < out->header.size; i += 2)
			mfmFromLittleEndian2((mfmU8*)out->pcmData + i, (mfmU8*)out->pcmData + i);

	return MF_ERROR_OKAY;
}
