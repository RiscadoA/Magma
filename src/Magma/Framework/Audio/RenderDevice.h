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

	// Getter functions
	typedef mfError(*mfaRDGetPropertyI)(mfaRenderDevice* rd, mfaEnum propID, mfmI32* value);
	typedef mfError(*mfaRDGetPropertyF)(mfaRenderDevice* rd, mfaEnum propID, mfmF32* value);

	// Error functions
	typedef mfmBool(*mfaRDGetErrorString)(mfaRenderDevice* rd, mfsUTF8CodeUnit* str, mfmU64 maxSize);

	struct mfaRenderDevice
	{
		mfmObject object;

		mfaRDCreateBufferFunction createBuffer;
		mfaRDDestroyBufferFunction destroyBuffer;
		mfaRDUpdateBufferFunction updateBuffer;

		mfaRDGetPropertyI getPropertyI;
		mfaRDGetPropertyF getPropertyF;

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
	///		Maps the constant buffer data to a accessible memory location.
	/// </summary>
	/// <param name="rd">Render device</param>
	/// <param name="cb">Constant buffer handle</param>
	/// <param name="memory">Pointer to memory pointer</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns the error code.
	/// </returns>
	mfError mfaUpdateBuffer(mfaRenderDevice* rd, mfaBuffer* cb, void** memory);

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
