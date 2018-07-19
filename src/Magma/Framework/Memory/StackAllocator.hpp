#pragma once

#include "Exception.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace Memory
		{
			/// <summary>
			///		Encapsulates a simple stack allocator
			/// </summary>
			class StackAllocator final
			{
			public:
				/// <summary>
				///		Creates a simple stack allocator
				/// </summary>
				/// <param name="size">Stack allocator size in bytes</param>
				StackAllocator(size_t size);

				/// <summary>
				///		Destroys a simple stack allocator
				/// </summary>
				~StackAllocator();

				/// <summary>
				///		Allocates on a simple stack allocator
				/// </summary>
				/// <param name="size">Allocation size</param>
				/// <returns>Returns the allocated memory pointer</returns>
				void* Allocate(size_t size);

				/// <summary>
				///		Gets the stack head of the allocator
				/// </summary>
				/// <returns>Returns the stack head of the allocator</returns>
				size_t GetHead();

				/// <summary>
				///		Sets the stack head of the allocator
				/// </summary>
				/// <param name="size">New allocator stack head</param>
				void SetHead(size_t head);

			private:
				size_t m_size;
				size_t m_head;
				void* m_memory;
			};
		}
	}
}
