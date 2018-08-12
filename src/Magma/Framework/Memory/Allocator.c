#include "Allocator.h"

mfmError mfmAllocate(void * allocator, void ** memory, mfmU64 size)
{
	return ((mfmAllocator*)allocator)->allocate(allocator, memory, size);
}

mfmError mfmDeallocate(void * allocator, void * memory)
{
	return ((mfmAllocator*)allocator)->deallocate(allocator, memory);
}
