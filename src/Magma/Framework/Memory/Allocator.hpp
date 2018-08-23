#pragma once

#include "Object.hpp"
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
			class Allocator : public Object
			{
			public:
				using Object::Object;
				using Object::operator=;
				inline Allocator(const Memory::Object& object) : Memory::Object(object) {}

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

			extern Allocator StandardAllocator;
		}
	}
}
