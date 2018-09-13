#pragma once

#include "FileSystem.hpp"
#include "../Memory/Allocator.hpp"
#include "../String/UTF8.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace File
		{
			/// <summary>
			///		Creates a folder archive.
			/// </summary>
			/// <param name="path">Folder archive path</param>
			/// <param name="allocator">AllocatorHandle used for internal allocations</param>
			/// <returns>Folder archive handle</returns>
			HArchive CreateFolderArchive(const mfsUTF8CodeUnit* path, Memory::HAllocator allocator = Memory::StandardAllocator);
		}
	}
}