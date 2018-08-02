#pragma once

#include <stdexcept>

namespace Magma
{
	namespace Framework
	{
		namespace Audio
		{
			/// <summary>
			///		Thrown when an audio render device encounters an exception
			/// </summary>
			class RenderDeviceError : public std::runtime_error
			{
			public:
				using std::runtime_error::runtime_error;
			};

		}
	}
}
