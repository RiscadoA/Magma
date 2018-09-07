#include "OALRenderDevice.h"
#include "Config.h"

#ifndef MFA_OAL_MAX_BUFFERS
#define MFA_OAL_MAX_BUFFERS	4096
#endif

#ifndef MFA_OAL_MAX_SOURCES
#define MFA_OAL_MAX_SOURCES	512
#endif

#ifndef MFA_OAL_MAX_QUEUED_BUFFERS
#define MFA_OAL_MAX_QUEUED_BUFFERS 16
#endif

#ifdef MAGMA_FRAMEWORK_USE_OPENAL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <AL/al.h>
#include <AL/alc.h>

#include "../Memory/PoolAllocator.h"

typedef struct
{
	mfaRenderDeviceObject base;
	ALuint id;
} mfaOALBuffer;

typedef struct
{
	mfaRenderDeviceObject base;
	ALuint id;
	mfaOALBuffer* m_queueBuffers[MFA_OAL_MAX_QUEUED_BUFFERS];
} mfaOALSource;

typedef struct
{
	mfaRenderDevice base;

	void* allocator;

	mfmPoolAllocator* bufferPool;
	mfmU8 bufferPoolMemory[MFM_POOL_ALLOCATOR_SIZE(MFA_OAL_MAX_BUFFERS, sizeof(mfaOALBuffer))];

	mfmPoolAllocator* sourcePool;
	mfmU8 sourcePoolMemory[MFM_POOL_ALLOCATOR_SIZE(MFA_OAL_MAX_SOURCES, sizeof(mfaOALSource))];

	mfsUTF8CodeUnit errorString[256];
	mfmU64 errorStringSize;

	ALCdevice* device;
	ALCcontext* context;
} mfaOALRenderDevice;

#define MFA_RETURN_ERROR(rd, code, msg) do {\
	mfaOALRenderDevice* oalRD = (mfaOALRenderDevice*)rd;\
	oalRD->errorStringSize = snprintf(oalRD->errorString, sizeof(oalRD->errorString),\
									 msg) + 1;\
	if (oalRD->errorStringSize > sizeof(oalRD->errorString))\
	oalRD->errorStringSize = sizeof(oalRD->errorString);\
	return code; } while(0)

#ifdef MAGMA_FRAMEWORK_DEBUG
#define MFA_CHECK_AL_ERROR(rd) do { ALenum err = alGetError();\
if (err != AL_NO_ERROR) MFA_RETURN_ERROR(rd, MFA_ERROR_INTERNAL, u8"alGetError() returned non zero on " __FUNCTION__);\
} while(0)
#else
#define MFA_CHECK_AL_ERROR(rd)
#endif

mfError mfaCreateOALRenderDevice(mfaRenderDevice ** renderDevice, void * allocator)
{
	mfError err;

	*renderDevice = NULL;

	// Check if params are valid
	if (renderDevice == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;

	// Allocate render device
	mfaOALRenderDevice* rd;
	err = mfmAllocate(allocator, &rd, sizeof(mfaOALRenderDevice));
	if (err != MF_ERROR_OKAY)
		return err;

	// Initialize properties
	err = mfmInitObject(&rd->base.object);
	if (err != MF_ERROR_OKAY)
		return err;
	rd->base.object.destructorFunc = &mfaDestroyOALRenderDevice;
	rd->allocator = allocator;
	memset(rd->errorString, '\0', sizeof(rd->errorString));
	rd->errorStringSize = 0;

	// Create buffer pool
	{
		mfmPoolAllocatorDesc desc;
		desc.expandable = MFM_FALSE;
		desc.slotCount = MFA_OAL_MAX_BUFFERS;
		desc.slotSize = sizeof(mfaOALBuffer);
		err = mfmCreatePoolAllocatorOnMemory(&rd->bufferPool, &desc, rd->bufferPoolMemory, sizeof(rd->bufferPoolMemory));
		if (err != MF_ERROR_OKAY)
		{
			mfmDeallocate(allocator, rd);
			return err;
		}
	}

	// Create source pool
	{
		mfmPoolAllocatorDesc desc;
		desc.expandable = MFM_FALSE;
		desc.slotCount = MFA_OAL_MAX_SOURCES;
		desc.slotSize = sizeof(mfaOALSource);
		err = mfmCreatePoolAllocatorOnMemory(&rd->sourcePool, &desc, rd->sourcePoolMemory, sizeof(rd->sourcePoolMemory));
		if (err != MF_ERROR_OKAY)
		{
			mfmDeallocate(allocator, rd);
			return err;
		}
	}

	// Init context
	rd->device = alcOpenDevice(NULL);
	rd->context = alcCreateContext(rd->device, NULL);
	
	// Check if context was created successfully
	{
		if (!alcMakeContextCurrent(rd->context))
		{
			mfmDeallocate(allocator, rd);
			return MFA_ERROR_INTERNAL;
		}

		ALenum alErr = alGetError();
		if (alErr != AL_NO_ERROR)
		{
			mfmDeallocate(allocator, rd);
			return MFA_ERROR_INTERNAL;
		}
	}
	
	*renderDevice = rd;

	return MF_ERROR_OKAY;
}

void mfaDestroyOALRenderDevice(void * renderDevice)
{
	mfaOALRenderDevice* rd = renderDevice;

	// Destroy context
	alcMakeContextCurrent(NULL);
	alcDestroyContext(rd->context);
	alcCloseDevice(rd->device);

	// Check if context was terminated successfully
	{
		ALenum alErr = alGetError();
		if (alErr != AL_NO_ERROR)
			abort();
	}

	// Destroy pools
	mfmDestroyPoolAllocator(rd->sourcePool);
	mfmDestroyPoolAllocator(rd->bufferPool);

	// Deallocate render device
	if (mfmDestroyObject(&rd->base.object) != MF_ERROR_OKAY)
		abort();
	if (mfmDeallocate(rd->allocator, rd) != MF_ERROR_OKAY)
		abort();
}

#else

mfError mfaCreateOALRenderDevice(mfaRenderDevice ** renderDevice, void * allocator)
{
	return MFA_ERROR_NOT_SUPPORTED;
}

void mfaDestroyOALRenderDevice(void * renderDevice)
{
	abort();
}

#endif
