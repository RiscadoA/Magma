#include "Allocator.h"

#include <stdlib.h>

mfError mfmAllocate(void * allocator, void ** memory, mfmU64 size)
{
	if (memory == NULL || size == 0)
		return MFM_ERROR_INVALID_ARGUMENTS;

	if (allocator == NULL)
	{
		*memory = malloc(size);
		if (*memory == NULL)
			return MFM_ERROR_ALLOCATION_FAILED;
		else
			return MF_ERROR_OKAY;
	}
	else
	{
		mfError err = ((mfmAllocator*)allocator)->allocate(allocator, memory, size);
		return err;
	}
}

mfError mfmDeallocate(void * allocator, void * memory)
{
	if (memory == NULL)
		return MFM_ERROR_INVALID_ARGUMENTS;

	if (allocator == NULL)
	{
		free(memory);
		return MF_ERROR_OKAY;
	}
	else
	{
		mfError err = ((mfmAllocator*)allocator)->deallocate(allocator, memory);
		return err;
	}
}

mfError mfmReallocate(void * allocator, void * memory, mfmU64 size, void ** newMemory)
{
	if (memory == NULL || newMemory  == NULL || size == 0)
		return MFM_ERROR_INVALID_ARGUMENTS;

	if (allocator == NULL)
	{
		*newMemory = realloc(memory, size);
		if (*newMemory == NULL)
			return MFM_ERROR_ALLOCATION_FAILED;
		else
			return MF_ERROR_OKAY;
	}
	else
	{
		mfError err = ((mfmAllocator*)allocator)->reallocate(allocator, memory, size, newMemory);
		return err;
	}
}

mfError mfmAllocateAligned(void * allocator, void ** memory, mfmU64 size, mfmU64 alignment)
{
	if (memory == NULL || size == 0 || alignment == 0)
		return MFM_ERROR_INVALID_ARGUMENTS;

	mfmU64 realSize = size + alignment;
	void* realMemory;

	mfError err = mfmAllocate(allocator, &realMemory, realSize);
	if (err != MF_ERROR_OKAY)
		return err;

	mfmU64 remainder = ((mfmU64)realMemory) % alignment;
	mfmU64 offset = alignment - remainder;

	*memory = (mfmU8*)realMemory + offset;
	((mfmU8*)*memory)[-1] = offset;

	return MF_ERROR_OKAY;
}

mfError mfmDeallocateAligned(void * allocator, void * memory)
{
	mfmU64 offset = ((mfmU8*)memory)[-1];
	void* realMemory = (mfmU8*)memory - offset;
	return mfmDeallocate(allocator, realMemory);
}
