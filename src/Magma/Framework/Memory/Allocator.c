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
