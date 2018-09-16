#pragma once

#include "Exception.hpp"
#include "LinearAllocator.h"
#include "Allocator.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace Memory
		{
			class HLinearAllocator : public HAllocator
			{
			public:
				using HAllocator::HAllocator;
				using HAllocator::operator=;
				inline HLinearAllocator(const Memory::Handle& object) : Memory::HAllocator(object) {}

				void Reset();
			};

			/// <summary>
			///		Creates a linear allocator.
			/// </summary>
			/// <param name="size">Linear allocator size</param>
			/// <returns>Linear allocator handle</returns>
			HLinearAllocator CreateLinearAllocator(mfmU64 size);

			/// <summary>
			///		Creates a linear allocator on a predefined memory region.
			///		Use the macro MFM_LINEAR_ALLOCATOR_SIZE to get the minimum memory size that a linear allocator of a certain size needs.
			/// </summary>
			/// <param name="size">Linear allocator size</param>
			/// <param name="memory">Memory region</param>
			/// <param name="memorySize">Memory region size</param>
			/// <returns>Linear allocator handle</returns>
			HLinearAllocator CreateLinearAllocatorOnMemory(mfmU64 size, void* memory, mfmU64 memorySize);
		}
	}
}
