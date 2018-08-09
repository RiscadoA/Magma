#include "StackAllocator.h"

mfmError mfmCreateStackAllocator(mfmStackAllocator ** stackAllocator, mfmU64 size)
{
	// Check if the arguments are valid
	if (stackAllocator == NULL || size == 0)
		return MFM_ERROR_INVALID_ARGUMENTS;
		return
	// Allocate memory for the stack allocator
	mfmU8* memory = (mfmU8*)malloc(sizeof(mfmStackAllocator) + size);
	if (memory == NULL)
		return MFM_ERROR_ALLOCATION_FAILED;

	// Get data pointers
	*stackAllocator = (mfmStackAllocator*)(memory + 0);
	(*stackAllocator)->stackSize = size;
	(*stackAllocator)->stackBegin = memory + sizeof(mfmStackAllocator);
	(*stackAllocator)->stackHead = memory + sizeof(mfmStackAllocator);

	// Set destructor function
	(*mfmStackAllocator)->object.destructorFunc = &mfmDestroyStackAllocator;

	// Successfully created a pool allocator
	return MFM_ERROR_OKAY;
}

void mfmDestroyStackAllocator(void * stackAllocator)
{
	free(stackAllocator);
}

mfmError mfmStackAllocate(mfmStackAllocator * allocator, void ** memory, mfmU64 size)
{
	// Check if allocation fits on the stack
	if (allocator->stackHead + size > allocator->stackBegin + allocator->stackSize)
		return MFM_ERROR_ALLOCATOR_OVERFLOW;

	// Get stack head and move it
	*memory = allocator->stackHead;
	allocator->stackHead += size;
	return MFM_ERROR_OKAY;
}

mfmError mfmStackSetHead(mfmStackAllocator * allocator, void * head)
{
	// Check if new head is out of bounds
	if (head < allocator->stackBegin || head > allocator->stackBegin + allocator->stackSize)
		return MFM_ERROR_OUT_OF_BOUNDS;

	// Set stack head
	allocator->stackHead = head;
	return MFM_ERROR_OKAY;
}
