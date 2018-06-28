#pragma once

#include <stdexcept>

namespace Magma
{
	namespace Framework
	{
		namespace Files
		{
			/// <summary>
			///		Thrown when there is a file related error
			/// </summary>
			class FileError : public std::runtime_error
			{
			public:
				using std::runtime_error::runtime_error;
			};
		}
	}
}