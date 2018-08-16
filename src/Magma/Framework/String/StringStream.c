#include "StringStream.h"
#include "../Memory/Allocator.h"

#include <stdlib.h>
#include <string.h>

typedef struct
{
	mfsStream base;
	void* allocator;
	mfmU64 writeHead;
	mfmU64 readHead;
} mfsStringStream;

mfsError mfsStringStreamRead(void* stream, mfmU8* data, mfmU64 size, mfmU64* readSize)
{
	if (stream == NULL || data == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;
	mfsStringStream* str = (mfsStringStream*)stream;
	
	if (str->readHead >= str->base.bufferSize)
	{
		str->readHead = str->base.bufferSize;
		if (readSize != NULL)
			*readSize = 0;
		return MFS_ERROR_EOF;
	}
	else if (str->readHead + size > str->base.bufferSize)
	{
		memcpy(data, str->base.buffer + str->readHead, str->base.bufferSize - str->readHead);
		if (readSize != NULL)
			*readSize = str->base.bufferSize - str->readHead;
		str->readHead = str->base.bufferSize;
		return MFS_ERROR_EOF;
	}
	else
	{
		memcpy(data, str->base.buffer + str->readHead, size);
		if (readSize != NULL)
			*readSize = size;
		str->readHead += size;
		return MFS_ERROR_OKAY;
	}
}

mfsError mfsStringStreamWrite(void* stream, const mfmU8* data, mfmU64 size, mfmU64* writeSize)
{
	if (stream == NULL || data == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;
	mfsStringStream* str = (mfsStringStream*)stream;

	if (str->writeHead >= str->base.bufferSize)
	{
		str->writeHead = str->base.bufferSize;
		if (writeSize != NULL)
			*writeSize = 0;
		return MFS_ERROR_EOF;
	}
	else if (str->writeHead + size > str->base.bufferSize)
	{
		memcpy(data, str->base.buffer + str->writeHead, str->base.bufferSize - str->writeHead);
		if (writeSize != NULL)
			*writeSize = str->base.bufferSize - str->writeHead;
		str->writeHead = str->base.bufferSize;
		return MFS_ERROR_EOF;
	}
	else
	{
		memcpy(data, str->base.buffer + str->writeHead, size);
		if (writeSize != NULL)
			*writeSize = size;
		str->writeHead += size;
		return MFS_ERROR_OKAY;
	}
}

mfsError mfsStringStreamFlush(void* stream)
{
	// Do nothing
}

mfsError mfsStringStreamSetBuffer(void* stream, mfmU8* buffer, mfmU64 size)
{
	mfsStringStream* str = (mfsStringStream*)stream;
	str->readHead = 0;
	str->writeHead = 0;
	str->base.buffer = buffer;
	str->base.bufferSize = size;
}

mfsError mfsCreateStringStream(mfsStream ** stream, mfmU8 * buffer, mfmU64 size, void * allocator)
{
	mfsStringStream* str = NULL;
	if (mfmAllocate(allocator, &str, sizeof(mfsStream)) != MFM_ERROR_OKAY)
		return MFM_ERROR_ALLOCATION_FAILED;

	str->base.object.destructorFunc = &mfsDestroyStringStream;
	str->base.object.referenceCount = 0;

	str->base.buffer = buffer;
	str->base.bufferSize = size;

	str->base.read = &mfsStringStreamRead;
	str->base.write = &mfsStringStreamWrite;
	str->base.flush = &mfsStringStreamFlush;
	str->base.setBuffer = &mfsStringStreamSetBuffer;

	str->allocator = allocator;
	str->writeHead = 0;
	str->readHead = 0;

	*stream = str;
}

void mfsDestroyStringStream(mfsStream * stream)
{
	if (stream == NULL)
		abort();
	mfsStringStream* str = (mfsStringStream*)stream;
	if (mfmDeallocate(str->allocator, str) != MFM_ERROR_OKAY)
		abort();
}

mfsError mfsClearStringStream(mfsStream * stream)
{
	if (stream == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;
	mfsStringStream* str = (mfsStringStream*)stream;
	str->writeHead = 0;
	str->readHead = 0;
	return MFS_ERROR_OKAY;
}
