#pragma once

/*
	Abstract allocator in C.
*/

#include "Object.h"
#include "Type.h"
#include "Error.h"

#ifdef __cplusplus
extern "C"
{
#endif
	typedef mfError (*mfmAllocateFunction)(void* allocator, void** memory, mfmU64 size);
	typedef mfError (*mfmDeallocateFunction)(void* allocator, void* memory);

	typedef struct
	{
		mfmObject object;
		mfmAllocateFunction allocate;
		mfmDeallocateFunction deallocate;
	} mfmAllocator;

	/// <summary>
	///		Allocates on a magma framework memory allocator.
	/// </summary>
	/// <param name="allocator">Magma framework memory allocator (set to NULL to use the default allocator)</param>
	/// <param name="memory">Pointer to allocated memory pointer</param>
	/// <param name="size">Memory allocation size in bytes</param>
	/// <returns>
	///		Returns MFM_ERROR_OKAY if there were no errors.
	///		Returns MFM_ERROR_UNSUPPORTED_FUNCTION if the allocator doesn't support this function.
	///		Returns MFM_ERROR_INVALID_ARGUMENTS if memory is NULL or size is 0.
	///		Returns other errors specific to the allocator type.
	/// </returns>
	mfError mfmAllocate(void* allocator, void** memory, mfmU64 size);

	/// <summary>
	///		Deallocates on a magma framework memory allocator.
	/// </summary>
	/// <param name="allocator">Magma framework memory allocator (set to NULL to use the default allocator)</param>
	/// <param name="memory">Pointer to memory</param>
	/// <returns>
	///		Returns MFM_ERROR_OKAY if there were no errors.
	///		Returns MFM_ERROR_UNSUPPORTED_FUNCTION if the allocator doesn't support this function.
	///		Returns MFM_ERROR_INVALID_ARGUMENTS if memory is NULL.
	///		Returns other errors specific to the allocator type.
	/// </returns>
	mfError mfmDeallocate(void* allocator, void* memory);

#ifdef __cplusplus
}
#endif
