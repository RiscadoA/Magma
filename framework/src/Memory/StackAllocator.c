#include <Magma/Framework/Memory/StackAllocator.h>

#include <stdlib.h>

mfError mfmInternalStackAllocate(void* allocator, void** memory, mfmU64 size)
{
	return mfmStackAllocate((mfmStackAllocator*)allocator, memory, size);
}

mfError mfmInternalStackDeallocate(void* allocator, void* memory)
{
	return MF_ERROR_OKAY;
	// return mfmStackSetHead(allocator, memory);
}

mfError mfmInternalStackReallocate(void* allocator, void* memory, mfmU64 prevSize, mfmU64 size, void** newMemory)
{
	mfmStackAllocator* stackAllocator = allocator;

	if (stackAllocator->stackHead - prevSize == memory)
	{
		if (size >= prevSize)
		{
			if (stackAllocator->stackHead - stackAllocator->stackBegin + (size - prevSize) > stackAllocator->stackSize)
				return MFM_ERROR_ALLOCATOR_OVERFLOW;
			stackAllocator->stackHead += size - prevSize;
		}
		else
		{
			if (stackAllocator->stackHead - (prevSize - size) < stackAllocator->stackBegin)
				return MFM_ERROR_ALLOCATOR_UNDERFLOW;;
			stackAllocator->stackHead -= prevSize - size;
		}
		*newMemory = stackAllocator->stackHead;
		return MF_ERROR_OKAY;
	}
	else return mfmStackAllocate(allocator, newMemory, size);
}

mfError mfmCreateStackAllocator(mfmStackAllocator ** stackAllocator, mfmU64 size)
{
	// Check if the arguments are valid
	if (stackAllocator == NULL || size == 0)
		return MFM_ERROR_INVALID_ARGUMENTS;
	
	// Allocate memory for the stack allocator
	mfmU8 * memory = NULL;
	if (mfmAllocate(NULL, &memory, sizeof(mfmStackAllocator) + size) != MF_ERROR_OKAY)
		return MFM_ERROR_ALLOCATION_FAILED;

	// Get data pointers
	*stackAllocator = (mfmStackAllocator*)(memory + 0);
	(*stackAllocator)->onMemory = MFM_FALSE;
	(*stackAllocator)->stackSize = size;
	(*stackAllocator)->stackBegin = memory + sizeof(mfmStackAllocator);
	(*stackAllocator)->stackHead = memory + sizeof(mfmStackAllocator);

	// Set functions
	(*stackAllocator)->base.allocate = &mfmInternalStackAllocate;
	(*stackAllocator)->base.deallocate = &mfmInternalStackDeallocate;
	(*stackAllocator)->base.reallocate = &mfmInternalStackReallocate;

	// Set destructor function
	(*stackAllocator)->base.object.destructorFunc = &mfmDestroyStackAllocator;

	// Successfully created a stack allocator
	return MF_ERROR_OKAY;
}

mfError mfmCreateStackAllocatorOnMemory(mfmStackAllocator ** stackAllocator, mfmU64 size, void * memory, mfmU64 memorySize)
{
	// Check if the arguments are valid
	if (stackAllocator == NULL || size == 0 || memory == NULL || memorySize < sizeof(mfmStackAllocator) + size)
		return MFM_ERROR_INVALID_ARGUMENTS;

	// Get data pointers
	*stackAllocator = (mfmStackAllocator*)((mfmU8*)memory + 0);
	(*stackAllocator)->onMemory = MFM_TRUE;
	(*stackAllocator)->stackSize = size;
	(*stackAllocator)->stackBegin = (mfmU8*)memory + sizeof(mfmStackAllocator);
	(*stackAllocator)->stackHead = (mfmU8*)memory + sizeof(mfmStackAllocator);

	// Set functions
	(*stackAllocator)->base.allocate = &mfmInternalStackAllocate;
	(*stackAllocator)->base.deallocate = &mfmInternalStackDeallocate;
	(*stackAllocator)->base.reallocate = &mfmInternalStackReallocate;

	// Set destructor function
	(*stackAllocator)->base.object.destructorFunc = &mfmDestroyStackAllocator;

	// Successfully created a stack allocator
	return MF_ERROR_OKAY;
}

void mfmDestroyStackAllocator(void * stackAllocator)
{
	if (((mfmStackAllocator*)stackAllocator)->onMemory == MFM_FALSE)
		if (mfmDeallocate(NULL, stackAllocator) != MF_ERROR_OKAY)
			abort();
}

mfError mfmStackAllocate(mfmStackAllocator * allocator, void ** memory, mfmU64 size)
{
	// Check if allocation fits on the stack
	if (allocator->stackHead + size > allocator->stackBegin + allocator->stackSize)
		return MFM_ERROR_ALLOCATOR_OVERFLOW;

	// Get stack head and move it
	*memory = allocator->stackHead;
	allocator->stackHead += size;
	return MF_ERROR_OKAY;
}

mfError mfmStackSetHead(mfmStackAllocator * allocator, void * head)
{
	// Check if new head is out of bounds
	if (head < allocator->stackBegin || head > allocator->stackBegin + allocator->stackSize)
		return MFM_ERROR_OUT_OF_BOUNDS;

	// Set stack head
	allocator->stackHead = head;
	return MF_ERROR_OKAY;
}
