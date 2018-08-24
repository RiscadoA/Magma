#include "RenderDevice.hpp"

#include "../Exception.hpp"
#include "../ErrorString.h"

Magma::Framework::Graphics::V2X::RenderDevice Magma::Framework::Graphics::V2X::CreateRenderDevice(const mfsUTF8CodeUnit * type, Input::Window window, const mfgV2XRenderDeviceDesc * desc, Memory::Allocator allocator)
{
	mfgV2XRenderDevice* rd = NULL;
	mfError err = mfgV2XCreateRenderDevice(type, &rd, reinterpret_cast<mfiWindow*>(&window.Get()), desc, allocator.GetNoChecks());
	if (err != MF_ERROR_OKAY)
		throw RenderDeviceError(mfgErrorToString(err));
	return rd;
}
