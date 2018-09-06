#include "StringStream.h"
#include "../Memory/Allocator.h"

#include <stdlib.h>
#include <string.h>

static mfError mfsStringStreamEOF(void* stream, mfmBool* eof)
{
	if (stream == NULL || eof == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;
	mfsStringStream* str = (mfsStringStream*)stream;
	if (str->readHead >= str->base.bufferSize)
		*eof = MFM_TRUE;
	else
		*eof = MFM_FALSE;
	return MF_ERROR_OKAY;
}

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
		return MF_ERROR_OKAY;
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
		return MF_ERROR_OKAY;
	}
}

mfError mfsStringStreamFlush(void* stream)
{
	// Do nothing
	return MF_ERROR_OKAY;
}

mfError mfsStringStreamSetBuffer(void* stream, mfmU8* buffer, mfmU64 size)
{
	mfsStringStream* str = (mfsStringStream*)stream;
	str->readHead = 0;
	str->writeHead = 0;
	str->base.buffer = buffer;
	str->base.bufferSize = size;
	memset(buffer, 0, size);

	return MF_ERROR_OKAY;
}

mfError mfsCreateStringStream(mfsStream ** stream, mfmU8 * buffer, mfmU64 size, void * allocator)
{
	mfsStringStream* str = NULL;
	if (mfmAllocate(allocator, &str, sizeof(mfsStringStream)) != MF_ERROR_OKAY)
		return MFM_ERROR_ALLOCATION_FAILED;

	{
		mfError err = mfmInitObject(&str->base.object);
		if (err != MF_ERROR_OKAY)
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
	str->base.seekBegin = NULL;
	str->base.seekEnd = NULL;
	str->base.seekHead = NULL;
	str->base.tell = NULL;
	str->base.eof = &mfsStringStreamEOF;

	str->allocator = allocator;
	if (str->allocator != NULL)
	{
		mfError err = mfmIncObjectRef(((mfmObject*)str->allocator));
		if (err != MF_ERROR_OKAY)
			return err;
	}
	str->writeHead = 0;
	str->readHead = 0;

	*stream = str;

	return MF_ERROR_OKAY;
}

void mfsDestroyStringStream(mfsStream * stream)
{
	if (stream == NULL)
		abort();
	mfsStringStream* str = (mfsStringStream*)stream;
	if (str->allocator != NULL)
	{
		mfError err = mfmDecObjectRef((mfmObject*)str->allocator);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	if (mfmDestroyObject(&str->base.object) != MF_ERROR_OKAY)
		abort();
	if (mfmDeallocate(str->allocator, str) != MF_ERROR_OKAY)
		abort();
}

mfError mfsCreateLocalStringStream(mfsStringStream * stream, mfmU8 * buffer, mfmU64 size)
{
	{
		mfError err = mfmInitObject(&stream->base.object);
		if (err != MF_ERROR_OKAY)
			return err;
	}

	stream->base.object.destructorFunc = &mfsDestroyLocalStringStream;

	stream->base.buffer = buffer;
	stream->base.bufferSize = size;
	memset(buffer, 0, size);

	stream->base.read = &mfsStringStreamRead;
	stream->base.write = &mfsStringStreamWrite;
	stream->base.flush = &mfsStringStreamFlush;
	stream->base.setBuffer = &mfsStringStreamSetBuffer;
	stream->base.seekBegin = NULL;
	stream->base.seekEnd = NULL;
	stream->base.seekHead = NULL;
	stream->base.tell = NULL;
	stream->base.eof = &mfsStringStreamEOF;

	stream->allocator = NULL;
	stream->writeHead = 0;
	stream->readHead = 0;

	return MF_ERROR_OKAY;
}

void mfsDestroyLocalStringStream(mfsStringStream * stream)
{
	if (stream == NULL)
		abort();
	if (mfmDestroyObject(&stream->base.object) != MF_ERROR_OKAY)
		abort();
}

mfError mfsClearStringStream(mfsStream * stream)
{
	if (stream == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;
	mfsStringStream* str = (mfsStringStream*)stream;
	str->writeHead = 0;
	str->readHead = 0;
	return MF_ERROR_OKAY;
}
