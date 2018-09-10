#pragma once

#include "Stream.hpp"
#include "StringStream.h"
#include "../Memory/Allocator.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace String
		{
			/// <summary>
			///		Used as an object handle.
			///		Destroys the object automatically when there are no more references to it.
			/// </summary>
			class HStringStream : public HStream
			{
			public:
				using HStream::HStream;

				/// <summary>
				///		Clears the string stream.
				/// </summary>
				void Clear();
			};

			/// <summary>
			///		Creates a new string stream.
			/// </summary>
			/// <param name="path">FileHandle path</param>
			/// <param name="mode">FileHandle open mode</param>
			/// <param name="allocator">AllocatorHandle where the string stream will be created</param>
			/// <returns>StreamHandle handle</returns>
			HStringStream CreateStringStream(void* buffer, mfmU64 size, Memory::HAllocator allocator = Memory::StandardAllocator);
		}
	}
}
