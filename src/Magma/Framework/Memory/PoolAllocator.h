#pragma once

/*
	Implementation of a pool allocator in C.
*/

#include "Allocator.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct
	{
		mfmBool expandable;
		mfmU64 slotSize;
		mfmU64 slotCount;
	} mfmPoolAllocatorDesc;

	typedef struct
	{
		mfmU8* slotStatesPtr;
		mfmBool* slotDataPtr;
		void* next;
	} mfmPoolAllocatorChunk;

	typedef struct
	{
		mfmAllocator base;
		mfmPoolAllocatorDesc desc;
		mfmPoolAllocatorChunk* firstChunk;
		mfmU64 currentSlotCount;
		mfmU64 currentFreeSlotCount;
		mfmU64 currentChunkCount;
		mfmBool onMemory;
	} mfmPoolAllocator;

#define MFM_POOL_ALLOCATOR_BASE_SIZE (sizeof(mfmPoolAllocator) + sizeof(mfmPoolAllocatorChunk))
#define MFM_POOL_ALLOCATOR_SIZE(slotCount, slotSize) (MFM_POOL_ALLOCATOR_BASE_SIZE + slotCount * slotSize + slotCount * sizeof(mfmBool))

	/// <summary>
	///		Creates a new magma framework memory pool allocator.
	/// </summary>
	/// <param name="poolAllocator">Pointer to allocator pointer</param>
	/// <param name="desc">Pointer to pool allocator description</param>
	/// <returns>
	///		Returns MFM_ERROR_OKAY if there were no errors.
	///		Returns MFM_ERROR_ALLOCATION_FAILED if the function couldn't allocate memory for the pool.
	///		Returns MFM_ERROR_INVALID_ARGUMENTS if the pool description or allocator pointers are NULL.
	/// </returns>
	mfmError mfmCreatePoolAllocator(mfmPoolAllocator** poolAllocator, const mfmPoolAllocatorDesc* desc);

	/// <summary>
	///		Creates a new magma framework memory pool allocator.
	/// </summary>
	/// <param name="poolAllocator">Pointer to allocator pointer</param>
	/// <param name="desc">Pointer to pool allocator description</param>
	/// <param name="memory">Pointer to memory adress where the pool allocator will be created</param>
	/// <param name="memorySize">Size of the reserved memory</param>
	/// <returns>
	///		Returns MFM_ERROR_OKAY if there were no errors.
	///		Returns MFM_ERROR_INVALID_ARGUMENTS if the pool description or allocator pointers are NULL or if the pool is marked as expandable or if the pool doesn't fit on the memory passed.
	/// </returns>
	mfmError mfmCreatePoolAllocatorOnMemory(mfmPoolAllocator** poolAllocator, const mfmPoolAllocatorDesc* desc, void* memory, mfmU64 memorySize);

	/// <summary>
	///		Destroys a magma framework memory pool allocator.
	/// </summary>
	/// <param name="poolAllocator">Pool allocator to destroy</param>
	void mfmDestroyPoolAllocator(void * poolAllocator);

	/// <summary>
	///		Allocates on a magma framework memory pool allocator.
	/// </summary>
	/// <param name="allocator">Magma framework memory pool allocator</param>
	/// <param name="memory">Pointer to allocated memory pointer</param>
	/// <param name="size">Memory allocation size in bytes</param>
	/// <returns>
	///		Returns MFM_ERROR_OKAY if there were no errors.
	///		Returns MFM_ERROR_ALLOCATOR_OVERFLOW if the pool has no free slots.
	///		Returns MFM_ERROR_ALLOCATION_TOO_BIG if the allocation size is bigger than the pool slot's size.
	///		Returns MFM_ERROR_ALLOCATION_FAILED if the pool failed to expand.
	/// </returns>
	mfmError mfmPoolAllocate(mfmPoolAllocator* allocator, void** memory, mfmU64 size);

	/// <summary>
	///		Deallocates on a magma framework memory pool allocator.
	/// </summary>
	/// <param name="allocator">Magma framework memory pool allocator</param>
	/// <param name="memory">Pointer to the allocated memory</param>
	/// <returns>
	///		Returns MFM_ERROR_OKAY if there were no errors.
	///		Returns MFM_ERROR_OUT_OF_BOUNDS if the memory is out of the pool bounds.
	/// </returns>
	mfmError mfmPoolDeallocate(mfmPoolAllocator* allocator, void* memory);

	/// <summary>
	///		Gets the number of slots in a pool allocator.
	/// </summary>
	/// <param name="allocator">Allocator pointer</param>
	/// <returns>Allocator slot count</returns>
	mfmU64 mfmPoolGetSlotCount(mfmPoolAllocator* allocator);

	/// <summary>
	///		Gets the number of chunks in a pool allocator.
	/// </summary>
	/// <param name="allocator">Allocator pointer</param>
	/// <returns>Allocator chunk count</returns>
	mfmU64 mfmPoolGetChunkCount(mfmPoolAllocator* allocator);

	/// <summary>
	///		Gets the number of occupied slots in a pool allocator.
	/// </summary>
	/// <param name="allocator">Allocator pointer</param>
	/// <returns>Allocator occupied slot count</returns>
	mfmU64 mfmPoolGetOccupiedSlotCount(mfmPoolAllocator* allocator);

	/// <summary>
	///		Gets the number of occupied slots in a pool allocator.
	/// </summary>
	/// <param name="allocator">Allocator pointer</param>
	/// <returns>Allocator free slot count</returns>
	mfmU64 mfmPoolGetFreeSlotCount(mfmPoolAllocator* allocator);

#ifdef __cplusplus
}
#endif
