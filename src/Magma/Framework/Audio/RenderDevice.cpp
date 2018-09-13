#include "RenderDevice.hpp"
#include "Config.h"
#include "../String/StringStream.h"
#include "../ErrorString.h"

#ifdef MAGMA_FRAMEWORK_DEBUG
#define CHECK_ERROR(rd, err) do { if (err != MF_ERROR_OKAY) { mfsStringStream ss;\
	mfsUTF8CodeUnit msg[256];\
	mfError e = mfsCreateLocalStringStream(&ss, (mfmU8*)msg, sizeof(msg));\
	if (e != MF_ERROR_OKAY)\
		throw Magma::Framework::Audio::RenderDeviceError(mfErrorToString(err));\
	mfsUTF8CodeUnit internalErrorMsg[256] = { '\0' };\
	mfaGetErrorString(rd, internalErrorMsg, sizeof(internalErrorMsg));\
	e = mfsPrintFormat((mfsStream*)&ss, u8"Error on function " __FUNCTION__ ": %s\n%s\n", mfErrorToString(err), internalErrorMsg);\
	if (e != MF_ERROR_OKAY)\
		throw Magma::Framework::Audio::RenderDeviceError(mfErrorToString(err));\
	mfsDestroyLocalStringStream(&ss);\
	throw Magma::Framework::Audio::RenderDeviceError(msg);\
 } } while(false)
#else
#define CHECK_ERROR(err) do {} while(false)
#endif

void Magma::Framework::Audio::HBuffer::Update(const void * data, mfmU64 size, Format format, mfmU64 frequency)
{
	auto buf = (mfaBuffer*)&this->Get();
	mfError err = mfaUpdateBuffer(buf->renderDevice, buf, data, size, (mfaEnum)format, frequency);
	CHECK_ERROR(buf->renderDevice, err);
}

void Magma::Framework::Audio::HSource::Play()
{
	auto src = (mfaSource*)&this->Get();
	mfError err = mfaPlaySource(src->renderDevice, src);
	CHECK_ERROR(src->renderDevice, err);
}

void Magma::Framework::Audio::HSource::Stop()
{
	auto src = (mfaSource*)&this->Get();
	mfError err = mfaStopSource(src->renderDevice, src);
	CHECK_ERROR(src->renderDevice, err);
}

void Magma::Framework::Audio::HSource::Rewind()
{
	auto src = (mfaSource*)&this->Get();
	mfError err = mfaRewindSource(src->renderDevice, src);
	CHECK_ERROR(src->renderDevice, err);
}

void Magma::Framework::Audio::HSource::Pause()
{
	auto src = (mfaSource*)&this->Get();
	mfError err = mfaPauseSource(src->renderDevice, src);
	CHECK_ERROR(src->renderDevice, err);
}

bool Magma::Framework::Audio::HSource::IsPlaying()
{
	auto src = (mfaSource*)&this->Get();
	mfmBool out = MFM_FALSE;
	mfError err = mfaIsSourcePlaying(src->renderDevice, src, &out);
	CHECK_ERROR(src->renderDevice, err);
	return out != MFM_FALSE;
}

mfmU64 Magma::Framework::Audio::HSource::GetProcessedBuffers()
{
	auto src = (mfaSource*)&this->Get();
	mfmU64 out = 0;
	mfError err = mfaGetSourceProcessedBuffers(src->renderDevice, src, &out);
	CHECK_ERROR(src->renderDevice, err);
	return out;
}

void Magma::Framework::Audio::HSource::QueueBuffer(HBuffer buffer)
{
	auto src = (mfaSource*)&this->Get();
	mfError err = mfaSourceQueueBuffer(src->renderDevice, src, (mfaBuffer*)buffer.GetNoChecks());
	CHECK_ERROR(src->renderDevice, err);
}

Magma::Framework::Audio::HBuffer Magma::Framework::Audio::HSource::UnqueueBuffer()
{
	auto src = (mfaSource*)&this->Get();
	mfaBuffer* buffer = NULL;
	mfError err = mfaSourceUnqueueBuffer(src->renderDevice, src, &buffer);
	CHECK_ERROR(src->renderDevice, err);
	return buffer;
}

void Magma::Framework::Audio::HSource::SetPosition(mfmF32 x, mfmF32 y, mfmF32 z)
{
	auto src = (mfaSource*)&this->Get();
	mfError err = mfaSetSourcePosition(src->renderDevice, src, x, y, z);
	CHECK_ERROR(src->renderDevice, err);
}

void Magma::Framework::Audio::HSource::SetVelocity(mfmF32 x, mfmF32 y, mfmF32 z)
{
	auto src = (mfaSource*)&this->Get();
	mfError err = mfaSetSourceVelocity(src->renderDevice, src, x, y, z);
	CHECK_ERROR(src->renderDevice, err);
}

void Magma::Framework::Audio::HSource::SetDirection(mfmF32 x, mfmF32 y, mfmF32 z)
{
	auto src = (mfaSource*)&this->Get();
	mfError err = mfaSetSourceDirection(src->renderDevice, src, x, y, z);
	CHECK_ERROR(src->renderDevice, err);
}

void Magma::Framework::Audio::HSource::SetPitch(mfmF32 pitch)
{
	auto src = (mfaSource*)&this->Get();
	mfError err = mfaSetSourcePitch(src->renderDevice, src, pitch);
	CHECK_ERROR(src->renderDevice, err);
}

void Magma::Framework::Audio::HSource::SetGain(mfmF32 gain)
{
	auto src = (mfaSource*)&this->Get();
	mfError err = mfaSetSourceGain(src->renderDevice, src, gain);
	CHECK_ERROR(src->renderDevice, err);
}

void Magma::Framework::Audio::HSource::SetMaxDistance(mfmF32 maxDistance)
{
	auto src = (mfaSource*)&this->Get();
	mfError err = mfaSetSourceMaxDistance(src->renderDevice, src, maxDistance);
	CHECK_ERROR(src->renderDevice, err);
}

void Magma::Framework::Audio::HSource::SetSecondsOffset(mfmF32 position)
{
	auto src = (mfaSource*)&this->Get();
	mfError err = mfaSetSourceSecondsOffset(src->renderDevice, src, position);
	CHECK_ERROR(src->renderDevice, err);
}

void Magma::Framework::Audio::HSource::SetSamplesOffset(mfmU64 position)
{
	auto src = (mfaSource*)&this->Get();
	mfError err = mfaSetSourceSamplesOffset(src->renderDevice, src, position);
	CHECK_ERROR(src->renderDevice, err);
}

void Magma::Framework::Audio::HSource::SetBytesOffset(mfmU64 position)
{
	auto src = (mfaSource*)&this->Get();
	mfError err = mfaSetSourceBytesOffset(src->renderDevice, src, position);
	CHECK_ERROR(src->renderDevice, err);
}

void Magma::Framework::Audio::HSource::SetLooping(bool looping)
{
	auto src = (mfaSource*)&this->Get();
	mfError err = mfaSetSourceLooping(src->renderDevice, src, looping ? MFM_TRUE : MFM_FALSE);
	CHECK_ERROR(src->renderDevice, err);
}

void Magma::Framework::Audio::HSource::Bind(HBuffer buffer)
{
	auto src = (mfaSource*)&this->Get();
	mfError err = mfaSetSourceBuffer(src->renderDevice, src, (mfaBuffer*)buffer.GetNoChecks());
	CHECK_ERROR(src->renderDevice, err);
}

Magma::Framework::Audio::HSource Magma::Framework::Audio::HRenderDevice::CreateSource()
{
	mfaSource* src = NULL;
	mfError err = mfaCreateSource((mfaRenderDevice*)&this->Get(), &src);
	CHECK_ERROR((mfaRenderDevice*)&this->Get(), err);
	return src;
}

Magma::Framework::Audio::HBuffer Magma::Framework::Audio::HRenderDevice::CreateBuffer(const void * data, mfmU64 size, Format format, mfmU64 frequency)
{
	mfaBuffer* buf = NULL;
	mfError err = mfaCreateBuffer((mfaRenderDevice*)&this->Get(), &buf, data, size, (mfaEnum)format, frequency);
	CHECK_ERROR((mfaRenderDevice*)&this->Get(), err);
	return buf;
}

void Magma::Framework::Audio::HRenderDevice::SetListenerGain(mfmF32 gain)
{
	mfError err = mfaSetListenerGain((mfaRenderDevice*)&this->Get(), gain);
	CHECK_ERROR((mfaRenderDevice*)&this->Get(), err);
}

void Magma::Framework::Audio::HRenderDevice::SetListenerPosition(mfmF32 x, mfmF32 y, mfmF32 z)
{
	mfError err = mfaSetListenerPosition((mfaRenderDevice*)&this->Get(), x, y, z);
	CHECK_ERROR((mfaRenderDevice*)&this->Get(), err);
}

void Magma::Framework::Audio::HRenderDevice::SetListenerVelocity(mfmF32 x, mfmF32 y, mfmF32 z)
{
	mfError err = mfaSetListenerVelocity((mfaRenderDevice*)&this->Get(), x, y, z);
	CHECK_ERROR((mfaRenderDevice*)&this->Get(), err);
}

void Magma::Framework::Audio::HRenderDevice::SetListenerOrientation(mfmF32 atX, mfmF32 atY, mfmF32 atZ, mfmF32 upX, mfmF32 upY, mfmF32 upZ)
{
	mfError err = mfaSetListenerOrientation((mfaRenderDevice*)&this->Get(), atX, atY, atZ, upX, upY, upZ);
	CHECK_ERROR((mfaRenderDevice*)&this->Get(), err);
}

Magma::Framework::Audio::HRenderDevice Magma::Framework::Audio::CreateRenderDevice(const mfsUTF8CodeUnit * type, Memory::HAllocator allocator)
{
	if (type == NULL)
	{
#if defined(MAGMA_FRAMEWORK_USE_OPENAL)
		type = u8"oal";
#endif
	}

	mfaRenderDevice* rd = NULL;
	mfError err = mfaCreateRenderDevice(type, &rd, allocator.GetNoChecks());
	if (err != MF_ERROR_OKAY)
		throw RenderDeviceError(mfErrorToString(err));
	return rd;
}