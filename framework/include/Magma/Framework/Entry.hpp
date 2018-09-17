#pragma once

#include "Memory/Type.h"
#include "Entry.h"

#include <stdexcept>

/*
	Magma framework library C++ entry point (core).
*/

namespace Magma
{
	namespace Framework
	{
		/// <summary>
		///		Thrown when there is an error related to library startup or termination.
		/// </summary>
		class CoreError : public std::runtime_error
		{
		public:
			inline CoreError(const char* msg, mfError error) : std::runtime_error(msg), err(error) {};

			mfError err;
		};

		/// <summary>
		///		Inits the magma framework library.
		/// </summary>
		/// <param name="argc">Number of arguments</param>
		/// <param name="argv">Argument array</param>
		void Init(int argc, char** argv);

		/// <summary>
		///		Terminates the magma framework library.
		/// </summary>
		void Terminate();
	}
}
