#pragma once

#include <Magma/Framework/Exception.hpp>

namespace Magma
{
	namespace Framework
	{
		namespace String
		{
			/// <summary>
			///		Thrown when there is a string related error.
			/// </summary>
			class StringError : public Exception
			{
			public:
				using Exception::Exception;
			};

			/// <summary>
			///		Thrown when there is a document related error.
			/// </summary>
			class DocumentError : public Exception
			{
			public:
				using Exception::Exception;
			};

			/// <summary>
			///		Thrown when there is a document related error.
			/// </summary>
			class StreamError : public Exception
			{
			public:
				using Exception::Exception;
			};
		}
	}
}