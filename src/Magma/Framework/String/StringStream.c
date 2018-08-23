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

mfError mfsStringStreamRead(void* stream, mfmU8* data, mfmU64 size, mfmU64* readSize)
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

mfError mfsStringStreamWrite(void* stream, const mfmU8* data, mfmU64 size, mfmU64* writeSize)
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
		memcpy(str->base.buffer + str->writeHead, data, str->base.bufferSize - str->writeHead);
		if (writeSize != NULL)
			*writeSize = str->base.bufferSize - str->writeHead;
		str->writeHead = str->base.bufferSize;
		return MFS_ERROR_EOF;
	}
	else
	{
		memcpy(str->base.buffer + str->writeHead, data, size);
		if (writeSize != NULL)
			*writeSize = size;
		str->writeHead += size;
		return MFS_ERROR_OKAY;
	}
}

mfError mfsStringStreamFlush(void* stream)
{
	// Do nothing
	return MFS_ERROR_OKAY;
}

mfError mfsStringStreamSetBuffer(void* stream, mfmU8* buffer, mfmU64 size)
{
	mfsStringStream* str = (mfsStringStream*)stream;
	str->readHead = 0;
	str->writeHead = 0;
	str->base.buffer = buffer;
	str->base.bufferSize = size;
	memset(buffer, 0, size);

	return MFS_ERROR_OKAY;
}

mfError mfsCreateStringStream(mfsStream ** stream, mfmU8 * buffer, mfmU64 size, void * allocator)
{
	mfsStringStream* str = NULL;
	if (mfmAllocate(allocator, &str, sizeof(mfsStringStream)) != MFM_ERROR_OKAY)
		return MFM_ERROR_ALLOCATION_FAILED;

	{
		mfError err = mfmInitObject(&str->base.object);
		if (err != MFM_ERROR_OKAY)
			return err;
	}
	str->base.object.destructorFunc = &mfsDestroyStringStream;

	str->base.buffer = buffer;
	str->base.bufferSize = size;
	memset(buffer, 0, size);

	str->base.read = &mfsStringStreamRead;
	str->base.write = &mfsStringStreamWrite;
	str->base.flush = &mfsStringStreamFlush;
	str->base.setBuffer = &mfsStringStreamSetBuffer;

	str->allocator = allocator;
	if (str->allocator != NULL)
		++((mfmObject*)str->allocator)->referenceCount;
	str->writeHead = 0;
	str->readHead = 0;

	*stream = str;

	return MFS_ERROR_OKAY;
}

void mfsDestroyStringStream(mfsStream * stream)
{
	if (stream == NULL)
		abort();
	mfsStringStream* str = (mfsStringStream*)stream;
	if (str->allocator != NULL)
		--((mfmObject*)str->allocator)->referenceCount;
	if (mfmDestroyObject(&str->base.object) != MFM_ERROR_OKAY)
		abort();
	if (mfmDeallocate(str->allocator, str) != MFM_ERROR_OKAY)
		abort();
}

mfError mfsClearStringStream(mfsStream * stream)
{
	if (stream == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;
	mfsStringStream* str = (mfsStringStream*)stream;
	str->writeHead = 0;
	str->readHead = 0;
	return MFS_ERROR_OKAY;
}
