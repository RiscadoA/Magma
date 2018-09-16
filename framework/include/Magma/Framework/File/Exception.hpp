#pragma once

#include <Magma/Framework/Exception.hpp>

namespace Magma
{
	namespace Framework
	{
		namespace File
		{
			/// <summary>
			///		Thrown when there is a file system related error.
			/// </summary>
			class FileSystemError : public Exception
			{
			public:
				using Exception::Exception;
			};

			/// <summary>
			///		Thrown when there is a archive related error.
			/// </summary>
			class ArchiveError : public Exception
			{
			public:
				using Exception::Exception;
			};
		}
	}
}