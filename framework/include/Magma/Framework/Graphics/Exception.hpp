#pragma once

#include <Magma/Framework/Exception.hpp>

namespace Magma
{
	namespace Framework
	{
		namespace Graphics
		{
			/// <summary>
			///		Thrown when there is an error related to shaders
			/// </summary>
			class ShaderError : public Exception
			{
			public:
				using Exception::Exception;
			};

			/// <summary>
			///		Thrown when a graphics render device encounters an exception
			/// </summary>
			class RenderDeviceError : public Exception
			{
			public:
				using Exception::Exception;
			};

			/// <summary>
			///		Thrown when there is a text related error
			/// </summary>
			class TextError : public Exception
			{
			public:
				using Exception::Exception;
			};
		}
	}
}
