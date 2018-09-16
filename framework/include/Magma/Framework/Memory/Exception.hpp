#pragma once

#include <Magma/Framework/Exception.hpp>

namespace Magma
{
	namespace Framework
	{
		namespace Memory
		{
			/// <summary>
			///		Thrown when there is an error related to memory allocation.
			/// </summary>
			class AllocatorError : public Exception
			{
			public:
				using Exception::Exception;
			};

			/// <summary>
			///		Thrown when there is an error related to null pointers.
			/// </summary>
			class NullPointerError : public Exception
			{
			public:
				using Exception::Exception;
			};

			/// <summary>
			///		Thrown when there is an error related to objects.
			/// </summary>
			class ObjectError : public Exception
			{
			public:
				using Exception::Exception;
			};
		}
	}
}
