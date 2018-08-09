#pragma once

/*
	Implementation of a pool allocator in C.
*/

#include "Object.h"
#include "Type.h"
#include "Error.h"

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
		mfmObject object;
		mfmPoolAllocatorDesc desc;
		
		mfmU8* slotStatesPtr;
		mfmBool* slotDataPtr;
	} mfmPoolAllocator;

	/// <summary>
	///		Creates a new magma framework memory pool allocator.
	/// </summary>
	/// <param name="poolAllocator">Pointer to allocator pointer</param>
	/// <param name="desc">Pointer to pool allocator description</param>
	/// <returns>
	///		Returns MFM_ERROR_OKAY if there were no errors.
	///		Returns MFM_ALLOCATION_ERROR if the function couldn't allocate memory for the pool.
	///		Returns MFM_ERROR_INVALID_ARGUMENTS if the pool description or allocator pointers are NULL.
	/// </returns>
	mfmI32 mfmCreatePoolAllocator(mfmPoolAllocator** poolAllocator, mfmPoolAllocatorDesc* desc);

	/// <summary>
	///		Destroys a magma framework memory pool allocator.
	/// </summary>
	void mfmDestroyPoolAllocator(void * poolAllocator);

	/// <summary>
	///		Allocates on a magma framework memory pool allocator.
	/// </summary>
	/// <param name="memory">Pointer to allocated memory pointer</param>
	/// <param name="allocator">Magma framework memory pool allocator</param>
	/// <param name="size">Memory allocation size in bytes</param>
	/// <returns>
	///		Returns MFM_ERROR_OKAY if there were no errors.
	///		Returns MFM_ERROR_ALLOCATOR_OVERFLOW if the pool has no free slots.
	///		Returns MFM_ERROR_ALLOCATION_TOO_BIG if the allocation size is bigger than the pool slot's size
	/// </returns>
	mfmI32 mfmPoolAllocate(void** memory, mfmPoolAllocator* allocator, mfmU64 size);

	/// <summary>
	///		Deallocates on a magma framework memory pool allocator.
	/// </summary>
	/// <param name="memory">Pointer to the allocated memory</param>
	/// <param name="allocator">Magma framework memory pool allocator</param>
	/// <returns>
	///		Returns MFM_ERROR_OKAY if there were no errors.
	///		Returns MFM_ERROR_MEMORY_OUT_OF_BOUNDS if the memory is out of the pool bounds.
	/// </returns>
	mfmI32 mfmPoolDeallocate(void* memory, mfmPoolAllocator* allocator);

#ifdef __cplusplus
}
#endif
