#pragma once

#include <string>
#include <vector>

#include "Exception.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace Files
		{
			/// <summary>
			///		Represents a file/directory path.
			///		Path examples:
			///			"/config.ini"
			///			A '/' in the beginning of the path marks the filesystem root.
			///			Paths that start with '/' are absolute, the others are relative, such as:
			///			"test.ini"
			///			This path is relative to a directory path.
			///			Directory paths end with a '/'
			/// </summary>
			class Path
			{
			public:
				/// <summary>
				///		Creates a new path
				/// </summary>
				/// <param name="path">Absolute path (must start with '/')</param>
				/// <exception cref="Magma::Framework::Files::FileError">Thrown if the path is invalid</exception>
				Path(const std::string& path);

				/// <summary>
				///		Creates a new path from another one
				/// </summary>
				/// <param name="path">Path to copy from</param>
				Path(const Path& path);

				/// <summary>
				///		Moves a path
				/// </summary>
				/// <param name="path">Path to move from</param>
				Path(Path&& path);

				/// <summary>
				///		Creates a new path
				/// </summary>
				/// <param name="relative">Path which this path is relative to</param>
				/// <param name="path">Relative path</param>
				/// <exception cref="Magma::Framework::Files::FileError">Thrown if the path is invalid olr if the relative path is not a directory</exception>
				Path(const Path& relative, const std::string& path);

				/// <summary>
				///		Sets a new value for this path
				/// </summary>
				/// <param name="path">New path value</param>
				/// <returns>Reference to path</returns>
				Path& operator=(const Path& path);

				/// <summary>
				///		Sets a new value for this path
				/// </summary>
				/// <param name="path">New path value</param>
				/// <returns>Reference to path</returns>
				/// <exception cref="Magma::Framework::Files::FileError">Thrown if the path is invalid</exception>
				Path& operator=(const std::string& path);

				/// <summary>
				///		Gets the extension of this path, if it points to a file.
				/// </summary>
				/// <returns>The extension of this file</returns>
				/// <exception cref="Magma::Framework::Files::FileError">Thrown if path does not point to a file or if the path is invalid</exception>
				std::string GetExtension() const;

				/// <summary>
				///		Returns the name of the file or direcvtory which this path points to.
				/// </summary>
				/// <returns>File/directory name</returns>
				/// <exception cref="Magma::Framework::Files::FileError">Thrown if the path is invalid</exception>
				std::string GetName() const;

				/// <summary>
				///		Checks if this path points to a file
				/// </summary>
				/// <returns>True if it points to a file, otherwise false</returns>
				bool IsFile() const;

				/// <summary>
				///		Checks if this path points to a directory
				/// </summary>
				/// <returns>True if it points to a directory, otherwise false</returns>
				bool IsDirectory() const;

				/// <summary>
				///		Gets the directory path where this path is present
				/// </summary>
				/// <returns>Directory wheere this path points to</returns>
				Path GetDirectory() const;

				/// <summary>
				///		Converts this path to a string
				/// </summary>
				/// <returns>This path's string</returns>
				std::string ToString() const;

				/// <summary>
				///		Converts this path to a string
				/// </summary>
				/// <returns>This path's string</returns>
				inline operator std::string() const { return this->ToString(); }

			private:
				void Parse(const std::string& str);

				// "/config/config.txt" -> { "config/", "config.txt" }
				// "/" -> {}
				// "/config/" -> { "config/" }
				std::vector<std::string> m_path;
			};
		}
	}
}