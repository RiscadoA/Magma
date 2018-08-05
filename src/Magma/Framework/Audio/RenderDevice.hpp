#pragma once

#include "Exception.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace Audio
		{
			/// <summary>
			///		Describes an audio format
			/// </summary>
			enum class Format
			{
				Invalid = -1,

				Mono8,
				Stereo8,
				
				Mono16,
				Stereo16,

				Count
			};

			/// <summary>
			///		Encapsulates an audio buffer
			/// </summary>
			class Buffer
			{
			public:
				virtual ~Buffer() = default;

				/// <summary>
				///		Updates the audio buffer contents (raw PCM data)
				/// </summary>
				/// <param name="data">Buffer data pointer</param>
				/// <param name="size">Buffer data size</param>
				/// <param name="format">Buffer data format</param>
				/// <param name="frequency">Buffer data frequency</param>
				virtual void Update(void* data, size_t size, Format format, size_t frequency) = 0;

			protected:
				/// <summary>
				///		Used to ensure that these are never created directly
				/// </summary>
				Buffer() = default;
			};

			/// <summary>
			///		Encapsulates an audio source
			/// </summary>
			class Source
			{
			public:
				virtual ~Source() = default;

				/// <summary>
				///		Plays this audio source (asynchronously)
				/// </summary>
				virtual void Play() = 0;

				/// <summary>
				///		Stops the audio playing on this source
				/// </summary>
				virtual void Stop() = 0;

				/// <summary>
				///		Rewinds the audio playing on this source back to the beginning
				/// </summary>
				virtual void Rewind() = 0;

				/// <summary>
				///		Pauses the audio playing on this source
				/// </summary>
				virtual void Pause() = 0;

				/// <summary>
				///		Gets the number of buffers that were processed since the last time this function was called
				/// </summary>
				/// <returns>Number of buffers processed</returns>
				virtual size_t GetProcessedBuffers() = 0;

				/// <summary>
				///		Queues an audio buffer to later play on this source
				/// </summary>
				/// <param name="buffer">Buffer handle</param>
				virtual void QueueBuffer(Buffer* buffer) = 0;

				/// <summary>
				///		Unqueues an audio buffer that was played by this source
				/// </summary>
				/// <returns>Buffer handle</returns>
				virtual Buffer* UnqueueBuffer() = 0;

				/// <summary>
				///		Sets the source position
				/// </summary>
				/// <param name="x">X coordinate</param>
				/// <param name="y">Y coordinate</param>
				/// <param name="z">Z coordinate</param>
				virtual void SetPosition(float x, float y, float z) = 0;

				/// <summary>
				///		Sets the source velocity
				/// </summary>
				/// <param name="x">X coordinate</param>
				/// <param name="y">Y coordinate</param>
				/// <param name="z">Z coordinate</param>
				virtual void SetVelocity(float x, float y, float z) = 0;

				/// <summary>
				///		Sets the source direction
				/// </summary>
				/// <param name="x">X coordinate</param>
				/// <param name="y">Y coordinate</param>
				/// <param name="z">Z coordinate</param>
				virtual void SetDirection(float x, float y, float z) = 0;

				/// <summary>
				///		Sets the source pitch multiplier
				/// </summary>
				/// <param name="pitch">Source pitch multiplier</param>
				virtual void SetPitch(float pitch) = 0;

				/// <summary>
				///		Sets the source gain
				/// </summary>
				/// <param name="gain">Source gain</param>
				virtual void SetGain(float gain) = 0;

				/// <summary>
				///		Sets the source maximum distance
				/// </summary>
				/// <param name="pitch">Source maximum distance</param>
				virtual void SetMaxDistance(float maxDistance) = 0;

				/// <summary>
				///		Sets the source playback position in seconds
				/// </summary>
				/// <param name="position">Source playback position in seconds</param>
				virtual void SetSecondsOffset(float position) = 0;

				/// <summary>
				///		Sets the source playback position in samples
				/// </summary>
				/// <param name="position">Source playback position in samples</param>
				virtual void SetSamplesOffset(float position) = 0;

				/// <summary>
				///		Sets the source playback position in bytes
				/// </summary>
				/// <param name="position">Source playback position in bytes</param>
				virtual void SetBytesOffset(float position) = 0;

				/// <summary>
				///		Turns looping on or off
				/// </summary>
				/// <param name="looping">If true, turns on looping, otherwise turns it off</param>
				virtual void SetLooping(bool looping) = 0;

				/// <summary>
				///		Binds an audio buffer to this source
				/// </summary>
				/// <param name="buffer">Audio buffer pointer (set to nullptr to unbind the current buffer)</param>
				virtual void Bind(Buffer* buffer) = 0;

			protected:
				/// <summary>
				///		Used to ensure that these are never created directly
				/// </summary>
				Source() = default;
			};

			/// <summary>
			///		Render device initialization settings
			/// </summary>
			struct RenderDeviceSettings
			{

			};

			/// <summary>
			///		Abstract class that encapsulates the low level API specific audio rendering calls
			/// </summary>
			class RenderDevice
			{
			public:
				virtual ~RenderDevice() = default;

				/// <summary>
				///		Inits the render device
				/// </summary>
				/// <param name="settings">Render device settings</param>
				virtual void Init(const RenderDeviceSettings& settings) = 0;

				/// <summary>
				///		Terminates the render device
				/// </summary>
				virtual void Terminate() = 0;

				/// <summary>
				///		Creates a new audio source object
				/// </summary>
				/// <returns>New audio source object handle</returns>
				virtual Source* CreateSource() = 0;

				/// <summary>
				///		Destroys an audio source object
				/// </summary>
				/// <param name="source">Audio source object handle</param>
				virtual void DestroySource(Source* source) = 0;

				/// <summary>
				///		Creates a new audio buffer object
				/// </summary>
				/// <returns>New audio buffer object handle</returns>
				virtual Buffer* CreateBuffer() = 0;

				/// <summary>
				///		Creates a new audio buffer object
				/// </summary>
				/// <param name="data">Buffer data pointer</param>
				/// <param name="size">Buffer data size</param>
				/// <param name="format">Buffer data format</param>
				/// <param name="frequency">Buffer data frequency</param>
				/// <returns>New audio buffer object handle</returns>
				virtual Buffer* CreateBuffer(void* data, size_t size, Format format, size_t frequency) = 0;

				/// <summary>
				///		Destroys an audio buffer object
				/// </summary>
				/// <param name="buffer">Audio buffer object handle</param>
				virtual void DestroyBuffer(Buffer* buffer) = 0;

				/// <summary>
				///		Sets the listener gain
				/// </summary>
				/// <param name="gain">Gain value (must be positive)</param>
				virtual void SetListenerGain(float gain) = 0;

				/// <summary>
				///		Sets the listener position
				/// </summary>
				/// <param name="x">Position X axis</param>
				/// <param name="y">Position Y axis</param>
				/// <param name="z">Position Z axis</param>
				virtual void SetListenerPosition(float x, float y, float z) = 0;

				/// <summary>
				///		Sets the listener velocity
				/// </summary>
				/// <param name="x">Velocity X axis</param>
				/// <param name="y">Velocity Y axis</param>
				/// <param name="z">Velocity Z axis</param>
				virtual void SetListenerVelocity(float x, float y, float z) = 0;

				/// <summary>
				///		Sets the listener orientation
				/// </summary>
				/// <param name="x">At X axis</param>
				/// <param name="y">At Y axis</param>
				/// <param name="z">At Z axis</param>
				/// <param name="x">Up X axis</param>
				/// <param name="y">Up Y axis</param>
				/// <param name="z">Up Z axis</param>
				virtual void SetListenerOrientation(float atX, float atY, float atZ, float upX, float upY, float upZ) = 0;
			};
		}
	}
}
