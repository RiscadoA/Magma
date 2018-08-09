#pragma once

/*
	Implementation of a linear allocator in C.
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
		mfmObject object;
		mfmU64 size;
		mfmU8* begin;
		mfmU8* head;
	} mfmLinearAllocator;

	/// <summary>
	///		Creates a new magma framework memory linear allocator.
	/// </summary>
	/// <param name="linearAllocator">Pointer to allocator pointer</param>
	/// <param name="size">Linear buffer size in bytes</param>
	/// <returns>
	///		Returns MFM_ERROR_OKAY if there were no errors.
	///		Returns MFM_ERROR_ALLOCATION_FAILED if the function couldn't allocate memory for the stack.
	///		Returns MFM_ERROR_INVALID_ARGUMENTS if the allocator pointer is null or the size is invalid.
	/// </returns>
	mfmError mfmCreateLinearAllocator(mfmLinearAllocator** linearAllocator, mfmU64 size);

	/// <summary>
	///		Destroys a magma framework memory linear allocator.
	/// </summary>
	/// <param name="linearAllocator">Linear allocator to destroy</param>
	void mfmDestroyLinearAllocator(void * linearAllocator);

	/// <summary>
	///		Allocates on a magma framework memory linear allocator.
	/// </summary>
	/// <param name="linearAllocator">Magma framework memory linear allocator</param>
	/// <param name="memory">Pointer to allocated memory pointer</param>
	/// <param name="size">Memory allocation size in bytes</param>
	/// <returns>
	///		Returns MFM_ERROR_OKAY if there were no errors.
	///		Returns MFM_ERROR_ALLOCATOR_OVERFLOW if the linear buffer overflows.
	/// </returns>
	mfmError mfmLinearAllocate(mfmLinearAllocator* linearAllocator, void** memory, mfmU64 size);

	/// <summary>
	///		Resets the head on a magma framework memory linear allocator.
	/// </summary>
	/// <param name="linearAllocator">Magma framework memory linear allocator</param>
	void mfmLinearReset(mfmLinearAllocator* linearAllocator);

#ifdef __cplusplus
}
#endif
