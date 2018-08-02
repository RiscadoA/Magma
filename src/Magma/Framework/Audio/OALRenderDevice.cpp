#include "OALRenderDevice.hpp"

#include <Config.hpp>

#if defined(MAGMA_FRAMEWORK_USE_OPENAL)
#include <AL/al.h>
#include <AL/alc.h>
#endif

void Magma::Framework::Audio::OALRenderDevice::Init(const RenderDeviceSettings & settings)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENAL)
	m_device = alcOpenDevice(NULL);
#else
	throw RenderDeviceError("Failed to call function on OALRenderDevice:\nMAGMA_FRAMEWORK_USE_OPENAL must be defined");
#endif
}

void Magma::Framework::Audio::OALRenderDevice::Terminate()
{
#if defined(MAGMA_FRAMEWORK_USE_OPENAL)
	alcCloseDevice((ALCdevice*)m_device);
#else
	throw RenderDeviceError("Failed to call function on OALRenderDevice:\nMAGMA_FRAMEWORK_USE_OPENAL must be defined");
#endif
}
