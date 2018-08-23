#pragma once

#include "RenderDevice.h"

#include "../../Input/GLWindow.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/// <summary>
	///		Creates a new OpenGL 4 render device.
	/// </summary>
	/// <param name="renderDevice">Pointer to render device handle</param>
	/// <param name="window">Window handle</param>
	/// <param name="desc">Render device description</param>
	/// <param name="allocator">Allocator to use on allocations</param>
	/// <returns>
	///		MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mfgV2XCreateOGL4RenderDevice(mfgV2XRenderDevice** renderDevice, mfiWindow* window, const mfgV2XRenderDeviceDesc* desc, void* allocator);

	/// <summary>
	///		Destroys an OpenGL 4 render device.
	/// </summary>
	/// <param name="renderDevice">Render device handle</param>
	void mfgV2XDestroyOGL4RenderDevice(void* renderDevice);

#ifdef __cplusplus
}
#endif
