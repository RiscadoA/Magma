#include "OALRenderDevice.hpp"

#include <Config.h>
#include <sstream>

#if defined(MAGMA_FRAMEWORK_USE_OPENAL)
#include <AL/al.h>
#include <AL/alc.h>
#endif

using namespace Magma::Framework::Audio;

class OALBuffer : public Buffer
{
public:
	OALBuffer()
	{
		alGenBuffers(1, &m_object);

#ifdef MAGMA_FRAMEWORK_DEBUG
		auto err = alGetError();
		if (err != AL_NO_ERROR)
		{
			std::stringstream ss;
			ss << "Failed to generate OALBuffer:" << std::endl;
			ss << "alGetError returned " << err;
			throw RenderDeviceError(ss.str());
		}
#endif
	}

	virtual ~OALBuffer()
	{
		alDeleteBuffers(1, &m_object);

#ifdef MAGMA_FRAMEWORK_DEBUG
		auto err = alGetError();
		if (err != AL_NO_ERROR)
		{
			std::stringstream ss;
			ss << "Failed to delete OALBuffer:" << std::endl;
			ss << "alGetError returned " << err;
			throw RenderDeviceError(ss.str());
		}
#endif
	}

	// Inherited via Buffer
	virtual void Update(void * data, size_t size, Format format, size_t frequency) override
	{
		ALenum al_format;

		switch (format)
		{
			case Format::Mono8: al_format = AL_FORMAT_MONO8; break;
			case Format::Mono16: al_format = AL_FORMAT_MONO16; break;
			case Format::Stereo8: al_format = AL_FORMAT_STEREO8; break;
			case Format::Stereo16: al_format = AL_FORMAT_STEREO16; break;

			case Format::Invalid:
			{
				std::stringstream ss;
				ss << "Failed to update OALBuffer:" << std::endl;
				ss << "Invalid format";
				throw RenderDeviceError(ss.str());;
				break;
			}

			default:
			{
				std::stringstream ss;
				ss << "Failed to update OALBuffer:" << std::endl;
				ss << "Unsupported format";
				throw RenderDeviceError(ss.str());;
				break;
			}
		}

		alBufferData(m_object, al_format, data, size, frequency);

#ifdef MAGMA_FRAMEWORK_DEBUG
		auto err = alGetError();
		if (err != AL_NO_ERROR)
		{
			std::stringstream ss;
			ss << "Failed to update OALBuffer:" << std::endl;
			ss << "alGetError returned " << err;
			throw RenderDeviceError(ss.str());
		}
#endif
	}

	ALuint m_object;
};

class OALSource : public Source
{
public:
	OALSource()
	{
		alGenSources(1, &m_object);

#ifdef MAGMA_FRAMEWORK_DEBUG
		auto err = alGetError();
		if (err != AL_NO_ERROR)
		{
			std::stringstream ss;
			ss << "Failed to generate OALSource:" << std::endl;
			ss << "alGetError returned " << err;
			throw RenderDeviceError(ss.str());
		}
#endif
	}

	virtual ~OALSource()
	{
		alDeleteSources(1, &m_object);

#ifdef MAGMA_FRAMEWORK_DEBUG
		auto err = alGetError();
		if (err != AL_NO_ERROR)
		{
			std::stringstream ss;
			ss << "Failed to delete OALSource:" << std::endl;
			ss << "alGetError returned " << err;
			throw RenderDeviceError(ss.str());
		}
#endif
	}

	ALuint m_object;

	// Inherited via Source
	virtual void Play() override
	{
		alSourcePlay(m_object);

#ifdef MAGMA_FRAMEWORK_DEBUG
		auto err = alGetError();
		if (err != AL_NO_ERROR)
		{
			std::stringstream ss;
			ss << "Failed to play OALSource:" << std::endl;
			ss << "alGetError returned " << err;
			throw RenderDeviceError(ss.str());
		}
#endif
	}

	virtual void Stop() override
	{
		alSourceStop(m_object);

#ifdef MAGMA_FRAMEWORK_DEBUG
		auto err = alGetError();
		if (err != AL_NO_ERROR)
		{
			std::stringstream ss;
			ss << "Failed to stop OALSource:" << std::endl;
			ss << "alGetError returned " << err;
			throw RenderDeviceError(ss.str());
		}
#endif
	}

	virtual void Rewind() override
	{
		alSourceRewind(m_object);

#ifdef MAGMA_FRAMEWORK_DEBUG
		auto err = alGetError();
		if (err != AL_NO_ERROR)
		{
			std::stringstream ss;
			ss << "Failed to rewind OALSource:" << std::endl;
			ss << "alGetError returned " << err;
			throw RenderDeviceError(ss.str());
		}
#endif
	}

	virtual void Pause() override
	{
		alSourcePause(m_object);

#ifdef MAGMA_FRAMEWORK_DEBUG
		auto err = alGetError();
		if (err != AL_NO_ERROR)
		{
			std::stringstream ss;
			ss << "Failed to pause OALSource:" << std::endl;
			ss << "alGetError returned " << err;
			throw RenderDeviceError(ss.str());
		}
#endif
	}

	virtual bool IsPlaying() override
	{
		ALint ret;
		alGetSourcei(m_object, AL_SOURCE_STATE, &ret);

#ifdef MAGMA_FRAMEWORK_DEBUG
		auto err = alGetError();
		if (err != AL_NO_ERROR)
		{
			std::stringstream ss;
			ss << "Failed to pause OALSource:" << std::endl;
			ss << "alGetError returned " << err;
			throw RenderDeviceError(ss.str());
		}
#endif

		return ret == AL_PLAYING;
	}

	virtual size_t GetProcessedBuffers() override
	{
		ALint ret = 0;
		alGetSourcei(m_object, AL_BUFFERS_PROCESSED, &ret);

#ifdef MAGMA_FRAMEWORK_DEBUG
		auto err = alGetError();
		if (err != AL_NO_ERROR)
		{
			std::stringstream ss;
			ss << "Failed to queue buffer on OALSource:" << std::endl;
			ss << "alGetError returned " << err;
			throw RenderDeviceError(ss.str());
		}
#endif

		return ret;
	}
	virtual void QueueBuffer(Buffer * buffer) override
	{
		m_queuedBuffers[m_nextSlot] = buffer;
		alSourceQueueBuffers(m_object, 1, &static_cast<OALBuffer*>(buffer)->m_object);

		++m_nextSlot;
		if (m_nextSlot >= 256)
			m_nextSlot = 0;

		if (m_firstSlot == m_nextSlot)
		{
			std::stringstream ss;
			ss << "Failed to queue buffer on OALSource:" << std::endl;
			ss << "Maximum number of queued buffers achieved (256)";
			throw RenderDeviceError(ss.str());
		}

#ifdef MAGMA_FRAMEWORK_DEBUG
		auto err = alGetError();
		if (err != AL_NO_ERROR)
		{
			std::stringstream ss;
			ss << "Failed to queue buffer on OALSource:" << std::endl;
			ss << "alGetError returned " << err;
			throw RenderDeviceError(ss.str());
		}
#endif
	}

	virtual Buffer* UnqueueBuffer() override
	{
		if (m_nextSlot == m_firstSlot)
		{
			std::stringstream ss;
			ss << "Failed to unqueue buffer from OALSource:" << std::endl;
			ss << "No buffers were queued";
			throw RenderDeviceError(ss.str());
		}

		ALuint buf;
		alSourceUnqueueBuffers(m_object, 1, &buf);

		if (buf != static_cast<OALBuffer*>(m_queuedBuffers[m_firstSlot])->m_object)
		{
			std::stringstream ss;
			ss << "Failed to unqueue buffer from OALSource:" << std::endl;
			ss << "Buffer returned from alSourceUnqueueBuffers does not match the buffer stored on the wrapper buffer queue";
			throw RenderDeviceError(ss.str());
		}

		auto ret = m_queuedBuffers[m_firstSlot];

		++m_firstSlot;
		if (m_firstSlot == 256)
			m_firstSlot = 0;

		return ret;
	}

	virtual void SetPosition(float x, float y, float z) override
	{
		alSource3f(m_object, AL_POSITION, x, y, z);

#ifdef MAGMA_FRAMEWORK_DEBUG
		auto err = alGetError();
		if (err != AL_NO_ERROR)
		{
			std::stringstream ss;
			ss << "Failed to set position on OALSource:" << std::endl;
			ss << "alGetError returned " << err;
			throw RenderDeviceError(ss.str());
		}
#endif
	}

	virtual void SetVelocity(float x, float y, float z) override
	{
		alSource3f(m_object, AL_VELOCITY, x, y, z);

#ifdef MAGMA_FRAMEWORK_DEBUG
		auto err = alGetError();
		if (err != AL_NO_ERROR)
		{
			std::stringstream ss;
			ss << "Failed to set velocity on OALSource:" << std::endl;
			ss << "alGetError returned " << err;
			throw RenderDeviceError(ss.str());
		}
#endif
	}

	virtual void SetDirection(float x, float y, float z) override
	{
		alSource3f(m_object, AL_DIRECTION, x, y, z);

#ifdef MAGMA_FRAMEWORK_DEBUG
		auto err = alGetError();
		if (err != AL_NO_ERROR)
		{
			std::stringstream ss;
			ss << "Failed to set direction on OALSource:" << std::endl;
			ss << "alGetError returned " << err;
			throw RenderDeviceError(ss.str());
		}
#endif
	}

	virtual void SetPitch(float pitch) override
	{
		alSourcef(m_object, AL_PITCH, pitch);

#ifdef MAGMA_FRAMEWORK_DEBUG
		auto err = alGetError();
		if (err != AL_NO_ERROR)
		{
			std::stringstream ss;
			ss << "Failed to set pitch on OALSource:" << std::endl;
			ss << "alGetError returned " << err;
			throw RenderDeviceError(ss.str());
		}
#endif
	}

	virtual void SetGain(float gain) override
	{
		alSourcef(m_object, AL_GAIN, gain);

#ifdef MAGMA_FRAMEWORK_DEBUG
		auto err = alGetError();
		if (err != AL_NO_ERROR)
		{
			std::stringstream ss;
			ss << "Failed to set gain on OALSource:" << std::endl;
			ss << "alGetError returned " << err;
			throw RenderDeviceError(ss.str());
		}
#endif
	}

	virtual void SetMaxDistance(float maxDistance) override
	{
		alSourcef(m_object, AL_MAX_DISTANCE, maxDistance);

#ifdef MAGMA_FRAMEWORK_DEBUG
		auto err = alGetError();
		if (err != AL_NO_ERROR)
		{
			std::stringstream ss;
			ss << "Failed to set max distance on OALSource:" << std::endl;
			ss << "alGetError returned " << err;
			throw RenderDeviceError(ss.str());
		}
#endif
	}

	virtual void SetSecondsOffset(float position) override
	{
		alSourcef(m_object, AL_SEC_OFFSET, position);

#ifdef MAGMA_FRAMEWORK_DEBUG
		auto err = alGetError();
		if (err != AL_NO_ERROR)
		{
			std::stringstream ss;
			ss << "Failed to set seconds offset on OALSource:" << std::endl;
			ss << "alGetError returned " << err;
			throw RenderDeviceError(ss.str());
		}
#endif
	}

	virtual void SetSamplesOffset(float position) override
	{
		alSourcef(m_object, AL_SAMPLE_OFFSET, position);

#ifdef MAGMA_FRAMEWORK_DEBUG
		auto err = alGetError();
		if (err != AL_NO_ERROR)
		{
			std::stringstream ss;
			ss << "Failed to set samples offset on OALSource:" << std::endl;
			ss << "alGetError returned " << err;
			throw RenderDeviceError(ss.str());
		}
#endif
	}

	virtual void SetBytesOffset(float position) override
	{
		alSourcef(m_object, AL_BYTE_OFFSET, position);

#ifdef MAGMA_FRAMEWORK_DEBUG
		auto err = alGetError();
		if (err != AL_NO_ERROR)
		{
			std::stringstream ss;
			ss << "Failed to set bytes offset on OALSource:" << std::endl;
			ss << "alGetError returned " << err;
			throw RenderDeviceError(ss.str());
		}
#endif
	}

	virtual void SetLooping(bool looping) override
	{
		alSourcei(m_object, AL_LOOPING, looping);

#ifdef MAGMA_FRAMEWORK_DEBUG
		auto err = alGetError();
		if (err != AL_NO_ERROR)
		{
			std::stringstream ss;
			ss << "Failed to set looping on OALSource:" << std::endl;
			ss << "alGetError returned " << err;
			throw RenderDeviceError(ss.str());
		}
#endif
	}

	virtual void Bind(Buffer * buffer) override
	{
		if (buffer == nullptr)
			alSourcei(m_object, AL_BUFFER, 0);
		else
			alSourcei(m_object, AL_BUFFER, static_cast<OALBuffer*>(buffer)->m_object);

#ifdef MAGMA_FRAMEWORK_DEBUG
		auto err = alGetError();
		if (err != AL_NO_ERROR)
		{
			std::stringstream ss;
			ss << "Failed to bind OALBuffer to OALSource:" << std::endl;
			ss << "alGetError returned " << err;
			throw RenderDeviceError(ss.str());
		}
#endif
	}

	Buffer* m_queuedBuffers[256];
	size_t m_firstSlot = 0;
	size_t m_nextSlot = 0;
};

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

Magma::Framework::Audio::Source * Magma::Framework::Audio::OALRenderDevice::CreateSource()
{
#if defined(MAGMA_FRAMEWORK_USE_OPENAL)
	return new OALSource();
#else
	throw RenderDeviceError("Failed to call function on OALRenderDevice:\nMAGMA_FRAMEWORK_USE_OPENAL must be defined");
	return nullptr;
#endif
}

void Magma::Framework::Audio::OALRenderDevice::DestroySource(Source * source)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENAL)
	delete source;
#else
	throw RenderDeviceError("Failed to call function on OALRenderDevice:\nMAGMA_FRAMEWORK_USE_OPENAL must be defined");
#endif
}
Magma::Framework::Audio::Buffer * Magma::Framework::Audio::OALRenderDevice::CreateBuffer()
{
#if defined(MAGMA_FRAMEWORK_USE_OPENAL)
	return new OALBuffer();
#else
	throw RenderDeviceError("Failed to call function on OALRenderDevice:\nMAGMA_FRAMEWORK_USE_OPENAL must be defined");
	return nullptr;
#endif
}

Magma::Framework::Audio::Buffer * Magma::Framework::Audio::OALRenderDevice::CreateBuffer(void* data, size_t size, Format format, size_t frequency)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENAL)
	auto buf = new OALBuffer();
	buf->Update(data, size, format, frequency);
	return buf;
#else
	throw RenderDeviceError("Failed to call function on OALRenderDevice:\nMAGMA_FRAMEWORK_USE_OPENAL must be defined");
	return nullptr;
#endif
}

void Magma::Framework::Audio::OALRenderDevice::DestroyBuffer(Buffer * buffer)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENAL)
	delete buffer;
#else
	throw RenderDeviceError("Failed to call function on OALRenderDevice:\nMAGMA_FRAMEWORK_USE_OPENAL must be defined");
#endif
}

void Magma::Framework::Audio::OALRenderDevice::SetListenerGain(float gain)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENAL)
	alListenerf(AL_GAIN, gain);

#ifdef MAGMA_FRAMEWORK_DEBUG
	auto err = alGetError();
	if (err != AL_NO_ERROR)
	{
		std::stringstream ss;
		ss << "Failed to set listener gain on OALRenderDevice:" << std::endl;
		ss << "alGetError returned " << err;
		throw RenderDeviceError(ss.str());
	}
#endif
#else
	throw RenderDeviceError("Failed to call function on OALRenderDevice:\nMAGMA_FRAMEWORK_USE_OPENAL must be defined");
#endif
}

void Magma::Framework::Audio::OALRenderDevice::SetListenerPosition(float x, float y, float z)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENAL)
	alListener3f(AL_POSITION, x, y, z);

#ifdef MAGMA_FRAMEWORK_DEBUG
	auto err = alGetError();
	if (err != AL_NO_ERROR)
	{
		std::stringstream ss;
		ss << "Failed to set listener position on OALRenderDevice:" << std::endl;
		ss << "alGetError returned " << err;
		throw RenderDeviceError(ss.str());
	}
#endif
#else
	throw RenderDeviceError("Failed to call function on OALRenderDevice:\nMAGMA_FRAMEWORK_USE_OPENAL must be defined");
#endif
}

void Magma::Framework::Audio::OALRenderDevice::SetListenerVelocity(float x, float y, float z)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENAL)
	alListener3f(AL_VELOCITY, x, y, z);

#ifdef MAGMA_FRAMEWORK_DEBUG
	auto err = alGetError();
	if (err != AL_NO_ERROR)
	{
		std::stringstream ss;
		ss << "Failed to set listener velocity on OALRenderDevice:" << std::endl;
		ss << "alGetError returned " << err;
		throw RenderDeviceError(ss.str());
	}
#endif
#else
	throw RenderDeviceError("Failed to call function on OALRenderDevice:\nMAGMA_FRAMEWORK_USE_OPENAL must be defined");
#endif
}

void Magma::Framework::Audio::OALRenderDevice::SetListenerOrientation(float atX, float atY, float atZ, float upX, float upY, float upZ)
{
#if defined(MAGMA_FRAMEWORK_USE_OPENAL)
	float params[] =
	{
		atX, atY, atZ,
		upX, upY, upZ,
	};
	alListenerfv(AL_ORIENTATION, params);

#ifdef MAGMA_FRAMEWORK_DEBUG
	auto err = alGetError();
	if (err != AL_NO_ERROR)
	{
		std::stringstream ss;
		ss << "Failed to set listener orientation on OALRenderDevice:" << std::endl;
		ss << "alGetError returned " << err;
		throw RenderDeviceError(ss.str());
	}
#endif
#else
	throw RenderDeviceError("Failed to call function on OALRenderDevice:\nMAGMA_FRAMEWORK_USE_OPENAL must be defined");
#endif
}