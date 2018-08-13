#include "Allocator.h"

#include <stdlib.h>

mfmError mfmAllocate(void * allocator, void ** memory, mfmU64 size)
{
	if (memory == NULL || size == 0)
		return MFM_ERROR_INVALID_ARGUMENTS;

	if (allocator == NULL)
	{
		*memory = malloc(size);
		if (*memory == NULL)
			return MFM_ERROR_ALLOCATION_FAILED;
		else
			return MFM_ERROR_OKAY;
	}
	else
		return ((mfmAllocator*)allocator)->allocate(allocator, memory, size);
}

mfmError mfmDeallocate(void * allocator, void * memory)
{
	if (memory == NULL)
		return MFM_ERROR_INVALID_ARGUMENTS;

	if (allocator == NULL)
	{
		free(memory);
		return MFM_ERROR_OKAY;
	}
	else
		return ((mfmAllocator*)allocator)->deallocate(allocator, memory);
}
