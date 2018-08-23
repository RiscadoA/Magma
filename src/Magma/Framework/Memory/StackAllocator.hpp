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
			///		Encapsulates the magma framework C stack allocator declared on StackAllocator.h.
			/// </summary>
			class StackAllocator final : public Allocator
			{
			public:
				using Allocator::Allocator;
			};

			/// <summary>
			///		Creates a stack allocator.
			/// </summary>
			/// <param name="size">Stack allocator size</param>
			/// <returns>Stack allocator handle</returns>
			StackAllocator CreateStackAllocator(mfmU64 size);

			/// <summary>
			///		Creates a stack allocator on a predefined memory region.
			///		Use the macro MFM_STACK_ALLOCATOR_SIZE to get the minimum memory size that a stack allocator of a certain size needs.
			/// </summary>
			/// <param name="size">Stack allocator size</param>
			/// <param name="memory">Memory region</param>
			/// <param name="memorySize">Memory region size</param>
			/// <returns>Stack allocator handle</returns>
			StackAllocator CreateStackAllocatorOnMemory(mfmU64 size, void* memory, mfmU64 memorySize);
		}
	}
}
