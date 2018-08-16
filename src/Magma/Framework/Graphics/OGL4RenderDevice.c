#include "OGL4RenderDevice.h"
	
#include "../Memory/PoolAllocator.h"

#include <stdlib.h>

typedef struct
{
	mfgRenderDevice base;
	void* allocator;
	mfmPoolAllocator* pool;
	mfmU8 poolMemory[280000];
} mfgOGL4RenderDevice;

mfgError mfgCreateOGL4RenderDevice(mfgRenderDevice ** renderDevice, mfiWindow* window, const mfgRenderDeviceDesc * desc, void * allocator)
{
	// Check if params are valid
	if (renderDevice == NULL || window == NULL || desc == NULL || window->type != MFI_OGLWINDOW)
		return MFG_ERROR_INVALID_ARGUMENTS;
	
	// Allocate render device
	mfgOGL4RenderDevice* rd;
	if (mfmAllocate(allocator, &rd, sizeof(mfgOGL4RenderDevice)) != MFM_ERROR_OKAY)
		return MFG_ERROR_ALLOCATION_FAILED;

	// Create pool allocator
	{
		mfmPoolAllocatorDesc desc;
		desc.expandable = MFM_FALSE;
		desc.slotCount = 8192;
		desc.slotSize = 32;
		mfmError err = mfmCreatePoolAllocatorOnMemory(&rd->pool, &desc, rd->poolMemory, sizeof(rd->poolMemory));
		if (err != MFM_ERROR_OKAY)
			return MFG_ERROR_ALLOCATION_FAILED;
	}

	// Initialize some properties
	rd->base.object.destructorFunc = &mfgDestroyOGL4RenderDevice;
	rd->base.object.referenceCount = 0;
	rd->allocator = allocator;

	*renderDevice = rd;
	return MFG_ERROR_OKAY;
}

void mfgDestroyOGL4RenderDevice(void * renderDevice)
{
	mfgOGL4RenderDevice* rd = (mfgOGL4RenderDevice*)renderDevice;

	// Destroy pool allocator
	mfmDestroyPoolAllocator(rd->pool);

	// Deallocate render device
	if (mfmDeallocate(rd->allocator, rd) != MFM_ERROR_OKAY)
		abort();

	// TO DO
	return MFG_ERROR_OKAY;
}
