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
			class HAllocator : public Handle
			{
			public:
				using Handle::Handle;
				using Handle::operator=;
				inline HAllocator(const Memory::Handle& object) : Memory::Handle(object) {}

				/// <summary>
				///		Allocates data on this allocator.
				/// </summary>
				/// <param name="size">Data size in bytes</param>
				/// <returns>Pointer to allocated data</returns>
				void* Allocate(mfmU64 size);

				/// <summary>
				///		Reallocates data on this allocator.
				/// </summary>
				/// <param name="memory">Old data</param>
				/// <param name="prevSize">Previous data size in bytes</param>
				/// <param name="newSize">New data size in bytes</param>
				/// <returns>Pointer to reallocated data</returns>
				void* Reallocate(void* memory, mfmU64 prevSize, mfmU64 newSize);

				/// <summary>
				///		Deallocates data previously allocated by Allocate.
				/// </summary>
				/// <param name="data">Data pointer</param>
				void Deallocate(void* memory);
			};

			extern HAllocator StandardAllocator;
		}
	}
}
