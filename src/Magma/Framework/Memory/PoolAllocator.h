#pragma once

/*
	Implementation of a pool allocator in C.
*/

#include "Type.h"
#include "Error.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct mfmPoolAllocator
	{
		mfmU64 slotSize;
		mfmU64 slotCount;
	};

	/// <summary>
	///		Creates a new magma framework memory pool allocator.
	/// </summary>
	/// <returns>
	///		Returns MFM_ERROR_OKAY if there were no errors.
	///		Returns MFM_ALLOCATION_ERROR if the function couldn't allocate memory for the pool.
	/// </returns>
	mfmI32 mfmCreatePoolAllocator(mfmPoolAllocator** poolAllocator, mfmU64 slotSize, mfmU64 slotCount);

	/// <summary>
	///		Destroys a magma framework memory pool allocator.
	/// </summary>
	void mfmDestroyPoolAllocator(mfmPoolAllocator** poolAllocator);

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

#ifdef __cplusplus
}
#endif
