#pragma once

#include "../Memory/Handle.hpp"
#include "../Memory/Allocator.hpp"
#include "Exception.hpp"
#include "RenderDevice.h"

namespace Magma
{
	namespace Framework
	{
		namespace Audio
		{
			/// <summary>
			///		Describes an audio format.
			/// </summary>
			enum class Format : mfaEnum
			{
				Mono8 = MFA_MONO8,
				Stereo8 = MFA_STEREO8,
				
				Mono16 = MFA_MONO16,
				Stereo16 = MFA_STEREO16,
			};

			/// <summary>
			///		Used as a buffer handle.
			///		Destroys the buffer automatically when there are no more references to it.
			/// </summary>
			class Buffer : public Memory::Handle
			{
			public:
				using Handle::Handle;
				using Handle::operator=;
				explicit inline Buffer(const Memory::Handle& object) : Memory::Handle(object) {}

				/// <summary>
				///		Updates a buffer's data.
				/// </summary>
				/// <param name="data">Buffer data pointer</param>
				/// <param name="size">Buffer data size</param>
				/// <param name="format">Buffer data format</param>
				/// <param name="frequency">Buffer data frequency</param>
				void Update(const void* data, mfmU64 size, Format format, mfmU64 frequency);
			};

			/// <summary>
			///		Used as a source handle.
			///		Destroys the source automatically when there are no more references to it.
			/// </summary>
			class Source : public Memory::Handle
			{
			public:
				using Handle::Handle;
				using Handle::operator=;
				explicit inline Source(const Memory::Handle& object) : Memory::Handle(object) {}

				/// <summary>
				///		Plays audio on this source (asynchronously).
				/// </summary>
				void Play();

				/// <summary>
				///		Stops the audio playing on this source.
				/// </summary>
				void Stop();

				/// <summary>
				///		Rewinds the audio playing on this source back to the beginning.
				/// </summary>
				void Rewind();

				/// <summary>
				///		Pauses the audio playing on this source.
				/// </summary>
				void Pause();

				/// <summary>
				///		Checks if this source is playing audio.
				/// </summary>
				/// <returns>Returns true if playing, otherwise false</returns>
				bool IsPlaying();

				/// <summary>
				///		Gets the number of buffers that were processed since the last time this function was called.
				/// </summary>
				/// <returns>Number of buffers processed</returns>
				mfmU64 GetProcessedBuffers();

				/// <summary>
				///		Queues an audio buffer to later play on this source.
				/// </summary>
				/// <param name="buffer">Buffer handle</param>
				void QueueBuffer(Buffer buffer);

				/// <summary>
				///		Unqueues an audio buffer that was played by this source.
				/// </summary>
				/// <returns>Buffer handle</returns>
				Buffer UnqueueBuffer();

				/// <summary>
				///		Sets the source position.
				/// </summary>
				/// <param name="x">X coordinate</param>
				/// <param name="y">Y coordinate</param>
				/// <param name="z">Z coordinate</param>
				void SetPosition(mfmF32 x, mfmF32 y, mfmF32 z);

				/// <summary>
				///		Sets the source velocity.
				/// </summary>
				/// <param name="x">X coordinate</param>
				/// <param name="y">Y coordinate</param>
				/// <param name="z">Z coordinate</param>
				void SetVelocity(mfmF32 x, mfmF32 y, mfmF32 z);

				/// <summary>
				///		Sets the source direction.
				/// </summary>
				/// <param name="x">X coordinate</param>
				/// <param name="y">Y coordinate</param>
				/// <param name="z">Z coordinate</param>
				void SetDirection(mfmF32 x, mfmF32 y, mfmF32 z);

				/// <summary>
				///		Sets the source pitch multiplier.
				/// </summary>
				/// <param name="pitch">Source pitch multiplier</param>
				void SetPitch(mfmF32 pitch);

				/// <summary>
				///		Sets the source gain.
				/// </summary>
				/// <param name="gain">Source gain</param>
				void SetGain(mfmF32 gain);

				/// <summary>
				///		Sets the source maximum distance.
				/// </summary>
				/// <param name="pitch">Source maximum distance</param>
				void SetMaxDistance(mfmF32 maxDistance);

				/// <summary>
				///		Sets the source playback position in seconds.
				/// </summary>
				/// <param name="position">Source playback position in seconds</param>
				void SetSecondsOffset(mfmF32 position);

				/// <summary>
				///		Sets the source playback position in samples.
				/// </summary>
				/// <param name="position">Source playback position in samples</param>
				void SetSamplesOffset(mfmU64 position);

				/// <summary>
				///		Sets the source playback position in bytes.
				/// </summary>
				/// <param name="position">Source playback position in bytes</param>
				void SetBytesOffset(mfmU64 position);

				/// <summary>
				///		Turns looping on or off.
				/// </summary>
				/// <param name="looping">If true, turns on looping, otherwise turns it off</param>
				void SetLooping(bool looping);

				/// <summary>
				///		Binds a buffer to this source.
				/// </summary>
				/// <param name="buffer">Buffer handle (set to NULL to unbind the current buffer)</param>
				void Bind(Buffer buffer);
			};

			/// <summary>
			///		Used as a render device handle.
			///		Destroys the render device automatically when there are no more references to it.
			/// </summary>
			class RenderDevice : public Memory::Handle
			{
			public:
				using Handle::Handle;
				using Handle::operator=;
				explicit inline RenderDevice(const Memory::Handle& object) : Memory::Handle(object) {}

				/// <summary>
				///		Creates a new source object.
				/// </summary>
				/// <returns>New source object handle</returns>
				Source CreateSource();

				/// <summary>
				///		Creates a new buffer object.
				/// </summary>
				/// <param name="data">Buffer data pointer</param>
				/// <param name="size">Buffer data size</param>
				/// <param name="format">Buffer data format</param>
				/// <param name="frequency">Buffer data frequency</param>
				/// <returns>New buffer object handle</returns>
				Buffer CreateBuffer(const void* data, mfmU64 size, Format format, mfmU64 frequency);

				/// <summary>
				///		Sets the listener gain.
				/// </summary>
				/// <param name="gain">Gain value (must be positive)</param>
				void SetListenerGain(mfmF32 gain);

				/// <summary>
				///		Sets the listener position.
				/// </summary>
				/// <param name="x">Position X axis</param>
				/// <param name="y">Position Y axis</param>
				/// <param name="z">Position Z axis</param>
				void SetListenerPosition(mfmF32 x, mfmF32 y, mfmF32 z);

				/// <summary>
				///		Sets the listener velocity.
				/// </summary>
				/// <param name="x">Velocity X axis</param>
				/// <param name="y">Velocity Y axis</param>
				/// <param name="z">Velocity Z axis</param>
				void SetListenerVelocity(mfmF32 x, mfmF32 y, mfmF32 z);

				/// <summary>
				///		Sets the listener orientation.
				/// </summary>
				/// <param name="x">At X axis</param>
				/// <param name="y">At Y axis</param>
				/// <param name="z">At Z axis</param>
				/// <param name="x">Up X axis</param>
				/// <param name="y">Up Y axis</param>
				/// <param name="z">Up Z axis</param>
				void SetListenerOrientation(mfmF32 atX, mfmF32 atY, mfmF32 atZ, mfmF32 upX, mfmF32 upY, mfmF32 upZ);
			};

			/// <summary>
			///		Creates a new render device.
			/// </summary>
			/// <param name="type">Render device type name</param>
			/// <param name="allocator">Render device allocator</param>
			/// <returns>Render device handle</returns>
			RenderDevice CreateRenderDevice(const mfsUTF8CodeUnit* type, Memory::AllocatorHandle allocator);
		}
	}
}
