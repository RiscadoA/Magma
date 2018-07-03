#pragma once

#include <stdexcept>

namespace Magma
{
	namespace Framework
	{
		namespace Graphics
		{
			/// <summary>
			///		Thrown when there is an error related to shaders
			/// </summary>
			class ShaderError : public std::runtime_error
			{
			public:
				using std::runtime_error::runtime_error;
			};
		}
	}
}
