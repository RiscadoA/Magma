#include "Deflate.h"

#include "../Memory/Allocator.h"

#include <zlib.h>
#include <stdlib.h>

typedef struct
{
	mfsStream base;
	z_stream stream;
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

}

static mfError mfmRead(void* stream, mfmU8* buffer, mfmU64 bufferSize, mfmU64* readSize)
{

}

static mfError mfmWrite(void* stream, mfmU8* buffer, mfmU64 bufferSize, mfmU64* readSize)
{

}

static mfError mfmSetBuffer(void* stream, mfmU8* buffer, mfmU64 bufferSize)
{

}

mfError mfmCreateDeflateStream(mfsStream ** outStream, mfmU8 * buffer, mfmU64 bufferSize, void * allocator)
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

	stream->base.flush = &mfmFlush;
	stream->base.read = &mfmRead;
	stream->base.write = &mfmWrite;
	stream->base.setBuffer = &mfmSetBuffer;

	stream->stream.zalloc = &mfmDeflateAllocate;
	stream->stream.zfree = &mfmDeflateDeallocate;
	stream->stream.opaque = allocator;

	deflateInit(&stream->stream, Z_BEST_COMPRESSION);

	return MF_ERROR_OKAY;
}

void mfmDestroyDeflateStream(mfsStream * stream)
{
	mfmDeflateStream* defStream = stream;

	deflateEnd(&defStream->stream);
}
