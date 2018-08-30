#include "LinearAllocator.h"

#include <stdlib.h>

mfError mfmInternalLinearAllocate(void* allocator, void** memory, mfmU64 size)
{
	return mfmLinearAllocate((mfmLinearAllocator*)allocator, memory, size);
}

mfError mfmInternalLinearDeallocate(void* allocator, void* memory)
{
	return MFM_ERROR_UNSUPPORTED_FUNCTION;
}

mfError mfmCreateLinearAllocator(mfmLinearAllocator ** linearAllocator, mfmU64 size)
{
	// Check if the arguments are valid
	if (linearAllocator == NULL || size == 0)
		return MFM_ERROR_INVALID_ARGUMENTS;
	
	// Allocate memory for the linear allocator
	mfmU8 * memory = NULL;
	if (mfmAllocate(NULL, &memory, sizeof(mfmLinearAllocator) + size) != MF_ERROR_OKAY)
		return MFM_ERROR_ALLOCATION_FAILED;

	// Get data pointers
	*linearAllocator = (mfmLinearAllocator*)(memory + 0);
	(*linearAllocator)->onMemory = MFM_FALSE;
	(*linearAllocator)->size = size;
	(*linearAllocator)->begin = memory + sizeof(mfmLinearAllocator);
	(*linearAllocator)->head = memory + sizeof(mfmLinearAllocator);

	// Set functions
	(*linearAllocator)->base.allocate = &mfmInternalLinearAllocate;
	(*linearAllocator)->base.deallocate = &mfmInternalLinearDeallocate;

	// Set destructor function
	(*linearAllocator)->base.object.destructorFunc = &mfmDestroyLinearAllocator;

	// Successfully created a linear allocator
	return MF_ERROR_OKAY;
}

mfError mfmCreateLinearAllocatorOnMemory(mfmLinearAllocator ** linearAllocator, mfmU64 size, void * memory, mfmU64 memorySize)
{
	// Check if the arguments are valid
	if (linearAllocator == NULL || size == 0 || memory == NULL || memorySize < sizeof(mfmLinearAllocator) + size)
		return MFM_ERROR_INVALID_ARGUMENTS;

	// Get data pointers
	*linearAllocator = (mfmLinearAllocator*)((mfmU8*)memory + 0);
	(*linearAllocator)->onMemory = MFM_TRUE;
	(*linearAllocator)->size = size;
	(*linearAllocator)->begin = (mfmU8*)memory + sizeof(mfmLinearAllocator);
	(*linearAllocator)->head = (mfmU8*)memory + sizeof(mfmLinearAllocator);

	// Set functions
	(*linearAllocator)->base.allocate = &mfmInternalLinearAllocate;
	(*linearAllocator)->base.deallocate = &mfmInternalLinearDeallocate;

	// Set destructor function
	(*linearAllocator)->base.object.destructorFunc = &mfmDestroyLinearAllocator;

	// Successfully created a linear allocator
	return MF_ERROR_OKAY;
}

void mfmDestroyLinearAllocator(void * linearAllocator)
{
	if (linearAllocator == NULL)
		abort();
	if (((mfmLinearAllocator*)linearAllocator)->onMemory == MFM_FALSE)
		if (mfmDeallocate(NULL, linearAllocator) != MF_ERROR_OKAY)
			abort();
}

mfError mfmLinearAllocate(mfmLinearAllocator * linearAllocator, void ** memory, mfmU64 size)
{
	if (linearAllocator == NULL || memory == NULL || size == 0)
		return MFM_ERROR_INVALID_ARGUMENTS;
	// Check if allocation fits on the linear buffer
	if (linearAllocator->head + size > linearAllocator->begin + linearAllocator->size)
		return MFM_ERROR_ALLOCATOR_OVERFLOW;

	// Get stack head and move it
	*memory = linearAllocator->head;
	linearAllocator->head += size;
	return MF_ERROR_OKAY;
}

mfError mfmLinearReset(mfmLinearAllocator * linearAllocator)
{
	if (linearAllocator == NULL)
		return MFM_ERROR_INVALID_ARGUMENTS;
	// Reset the linear allocator head
	linearAllocator->head = linearAllocator->begin;
}
