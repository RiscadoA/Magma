#include "Deflate.h"

#include "../Memory/Allocator.h"

#include <zlib.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
	mfsStream base;
	z_stream stream;
	mfmU64 bufferHead;
} mfmDeflateStream;

static voidpf mfmDeflateAllocate(voidpf opaque, uInt items, uInt size)
{
	voidpf address;
	mfError err = mfmAllocate(opaque, &address, items * size);
	if (err != MF_ERROR_OKAY)
		return NULL;
	return address;
}

static void mfmDeflateDeallocate(voidpf opaque, voidpf address)
{
	mfError err = mfmDeallocate(opaque, address);
	if (err != MF_ERROR_OKAY)
		abort();
}

static mfError mfmFlush(void* stream)
{
	return MF_ERROR_OKAY;
}

static mfError mfmRead(void* stream, mfmU8* buffer, mfmU64 bufferSize, mfmU64* readSize)
{
	mfmDeflateStream* defStream = stream;

	if (bufferSize >= defStream->bufferHead)
	{
		memcpy(buffer, defStream->base.buffer, defStream->bufferHead);
		if (readSize != NULL)
			*readSize = defStream->bufferHead;
		defStream->bufferHead = 0;
	}
	else
	{
		memcpy(buffer, defStream->base.buffer, bufferSize);
		memcpy(defStream->base.buffer, defStream->base.buffer + bufferSize, defStream->bufferHead - bufferSize);
		defStream->bufferHead -= bufferSize;
		if (readSize != NULL)
			*readSize = bufferSize;
	}

	return MF_ERROR_OKAY;
}

static mfError mfmWrite(void* stream, const mfmU8* buffer, mfmU64 bufferSize, mfmU64* writeSize)
{
	mfmDeflateStream* defStream = stream;
	defStream->stream.avail_in = bufferSize;
	defStream->stream.next_in = buffer;
	defStream->stream.avail_out = defStream->base.bufferSize - defStream->bufferHead;
	defStream->stream.next_out = defStream->base.buffer + defStream->bufferHead;

	if (defStream->stream.avail_out == 0)
		return MFM_ERROR_BUFFER_FULL;

	int err = deflate(&defStream->stream, Z_FULL_FLUSH);
	if (err != Z_OK)
		return MFM_ERROR_INTERNAL;
	defStream->bufferHead += (defStream->base.bufferSize - defStream->bufferHead) - defStream->stream.avail_out;

	if (writeSize != NULL)
		*writeSize = bufferSize - defStream->stream.avail_in;

	return MF_ERROR_OKAY;
}

static mfError mfmSetBuffer(void* stream, mfmU8* buffer, mfmU64 bufferSize)
{
	mfmDeflateStream* defStream = stream;

	if (defStream->bufferHead != 0)
		return MFM_ERROR_BUFFER_FULL;

	defStream->base.buffer = buffer;
	defStream->base.bufferSize = bufferSize;

	return MF_ERROR_OKAY;
}

mfError mfmCreateDeflateStream(mfsStream ** outStream, mfmU8 * buffer, mfmU64 bufferSize, mfmEnum compression, void * allocator)
{
	mfError err;

	mfmDeflateStream* stream;

	err = mfmAllocate(allocator, &stream, sizeof(mfmDeflateStream));
	if (err != MF_ERROR_OKAY)
		return err;

	err = mfmInitObject(&stream->base.object);
	if (err != MF_ERROR_OKAY)
		return err;
	stream->base.object.destructorFunc = &mfmDestroyDeflateStream;

	stream->base.buffer = buffer;
	stream->base.bufferSize = bufferSize;
	stream->bufferHead = 0;

	stream->base.flush = &mfmFlush;
	stream->base.read = &mfmRead;
	stream->base.write = &mfmWrite;
	stream->base.setBuffer = &mfmSetBuffer;
	stream->base.seekBegin = NULL;
	stream->base.seekEnd = NULL;
	stream->base.seekHead = NULL;
	stream->base.tell = NULL;

	stream->stream.zalloc = &mfmDeflateAllocate;
	stream->stream.zfree = &mfmDeflateDeallocate;
	stream->stream.opaque = allocator;

	if (compression == MFM_BEST_COMPRESSION)
	{
		if (deflateInit(&stream->stream, Z_BEST_COMPRESSION) != Z_OK)
			return MFM_ERROR_INTERNAL;
	}
	else if (compression == MFM_FAST_COMPRESSION)
	{
		if (deflateInit(&stream->stream, Z_BEST_SPEED) != Z_OK)
			return MFM_ERROR_INTERNAL;
	}
	else return MFM_ERROR_INVALID_ARGUMENTS;

	*outStream = stream;

	return MF_ERROR_OKAY;
}

void mfmDestroyDeflateStream(mfsStream * stream)
{
	mfmDeflateStream* defStream = stream;

	deflateEnd(&defStream->stream);
}
