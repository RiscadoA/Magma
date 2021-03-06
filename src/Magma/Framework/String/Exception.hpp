#pragma once

#include <stdexcept>

#include "UTF8.h"
#include "Error.h"

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

			/// <summary>
			///		Thrown when there is a document related error
			/// </summary>
			class StreamError : public std::runtime_error
			{
			public:
				using std::runtime_error::runtime_error;
			};

			/// <summary>
			///		Gets a string that describes an error code.
			/// </summary>
			/// <param name="err">Error code</param>
			/// <returns>String pointer</returns>
			const mfsUTF8CodeUnit* ErrorToString(mfError err);
		}
	}
}