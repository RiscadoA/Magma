#pragma once

#include "FileSystem.hpp"
#include "../Memory/Allocator.hpp"
#include "../String/UTF8.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace FileHandle
		{
			/// <summary>
			///		Creates a folder archive.
			/// </summary>
			/// <param name="path">Folder archive path</param>
			/// <param name="allocator">AllocatorHandle used for internal allocations</param>
			/// <returns>Folder archive handle</returns>
			ArchiveHandle CreateFolderArchive(const mfsUTF8CodeUnit* path, Memory::AllocatorHandle allocator = Memory::StandardAllocator);
		}
	}
}