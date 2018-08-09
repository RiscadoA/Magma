#pragma once

#include "Exception.hpp"
#include "LinearAllocator.h"

namespace Magma
{
	namespace Framework
	{
		namespace Memory
		{
			/// <summary>
			///		Encapsulates the magma framework C linear allocator declared on LinearAllocator.h.
			/// </summary>
			class LinearAllocator final
			{
			public:
				/// <summary>
				///		Creates a simple linear allocator
				/// </summary>
				/// <param name="size">Linear allocator size in bytes</param>
				LinearAllocator(mfmU64 size);

				/// <summary>
				///		Destroys a simple linear allocator
				/// </summary>
				~LinearAllocator();

				/// <summary>
				///		Allocates on a linear allocator
				/// </summary>
				/// <param name="size">Allocation size</param>
				/// <returns>Returns the allocated memory pointer</returns>
				void* Allocate(mfmU64 size);

				/// <summary>
				///		Resets the linear allocator head.
				/// </summary>
				void Reset();

			private:
				::mfmLinearAllocator * m_linear;
			};
		}
	}
}
