#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "Error.h"

#include "../Memory/Object.h"
#include "../String/UTF8.h"

#define MFA_MAX_RENDER_DEVICE_CREATOR_REGISTER_ENTRIES 16

#define MFA_MONO8		0x01
#define MFA_STEREO8		0x02
#define MFA_MONO16		0x03
#define MFA_STEREO16	0x04

	typedef mfmU32 mfaEnum;

	typedef struct mfaRenderDevice mfaRenderDevice;

	typedef struct
	{
		mfmObject object;
		mfaRenderDevice* renderDevice;
	} mfaRenderDeviceObject;

	typedef mfaRenderDeviceObject mfaBuffer;
	typedef mfaRenderDeviceObject mfaSource;

	// Buffer functions
	typedef mfError(*mfaRDCreateBufferFunction)(mfaRenderDevice* rd, mfaBuffer** buf, const void* data, mfmU64 size, mfaEnum format, mfmU64 frequency);
	typedef void(*mfaRDDestroyBufferFunction)(void* buf);
	typedef mfError(*mfaRDUpdateBufferFunction)(mfaRenderDevice* rd, mfaBuffer* buf, const void* data, mfmU64 size, mfaEnum format, mfmU64 frequency);

	// Source functions
	typedef mfError(*mfaRDCreateSourceFunction)(mfaRenderDevice* rd, mfaSource** source);
	typedef void(*mfaRDDestroySourceFunction)(void* source);
	typedef mfError(*mfaRDPlaySourceFunction)(mfaRenderDevice* rd, mfaSource* source);
	typedef mfError(*mfaRDStopSourceFunction)(mfaRenderDevice* rd, mfaSource* source);
	typedef mfError(*mfaRDRewindSourceFunction)(mfaRenderDevice* rd, mfaSource* source);
	typedef mfError(*mfaRDPauseSourceFunction)(mfaRenderDevice* rd, mfaSource* source);
	typedef mfError(*mfaRDIsSourcePlayingFunction)(mfaRenderDevice* rd, mfaSource* source, mfmBool* out);
	typedef mfError(*mfaRDGetSourceProcessedBuffersFunction)(mfaRenderDevice* rd, mfaSource* source, mfmU64* out);
	typedef mfError(*mfaRDSourceQueueBufferFunction)(mfaRenderDevice* rd, mfaSource* source, mfaBuffer* buffer);
	typedef mfError(*mfaRDSourceUnqueueBufferFunction)(mfaRenderDevice* rd, mfaSource* source, mfaBuffer** buffer);
	typedef mfError(*mfaRDSetSourcePositionFunction)(mfaRenderDevice* rd, mfaSource* source, mfmF32 x, mfmF32 y, mfmF32 z);
	typedef mfError(*mfaRDSetSourceVelocityFunction)(mfaRenderDevice* rd, mfaSource* source, mfmF32 x, mfmF32 y, mfmF32 z);
	typedef mfError(*mfaRDSetSourceDirectionFunction)(mfaRenderDevice* rd, mfaSource* source, mfmF32 x, mfmF32 y, mfmF32 z);
	typedef mfError(*mfaRDSetSourcePitchFunction)(mfaRenderDevice* rd, mfaSource* source, mfmF32 pitch);
	typedef mfError(*mfaRDSetSourceGainFunction)(mfaRenderDevice* rd, mfaSource* source, mfmF32 gain);
	typedef mfError(*mfaRDSetSourceMaxDistanceFunction)(mfaRenderDevice* rd, mfaSource* source, mfmF32 maxDistance);
	typedef mfError(*mfaRDSetSourceSecondsOffsetFunction)(mfaRenderDevice* rd, mfaSource* source, mfmF32 position);
	typedef mfError(*mfaRDSetSourceSamplesOffsetFunction)(mfaRenderDevice* rd, mfaSource* source, mfmU64 position);
	typedef mfError(*mfaRDSetSourceBytesOffsetFunction)(mfaRenderDevice* rd, mfaSource* source, mfmU64 position);
	typedef mfError(*mfaRDSetSourceLoopingFunction)(mfaRenderDevice* rd, mfaSource* source, mfmBool looping);
	typedef mfError(*mfaRDSetSourceBufferFunction)(mfaRenderDevice* rd, mfaSource* source, mfaBuffer* buffer);

	// Listener functions
	typedef mfError(*mfaRDSetListenerPositionFunction)(mfaRenderDevice* rd, mfmF32 x, mfmF32 y, mfmF32 z);
	typedef mfError(*mfaRDSetListenerVelocityFunction)(mfaRenderDevice* rd, mfmF32 x, mfmF32 y, mfmF32 z);
	typedef mfError(*mfaRDSetListenerOrientationFunction)(mfaRenderDevice* rd, mfmF32 atX, mfmF32 atY, mfmF32 atZ, mfmF32 upX, mfmF32 upY, mfmF32 upZ);

	// Getter functions
	typedef mfError(*mfaRDGetPropertyI)(mfaRenderDevice* rd, mfaEnum propID, mfmI32* value);
	typedef mfError(*mfaRDGetPropertyF)(mfaRenderDevice* rd, mfaEnum propID, mfmF32* value);

	// Error functions
	typedef mfmBool(*mfaRDGetErrorString)(mfaRenderDevice* rd, mfsUTF8CodeUnit* str, mfmU64 maxSize);

	struct mfaRenderDevice
	{
		mfmObject object;

		// Buffer functions
		mfaRDCreateBufferFunction createBuffer;
		mfaRDDestroyBufferFunction destroyBuffer;
		mfaRDUpdateBufferFunction updateBuffer;

		// Source functions
		mfaRDCreateSourceFunction createSource;
		mfaRDDestroySourceFunction destroySource;
		mfaRDPlaySourceFunction playSource;
		mfaRDStopSourceFunction stopSource;
		mfaRDRewindSourceFunction rewindSource;
		mfaRDPauseSourceFunction pauseSource;
		mfaRDIsSourcePlayingFunction isSourcePlaying;
		mfaRDGetSourceProcessedBuffersFunction getSourceProcessedBuffers;
		mfaRDSourceQueueBufferFunction sourceQueueBuffer;
		mfaRDSourceUnqueueBufferFunction sourceUnqueueBuffer;
		mfaRDSetSourcePositionFunction setSourcePosition;
		mfaRDSetSourceVelocityFunction setSourceVelocity;
		mfaRDSetSourceDirectionFunction setSourceDirection;
		mfaRDSetSourcePitchFunction setSourcePitch;
		mfaRDSetSourceGainFunction setSourceGain;
		mfaRDSetSourceMaxDistanceFunction setSourceMaxDistance;
		mfaRDSetSourceSecondsOffsetFunction setSourceSecondsOffset;
		mfaRDSetSourceSamplesOffsetFunction setSourceSamplesOffset;
		mfaRDSetSourceBytesOffsetFunction setSourceBytesOffset;
		mfaRDSetSourceLoopingFunction setSourceLooping;
		mfaRDSetSourceBufferFunction setSourceBuffer;

		// Listener functions
		mfaRDSetListenerPositionFunction setListenerPosition;
		mfaRDSetListenerVelocityFunction setListenerVelocity;
		mfaRDSetListenerOrientationFunction setListenerOrientation;

		// Getter functions
		mfaRDGetPropertyI getPropertyI;
		mfaRDGetPropertyF getPropertyF;

		// Error functions
		mfaRDGetErrorString getErrorString;
	};

	/// <summary>
	///		Creates a new buffer.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="buf">Pointer to buffer handle</param>
	/// <param name="data">Buffer initial data (set to NULL to create empty buffer)</param>
	/// <param name="size">Buffer size in bytes</param>
	/// <param name="format">Buffer audio format</param>
	/// <param name="frequency">Buffer audio frequency</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfaCreateBuffer(mfaRenderDevice* rd, mfaBuffer** buf, const void* data, mfmU64 size, mfaEnum format, mfmU64 frequency);

	/// <summary>
	///		Destroys a buffer.
	/// </summary>
	/// <param name="buf">Buffer handle</param>
	void mfaDestroyBuffer(void* buf);

	/// <summary>
	///		Updates a buffer's data.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="buf">Buffer handle</param>
	/// <param name="data">New data</param>
	/// <param name="size">New data size in bytes</param>
	/// <param name="format">New buffer audio format</param>
	/// <param name="frequency">New buffer audio frequency</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfaUpdateBuffer(mfaRenderDevice* rd, mfaBuffer* buf, const void* data, mfmU64 size, mfaEnum format, mfmU64 frequency);

	/// <summary>
	///		Creates a new source.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="source">Pointer to source handle</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfaCreateSource(mfaRenderDevice* rd, mfaSource** source);

	/// <summary>
	///		Destroys a source.
	/// </summary>
	/// <param name="source">Source handle</param>
	void mfaDestroySource(void* source);

	/// <summary>
	///		Plays audio on a source.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="source">Source handle</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfaPlaySource(mfaRenderDevice* rd, mfaSource* source);

	/// <summary>
	///		Stops the audio playing on a source.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="source">Source handle</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfaStopSource(mfaRenderDevice* rd, mfaSource* source);

	/// <summary>
	///		Rewinds the audio playing on a source.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="source">Source handle</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfaRewindSource(mfaRenderDevice* rd, mfaSource* source);

	/// <summary>
	///		Pauses the audio playing on a source.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="source">Source handle</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfaPauseSource(mfaRenderDevice* rd, mfaSource* source);

	/// <summary>
	///		Stops the audio playing on a source.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="source">Source handle</param>
	/// <param name="out">Out value</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfaIsSourcePlaying(mfaRenderDevice* rd, mfaSource* source, mfmBool* out);

	/// <summary>
	///		Gets the number of buffers that were processed by a source.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="source">Source handle</param>
	/// <param name="out">Out value</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfaGetSourceProcessedBuffers(mfaRenderDevice* rd, mfaSource* source, mfmU64* out);

	/// <summary>
	///		Queues a buffer on a source.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="source">Source handle</param>
	/// <param name="buf">Buffer handle</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfaSourceQueueBuffer(mfaRenderDevice* rd, mfaSource* source, mfaBuffer* buf);

	/// <summary>
	///		Unqueues a buffer on a source (first buffer to be processed).
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="source">Source handle</param>
	/// <param name="buf">Out buffer handle</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfaSourceUnqueueBuffer(mfaRenderDevice* rd, mfaSource* source, mfaBuffer** buf);

	/// <summary>
	///		Sets a source's position.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="source">Source handle</param>
	/// <param name="x">X position</param>
	/// <param name="y">Y position</param>
	/// <param name="z">Z position</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfaSetSourcePosition(mfaRenderDevice* rd, mfaSource* source, mfmF32 x, mfmF32 y, mfmF32 z);

	/// <summary>
	///		Sets a source's velocity.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="source">Source handle</param>
	/// <param name="x">X velocity</param>
	/// <param name="y">Y velocity</param>
	/// <param name="z">Z velocity</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfaSetSourceVelocity(mfaRenderDevice* rd, mfaSource* source, mfmF32 x, mfmF32 y, mfmF32 z);

	/// <summary>
	///		Sets a source's direction.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="source">Source handle</param>
	/// <param name="x">X direction</param>
	/// <param name="y">Y direction</param>
	/// <param name="z">Z direction</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfaSetSourceDirection(mfaRenderDevice* rd, mfaSource* source, mfmF32 x, mfmF32 y, mfmF32 z);

	/// <summary>
	///		Sets a source's pitch.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="source">Source handle</param>
	/// <param name="value">Pitch</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfaSetSourcePitch(mfaRenderDevice* rd, mfaSource* source, mfmF32 value);

	/// <summary>
	///		Sets a source's gain.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="source">Source handle</param>
	/// <param name="value">Gain</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfaSetSourceGain(mfaRenderDevice* rd, mfaSource* source, mfmF32 value);

	/// <summary>
	///		Sets a source's maximum distance.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="source">Source handle</param>
	/// <param name="value">Maximum distance</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfaSetSourceMaxDistance(mfaRenderDevice* rd, mfaSource* source, mfmF32 value);

	/// <summary>
	///		Sets the source's playback position in seconds.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="source">Source handle</param>
	/// <param name="position">Position in seconds</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfaSetSourceSecondsOffset(mfaRenderDevice* rd, mfaSource* source, mfmF32 position);

	/// <summary>
	///		Sets the source's playback position in samples.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="source">Source handle</param>
	/// <param name="position">Position in samples</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfaSetSourceSamplesOffset(mfaRenderDevice* rd, mfaSource* source, mfmU64 position);

	/// <summary>
	///		Sets the source's playback position in bytes.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="source">Source handle</param>
	/// <param name="position">Position in bytes</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfaSetSourceBytesOffset(mfaRenderDevice* rd, mfaSource* source, mfmU64 position);

	/// <summary>
	///		Sets a source's maximum distance.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="source">Source handle</param>
	/// <param name="looping">If MFM_FALSE, sets source as non looping, otherwise as looping</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfaSetSourceLooping(mfaRenderDevice* rd, mfaSource* source, mfmBool looping);

	/// <summary>
	///		Sets a source's current buffer.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="source">Source handle</param>
	/// <param name="buffer">New buffer handle</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfaSetSourceBuffer(mfaRenderDevice* rd, mfaSource* source, mfaBuffer* buffer);

	/// <summary>
	///		Sets the listener's position.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="x">X position</param>
	/// <param name="y">Y position</param>
	/// <param name="z">Z position</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfaSetListenerPosition(mfaRenderDevice* rd, mfmF32 x, mfmF32 y, mfmF32 z);

	/// <summary>
	///		Sets the listener's velocity.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="x">X velocity</param>
	/// <param name="y">Y velocity</param>
	/// <param name="z">Z velocity</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfaSetListenerVelocity(mfaRenderDevice* rd, mfmF32 x, mfmF32 y, mfmF32 z);

	/// <summary>
	///		Sets the listener's orientation.
	/// </summary>
	/// <param name="rd">Render device</param>
	///	<param name="atX">Forward X direction</param>
	/// <param name="atY">Forward Y direction</param>
	/// <param name="atZ">Forward Z direction</param>
	/// <param name="upX">Up X directory</param>
	/// <param name="upY">Up Y direction</param>
	/// <param name="upZ">Up Z direction</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfaSetListenerOrientation(mfaRenderDevice* rd, mfmF32 atX, mfmF32 atY, mfmF32 atZ, mfmF32 upX, mfmF32 upY, mfmF32 upZ);

	/// <summary>
	///		Gets a render device integer property.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="id">Property identifier</param>
	/// <param name="value">Output property value</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfaGetPropertyI(mfaRenderDevice* rd, mfaEnum id, mfmI32* value);

	/// <summary>
	///		Gets a render device floating point property.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="id">Property identifier</param>
	/// <param name="value">Output property value</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfaGetPropertyF(mfaRenderDevice* rd, mfaEnum id, mfmF32* value);

	/// <summary>
	///		Gets the last error string.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="str">Output error string</param>
	/// <param name="maxSize">Maximum error string size</param>
	/// <returns>
	///		True if there was an error, otherwise false.
	/// </returns>
	mfmBool mfaGetErrorString(mfaRenderDevice* rd, mfsUTF8CodeUnit* str, mfmU64 maxSize);

	typedef mfError(*mfaRenderDeviceCreatorFunction)(mfaRenderDevice** renderDevice, void* allocator);

	/// <summary>
	///		Inits the render devices library.
	/// </summary>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mfaInitRenderDevices(void);

	/// <summary>
	///		Terminates the render devices library.
	/// </summary>
	void mfaTerminateRenderDevices(void);

	/// <summary>
	///		Registers a new render device creator.
	/// </summary>
	/// <param name="type">Render device type name (with a maximum size of 16 bytes)</param>
	/// <param name="func">Render device creator function</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFG_ERROR_NO_REGISTER_ENTRIES if there are no more creator slots in the register.
	/// </returns>
	mfError mfaRegisterRenderDeviceCreator(const mfsUTF8CodeUnit* type, mfaRenderDeviceCreatorFunction func);

	/// <summary>
	///		Creates a new render device.
	/// </summary>
	/// <param name="type">Render device type name</param>
	/// <param name="renderDevice">Out render device handle</param>
	/// <param name="allocator">Render device allocator</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFA_ERROR_TYPE_NOT_REGISTERED if there isn't a creator with the type registered.
	///		Otherwise returns a render device creation error code.
	/// </returns>
	mfError mfaCreateRenderDevice(const mfsUTF8CodeUnit* type, mfaRenderDevice** renderDevice, void* allocator);

	/// <summary>
	///		Destroys a render device.
	/// </summary>
	/// <param name="renderDevice">Render device handle</param>
	void mfaDestroyRenderDevice(void* renderDevice);

#ifdef __cplusplus
}
#endif
