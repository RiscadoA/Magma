#pragma once

#include "../Memory/Handle.hpp"
#include "../String/UTF8.hpp"
#include "../String/Stream.hpp"

#include "FileSystem.h"
#include "Path.h"

namespace Magma
{
	namespace Framework
	{
		namespace File
		{
			/// <summary>
			///		Represents a file type.
			/// </summary>
			enum class FileType : mffEnum
			{
				File		= MFF_FILE,
				Directory	= MFF_DIRECTORY,
			};

			/// <summary>
			///		Represents a file stream mode.
			/// </summary>
			enum class FileMode : mffEnum
			{
				Write		= MFF_FILE_WRITE,
				Read		= MFF_FILE_READ,		
			};

			/// <summary>
			///		Used as an handle to an archive in the file system.
			///		Automatically disposes of the archive if there are no more references to it.
			/// </summary>
			class HArchive : public Memory::Handle
			{
			public:
				using Handle::Handle;
				using Handle::operator=;
				inline HArchive(const Memory::Handle& object) : Memory::Handle(object) {}
			};

			/// <summary>
			///		Used as an handle to a file in the file system.
			///		Prevents the file from being deleted while there are still references to it.
			/// </summary>
			class HFile : public Memory::Handle
			{
			public:
				using Handle::Handle;
				using Handle::operator=;
				inline HFile(const Memory::Handle& object) : Memory::Handle(object) {}

				/// <summary>
				///		Gets the file's type.
				/// </summary>
				/// <returns>FileHandle's type</returns>
				FileType GetType();

				/// <summary>
				///		Gets the first file in in this directory (if this file is a directory).
				/// </summary>
				/// <returns>First file in directory</returns>
				HFile GetFirst();

				/// <summary>
				///		Gets the next file in the directory.
				/// </summary>
				/// <returns>Next file in directory</returns>
				HFile GetNext();

				/// <summary>
				///		Gets this file's parent directory.
				/// </summary>
				/// <returns>FileHandle's parent directory</returns>
				HFile GetParent();
			};

			/// <summary>
			///		Registers an archive on the file system.
			/// </summary>
			/// <param name="archive">ArchiveHandle handle</param>
			/// <param name="name">ArchiveHandle name</param>
			void RegisterArchive(HArchive archive, const mfsUTF8CodeUnit* name);

			/// <summary>
			///		Unregisters an archive from the file system.
			/// </summary>
			/// <param name="archive">ArchiveHandle handle</param>
			void UnregisterArchive(HArchive archive);

			/// <summary>
			///		Gets a file from the file system.
			/// </summary>
			/// <param name="path">FileHandle path</param>
			/// <returns>FileHandle handle (NULL if no file was found)</returns>
			HFile GetFile(const mfsUTF8CodeUnit* path);

			/// <summary>
			///		Creates a new directory in the file system.
			/// </summary>
			/// <param name="path">Directory path</param>
			/// <returns>Directory handle</returns>
			HFile CreateDirectory(const mfsUTF8CodeUnit* path);

			/// <summary>
			///		Deletes a directory in the file system.
			/// </summary>
			/// <param name="dir">Directory handle</param>
			void DeleteDirectory(HFile& dir);

			/// <summary>
			///		Creates a new file in the file system.
			/// </summary>
			/// <param name="path">FileHandle path</param>
			/// <returns>FileHandle handle</returns>
			HFile CreateFile(const mfsUTF8CodeUnit* path);

			/// <summary>
			///		Deletes a file in the file system.
			/// </summary>
			/// <param name="file">FileHandle handle</param>
			void DeleteFile(HFile& file);

			/// <summary>
			///		Opens a file stream.
			/// </summary>
			/// <param name="file">FileHandle handle</param>
			/// <returns>FileHandle stream handle</returns>
			String::HStream OpenFile(HFile file, FileMode mode);
		}
	}
}