#pragma once

#include <Magma/Framework/Exception.hpp>

namespace Magma
{
	namespace Framework
	{
		namespace Audio
		{
			/// <summary>
			///		Thrown when an audio render device encounters an exception
			/// </summary>
			class RenderDeviceError : public Exception
			{
			public:
				using Exception::Exception;
			};

			/// <summary>
			///		Thrown when there is an error related to WAV loading
			/// </summary>
			class LoaderError : public Exception
			{
			public:
				using Exception::Exception;
			};
		}
	}
}
