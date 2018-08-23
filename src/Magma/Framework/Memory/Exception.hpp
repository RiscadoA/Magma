#pragma once

#include <stdexcept>

#include "Error.h"
#include "../String/UTF8.h"

namespace Magma
{
	namespace Framework
	{
		namespace Memory
		{
			/// <summary>
			///		Thrown when there is an error related to memory allocation.
			/// </summary>
			class AllocatorError : public std::runtime_error
			{
			public:
				using std::runtime_error::runtime_error;
			};

			/// <summary>
			///		Thrown when there is an error related to null pointers.
			/// </summary>
			class NullPointerError : public std::runtime_error
			{
			public:
				using std::runtime_error::runtime_error;
			};

			/// <summary>
			///		Gets a string that describes an error code.
			/// </summary>
			/// <param name="err">Error code</param>
			/// <returns>String pointer</returns>
			const mfsUTF8CodeUnit* ErrorToString(mfError err);
		}
	}
}
