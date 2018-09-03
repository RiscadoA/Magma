#include "RenderDevice.hpp"
#include "Config.h"

#include "../Exception.hpp"
#include "../ErrorString.h"

void Magma::Framework::Graphics::V2X::RenderDevice::ClearColor(mfmF32 r, mfmF32 g, mfmF32 b, mfmF32 a)
{
	mfError err = mfgV2XClearColor(reinterpret_cast<mfgV2XRenderDevice*>(&this->Get()), r, g, b, a);
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (err != MF_ERROR_OKAY)
		throw RenderDeviceError(mfgErrorToString(err));
#endif
}

void Magma::Framework::Graphics::V2X::RenderDevice::SwapBuffers()
{
	mfError err = mfgV2XSwapBuffers(reinterpret_cast<mfgV2XRenderDevice*>(&this->Get()));
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (err != MF_ERROR_OKAY)
		throw RenderDeviceError(mfgErrorToString(err));
#endif
}

Magma::Framework::Graphics::V2X::RenderDevice Magma::Framework::Graphics::V2X::CreateRenderDevice(const mfsUTF8CodeUnit * type, Input::WindowHandle window, const RenderDeviceDesc * desc, Memory::AllocatorHandle allocator)
{
	if (desc == NULL)
		throw RenderDeviceError("Failed to create render device:\nThe render device description pointer is NULL");

	if (type == NULL)
	{
#if defined(MAGMA_FRAMEWORK_USE_DIRECTX)
		type = u8"d3d11";
#elif defined(MAGMA_FRAMEWORK_USE_OPENGL)
		type = u8"ogl4";
#endif
	}

	mfgV2XRenderDeviceDesc cDesc;
	mfgV2XDefaultRenderDeviceDesc(&cDesc);
	cDesc.vsyncEnabled = (desc->vsyncEnabled ? MFM_TRUE : MFM_FALSE);

	mfgV2XRenderDevice* rd = NULL;
	mfError err = mfgV2XCreateRenderDevice(type, &rd, reinterpret_cast<mfiWindow*>(&window.Get()), &cDesc, allocator.GetNoChecks());
	if (err != MF_ERROR_OKAY)
		throw RenderDeviceError(mfgErrorToString(err));
	return rd;
}
