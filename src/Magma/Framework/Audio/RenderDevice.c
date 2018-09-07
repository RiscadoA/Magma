#include "RenderDevice.h"
#include <string.h>

struct
{
	mfmBool active;
	mfaRenderDeviceCreatorFunction func;
	mfsUTF8CodeUnit type[16];
} mfaRenderDeviceCreatorRegisterEntries[MFA_MAX_RENDER_DEVICE_CREATOR_REGISTER_ENTRIES];

mfError mfaInitRenderDevices(void)
{
	// Init all entries

	for (mfmU64 i = 0; i < MFA_MAX_RENDER_DEVICE_CREATOR_REGISTER_ENTRIES; ++i)
	{
		mfaRenderDeviceCreatorRegisterEntries[i].active = MFM_FALSE;
		mfaRenderDeviceCreatorRegisterEntries[i].func = NULL;
	}

	return MF_ERROR_OKAY;
}

void mfaTerminateRenderDevices(void)
{
	// Reset all entries

	for (mfmU64 i = 0; i < MFA_MAX_RENDER_DEVICE_CREATOR_REGISTER_ENTRIES; ++i)
	{
		mfaRenderDeviceCreatorRegisterEntries[i].active = MFM_FALSE;
		mfaRenderDeviceCreatorRegisterEntries[i].func = NULL;
	}
}

mfError mfaRegisterRenderDeviceCreator(const mfsUTF8CodeUnit * type, mfaRenderDeviceCreatorFunction func)
{
	for (mfmU64 i = 0; i < MFA_MAX_RENDER_DEVICE_CREATOR_REGISTER_ENTRIES; ++i)
		if (mfaRenderDeviceCreatorRegisterEntries[i].active == MFM_FALSE)
		{
			mfaRenderDeviceCreatorRegisterEntries[i].active = MFM_TRUE;
			mfaRenderDeviceCreatorRegisterEntries[i].func = func;

			for (mfmU32 j = 0; j < 16; ++j)
			{
				mfaRenderDeviceCreatorRegisterEntries[i].type[j] = type[j];
				if (type[j] == '\0')
					break;
			}

			return MF_ERROR_OKAY;
		}

	return MFA_ERROR_NO_REGISTER_ENTRIES;
}

mfError mfaCreateRenderDevice(const mfsUTF8CodeUnit * type, mfaRenderDevice ** renderDevice, void * allocator)
{
	for (mfmU64 i = 0; i < MFA_MAX_RENDER_DEVICE_CREATOR_REGISTER_ENTRIES; ++i)
		if (mfaRenderDeviceCreatorRegisterEntries[i].active == MFM_TRUE &&
			strcmp(type, mfaRenderDeviceCreatorRegisterEntries[i].type) == 0)
			return mfaRenderDeviceCreatorRegisterEntries[i].func(renderDevice, allocator);
	return MFA_ERROR_TYPE_NOT_REGISTERED;
}

void mfaDestroyRenderDevice(void * renderDevice)
{
	((mfmObject*)renderDevice)->destructorFunc(renderDevice);
}

mfError mfaCreateBuffer(mfaRenderDevice * rd, mfaBuffer ** buf, const void * data, mfmU64 size, mfaEnum format, mfmU64 frequency)
{
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	return rd->createBuffer(rd, buf, data, size, format, frequency);
}

void mfaDestroyBuffer(void * buf)
{
	if (buf == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	((mfmObject*)buf)->destructorFunc(buf);
}

mfError mfaUpdateBuffer(mfaRenderDevice* rd, mfaBuffer* buf, const void* data, mfmU64 size, mfaEnum format, mfmU64 frequency)
{
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	return rd->updateBuffer(rd, buf, data, size, format, frequency);
}

mfError mfaCreateSource(mfaRenderDevice * rd, mfaSource ** source)
{
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	return rd->createSource(rd, source);
}

void mfaDestroySource(void * source)
{
	if (source == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	((mfmObject*)source)->destructorFunc(source);
}

mfError mfaPlaySource(mfaRenderDevice * rd, mfaSource * source)
{
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	return rd->playSource(rd, source);
}

mfError mfaStopSource(mfaRenderDevice * rd, mfaSource * source)
{
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	return rd->stopSource(rd, source);
}

mfError mfaRewindSource(mfaRenderDevice * rd, mfaSource * source)
{
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	return rd->rewindSource(rd, source);
}

mfError mfaPauseSource(mfaRenderDevice * rd, mfaSource * source)
{
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	return rd->pauseSource(rd, source);
}

mfError mfaIsSourcePlaying(mfaRenderDevice * rd, mfaSource * source, mfmBool * out)
{
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	return rd->isSourcePlaying(rd, source, out);
}

mfError mfaGetSourceProcessedBuffers(mfaRenderDevice * rd, mfaSource * source, mfmU64 * out)
{
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	return rd->getSourceProcessedBuffers(rd, source, out);
}

mfError mfaSourceQueueBuffer(mfaRenderDevice * rd, mfaSource * source, mfaBuffer * buf)
{
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	return rd->sourceQueueBuffer(rd, source, buf);
}

mfError mfaSourceUnqueueBuffer(mfaRenderDevice * rd, mfaSource * source, mfaBuffer ** buf)
{
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	return rd->sourceUnqueueBuffer(rd, source, buf);
}

mfError mfaSetSourcePosition(mfaRenderDevice * rd, mfaSource * source, mfmF32 x, mfmF32 y, mfmF32 z)
{
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	return rd->setSourcePosition(rd, source, x, y, z);
}

mfError mfaSetSourceVelocity(mfaRenderDevice * rd, mfaSource * source, mfmF32 x, mfmF32 y, mfmF32 z)
{
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	return rd->setSourceVelocity(rd, source, x, y, z);
}

mfError mfaSetSourceDirection(mfaRenderDevice * rd, mfaSource * source, mfmF32 x, mfmF32 y, mfmF32 z)
{
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	return rd->setSourceDirection(rd, source, x, y, z);
}

mfError mfaSetSourcePitch(mfaRenderDevice * rd, mfaSource * source, mfmF32 value)
{
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	return rd->setSourcePitch(rd, source, value);
}

mfError mfaSetSourceGain(mfaRenderDevice * rd, mfaSource * source, mfmF32 value)
{
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	return rd->setSourceGain(rd, source, value);
}

mfError mfaSetSourceMaxDistance(mfaRenderDevice * rd, mfaSource * source, mfmF32 value)
{
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	return rd->setSourceMaxDistance(rd, source, value);
}

mfError mfaSetSourceSecondsOffset(mfaRenderDevice * rd, mfaSource * source, mfmF32 position)
{
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	return rd->setSourceSecondsOffset(rd, source, position);
}

mfError mfaSetSourceSamplesOffset(mfaRenderDevice * rd, mfaSource * source, mfmU64 position)
{
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	return rd->setSourceSamplesOffset(rd, source, position);
}

mfError mfaSetSourceBytesOffset(mfaRenderDevice * rd, mfaSource * source, mfmU64 position)
{
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	return rd->setSourceBytesOffset(rd, source, position);
}

mfError mfaSetSourceLooping(mfaRenderDevice * rd, mfaSource * source, mfmBool looping)
{
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	return rd->setSourceLooping(rd, source, looping);
}

mfError mfaSetSourceBuffer(mfaRenderDevice * rd, mfaSource * source, mfaBuffer * buffer)
{
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	return rd->setSourceBuffer(rd, source, buffer);
}

mfError mfaSetListenerPosition(mfaRenderDevice * rd, mfmF32 x, mfmF32 y, mfmF32 z)
{
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	return rd->setListenerPosition(rd, x, y, z);
}

mfError mfaSetListenerVelocity(mfaRenderDevice * rd, mfmF32 x, mfmF32 y, mfmF32 z)
{
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	return rd->setListenerVelocity(rd, x, y, z);
}

mfError mfaSetListenerOrientation(mfaRenderDevice * rd, mfmF32 atX, mfmF32 atY, mfmF32 atZ, mfmF32 upX, mfmF32 upY, mfmF32 upZ)
{
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	return rd->setListenerOrientation(rd, atX, atY, atZ, upX, upY, upZ);
}

mfError mfaGetPropertyI(mfaRenderDevice * rd, mfaEnum id, mfmI32 * value)
{
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	return rd->getPropertyI(rd, id, value);
}

mfError mfaGetPropertyF(mfaRenderDevice * rd, mfaEnum id, mfmF32 * value)
{
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	return rd->getPropertyF(rd, id, value);
}

mfmBool mfaGetErrorString(mfaRenderDevice * rd, mfsUTF8CodeUnit * str, mfmU64 maxSize)
{
	if (rd == NULL)
		return MFA_ERROR_INVALID_ARGUMENTS;
	return rd->getErrorString(rd, str, maxSize);
}

