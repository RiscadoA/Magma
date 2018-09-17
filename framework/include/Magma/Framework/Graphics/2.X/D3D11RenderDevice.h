#pragma once

#include "RenderDevice.h"

#include "../../Input/D3DWindow.h"

#define MFG_D3D11RENDERDEVICE_TYPE_NAME u8"d3d11"

#ifdef __cplusplus
extern "C"
{
#endif

	/// <summary>
	///		Creates a new Direct3D 11 render device.
	/// </summary>
	/// <param name="renderDevice">Pointer to render device handle</param>
	/// <param name="window">WindowHandle handle</param>
	/// <param name="desc">Render device description</param>
	/// <param name="allocator">AllocatorHandle to use on allocations</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mfgV2XCreateD3D11RenderDevice(mfgV2XRenderDevice** renderDevice, mfiWindow* window, const mfgV2XRenderDeviceDesc* desc, void* allocator);

	/// <summary>
	///		Destroys a Direct3D 11 render device.
	/// </summary>
	/// <param name="renderDevice">Render device handle</param>
	void mfgV2XDestroyD3D11RenderDevice(void* renderDevice);

#ifdef __cplusplus
}
#endif
