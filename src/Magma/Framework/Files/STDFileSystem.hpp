#pragma once

#include "FileSystem.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace Files
		{
			/// <summary>
			///		Implements the FileSystem class with the standard library.
			/// </summary>
			class STDFileSystem : public FileSystem
			{
			public:
				using FileSystem::FileSystem;

				// Inherited via FileSystem
				virtual void * OpenFile(FileMode mode, const Path & path) final;
				virtual void CloseFile(void * file) final;
				virtual void Read(void * file, void * buffer, size_t size) final;
				virtual void Write(void * file, void * buffer, size_t size) final;
				virtual void Create(const Path & path) final;
				virtual void Delete(const Path & path) final;
				virtual void GetDirectoryContents(const Path & path, std::vector<Path>& out) final;
				virtual bool Exists(const Path & path) final;
				virtual size_t GetSize(void* file) final;
			};
		}
	}
}