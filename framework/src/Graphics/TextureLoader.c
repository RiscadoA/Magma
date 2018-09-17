#include <Magma/Framework/Graphics/TextureLoader.h>
#include <Magma/Framework/Thread/Mutex.h>
#include <Magma/Framework/Memory/Allocator.h>
#include <Magma/Framework/String/Stream.h>

#include <stdlib.h>

static void* currentAllocator = NULL;
static mftMutex* currentAllocatorMutex = NULL;

static void* mfgSTBImalloc(size_t size)
{
	void* out = NULL;
	mfError err = mfmAllocate(currentAllocator, &out, size);
	if (err != MF_ERROR_OKAY)
		abort();
	return out;
}

static void* mfgSTBIrealloc(void* mem, size_t prev, size_t next)
{
	void* out = NULL;
	mfError err = mfmReallocate(currentAllocator, mem, prev, next, &out);
	if (err != MF_ERROR_OKAY)
		abort();
	return out;
}

static void mfgSTBIfree(void* mem)
{
	if (mem == NULL)
		return;
	mfError err = mfmDeallocate(currentAllocator, mem);
	if (err != MF_ERROR_OKAY)
		abort();
}

#define STBI_MALLOC(size) mfgSTBImalloc(size)
#define STBI_REALLOC_SIZED(p,prev,next) mfgSTBIrealloc(p, prev, next)
#define STBI_FREE(p) mfgSTBIfree(p)

#define STBI_NO_GIF
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int mfgSTBIOEOF(void* user)
{
	mfmBool eof;
	mfError err = mfsEOF(user, &eof);
	if (err != MF_ERROR_OKAY)
		abort();
	return (eof == MFM_FALSE) ? 0 : 1;
}

int mfgSTBIORead(void* user, char* data, int size)
{
	mfmU64 outSize;
	mfError err = mfsRead(user, data, size, &outSize);
	if (err != MF_ERROR_OKAY)
		abort();
	return outSize;
}

int mfgSTBIOSkip(void* user, int n)
{
	mfError err = mfsSeekHead(user, n);
	if (err != MF_ERROR_OKAY)
		abort();
}

mfError mfgInitTextureLoader(void)
{
	currentAllocator = NULL;
	currentAllocatorMutex = NULL;
	mfError err = mftCreateMutex(&currentAllocatorMutex, NULL);
	return err;
}

mfError mfgTerminateTextureLoader(void)
{
	mfError err = mftDestroyMutex(currentAllocatorMutex);
	return err;
}

mfError mfgLoadTexture(void * stream, mfgTextureData * textureData, mfgEnum format, void * allocator)
{
	mfError err;

	err = mftLockMutex(currentAllocatorMutex, 0);
	if (err != MF_ERROR_OKAY)
		return err;

	currentAllocator = allocator;

	stbi_io_callbacks callbacks;

	callbacks.eof = &mfgSTBIOEOF;
	callbacks.read = &mfgSTBIORead;
	callbacks.skip = &mfgSTBIOSkip;

	int req_comp = 0;
	switch (format)
	{
		case MFG_R8SINT:
		case MFG_R8UINT:
		case MFG_R8SNORM:
		case MFG_R8UNORM:
			req_comp = 1;
			break;

		case MFG_RG8SINT:
		case MFG_RG8UINT:
		case MFG_RG8SNORM:
		case MFG_RG8UNORM:
			req_comp = 2;
			break;

		case MFG_RGBA8SINT:
		case MFG_RGBA8UINT:
		case MFG_RGBA8SNORM:
		case MFG_RGBA8UNORM:
			req_comp = 4;
			break;

		default:
			mftUnlockMutex(currentAllocatorMutex);
			return MFG_ERROR_UNSUPPORTED_TYPE;
	}

	int x, y;
	textureData->data = stbi_load_from_callbacks(&callbacks, stream, &x, &y, NULL, req_comp);
	textureData->width = x;
	textureData->height = y;
	textureData->allocator = allocator;

	err = mftUnlockMutex(currentAllocatorMutex);
	if (err != MF_ERROR_OKAY)
		return err;
}
