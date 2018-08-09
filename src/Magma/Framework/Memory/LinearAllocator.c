#include "LinearAllocator.h"

mfmError mfmCreateLinearAllocator(mfmLinearAllocator ** linearAllocator, mfmU64 size)
{
	// Check if the arguments are valid
	if (linearAllocator == NULL || size == 0)
		return MFM_ERROR_INVALID_ARGUMENTS;
	
	// Allocate memory for the linear allocator
	mfmU8 * memory = (mfmU8*)malloc(sizeof(mfmLinearAllocator) + size);
	if (memory == NULL)
		return MFM_ERROR_ALLOCATION_FAILED;

	// Get data pointers
	*linearAllocator = (mfmLinearAllocator*)(memory + 0);
	(*linearAllocator)->size = size;
	(*linearAllocator)->begin = memory + sizeof(mfmLinearAllocator);
	(*linearAllocator)->head = memory + sizeof(mfmLinearAllocator);

	// Set destructor function
	(*linearAllocator)->object.destructorFunc = &mfmDestroyLinearAllocator;

	// Successfully created a linear allocator
	return MFM_ERROR_OKAY;
}

void mfmDestroyLinearAllocator(void * linearAllocator)
{
	free(linearAllocator);
}

mfmError mfmLinearAllocate(mfmLinearAllocator * linearAllocator, void ** memory, mfmU64 size)
{
	// Check if allocation fits on the linear buffer
	if (linearAllocator->head + size > linearAllocator->begin + linearAllocator->size)
		return MFM_ERROR_ALLOCATOR_OVERFLOW;

	// Get stack head and move it
	*memory = linearAllocator->head;
	linearAllocator->head += size;
	return MFM_ERROR_OKAY;
}

void mfmLinearReset(mfmLinearAllocator * linearAllocator)
{
	// Reset the linear allocator head
	linearAllocator->head = linearAllocator->begin;
}
