#include "OALRenderDevice.hpp"

#include <Config.hpp>
#include <sstream>

#if defined(MAGMA_FRAMEWORK_USE_OPENAL)
#include <AL/al.h>
#include <AL/alc.h>
#endif

void Magma::Framework::Audio::OALRenderDevice::Init(const RenderDeviceSettings & settings)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENAL)
	ALCenum err;

	m_device = alcOpenDevice(NULL);
	m_context = alcCreateContext((ALCdevice*)m_device, NULL);
	if (!alcMakeContextCurrent((ALCcontext*)m_context))
	{
		std::stringstream ss;
		ss << "Failed to init OALRenderDevice:" << std::endl;
		ss << "Failed to make context current";
		throw RenderDeviceError(ss.str());
	}
	
	err = alGetError();
	if (err != AL_NO_ERROR)
	{
		std::stringstream ss;
		ss << "Failed to init OALRenderDevice:" << std::endl;
		ss << "alGetError returned " << err;
		throw RenderDeviceError(ss.str());
	}

#else
	throw RenderDeviceError("Failed to call function on OALRenderDevice:\nMAGMA_FRAMEWORK_USE_OPENAL must be defined");
#endif
}

void Magma::Framework::Audio::OALRenderDevice::Terminate()
{
#if defined(MAGMA_FRAMEWORK_USE_OPENAL)
	alcMakeContextCurrent(nullptr);
	alcDestroyContext((ALCcontext*)m_context);
	alcCloseDevice((ALCdevice*)m_device);

	auto err = alGetError();
	if (err != AL_NO_ERROR)
	{
		std::stringstream ss;
		ss << "Failed to terminate OALRenderDevice:" << std::endl;
		ss << "alGetError returned " << err;
		throw RenderDeviceError(ss.str());
	}

#else
	throw RenderDeviceError("Failed to call function on OALRenderDevice:\nMAGMA_FRAMEWORK_USE_OPENAL must be defined");
#endif
}
