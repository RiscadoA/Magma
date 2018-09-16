#pragma once

#include <Magma/Framework/Exception.hpp>

namespace Magma
{
	namespace Framework
	{
		namespace Thread
		{
			/// <summary>
			///		Thrown when there is an error related to atomics.
			/// </summary>
			class AtomicError : public Exception
			{
			public:
				using Exception::Exception;
			};

			/// <summary>
			///		Thrown when there is an error related to mutexes.
			/// </summary>
			class MutexError : public Exception
			{
			public:
				using Exception::Exception;
			};

			/// <summary>
			///		Thrown when there is an error related to threads.
			/// </summary>
			class ThreadError : public Exception
			{
			public:
				using Exception::Exception;
			};
		}
	}
}
