#pragma once

#include "Error.h"
#include "../String/UTF8.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/// <summary>
	///		Gets a string that describes an error code.
	/// </summary>
	/// <param name="err">Error code</param>
	/// <returns>String pointer (NULL if the error code isn't part of the magma framework thread library</returns>
	const mfsUTF8CodeUnit* mftErrorToString(mfError err);

#ifdef __cplusplus
}
#endif
