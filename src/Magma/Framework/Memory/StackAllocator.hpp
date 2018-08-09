#pragma once

#include "Exception.hpp"
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
			class StackAllocator final
			{
			public:
				/// <summary>
				///		Creates a simple stack allocator
				/// </summary>
				/// <param name="size">Stack allocator size in bytes</param>
				StackAllocator(mfmU64 size);

				/// <summary>
				///		Destroys a simple stack allocator
				/// </summary>
				~StackAllocator();

				/// <summary>
				///		Allocates on a simple stack allocator
				/// </summary>
				/// <param name="size">Allocation size</param>
				/// <returns>Returns the allocated memory pointer</returns>
				void* Allocate(mfmU64 size);

				/// <summary>
				///		Sets the stack head of the allocator.
				/// </summary>
				/// <param name="head">New allocator stack head</param>
				void SetHead(void* head);

			private:
				::mfmStackAllocator* m_stack;
			};
		}
	}
}
