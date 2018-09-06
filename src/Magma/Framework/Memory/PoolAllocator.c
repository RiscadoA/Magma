#include "PoolAllocator.h"

#include <string.h>
#include <stdlib.h>

mfError mfmInternalPoolAllocate(void* allocator, void** memory, mfmU64 size)
{
	return mfmPoolAllocate((mfmPoolAllocator*)allocator, memory, size);
}

mfError mfmInternalPoolDeallocate(void* allocator, void* memory)
{
	return mfmPoolDeallocate((mfmPoolAllocator*)allocator, memory);
}

mfError mfmInternalPoolReallocate(void* allocator, void* memory, mfmU64 prevSize, mfmU64 size, void** newMemory)
{
	mfmPoolAllocator* poolAllocator = allocator;

	// Check if the arguments are valid
	if (size > poolAllocator->desc.slotSize)
		return MFM_ERROR_ALLOCATION_TOO_BIG;

	*newMemory = memory;
	return MF_ERROR_OKAY;
}

mfError mfmCreatePoolAllocator(mfmPoolAllocator ** poolAllocator, const mfmPoolAllocatorDesc* desc)
{
	// Check if the arguments are valid
	if (poolAllocator == NULL || desc == NULL)
		return MFM_ERROR_INVALID_ARGUMENTS;

	// Allocate memory for the pool allocator and its first chunk
	mfmU8 * memory = NULL;
	if (mfmAllocate(NULL, &memory, sizeof(mfmPoolAllocator) + sizeof(mfmPoolAllocatorChunk) + desc->slotCount * sizeof(mfmBool) + desc->slotSize * desc->slotCount) != MF_ERROR_OKAY)
		return MFM_ERROR_ALLOCATION_FAILED;

	// Get data pointers
	*poolAllocator = (mfmPoolAllocator*)(memory + 0);
	(*poolAllocator)->firstChunk = (mfmPoolAllocatorChunk*)(memory + sizeof(mfmPoolAllocator));
	(*poolAllocator)->firstChunk->slotStatesPtr = memory + sizeof(mfmPoolAllocator) + sizeof(mfmPoolAllocatorChunk);
	(*poolAllocator)->firstChunk->slotDataPtr = memory + sizeof(mfmPoolAllocator) + sizeof(mfmPoolAllocatorChunk) + desc->slotCount * sizeof(mfmBool);
	(*poolAllocator)->firstChunk->next = NULL;
	(*poolAllocator)->currentFreeSlotCount = desc->slotCount;
	(*poolAllocator)->currentSlotCount = desc->slotCount;
	(*poolAllocator)->currentChunkCount = 1;
	(*poolAllocator)->onMemory = MFM_FALSE;

	// Get description
	memcpy(&(*poolAllocator)->desc, desc, sizeof(mfmPoolAllocatorDesc));

	// Set functions
	(*poolAllocator)->base.allocate = &mfmInternalPoolAllocate;
	(*poolAllocator)->base.deallocate = &mfmInternalPoolDeallocate;
	(*poolAllocator)->base.reallocate = &mfmInternalPoolReallocate;

	// Set destructor function
	(*poolAllocator)->base.object.destructorFunc = &mfmDestroyPoolAllocator;

	// Initialize slot states to empty
	memset((*poolAllocator)->firstChunk->slotStatesPtr, MFM_FALSE, desc->slotCount * sizeof(mfmBool));

	// Successfully created a pool allocator
	return MF_ERROR_OKAY;
}

mfError mfmCreatePoolAllocatorOnMemory(mfmPoolAllocator ** poolAllocator, const mfmPoolAllocatorDesc * desc, void * memPtr, mfmU64 memSize)
{
	// Check if the arguments are valid
	if (poolAllocator == NULL || desc == NULL || desc->expandable == MFM_TRUE)
		return MFM_ERROR_INVALID_ARGUMENTS;

	// Check if there is enough size for the pool
	if (sizeof(mfmPoolAllocator) + sizeof(mfmPoolAllocatorChunk) + desc->slotCount * sizeof(mfmBool) + desc->slotSize * desc->slotCount > memSize)
		return MFM_ERROR_INVALID_ARGUMENTS;

	// Allocate memory for the pool allocator and its first chunk
	mfmU8* memory = (mfmU8*)memPtr;
	if (memory == NULL)
		return MFM_ERROR_ALLOCATION_FAILED;

	// Get data pointers
	*poolAllocator = (mfmPoolAllocator*)(memory + 0);
	(*poolAllocator)->firstChunk = (mfmPoolAllocatorChunk*)(memory + sizeof(mfmPoolAllocator));
	(*poolAllocator)->firstChunk->slotStatesPtr = memory + sizeof(mfmPoolAllocator) + sizeof(mfmPoolAllocatorChunk);
	(*poolAllocator)->firstChunk->slotDataPtr = memory + sizeof(mfmPoolAllocator) + sizeof(mfmPoolAllocatorChunk) + desc->slotCount * sizeof(mfmBool);
	(*poolAllocator)->firstChunk->next = NULL;
	(*poolAllocator)->currentFreeSlotCount = desc->slotCount;
	(*poolAllocator)->currentSlotCount = desc->slotCount;
	(*poolAllocator)->currentChunkCount = 1;
	(*poolAllocator)->onMemory = MFM_TRUE;

	// Get description
	memcpy(&(*poolAllocator)->desc, desc, sizeof(mfmPoolAllocatorDesc));

	// Set functions
	(*poolAllocator)->base.allocate = &mfmInternalPoolAllocate;
	(*poolAllocator)->base.deallocate = &mfmInternalPoolDeallocate;
	(*poolAllocator)->base.reallocate = &mfmInternalPoolReallocate;

	// Set destructor function
	(*poolAllocator)->base.object.destructorFunc = &mfmDestroyPoolAllocator;

	// Initialize slot states to empty
	memset((*poolAllocator)->firstChunk->slotStatesPtr, MFM_FALSE, desc->slotCount * sizeof(mfmBool));

	// Successfully created a pool allocator
	return MF_ERROR_OKAY;
}

void mfmDestroyPoolAllocator(void * poolAllocator)
{
	// Free extra pool allocator chunks
	mfmPoolAllocatorChunk* chunk = ((mfmPoolAllocator*)poolAllocator)->firstChunk->next;
	while (chunk != NULL)
	{
		mfmPoolAllocatorChunk* next = (mfmPoolAllocatorChunk*)chunk->next;
		if (mfmDeallocate(NULL, chunk) != MF_ERROR_OKAY)
			abort();
		chunk = next;
	}

	// Free the pool allocator and its main chunk
	if (((mfmPoolAllocator*)poolAllocator)->onMemory == MFM_FALSE)
		if (mfmDeallocate(NULL, poolAllocator) != MF_ERROR_OKAY)
			abort();
}

mfError mfmExpandPoolAllocator(mfmPoolAllocator* poolAllocator, mfmPoolAllocatorChunk** newChunk)
{
	// Allocate memory for the chunk
	mfmU8 * memory = NULL;
	if (mfmAllocate(NULL, &memory, sizeof(mfmPoolAllocatorChunk) + poolAllocator->desc.slotCount * sizeof(mfmBool) + poolAllocator->desc.slotSize * poolAllocator->desc.slotCount) != MF_ERROR_OKAY)
		return MFM_ERROR_ALLOCATION_FAILED;

	// Get data pointers
	mfmPoolAllocatorChunk* chunk = (mfmPoolAllocatorChunk*)(memory + 0);
	chunk->slotStatesPtr = memory + sizeof(mfmPoolAllocatorChunk);
	chunk->slotDataPtr = memory + sizeof(mfmPoolAllocatorChunk) + poolAllocator->desc.slotCount * sizeof(mfmBool);
	chunk->next = NULL;

	// Initialize slot states to empty
	memset(chunk->slotStatesPtr, MFM_FALSE, poolAllocator->desc.slotCount * sizeof(mfmBool));

	// Add chunk to pool
	mfmPoolAllocatorChunk* ck = ((mfmPoolAllocator*)poolAllocator)->firstChunk;
	while (ck->next != NULL)
		ck = ck->next;
	ck->next = chunk;
	++poolAllocator->currentChunkCount;
	poolAllocator->currentFreeSlotCount += poolAllocator->desc.slotCount;
	poolAllocator->currentSlotCount += poolAllocator->desc.slotCount;

	// Set out chunk
	if (newChunk != NULL)
		*newChunk = chunk;
	return MF_ERROR_OKAY;
}

mfError mfmPoolChunkAllocate(mfmPoolAllocator * allocator, mfmPoolAllocatorChunk * chunk, void ** memory, mfmU64 size)
{
	// Check if the allocation size fits on a pool slot
	if (size > allocator->desc.slotSize)
		return MFM_ERROR_ALLOCATION_TOO_BIG;

	// Search for a free slot
	for (mfmU64 i = 0; i < allocator->desc.slotCount; ++i)
		// If this slot is free
		if (chunk->slotStatesPtr[i] == MFM_FALSE)
		{
			chunk->slotStatesPtr[i] = MFM_TRUE;
			*memory = chunk->slotDataPtr + allocator->desc.slotSize * i;
			--allocator->currentFreeSlotCount;
			return MF_ERROR_OKAY;
		}

	// No free slots
	return MFM_ERROR_ALLOCATOR_OVERFLOW;
}

mfError mfmPoolChunkDeallocate(void * memory, mfmPoolAllocator * allocator, mfmPoolAllocatorChunk * chunk)
{
	// Get slot index from memory pointer
	mfmU64 slotIndex = ((mfmU64)memory - (mfmU64)chunk->slotDataPtr) / allocator->desc.slotSize;

	// Check if slot index is out of bounds
	if (slotIndex >= allocator->desc.slotCount)
		return MFM_ERROR_OUT_OF_BOUNDS;

	// Free slot
	++allocator->currentFreeSlotCount;
	chunk->slotStatesPtr[slotIndex] = MFM_FALSE;
	return MF_ERROR_OKAY;
}

mfError mfmPoolAllocate(mfmPoolAllocator * allocator, void ** memory, mfmU64 size)
{
	// Check if the allocation size fits on a pool slot
	if (size > allocator->desc.slotSize)
		return MFM_ERROR_ALLOCATION_TOO_BIG;

	// Search for a free slot
	mfmPoolAllocatorChunk* ck = allocator->firstChunk;
	while (ck != NULL)
	{
		mfError err = mfmPoolChunkAllocate(allocator, ck, memory, size);
		if (err == MF_ERROR_OKAY)
			return MF_ERROR_OKAY;
		else if (err != MFM_ERROR_ALLOCATOR_OVERFLOW)
			return err;
		ck = (mfmPoolAllocatorChunk*)ck->next;
	}

	// No free slots
	// If the pool is expandable, expand it
	if (allocator->desc.expandable)
	{
		mfError err = mfmExpandPoolAllocator(allocator, &ck);
		if (err != MF_ERROR_OKAY)
			return err;
		else
			return mfmPoolChunkAllocate(allocator, ck, memory, size);
	}
	// Otherwise return error
	else return MFM_ERROR_ALLOCATOR_OVERFLOW;
}

mfError mfmPoolDeallocate(mfmPoolAllocator * allocator, void * memory)
{
	// Search for the chunk where the memory was allocated on
	mfmPoolAllocatorChunk* ck = allocator->firstChunk;
	while (ck != NULL)
	{
		mfError err = mfmPoolChunkDeallocate(memory, allocator, ck);
		if (err == MF_ERROR_OKAY)
			return MF_ERROR_OKAY;
		else if (err != MFM_ERROR_OUT_OF_BOUNDS)
			return err;
		ck = (mfmPoolAllocatorChunk*)ck->next;
	}

	return MFM_ERROR_OUT_OF_BOUNDS;
}

mfmU64 mfmPoolGetSlotCount(mfmPoolAllocator * allocator)
{
	return allocator->currentSlotCount;
}

mfmU64 mfmPoolGetChunkCount(mfmPoolAllocator * allocator)
{
	return allocator->currentChunkCount;
}

mfmU64 mfmPoolGetOccupiedSlotCount(mfmPoolAllocator * allocator)
{
	return allocator->currentSlotCount - allocator->currentFreeSlotCount;
}

mfmU64 mfmPoolGetFreeSlotCount(mfmPoolAllocator * allocator)
{
	return allocator->currentFreeSlotCount;
}

