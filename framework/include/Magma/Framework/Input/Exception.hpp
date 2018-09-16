#pragma once

#include <Magma/Framework/Exception.hpp>

namespace Magma
{
	namespace Framework
	{
		namespace Input
		{
			/// <summary>
			///		Thrown when there is an error related to window handling.
			/// </summary>
			class WindowError : public Exception
			{
			public:
				using Exception::Exception;
			};
		}
	}
}
