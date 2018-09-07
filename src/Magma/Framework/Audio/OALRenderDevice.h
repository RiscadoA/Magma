#pragma once

#include "RenderDevice.h"

#define MFA_OALRENDERDEVICE_TYPE_NAME u8"oal"

#ifdef __cplusplus
extern "C"
{
#endif

	/// <summary>
	///		Creates a new OpenAL render device.
	/// </summary>
	/// <param name="renderDevice">Pointer to render device handle</param>
	/// <param name="window">Window handle</param>
	/// <param name="desc">Render device description</param>
	/// <param name="allocator">Allocator to use on allocations</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mfaCreateOALRenderDevice(mfaRenderDevice** renderDevice, void* allocator);

	/// <summary>
	///		Destroys an OpenAL render device.
	/// </summary>
	/// <param name="renderDevice">Render device handle</param>
	void mfaDestroyOALRenderDevice(void* renderDevice);

#ifdef __cplusplus
}
#endif
