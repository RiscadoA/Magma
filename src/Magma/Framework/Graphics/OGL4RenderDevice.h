#pragma once

#include "RenderDevice.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/// <summary>
	///		Creates a new OpenGL 4 render device.
	/// </summary>
	/// <param name="renderDevice">Pointer to render device handle</param>
	/// <param name="desc">Render device description</param>
	/// <param name="allocator">Allocator to use on allocations</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns an error code.
	/// </returns>
	mfgError mfgCreateOGL4RenderDevice(mfgRenderDevice** renderDevice, const mfgRenderDeviceDesc* desc, void* allocator);

	/// <summary>
	///		Destroys an OpenGL 4 render device.
	/// </summary>
	/// <param name="renderDevice">Render device handle</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns an error code.
	/// </returns>
	mfgError mfgDestroyOGL4RenderDevice(mfgRenderDevice* renderDevice);

#ifdef __cplusplus
}
#endif
