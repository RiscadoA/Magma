#include "PoolAllocator.h"

#include <string.h>

mfmI32 mfmCreatePoolAllocator(mfmPoolAllocator ** poolAllocator, mfmPoolAllocatorDesc* desc)
{
	// Check if the arguments are valid
	if (poolAllocator == NULL || desc == NULL)
		return MFM_ERROR_INVALID_ARGUMENTS;

	// Allocate memory for the pool allocator
	mfmU8* memory = (mfmU8*)malloc(sizeof(mfmPoolAllocator) + desc->slotCount * sizeof(mfmBool) + desc->slotSize * desc->slotCount);
	if (memory == NULL)
		return MFM_ERROR_ALLOCATION_FAILED;

	// Get description
	memcpy(&(*poolAllocator)->desc, desc, sizeof(mfmPoolAllocatorDesc));

	// Get data pointers
	*poolAllocator = (mfmPoolAllocator*)(memory + 0);
	(*poolAllocator)->slotStatesPtr = memory + sizeof(mfmPoolAllocator);
	(*poolAllocator)->slotDataPtr = memory + sizeof(mfmPoolAllocator) + desc->slotCount * sizeof(mfmBool);

	// Set destructor function
	(*poolAllocator)->object.destructorFunc = &mfmDestroyPoolAllocator;

	// Initialize slot states to empty
	memset((*poolAllocator)->slotDataPtr, MFM_FALSE, desc->slotCount * sizeof(mfmBool));

	// Successfully created a pool allocator
	return MFM_ERROR_OKAY;
}

void mfmDestroyPoolAllocator(void * poolAllocator)
{
	// Free the pool allocator
	free(poolAllocator);
}

mfmI32 mfmPoolAllocate(void ** memory, mfmPoolAllocator * allocator, mfmU64 size)
{
	// Check if the allocation size fits on a pool slot
	if (size > allocator->desc.slotSize)
		return MFM_ERROR_ALLOCATION_TOO_BIG;

	// Search for a free slot
	for (mfmU64 i = 0; i < allocator->desc.slotCount; ++i)
		// If this slot is free
		if (allocator->slotStatesPtr[i] == MFM_FALSE)
		{
			allocator->slotStatesPtr[i] = MFM_TRUE;
			*memory = allocator->slotDataPtr + allocator->desc.slotSize * i;
			return MFM_ERROR_OKAY;
		}

	// No free slots
	return MFM_ERROR_ALLOCATOR_OVERFLOW;
}

mfmI32 mfmPoolDeallocate(void * memory, mfmPoolAllocator * allocator)
{
	return MFM_ERROR_OKAY;
}
