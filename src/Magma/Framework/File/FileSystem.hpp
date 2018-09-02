#pragma once

#include "../Memory/Object.hpp"
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
			class Archive : public Memory::Object
			{
			public:
				using Object::Object;
				using Object::operator=;
				inline Archive(const Memory::Object& object) : Memory::Object(object) {}
			};

			/// <summary>
			///		Used as an handle to a file in the file system.
			///		Prevents the file from being deleted while there are still references to it.
			/// </summary>
			class File : public Memory::Object
			{
			public:
				using Object::Object;
				using Object::operator=;
				inline File(const Memory::Object& object) : Memory::Object(object) {}

				/// <summary>
				///		Gets the file's type.
				/// </summary>
				/// <returns>File's type</returns>
				FileType GetType();

				/// <summary>
				///		Gets the first file in in this directory (if this file is a directory).
				/// </summary>
				/// <returns>First file in directory</returns>
				File GetFirst();

				/// <summary>
				///		Gets the next file in the directory.
				/// </summary>
				/// <returns>Next file in directory</returns>
				File GetNext();

				/// <summary>
				///		Gets this file's parent directory.
				/// </summary>
				/// <returns>File's parent directory</returns>
				File GetParent();
			};

			/// <summary>
			///		Registers an archive on the file system.
			/// </summary>
			/// <param name="archive">Archive handle</param>
			/// <param name="name">Archive name</param>
			void RegisterArchive(Archive archive, const mfsUTF8CodeUnit* name);

			/// <summary>
			///		Unregisters an archive from the file system.
			/// </summary>
			/// <param name="archive">Archive handle</param>
			void UnregisterArchive(Archive archive);

			/// <summary>
			///		Gets a file from the file system.
			/// </summary>
			/// <param name="path">File path</param>
			/// <returns>File handle (NULL if no file was found)</returns>
			File GetFile(const mfsUTF8CodeUnit* path);

			/// <summary>
			///		Creates a new directory in the file system.
			/// </summary>
			/// <param name="path">Directory path</param>
			/// <returns>Directory handle</returns>
			File CreateDirectory(const mfsUTF8CodeUnit* path);

			/// <summary>
			///		Deletes a directory in the file system.
			/// </summary>
			/// <param name="dir">Directory handle</param>
			void DeleteDirectory(File& dir);

			/// <summary>
			///		Creates a new file in the file system.
			/// </summary>
			/// <param name="path">File path</param>
			/// <returns>File handle</returns>
			File CreateFile(const mfsUTF8CodeUnit* path);

			/// <summary>
			///		Deletes a file in the file system.
			/// </summary>
			/// <param name="file">File handle</param>
			void DeleteFile(File& file);

			/// <summary>
			///		Opens a file stream.
			/// </summary>
			/// <param name="file">File handle</param>
			/// <returns>File stream handle</returns>
			String::Stream OpenFile(File file, FileMode mode);
		}
	}
}