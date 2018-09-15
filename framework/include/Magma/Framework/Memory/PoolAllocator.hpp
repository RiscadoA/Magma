#pragma once

#include "Exception.hpp"
#include "PoolAllocator.h"

namespace Magma
{
	namespace Framework
	{
		namespace Memory
		{
			/// <summary>
			///		Encapsulates the magma framework C pool48 allocator (declared on PoolAllocator.h)
			/// </summary>
			class PoolAllocator final
			{
			public:
				/// <summary>
				///		Creates a simple pool48 allocator.
				/// </summary>
				/// <param name="elementSize">Pool allocator slot size in bytes</param>
				/// <param name="elementCount">Pool allocator slot count</param>
				/// <param name="expandable">Is the pool48 expandable?</param>
				PoolAllocator(mfmU64 slotSize, mfmU64 slotCount, bool expandable = false);

				/// <summary>
				///		Destroys a simple pool48 allocator.
				/// </summary>
				~PoolAllocator();

				/// <summary>
				///		Allocates on a simple pool48 allocator.
				/// </summary>
				/// <param name="size">Allocation size</param>
				/// <returns>Returns the allocated memory pointer</returns>
				void* Allocate(mfmU64 size);

				/// <summary>
				///		Deallocates on a simple pool48 allocator.
				/// </summary>
				/// <param name="ptr">Pointer to previously allocated memory returned by Allocate</param>
				void Deallocate(void* ptr);

				/// <summary>
				///		Gets the slot size in bytes.
				/// </summary>
				/// <returns>Pool's slot size in bytes</returns>
				inline mfmU64 GetSlotSize() const { return m_pool->desc.slotSize; }

				/// <summary>
				///		Gets the slot count.
				/// </summary>
				/// <returns>Pool's slot count</returns>
				inline mfmU64 GetSlotCount() const { return mfmPoolGetSlotCount(m_pool); }

				/// <summary>
				///		Gets the free slot count.
				/// </summary>
				/// <returns>Free slot count</returns>
				inline mfmU64 GetFreeSlotCount() const { return mfmPoolGetFreeSlotCount(m_pool); }

				/// <summary>
				///		Gets the occupied slot count.
				/// </summary>
				/// <returns>Occupied slot count</returns>
				inline mfmU64 GetOccupiedSlotCount() const { return mfmPoolGetOccupiedSlotCount(m_pool); }

				/// <summary>
				///		Gets the chunk count.
				/// </summary>
				/// <returns>Chunk count</returns>
				inline mfmU64 GetChunkCount() const { return mfmPoolGetChunkCount(m_pool); }

			private:
				::mfmPoolAllocator* m_pool;
			};
		}
	}
}
