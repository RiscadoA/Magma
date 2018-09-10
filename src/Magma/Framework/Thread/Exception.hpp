#pragma once

#include <stdexcept>

#include "Error.h"

namespace Magma
{
	namespace Framework
	{
		namespace Thread
		{
			/// <summary>
			///		Thrown when there is an error related to mutexes.
			/// </summary>
			class MutexError : public std::runtime_error
			{
			public:
				using std::runtime_error::runtime_error;
			};

			/// <summary>
			///		Thrown when there is an error related to threads.
			/// </summary>
			class ThreadError : public std::runtime_error
			{
			public:
				using std::runtime_error::runtime_error;
			};
		}
	}
}
