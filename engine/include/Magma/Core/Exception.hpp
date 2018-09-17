#pragma once

#include <Magma/Framework/Exception.hpp>

namespace Magma
{
	namespace Core
	{
		class Exception : public Framework::Exception
		{
		public:
			using Framework::Exception::Exception;
		};
	}
}
