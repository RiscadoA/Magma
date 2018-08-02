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

			/// <summary>
			///		Thrown when a graphics render device encounters an exception
			/// </summary>
			class RenderDeviceError : public std::runtime_error
			{
			public:
				using std::runtime_error::runtime_error;
			};

			/// <summary>
			///		Thrown when there is a text related error
			/// </summary>
			class TextError : public std::runtime_error
			{
			public:
				using std::runtime_error::runtime_error;
			};
		}
	}
}
