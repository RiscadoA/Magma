#pragma once

#include <stdexcept>

namespace Magma
{
	namespace Framework
	{
		namespace String
		{
			/// <summary>
			///		Thrown when there is a string related error
			/// </summary>
			class StringError : public std::runtime_error
			{
			public:
				using std::runtime_error::runtime_error;
			};

			/// <summary>
			///		Thrown when there is a document related error
			/// </summary>
			class DocumentError : public std::runtime_error
			{
			public:
				using std::runtime_error::runtime_error;
			};
		}
	}
}