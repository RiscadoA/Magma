#pragma once

#include "Exception.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace Memory
		{
			/// <summary>
			///		Encapsulates a simple pool allocator
			/// </summary>
			class PoolAllocator final
			{
			public:
				/// <summary>
				///		Creates a simple pool allocator
				/// </summary>
				/// <param name="elementSize">Pool allocator element size in bytes</param>
				/// <param name="elementCount">Pool allocator element count</param>
				PoolAllocator(size_t elementSize, size_t elementCount);

				/// <summary>
				///		Destroys a simple pool allocator
				/// </summary>
				~PoolAllocator();

				/// <summary>
				///		Allocates on a simple pool allocator
				/// </summary>
				/// <param name="size">Allocation size</param>
				/// <returns>Returns the allocated memory pointer</returns>
				void* Allocate(size_t size);

				/// <summary>
				///		Deallocates on a simple pool allocator
				/// </summary>
				/// <param name="ptr">Pointer to previously allocated memory returned by Allocate</param>
				void Deallocate(void* ptr);

				/// <summary>
				///		Gets the max element size in bytes
				/// </summary>
				/// <returns>Pool's max element size in bytes</returns>
				inline size_t GetElementSize() const { return m_elementSize; }

				/// <summary>
				///		Gets the max element count
				/// </summary>
				/// <returns>Pool's max element count</returns>
				inline size_t GetElementCount() const { return m_elementCount; }

			private:
				size_t m_elementSize;
				size_t m_elementCount;
				char* m_memory;
				bool* m_occupied;
			};
		}
	}
}
