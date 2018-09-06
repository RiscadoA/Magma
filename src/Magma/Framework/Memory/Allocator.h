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
	typedef mfError(*mfmDeallocateFunction)(void* allocator, void* memory);
	typedef mfError (*mfmReallocateFunction)(void* allocator, void* memory, mfmU64 size, void** newMemory);

	typedef struct
	{
		mfmObject object;
		mfmAllocateFunction allocate;
		mfmDeallocateFunction deallocate;
		mfmReallocateFunction reallocate;
	} mfmAllocator;

	/// <summary>
	///		Allocates on a magma framework memory allocator.
	/// </summary>
	/// <param name="allocator">Magma framework memory allocator (set to NULL to use the default allocator)</param>
	/// <param name="memory">Pointer to allocated memory pointer</param>
	/// <param name="size">Memory allocation size in bytes</param>
	/// <returns>
	///		Returns MF_ERROR_OKAY if there were no errors.
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
	///		Returns MF_ERROR_OKAY if there were no errors.
	///		Returns MFM_ERROR_UNSUPPORTED_FUNCTION if the allocator doesn't support this function.
	///		Returns MFM_ERROR_INVALID_ARGUMENTS if memory is NULL.
	///		Returns other errors specific to the allocator type.
	/// </returns>
	mfError mfmDeallocate(void* allocator, void* memory);

	/// <summary>
	///		Reallocates on a magma framework memory allocator.
	/// </summary>
	/// <param name="allocator">Magma framework memory allocator (set to NULL to use the default allocator)</param>
	/// <param name="memory">Pointer to previously allocated memory pointer</param>
	/// <param name="size">New memory allocation size in bytes</param>
	/// <param name="newMemory">New memory adress</param>
	/// <returns>
	///		Returns MF_ERROR_OKAY if there were no errors.
	///		Returns MFM_ERROR_UNSUPPORTED_FUNCTION if the allocator doesn't support this function.
	///		Returns MFM_ERROR_INVALID_ARGUMENTS if memory is NULL or size is 0.
	///		Returns other errors specific to the allocator type.
	/// </returns>
	mfError mfmReallocate(void* allocator, void* memory, mfmU64 size, void** newMemory);

	/// <summary>
	///		Allocates on a magma framework memory allocator using a specified alignment.
	/// </summary>
	/// <param name="allocator">Magma framework memory allocator (set to NULL to use the default allocator)</param>
	/// <param name="memory">Pointer to allocated memory pointer</param>
	/// <param name="size">Memory allocation size in bytes</param>
	/// <param name="alignment">Memory alignment in bytes (MUST be less than or equal to 256 bytes)</param>
	/// <returns>
	///		Returns MF_ERROR_OKAY if there were no errors.
	///		Returns MFM_ERROR_UNSUPPORTED_FUNCTION if the allocator doesn't support this function.
	///		Returns MFM_ERROR_INVALID_ARGUMENTS if memory is NULL or size is 0.
	///		Returns other errors specific to the allocator type.
	/// </returns>
	mfError mfmAllocateAligned(void* allocator, void** memory, mfmU64 size, mfmU64 alignment);

	/// <summary>
	///		Deallocates aligned memory on a magma framework memory allocator.
	/// </summary>
	/// <param name="allocator">Magma framework memory allocator (set to NULL to use the default allocator)</param>
	/// <param name="memory">Pointer to memory</param>
	/// <returns>
	///		Returns MF_ERROR_OKAY if there were no errors.
	///		Returns MFM_ERROR_UNSUPPORTED_FUNCTION if the allocator doesn't support this function.
	///		Returns MFM_ERROR_INVALID_ARGUMENTS if memory is NULL.
	///		Returns other errors specific to the allocator type.
	/// </returns>
	mfError mfmDeallocateAligned(void* allocator, void* memory);

#ifdef __cplusplus
}
#endif
