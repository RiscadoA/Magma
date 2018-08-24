#pragma once

#include <stdexcept>

namespace Magma
{
	namespace Framework
	{
		namespace Input
		{
			/// <summary>
			///		Thrown when there is an error related to window handling.
			/// </summary>
			class WindowError : public std::runtime_error
			{
			public:
				using std::runtime_error::runtime_error;
			};
		}
	}
}
