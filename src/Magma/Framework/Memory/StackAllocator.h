#pragma once

/*
	Implementation of a stack allocator in C.
*/

#include "Allocator.h"

#ifdef __cplusplus
extern "C"
{
#endif
	typedef struct
	{
		mfmAllocator base;
		mfmU64 stackSize;
		mfmU8* stackBegin;
		mfmU8* stackHead;
		mfmBool onMemory;
	} mfmStackAllocator;

#define MFM_STACK_ALLOCATOR_BASE_SIZE (sizeof(mfmStackAllocator))

	/// <summary>
	///		Creates a new magma framework memory stack allocator.
	/// </summary>
	/// <param name="stackAllocator">Pointer to allocator pointer</param>
	/// <param name="size">Stack size in bytes</param>
	/// <returns>
	///		Returns MFM_ERROR_OKAY if there were no errors.
	///		Returns MFM_ERROR_ALLOCATION_FAILED if the function couldn't allocate memory for the stack.
	///		Returns MFM_ERROR_INVALID_ARGUMENTS if the allocator pointer is null or the size is invalid.
	/// </returns>
	mfmError mfmCreateStackAllocator(mfmStackAllocator** stackAllocator, mfmU64 size);

	/// <summary>
	///		Creates a new magma framework memory stack allocator.
	/// </summary>
	/// <param name="stackAllocator">Pointer to allocator pointer</param>
	/// <param name="size">Stack size in bytes</param>
	///	<param name="memory">Pointer to memory adress where the stack allocator will be created</param>
	/// <param name="memorySize">Size of the reserved memory</param>
	/// <returns>
	///		Returns MFM_ERROR_OKAY if there were no errors.
	///		Returns MFM_ERROR_ALLOCATION_FAILED if the function couldn't allocate memory for the stack.
	///		Returns MFM_ERROR_INVALID_ARGUMENTS if the allocator pointer is null or the size is invalid.
	/// </returns>
	mfmError mfmCreateStackAllocatorOnMemory(mfmStackAllocator** stackAllocator, mfmU64 size, void* memory, mfmU64 memorySize);

	/// <summary>
	///		Destroys a magma framework memory stack allocator.
	/// </summary>
	/// <param name="stackAllocator">Stack allocator to destroy</param>
	void mfmDestroyStackAllocator(void * stackAllocator);

	/// <summary>
	///		Allocates on a magma framework memory stack allocator.
	/// </summary>
	/// <param name="allocator">Magma framework memory stack allocator</param>
	/// <param name="memory">Pointer to allocated memory pointer</param>
	/// <param name="size">Memory allocation size in bytes</param>
	/// <returns>
	///		Returns MFM_ERROR_OKAY if there were no errors.
	///		Returns MFM_ERROR_ALLOCATOR_OVERFLOW if the stack overflowed.
	/// </returns>
	mfmError mfmStackAllocate(mfmStackAllocator* allocator, void** memory, mfmU64 size);

	/// <summary>
	///		Sets the stack head on a magma framework memory stack allocator.
	/// </summary>
	/// <param name="allocator">Magma framework memory stack allocator</param>
	/// <param name="memory">Stack head pointer</param>
	/// <returns>
	///		Returns MFM_ERROR_OKAY if there were no errors.
	///		Returns MFM_ERROR_OUT_OF_BOUNDS if the head is out of the stack bounds.
	/// </returns>
	mfmError mfmStackSetHead(mfmStackAllocator* allocator, void* head);

#ifdef __cplusplus
}
#endif
