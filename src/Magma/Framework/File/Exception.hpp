#pragma once

#include <stdexcept>

namespace Magma
{
	namespace Framework
	{
		namespace FileHandle
		{
			/// <summary>
			///		Thrown when there is a file system related error.
			/// </summary>
			class FileSystemError : public std::runtime_error
			{
			public:
				using std::runtime_error::runtime_error;
			};

			/// <summary>
			///		Thrown when there is a archive related error.
			/// </summary>
			class ArchiveError : public std::runtime_error
			{
			public:
				using std::runtime_error::runtime_error;
			};
		}
	}
}