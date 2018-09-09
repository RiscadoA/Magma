#include "OALRenderDevice.h"
#include "Config.h"

#include <errno.h>

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
	mfaOALBuffer* queueBuffers[MFA_OAL_MAX_QUEUED_BUFFERS];
	mfmU64 firstSlot;
	mfmU64 nextSlot;
	mfaOALBuffer* boundBuffer;
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
#define MFA_CHECK_AL_ERROR_ABORT() do { ALenum err = alGetError();\
if (err != AL_NO_ERROR) abort();\
} while(0)
#else
#define MFA_CHECK_AL_ERROR(rd)
#endif

static void mfaOALDestroyBuffer(void* buf);

static mfError mfaOALCreateBuffer(mfaRenderDevice* rd, mfaBuffer** buf, const void* data, mfmU64 size, mfaEnum format, mfmU64 frequency)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	if (buf == NULL)
		MFA_RETURN_ERROR(rd, MFA_ERROR_INVALID_ARGUMENTS, u8"Out buffer handle pointer is NULL");
#endif

	mfaOALRenderDevice* oalRD = rd;
	mfError err;

	// Allocate buffer
	mfaOALBuffer* oalBuf;
	err = mfmAllocate(oalRD->bufferPool, &oalBuf, sizeof(mfaOALBuffer));
	if (err != MF_ERROR_OKAY)
		return err;
	
	// Init buffer
	err = mfmInitObject(&oalBuf->base.object);
	if (err != MF_ERROR_OKAY)
	{
		mfmDeallocate(oalRD->bufferPool, oalBuf);
		return err;
	}
	oalBuf->base.object.destructorFunc = &mfaOALDestroyBuffer;
	oalBuf->base.renderDevice = rd;

	// Generate buffer
	alGenBuffers(1, &oalBuf->id);

	// If has initial data
	if (data != NULL)
	{
		err = mfaOALUpdateBuffer(rd, oalBuf, data, size, format, frequency);
		if (err != MF_ERROR_OKAY)
		{
			mfmDeallocate(oalRD->bufferPool, oalBuf);
			return err;
		}
	}

	*buf = oalBuf;
	return MF_ERROR_OKAY;
}

static void mfaOALDestroyBuffer(void* buf)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (buf == NULL)
		abort();
#endif

	mfaOALBuffer* oalBuf = buf;
	mfError err;

	// Deinit source
	err = mfmDestroyObject(&oalBuf->base.object);
	if (err != MF_ERROR_OKAY)
		return abort();

	// Delete buffer
	alDeleteBuffers(1, &oalBuf);

	// Deallocate buffer
	err = mfmDeallocate(((mfaOALRenderDevice*)oalBuf->base.renderDevice)->bufferPool, oalBuf);
	if (err != MF_ERROR_OKAY)
		return abort();

	MFA_CHECK_AL_ERROR_ABORT();
}

static mfError mfaOALUpdateBuffer(mfaRenderDevice* rd, mfaBuffer* buf, const void* data, mfmU64 size, mfaEnum format, mfmU64 frequency)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	if (buf == NULL)
		MFA_RETURN_ERROR(rd, MFA_ERROR_INVALID_ARGUMENTS, u8"Buffer handle is NULL");
	if (data == NULL)
		MFA_RETURN_ERROR(rd, MFA_ERROR_INVALID_ARGUMENTS, u8"Data is NULL");
#endif

	ALenum al_format;

	switch (format)
	{
		case MFA_MONO8: al_format = AL_FORMAT_MONO8; break;
		case MFA_STEREO8: al_format = AL_FORMAT_STEREO8; break;
		case MFA_MONO16: al_format = AL_FORMAT_MONO16; break;
		case MFA_STEREO16: al_format = AL_FORMAT_STEREO16; break;
		default:
			MFA_RETURN_ERROR(rd, MFA_ERROR_INVALID_ARGUMENTS, "Invalid/unsupported audio format");
			break;
	}

	alBufferData(((mfaOALBuffer*)buf)->id, al_format, data, size, frequency);

	MFA_CHECK_AL_ERROR(rd);
	return MF_ERROR_OKAY;
}

static void mfaOALDestroySource(void* source);

static mfError mfaOALCreateSource(mfaRenderDevice* rd, mfaSource** source)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	if (source == NULL)
		MFA_RETURN_ERROR(rd, MFA_ERROR_INVALID_ARGUMENTS, u8"Out source handle pointer is NULL");
#endif

	mfaOALRenderDevice* oalRD = rd;
	mfError err;

	// Allocate source
	mfaOALSource* oalSource;
	err = mfmAllocate(oalRD->sourcePool, &oalSource, sizeof(mfaOALSource));
	if (err != MF_ERROR_OKAY)
		return err;

	// Init buffer
	err = mfmInitObject(&oalSource->base.object);
	if (err != MF_ERROR_OKAY)
	{
		mfmDeallocate(oalRD->sourcePool, oalSource);
		return err;
	}
	oalSource->base.object.destructorFunc = &mfaOALDestroySource;
	oalSource->base.renderDevice = rd;
	oalSource->boundBuffer = NULL;

	// Generate buffer
	alGenSources(1, &oalSource->id);

	// Init queue
	oalSource->firstSlot = 0;
	oalSource->nextSlot = 0;
	for (mfmU64 i = 0; i < MFA_OAL_MAX_QUEUED_BUFFERS; ++i)
		oalSource->queueBuffers[i] = NULL;

	*source = oalSource;
	return MF_ERROR_OKAY;
}

static void mfaOALDestroySource(void* source)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (source == NULL)
		abort();
#endif

	mfaOALSource* oalSource = source;
	mfError err;

	for (mfmU32 i = 0; i < MFA_OAL_MAX_QUEUED_BUFFERS; ++i)
		if (oalSource->queueBuffers[i] != NULL && mfmDecObjectRef(oalSource->queueBuffers[i]) != MF_ERROR_OKAY)
			abort();

	if (oalSource->boundBuffer != NULL)
		if (mfmDecObjectRef(oalSource->boundBuffer) != MF_ERROR_OKAY)
			abort();

	// Deinit source
	err = mfmDestroyObject(&oalSource->base.object);
	if (err != MF_ERROR_OKAY)
		return abort();

	// Delete source
	alDeleteSources(1, &oalSource->id);

	// Deallocate source
	err = mfmDeallocate(((mfaOALRenderDevice*)oalSource->base.renderDevice)->sourcePool, oalSource);
	if (err != MF_ERROR_OKAY)
		return abort();

	MFA_CHECK_AL_ERROR_ABORT();
}

static mfError mfaOALPlaySource(mfaRenderDevice* rd, mfaSource* source)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	if (source == NULL)
		MFA_RETURN_ERROR(rd, MFA_ERROR_INVALID_ARGUMENTS, u8"Handle is NULL");
#endif

	mfaOALSource* oalSource = source;
	alSourcePlay(oalSource->id);
	MFA_CHECK_AL_ERROR(rd);
	return MF_ERROR_OKAY;
}

static mfError mfaOALStopSource(mfaRenderDevice* rd, mfaSource* source)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	if (source == NULL)
		MFA_RETURN_ERROR(rd, MFA_ERROR_INVALID_ARGUMENTS, u8"Handle is NULL");
#endif

	mfaOALSource* oalSource = source;
	alSourceStop(oalSource->id);
	MFA_CHECK_AL_ERROR(rd);
	return MF_ERROR_OKAY;
}

static mfError mfaOALRewindSource(mfaRenderDevice* rd, mfaSource* source)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	if (source == NULL)
		MFA_RETURN_ERROR(rd, MFA_ERROR_INVALID_ARGUMENTS, u8"Handle is NULL");
#endif

	mfaOALSource* oalSource = source;
	alSourceRewind(oalSource->id);
	MFA_CHECK_AL_ERROR(rd);
	return MF_ERROR_OKAY;
}

static mfError mfaOALPauseSource(mfaRenderDevice* rd, mfaSource* source)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	if (source == NULL)
		MFA_RETURN_ERROR(rd, MFA_ERROR_INVALID_ARGUMENTS, u8"Handle is NULL");
#endif

	mfaOALSource* oalSource = source;
	alSourcePause(oalSource->id);
	MFA_CHECK_AL_ERROR(rd);
	return MF_ERROR_OKAY;
}

static mfError mfaOALIsSourcePlaying(mfaRenderDevice* rd, mfaSource* source, mfmBool* out)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	if (source == NULL)
		MFA_RETURN_ERROR(rd, MFA_ERROR_INVALID_ARGUMENTS, u8"Handle is NULL");
	if (out == NULL)
		MFA_RETURN_ERROR(rd, MFA_ERROR_INVALID_ARGUMENTS, u8"Out is NULL");
#endif

	mfaOALSource* oalSource = source;
	ALint ret;
	alGetSourcei(oalSource->id, AL_SOURCE_STATE, &ret);
	MFA_CHECK_AL_ERROR(rd);

	*out = (ret == AL_PLAYING) ? MFM_TRUE : MFM_FALSE;
	return MF_ERROR_OKAY;
}

static mfError mfaOALGetSourceProcessedBuffers(mfaRenderDevice* rd, mfaSource* source, mfmU64* out)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	if (source == NULL)
		MFA_RETURN_ERROR(rd, MFA_ERROR_INVALID_ARGUMENTS, u8"Handle is NULL");
	if (out == NULL)
		MFA_RETURN_ERROR(rd, MFA_ERROR_INVALID_ARGUMENTS, u8"Out is NULL");
#endif

	mfaOALSource* oalSource = source;
	ALint ret;
	alGetSourcei(oalSource->id, AL_BUFFERS_PROCESSED, &ret);
	MFA_CHECK_AL_ERROR(rd);

	*out = ret;
	return MF_ERROR_OKAY;
}

static mfError mfaOALSourceQueueBuffer(mfaRenderDevice* rd, mfaSource* source, mfaBuffer* buffer)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	if (source == NULL)
		MFA_RETURN_ERROR(rd, MFA_ERROR_INVALID_ARGUMENTS, u8"Handle is NULL");
	if (buffer == NULL)
		MFA_RETURN_ERROR(rd, MFA_ERROR_INVALID_ARGUMENTS, u8"Buffer is NULL");
#endif

	mfaOALSource* oalSource = source;
	mfaOALBuffer* oalBuffer = buffer;
	ALint ret;
	if (oalSource->queueBuffers[oalSource->nextSlot] != NULL)
		MFA_RETURN_ERROR(rd, MFA_ERROR_TOO_MANY_QUEUED_BUFFERS, u8"Queued buffer limit reached");
	alSourceQueueBuffers(oalSource->id, 1, &oalBuffer->id);
	oalSource->queueBuffers[oalSource->nextSlot] = buffer;
	++oalSource->nextSlot;
	if (oalSource->nextSlot >= MFA_OAL_MAX_QUEUED_BUFFERS)
		oalSource->nextSlot = 0;
	MFA_CHECK_AL_ERROR(rd);
	return MF_ERROR_OKAY;
}

static mfError mfaOALSourceUnqueueBuffer(mfaRenderDevice* rd, mfaSource* source, mfaBuffer** buffer)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	if (source == NULL)
		MFA_RETURN_ERROR(rd, MFA_ERROR_INVALID_ARGUMENTS, u8"Handle is NULL");
	if (buffer == NULL)
		MFA_RETURN_ERROR(rd, MFA_ERROR_INVALID_ARGUMENTS, u8"Out buffer is NULL");
#endif

	mfaOALSource* oalSource = source;
	ALuint bufId;
	alSourceUnqueueBuffers(oalSource->id, 1, &bufId);

	if (oalSource->queueBuffers[oalSource->firstSlot] == NULL)
		MFA_RETURN_ERROR(rd, MFA_ERROR_NO_QUEUED_BUFFERS, u8"No buffers queued");
	*buffer = oalSource->queueBuffers[oalSource->firstSlot];
	oalSource->queueBuffers[oalSource->firstSlot] = NULL;
	++oalSource->firstSlot;
	if (oalSource->firstSlot >= MFA_MAX_QUEUED_BUFFERS)
		oalSource->firstSlot = 0;

	if (bufId != ((mfaOALBuffer*)*buffer)->id)
		MFA_RETURN_ERROR(rd, MFA_ERROR_INTERNAL, u8"Unqueued buffer ID doesn't match to buffer stored on queue");

	MFA_CHECK_AL_ERROR(rd);
	return MF_ERROR_OKAY;
}

static mfError mfaOALSetSourcePosition(mfaRenderDevice* rd, mfaSource* source, mfmF32 x, mfmF32 y, mfmF32 z)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	if (source == NULL)
		MFA_RETURN_ERROR(rd, MFA_ERROR_INVALID_ARGUMENTS, u8"Handle is NULL");
#endif

	mfaOALSource* oalSource = source;
	alSource3f(oalSource->id, AL_POSITION, x, y, z);
	MFA_CHECK_AL_ERROR(rd);
	return MF_ERROR_OKAY;
}

static mfError mfaOALSetSourceVelocity(mfaRenderDevice* rd, mfaSource* source, mfmF32 x, mfmF32 y, mfmF32 z)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	if (source == NULL)
		MFA_RETURN_ERROR(rd, MFA_ERROR_INVALID_ARGUMENTS, u8"Handle is NULL");
#endif

	mfaOALSource* oalSource = source;
	alSource3f(oalSource->id, AL_VELOCITY, x, y, z);
	MFA_CHECK_AL_ERROR(rd);
	return MF_ERROR_OKAY;
}

static mfError mfaOALSetSourceDirection(mfaRenderDevice* rd, mfaSource* source, mfmF32 x, mfmF32 y, mfmF32 z)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	if (source == NULL)
		MFA_RETURN_ERROR(rd, MFA_ERROR_INVALID_ARGUMENTS, u8"Handle is NULL");
#endif

	mfaOALSource* oalSource = source;
	alSource3f(oalSource->id, AL_DIRECTION, x, y, z);
	MFA_CHECK_AL_ERROR(rd);
	return MF_ERROR_OKAY;
}

static mfError mfaOALSetSourcePitch(mfaRenderDevice* rd, mfaSource* source, mfmF32 pitch)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	if (source == NULL)
		MFA_RETURN_ERROR(rd, MFA_ERROR_INVALID_ARGUMENTS, u8"Handle is NULL");
#endif

	mfaOALSource* oalSource = source;
	alSourcef(oalSource->id, AL_PITCH, pitch);
	MFA_CHECK_AL_ERROR(rd);
	return MF_ERROR_OKAY;
}

static mfError mfaOALSetSourceGain(mfaRenderDevice* rd, mfaSource* source, mfmF32 gain)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	if (source == NULL)
		MFA_RETURN_ERROR(rd, MFA_ERROR_INVALID_ARGUMENTS, u8"Handle is NULL");
#endif

	mfaOALSource* oalSource = source;
	alSourcef(oalSource->id, AL_GAIN, gain);
	MFA_CHECK_AL_ERROR(rd);
	return MF_ERROR_OKAY;
}

static mfError mfaOALSetSourceMaxDistance(mfaRenderDevice* rd, mfaSource* source, mfmF32 maxDistance)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	if (source == NULL)
		MFA_RETURN_ERROR(rd, MFA_ERROR_INVALID_ARGUMENTS, u8"Handle is NULL");
#endif

	mfaOALSource* oalSource = source;
	alSourcef(oalSource->id, AL_MAX_DISTANCE, maxDistance);
	MFA_CHECK_AL_ERROR(rd);
	return MF_ERROR_OKAY;
}

static mfError mfaOALSetSourceSecondsOffset(mfaRenderDevice* rd, mfaSource* source, mfmF32 position)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	if (source == NULL)
		MFA_RETURN_ERROR(rd, MFA_ERROR_INVALID_ARGUMENTS, u8"Handle is NULL");
#endif

	mfaOALSource* oalSource = source;
	alSourcef(oalSource->id, AL_SEC_OFFSET, position);
	MFA_CHECK_AL_ERROR(rd);
	return MF_ERROR_OKAY;
}

static mfError mfaOALSetSourceSamplesOffset(mfaRenderDevice* rd, mfaSource* source, mfmU64 position)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	if (source == NULL)
		MFA_RETURN_ERROR(rd, MFA_ERROR_INVALID_ARGUMENTS, u8"Handle is NULL");
#endif

	mfaOALSource* oalSource = source;
	alSourcei(oalSource->id, AL_SAMPLE_OFFSET, position);
	MFA_CHECK_AL_ERROR(rd);
	return MF_ERROR_OKAY;
}

static mfError mfaOALSetSourceBytesOffset(mfaRenderDevice* rd, mfaSource* source, mfmU64 position)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	if (source == NULL)
		MFA_RETURN_ERROR(rd, MFA_ERROR_INVALID_ARGUMENTS, u8"Handle is NULL");
#endif

	mfaOALSource* oalSource = source;
	alSourcei(oalSource->id, AL_BYTE_OFFSET, position);
	MFA_CHECK_AL_ERROR(rd);
	return MF_ERROR_OKAY;
}

static mfError mfaOALSetSourceLooping(mfaRenderDevice* rd, mfaSource* source, mfmBool looping)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	if (source == NULL)
		MFA_RETURN_ERROR(rd, MFA_ERROR_INVALID_ARGUMENTS, u8"Handle is NULL");
#endif

	mfaOALSource* oalSource = source;
	alSourcei(oalSource->id, AL_LOOPING, (looping == MFM_FALSE) ? AL_FALSE : AL_TRUE);
	MFA_CHECK_AL_ERROR(rd);
	return MF_ERROR_OKAY;
}

static mfError mfaOALSetSourceBuffer(mfaRenderDevice* rd, mfaSource* source, mfaBuffer* buffer)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	if (source == NULL)
		MFA_RETURN_ERROR(rd, MFA_ERROR_INVALID_ARGUMENTS, u8"Source handle is NULL");
#endif

	mfError err;
	mfaOALSource* oalSource = source;
	mfaOALBuffer* oalBuffer = buffer;

	if (oalSource->boundBuffer != NULL)
	{
		err = mfmDecObjectRef(oalSource->boundBuffer);
		if (err != MF_ERROR_OKAY)
			return err;
	}

	if (oalBuffer == NULL)
	{
		oalSource->boundBuffer = NULL;
		alSourcei(oalSource->id, AL_BUFFER, 0);
	}
	else
	{
		oalSource->boundBuffer = oalBuffer;
		err = mfmIncObjectRef(oalSource->boundBuffer);
		if (err != MF_ERROR_OKAY)
			return err;
		alSourcei(oalSource->id, AL_BUFFER, oalBuffer->id);
	}
	MFA_CHECK_AL_ERROR(rd);
	return MF_ERROR_OKAY;
}

static mfError mfaOALSetListenerGain(mfaRenderDevice* rd, mfmF32 gain)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
#endif

	alListenerf(AL_GAIN, gain);
	MFA_CHECK_AL_ERROR(rd);
	return MF_ERROR_OKAY;
}

static mfError mfaOALSetListenerPosition(mfaRenderDevice* rd, mfmF32 x, mfmF32 y, mfmF32 z)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
#endif

	alListener3f(AL_POSITION, x, y, z);
	MFA_CHECK_AL_ERROR(rd);
	return MF_ERROR_OKAY;
}

static mfError mfaOALSetListenerVelocity(mfaRenderDevice* rd, mfmF32 x, mfmF32 y, mfmF32 z)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
#endif

	alListener3f(AL_VELOCITY, x, y, z);
	MFA_CHECK_AL_ERROR(rd);
	return MF_ERROR_OKAY;
}

static mfError mfaOALSetListenerOrientation(mfaRenderDevice* rd, mfmF32 atX, mfmF32 atY, mfmF32 atZ, mfmF32 upX, mfmF32 upY, mfmF32 upZ)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
#endif

	float params[] =
	{
		atX, atY, atZ,
		upX, upY, upZ,
	};
	alListenerfv(AL_ORIENTATION, params);
	MFA_CHECK_AL_ERROR(rd);
	return MF_ERROR_OKAY;
}

static mfError mfaOALGetPropertyI(mfaRenderDevice* rd, mfaEnum id, mfmI32* value)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || value == NULL) return MFA_ERROR_INVALID_ARGUMENTS;
#endif

	if (id == MFA_MAX_BUFFERS)
		*value = MFA_OAL_MAX_BUFFERS;
	else if (id == MFA_MAX_SOURCES)
		*value = MFA_OAL_MAX_SOURCES;
	else if (id == MFA_MAX_QUEUED_BUFFERS)
		*value = MFA_OAL_MAX_QUEUED_BUFFERS;
	else
		MFA_RETURN_ERROR(rd, MFA_ERROR_INVALID_ARGUMENTS, u8"Unsupported property ID");

	return MF_ERROR_OKAY;
}

static mfError mfaOALGetPropertyF(mfaRenderDevice* rd, mfaEnum id, mfmF32* value)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (rd == NULL || value == NULL) return MFA_ERROR_INVALID_ARGUMENTS;
#endif

	if (id == MFA_MAX_BUFFERS)
		*value = MFA_OAL_MAX_BUFFERS;
	else if (id == MFA_MAX_SOURCES)
		*value = MFA_OAL_MAX_SOURCES;
	else if (id == MFA_MAX_QUEUED_BUFFERS)
		*value = MFA_OAL_MAX_QUEUED_BUFFERS;
	else
		MFA_RETURN_ERROR(rd, MFA_ERROR_INVALID_ARGUMENTS, u8"Unsupported property ID");

	return MF_ERROR_OKAY;
}

static mfmBool mfaOALGetErrorString(mfaRenderDevice* rd, mfsUTF8CodeUnit* str, mfmU64 maxSize)
{
	if (rd == NULL || str == NULL || maxSize == 0)
		abort();

	mfaOALRenderDevice* oalRD = (mfaRenderDevice*)rd;

	if (oalRD->errorStringSize == 0)
	{
		str[0] = '\0';
		return MFM_FALSE;
	}

	if (maxSize >= oalRD->errorStringSize)
	{
		memcpy(str, oalRD->errorString, oalRD->errorStringSize);
		str[oalRD->errorStringSize] = '\0';
	}
	else
	{
		memcpy(str, oalRD->errorString, maxSize - 1);
		str[maxSize] = '\0';
	}

	return MFM_TRUE;
}

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
	errno = 0; // Necessary because this function sets errno to ENOENT
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
	
	// Set render device functions
	// Buffer functions
	rd->base.createBuffer = &mfaOALCreateBuffer;
	rd->base.destroyBuffer = &mfaOALDestroyBuffer;
	rd->base.updateBuffer = &mfaOALUpdateBuffer;

	// Source functions
	rd->base.createSource = &mfaOALCreateSource;
	rd->base.destroySource = &mfaOALDestroySource;
	rd->base.playSource = &mfaOALPlaySource;
	rd->base.stopSource = &mfaOALStopSource;
	rd->base.rewindSource = &mfaOALRewindSource;
	rd->base.pauseSource = &mfaOALPauseSource;
	rd->base.isSourcePlaying = &mfaOALIsSourcePlaying;
	rd->base.getSourceProcessedBuffers = &mfaOALGetSourceProcessedBuffers;
	rd->base.sourceQueueBuffer = &mfaOALSourceQueueBuffer;
	rd->base.sourceUnqueueBuffer = &mfaOALSourceUnqueueBuffer;
	rd->base.setSourcePosition = &mfaOALSetSourcePosition;
	rd->base.setSourceVelocity = &mfaOALSetSourceVelocity;
	rd->base.setSourceDirection = &mfaOALSetSourceDirection;
	rd->base.setSourcePitch = &mfaOALSetSourcePitch;
	rd->base.setSourceGain = &mfaOALSetSourceGain;
	rd->base.setSourceMaxDistance = &mfaOALSetSourceMaxDistance;
	rd->base.setSourceSecondsOffset = &mfaOALSetSourceSecondsOffset;
	rd->base.setSourceSamplesOffset = &mfaOALSetSourceSamplesOffset;
	rd->base.setSourceBytesOffset = &mfaOALSetSourceBytesOffset;
	rd->base.setSourceLooping = &mfaOALSetSourceLooping;
	rd->base.setSourceBuffer = &mfaOALSetSourceBuffer;

	// Listener functions
	rd->base.setListenerGain = &mfaOALSetListenerGain;
	rd->base.setListenerPosition = &mfaOALSetListenerPosition;
	rd->base.setListenerVelocity = &mfaOALSetListenerVelocity;
	rd->base.setListenerOrientation = &mfaOALSetListenerOrientation;

	// Getter functions
	rd->base.getPropertyI = &mfaOALGetPropertyI;
	rd->base.getPropertyF = &mfaOALGetPropertyF;

	// Error functions
	rd->base.getErrorString = &mfaOALGetErrorString;

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
