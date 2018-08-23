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
			class StringStream : public Stream
			{
			public:
				using Stream::Stream;

				/// <summary>
				///		Clears the string stream.
				/// </summary>
				void Clear();
			};

			/// <summary>
			///		Creates a new string stream.
			/// </summary>
			/// <param name="path">File path</param>
			/// <param name="mode">File open mode</param>
			/// <param name="allocator">Allocator where the string stream will be created</param>
			/// <returns>Stream handle</returns>
			StringStream CreateStringStream(void* buffer, mfmU64 size, Memory::Allocator allocator = Memory::StandardAllocator);
		}
	}
}
