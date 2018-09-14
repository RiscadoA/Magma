#include "StringStream.h"
#include "../Memory/Allocator.h"

#include <stdlib.h>
#include <string.h>

static mfError mfsStringStreamEOF(void* stream, mfmBool* eof)
{
	if (stream == NULL || eof == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;
	mfsStringStream* str = (mfsStringStream*)stream;
	if (str->head >= str->base.bufferSize)
		*eof = MFM_TRUE;
	else
		*eof = MFM_FALSE;
	return MF_ERROR_OKAY;
}

static mfError mfsStringStreamRead(void* stream, mfmU8* data, mfmU64 size, mfmU64* readSize)
{
	if (stream == NULL || data == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;
	mfsStringStream* str = (mfsStringStream*)stream;
	
	if (str->head >= str->base.bufferSize)
	{
		str->head = str->base.bufferSize;
		if (readSize != NULL)
			*readSize = 0;
		return MFS_ERROR_EOF;
	}
	else if (str->head + size > str->base.bufferSize)
	{
		memcpy(data, str->base.buffer + str->head, str->base.bufferSize - str->head);
		if (readSize != NULL)
			*readSize = str->base.bufferSize - str->head;
		str->head = str->base.bufferSize;
		return MFS_ERROR_EOF;
	}
	else
	{
		memcpy(data, str->base.buffer + str->head, size);
		if (readSize != NULL)
			*readSize = size;
		str->head += size;
		return MF_ERROR_OKAY;
	}
}

static mfError mfsStringStreamWrite(void* stream, const mfmU8* data, mfmU64 size, mfmU64* writeSize)
{
	if (stream == NULL || data == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;
	mfsStringStream* str = (mfsStringStream*)stream;

	if (str->head >= str->base.bufferSize)
	{
		str->head = str->base.bufferSize;
		if (writeSize != NULL)
			*writeSize = 0;
		return MFS_ERROR_EOF;
	}
	else if (str->head + size > str->base.bufferSize)
	{
		memcpy(str->base.buffer + str->head, data, str->base.bufferSize - str->head);
		if (writeSize != NULL)
			*writeSize = str->base.bufferSize - str->head;
		str->head = str->base.bufferSize;
		return MFS_ERROR_EOF;
	}
	else
	{
		memcpy(str->base.buffer + str->head, data, size);
		if (writeSize != NULL)
			*writeSize = size;
		str->head += size;
		return MF_ERROR_OKAY;
	}
}

static mfError mfsStringStreamFlush(void* stream)
{
	// Do nothing
	return MF_ERROR_OKAY;
}

static mfError mfsStringStreamSetBuffer(void* stream, mfmU8* buffer, mfmU64 size)
{
	mfsStringStream* str = (mfsStringStream*)stream;
	str->head = 0;
	str->base.buffer = buffer;
	str->base.bufferSize = size;
	memset(buffer, 0, size);

	return MF_ERROR_OKAY;
}

static mfError mfsStringStreamSeekBegin(void* stream, mfmU64 position)
{
	mfsStringStream* str = (mfsStringStream*)stream;
	if (position > str->base.bufferSize)
	{
		str->head = str->base.bufferSize;
		return MFS_ERROR_EOF;
	}
	
	str->head = position;
	return MF_ERROR_OKAY;
}

static mfError mfsStringStreamSeekEnd(void* stream, mfmU64 position)
{
	mfsStringStream* str = (mfsStringStream*)stream;
	if (position > str->base.bufferSize - 1)
	{
		str->head = 0;
		return MFS_ERROR_EOF;
	}

	str->head = str->base.bufferSize - position - 1;
	return MF_ERROR_OKAY;
}

static mfError mfsStringStreamSeekHead(void* stream, mfmI64 offset)
{
	mfsStringStream* str = (mfsStringStream*)stream;
	if (offset >= 0 && str->head + offset > str->base.bufferSize)
	{
		str->head = str->base.bufferSize;
		return MFS_ERROR_EOF;
	}
	else if (offset < 0 && -offset > str->head)
	{
		str->head = 0;
		return MFS_ERROR_EOF;
	}

	str->head += offset;
	return MF_ERROR_OKAY;
}

static mfError mfsStringStreamTell(void* stream, mfmU64* out)
{
	mfsStringStream* str = (mfsStringStream*)stream;
	*out =  str->head;
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
	str->base.seekBegin = &mfsStringStreamSeekBegin;
	str->base.seekEnd = &mfsStringStreamSeekEnd;
	str->base.seekHead = &mfsStringStreamSeekHead;
	str->base.tell = &mfsStringStreamTell;
	str->base.eof = &mfsStringStreamEOF;

	str->allocator = allocator;
	if (str->allocator != NULL)
	{
		mfError err = mfmAcquireObject(((mfmObject*)str->allocator));
		if (err != MF_ERROR_OKAY)
			return err;
	}
	str->head = 0;

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
		mfError err = mfmReleaseObject((mfmObject*)str->allocator);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	if (mfmDeinitObject(&str->base.object) != MF_ERROR_OKAY)
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
	stream->base.seekBegin = &mfsStringStreamSeekBegin;
	stream->base.seekEnd = &mfsStringStreamSeekEnd;
	stream->base.seekHead = &mfsStringStreamSeekHead;
	stream->base.tell = &mfsStringStreamTell;
	stream->base.eof = &mfsStringStreamEOF;

	stream->allocator = NULL;
	stream->head = 0;

	return MF_ERROR_OKAY;
}

void mfsDestroyLocalStringStream(mfsStringStream * stream)
{
	if (stream == NULL)
		abort();
	if (mfmDeinitObject(&stream->base.object) != MF_ERROR_OKAY)
		abort();
}

mfError mfsClearStringStream(mfsStream * stream)
{
	if (stream == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;
	mfsStringStream* str = (mfsStringStream*)stream;
	str->head = 0;
	return MF_ERROR_OKAY;
}
