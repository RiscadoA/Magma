#pragma once

#include "Path.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace File
		{
			/// <summary>
			///		File open modes
			/// </summary>
			enum class FileMode
			{
				Invalid = -1,

				Read,
				Write,
				ReadWrite,

				Count
			};

			/// <summary>
			///		Abstract class that creates an abstraction layer between the game and the library used for File IO.
			/// </summary>
			class FileSystem
			{
			public:
				/// <summary>
				///		Creates a file system
				/// </summary>
				/// <param name="root">File system root directory</param>
				inline FileSystem(const std::string& root) : m_root(root) {}

				/// <summary>
				///		Opens a file.
				///		If the mode is set to FileMode::Write or FileMode::ReadWrite and the file doesn't exist, a new one will created.
				/// </summary>
				/// <param name="mode">The mode in which the file will be opened</param>
				/// <param name="path">The file path relative to this file system root</param>
				/// <exception cref="Magma::Framework::File::FileError">Thrown if the file couldn't be opened</exception>
				/// <returns>File handle</returns>
				virtual void* OpenFile(FileMode mode, const Path& path) = 0;

				/// <summary>
				///		Closes a file that was opened by OpenFile.
				/// </summary>
				/// <param name="file">File handle</param>
				/// <exception cref="Magma::Framework::File::FileError">Thrown if the file wasn't open</exception>
				virtual void CloseFile(void* file) = 0;

				/// <summary>
				///		Gets the read/write position of a file
				/// </summary>
				/// <param name="file">File handle</param>
				/// <returns>Read/write position</returns>
				virtual size_t GetPosition(void* file) = 0;

				/// <summary>
				///		Changes the read/write position in a file.
				/// </summary>
				/// <param name="file">File handle</param>
				/// <param name="position">Read/write position</param>
				virtual void Seek(void* file, size_t position) = 0;

				/// <summary>
				///		Changes the read position in a file (skips some data).
				/// </summary>
				/// <param name="file">File handle</param>
				/// <param name="amount">Amount of data to skip</param>
				virtual void Skip(void* file, size_t amount) = 0;

				/// <summary>
				///		Reads data from a file into a buffer.
				/// </summary>
				/// <param name="file">File handle</param>
				/// <param name="buffer">Buffer where data will be written to</param>
				/// <param name="size">Buffer size (how much data to read)</param>
				/// <exception cref="Magma::Framework::File::FileError">Thrown if the file wasn't open</exception>
				virtual void Read(void* file, void* buffer, size_t size) = 0;

				/// <summary>
				///		Writes data into a file (replaces previous data stored in the file).
				/// </summary>
				/// <param name="file">File handle</param>
				/// <param name="buffer">Buffer where data will be read from</param>
				/// <param name="size">Buffer size</param>
				/// <exception cref="Magma::Framework::File::FileError">Thrown if the file wasn't open</exception>
				virtual void Write(void* file, void* buffer, size_t size) = 0;

				/// <summary>
				///		Creates a file/directory and all the directories necessary to get to its path.
				/// </summary>
				/// <param name="path">File/directory path</param>
				virtual void Create(const Path& path) = 0;

				/// <summary>
				///		Deletes a file/directory.
				/// </summary>
				/// <param name="path">File/directory path</param>
				/// <exception cref="Magma::Framework::File::FileError">Thrown if the file/directory doesn't exist</exception>
				virtual void Delete(const Path& path) = 0;

				/// <summary>
				///		Gets the files and directories' paths inside a directory
				/// </summary>
				/// <param name="path">Directory path</param>
				/// <param name="out">File and directories' paths</param>
				/// <exception cref="Magma::Framework::File::FileError">Thrown if the directory doesn't exist</exception>
				virtual void GetDirectoryContents(const Path& path, std::vector<Path>& out) = 0;

				/// <summary>
				///		Checks if a file/directory exists.
				/// </summary>
				/// <param name="path">File/directory path</param>
				/// <returns>True if exists, otherwise false</returns>
				virtual bool Exists(const Path& path) = 0;

				/// <summary>
				///		Gets the size of a opened file
				/// </summary>
				/// <param name="file">File handle</param>
				/// <returns>File size</returns>
				/// <exception cref="Magma::Framework::File::FileError">Thrown if the file wasn't open</exception>
				virtual size_t GetSize(void* file) = 0;

				/// <summary>
				///		Gets this filesystem's root path
				/// </summary>
				/// <returns>Root path</returns>
				inline const std::string& GetRoot() const { return m_root; }

			private:
				std::string m_root;
			};
		}
	}
}