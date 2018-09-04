#pragma once

#include "Bytecode.h"

#include "../../Memory/Handle.hpp"
#include "../../Memory/Allocator.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace Graphics
		{
			namespace V2X
			{
				/// <summary>
				///		Used as a shader meta data handle.
				///		Destroys the meta data automatically when there are no more references to it.
				/// </summary>
				class MetaDataHandle : public Memory::Handle
				{
				public:
					using Handle::Handle;
					using Handle::operator=;
					explicit inline MetaDataHandle(const Memory::Handle& object) : Memory::Handle(object) {}
				};

				/// <summary>
				///		Loads shader meta data from binary data.
				/// </summary>
				/// <param name="binaryData">Binary meta data</param>
				/// <param name="binarySize">Binary meta data size</param>
				/// <param name="allocator">Allocator where the meta data will be allocated on</param>
				/// <returns>Meta data handle</returns>
				MetaDataHandle LoadMetaData(const mfmU8* binaryData, mfmU64 binarySize, Memory::AllocatorHandle allocator = Memory::StandardAllocator);
			}
		}
	}
}
