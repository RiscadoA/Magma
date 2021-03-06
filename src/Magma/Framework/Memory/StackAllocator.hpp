#pragma once

#include "Exception.hpp"
#include "Allocator.hpp"
#include "StackAllocator.h"

namespace Magma
{
	namespace Framework
	{
		namespace Memory
		{
			/// <summary>
			///		Encapsulates the magma framework C stack allocator declared on StackAllocatorHandle.h.
			/// </summary>
			class StackAllocatorHandle final : public HAllocator
			{
			public:
				using HAllocator::HAllocator;
			};

			/// <summary>
			///		Creates a stack allocator.
			/// </summary>
			/// <param name="size">Stack allocator size</param>
			/// <returns>Stack allocator handle</returns>
			StackAllocatorHandle CreateStackAllocator(mfmU64 size);

			/// <summary>
			///		Creates a stack allocator on a predefined memory region.
			///		Use the macro MFM_STACK_ALLOCATOR_SIZE to get the minimum memory size that a stack allocator of a certain size needs.
			/// </summary>
			/// <param name="size">Stack allocator size</param>
			/// <param name="memory">Memory region</param>
			/// <param name="memorySize">Memory region size</param>
			/// <returns>Stack allocator handle</returns>
			StackAllocatorHandle CreateStackAllocatorOnMemory(mfmU64 size, void* memory, mfmU64 memorySize);
		}
	}
}
