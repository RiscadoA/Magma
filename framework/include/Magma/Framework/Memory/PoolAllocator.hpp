#pragma once

#include "Exception.hpp"
#include "Allocator.hpp"
#include "PoolAllocator.h"

namespace Magma
{
	namespace Framework
	{
		namespace Memory
		{
			/// <summary>
			///		Creates a pool allocator.
			/// </summary>
			/// <param name="desc">Pool allocator description</param>
			/// <returns>Pool allocator handle</returns>
			HAllocator CreatePoolAllocator(const mfmPoolAllocatorDesc& desc);

			/// <summary>
			///		Creates a pool allocator on a predefined memory region.
			///		Use the macro MFM_POOL_ALLOCATOR_SIZE to get the minimum memory size that a pool allocator of a certain size needs.
			/// </summary>
			/// <param name="desc">Pool allocator description</param>
			/// <param name="memory">Memory region</param>
			/// <param name="memorySize">Memory region size</param>
			/// <returns>Pool allocator handle</returns>
			HAllocator CreatePoolAllocatorOnMemory(const mfmPoolAllocatorDesc& desc, void* memory, mfmU64 memorySize);
		}
	}
}
