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
			class HStackAllocator : public HAllocator
			{
			public:
				using HAllocator::HAllocator;
				using HAllocator::operator=;
				inline HStackAllocator(const Memory::Handle& object) : Memory::HAllocator(object) {}

				void SetHead(void* head);
			};

			/// <summary>
			///		Creates a stack allocator.
			/// </summary>
			/// <param name="size">Stack allocator size</param>
			/// <returns>Stack allocator handle</returns>
			HStackAllocator CreateStackAllocator(mfmU64 size);

			/// <summary>
			///		Creates a stack allocator on a predefined memory region.
			///		Use the macro MFM_STACK_ALLOCATOR_SIZE to get the minimum memory size that a stack allocator of a certain size needs.
			/// </summary>
			/// <param name="size">Stack allocator size</param>
			/// <param name="memory">Memory region</param>
			/// <param name="memorySize">Memory region size</param>
			/// <returns>Stack allocator handle</returns>
			HStackAllocator CreateStackAllocatorOnMemory(mfmU64 size, void* memory, mfmU64 memorySize);
		}
	}
}
