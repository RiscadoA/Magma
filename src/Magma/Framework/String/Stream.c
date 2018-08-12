#include "Stream.h"

#include <stdio.h>

mfsStream* mfsIn = NULL;
mfsStream* mfsOut = NULL;

static mfmU8 mfsInDefaultBuffer[512];
static mfmU8 mfsOutDefaultBuffer[512];

typedef struct
{
	mfsStream base;
	FILE* file;
} mfsFileStream;

mfsError mfsFileWrite(void* stream, const mfmU8* data, mfmU64 dataSize, mfmU64* outSize)
{
	mfsFileStream* fileStream = (mfsFileStream*)stream;

	mfmU64 size = fwrite(data, sizeof(mfmU8), dataSize, fileStream->file);
	if (outSize != NULL)
		*outSize = size;

	return MFS_ERROR_OKAY;
}

mfsError mfsFileRead(void* stream, mfmU8* data, mfmU64 dataSize, mfmU64* outSize)
{
	mfsFileStream* fileStream = (mfsFileStream*)stream;

	mfmU64 size = fread(data, sizeof(mfmU8), dataSize, fileStream->file);
	if (outSize != NULL)
		*outSize = size;

	return MFS_ERROR_OKAY;
}

mfsError mfsFileFlush(void* stream)
{
	mfsFileStream* fileStream = (mfsFileStream*)stream;
	int ret = fflush(fileStream->file);
	if (ret == EOF)
		return MFS_ERROR_EOF;
	return MFS_ERROR_OKAY;
}

mfsError mfsFileSetBuffer(void* stream, mfmU8* buffer, mfmU64 bufferSize)
{
	mfsFileStream* fileStream = (mfsFileStream*)stream;

	if (buffer != NULL)
	{
		int ret = setvbuf(fileStream->file, buffer, _IOFBF, bufferSize);
		if (ret != 0)
			return MFS_ERROR_INTERNAL;
	}
	else
	{
		int ret = setvbuf(fileStream->file, NULL, _IONBF, 0);
		if (ret != 0)
			return MFS_ERROR_INTERNAL;
	}

	return MFS_ERROR_OKAY;
}

void mfsDestroyFileStream(void* stream)
{
	free(stream);
}

mfsStream* mfsCreateFileStream(FILE* file, mfmU8* buffer, mfmU64 bufferSize)
{
	mfsFileStream* stream = (mfsFileStream*)malloc(sizeof(mfsFileStream));

	stream->base.object.destructorFunc = &mfsDestroyFileStream;

	stream->base.buffer = buffer;
	stream->base.bufferSize = bufferSize;
	stream->base.bufferHead = 0;
	stream->file = file;

	stream->base.read = &mfsFileRead;
	stream->base.write = &mfsFileWrite;
	stream->base.flush = &mfsFileFlush;
	stream->base.setBuffer = &mfsFileSetBuffer;
}

void mfsInitStream()
{
	// Initialize mfsIn stream
	mfsIn = mfsCreateFileStream(stdin, mfsInDefaultBuffer, sizeof(mfsInDefaultBuffer));
	
	// Initialize mfsOut stream
	mfsOut = mfsCreateFileStream(stdout, mfsOutDefaultBuffer, sizeof(mfsOutDefaultBuffer));
}

void mfsTerminateStream()
{
	if (mfsIn != NULL)
		mfsIn->object.destructorFunc(mfsIn);
	if (mfsOut != NULL)
		mfsOut->object.destructorFunc(mfsOut);
}

mfsError mfsWrite(mfsStream * stream, const mfmU8 * data, mfmU64 dataSize, mfmU64 * outSize)
{
	if (stream == NULL || data == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;
	if (dataSize == 0)
		return MFS_ERROR_OKAY;
	return stream->write(stream, data, dataSize, outSize);
}

mfsError mfsRead(mfsStream * stream, mfmU8 * data, mfmU64 dataSize, mfmU64 * outSize)
{
	if (stream == NULL || data == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;
	if (dataSize == 0)
		return MFS_ERROR_OKAY;
	return stream->read(stream, data, dataSize, outSize);
}

mfsError mfsFlush(mfsStream * stream)
{
	if (stream == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;
	return stream->flush(stream);
}

mfsError mfsSetBuffer(mfsStream * stream, mfmU8 * buffer, mfmU64 bufferSize)
{
	if (stream == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;

	if (stream->setBuffer == NULL)
	{
		if (stream->buffer != NULL)
		{
			if (bufferSize == 0)
				return MFS_ERROR_INVALID_ARGUMENTS;

			mfsError err = stream->flush(stream);
			if (err != MFS_ERROR_OKAY)
				return err;
		}

		stream->buffer = buffer;
		stream->bufferSize = bufferSize;
		return MFS_ERROR_OKAY;
	}
	else
		return stream->setBuffer(stream, buffer, bufferSize);
}

mfsError mfsOpenFile(mfsStream ** stream, mfmU32 mode, const mfsUTF8CodeUnit * path)
{
	if (stream == NULL || path == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;

	FILE* file;
	errno_t err;
	if (mode == MFS_FILE_READ)
		err = fopen_s(&file, path, "rb");
	else if (mode == MFS_FILE_WRITE)
		err = fopen_s(&file, path, "wb");
	else
		return MFS_ERROR_INVALID_ARGUMENTS;

	if (err != 0)
		return MFS_ERROR_INTERNAL;

	*stream = mfsCreateFileStream(file, NULL, 0);

	return MFS_ERROR_OKAY;
}

void mfsCloseFile(mfsStream * stream)
{
	fclose(((mfsFileStream*)stream)->file);
	mfsDestroyFileStream(stream);
}
