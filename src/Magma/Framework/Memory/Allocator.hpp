#pragma once

#include "Handle.hpp"
#include "Allocator.h"

namespace Magma
{
	namespace Framework
	{
		namespace Memory
		{
			/// <summary>
			///		Used as an allocator handle.
			///		Destroys the allocator automatically when there are no more references to it.
			/// </summary>
			class AllocatorHandle : public Handle
			{
			public:
				using Handle::Handle;
				using Handle::operator=;
				inline AllocatorHandle(const Memory::Handle& object) : Memory::Handle(object) {}

				/// <summary>
				///		Allocates data on this allocator.
				/// </summary>
				/// <param name="size">Data size in bytes</param>
				/// <returns>Pointer to allocated data</returns>
				void* Allocate(mfmU64 size);

				/// <summary>
				///		Deallocates data previously allocated by Allocate.
				/// </summary>
				/// <param name="data">Data pointer</param>
				void Deallocate(void* memory);
			};

			extern AllocatorHandle StandardAllocator;
		}
	}
}
