#pragma once

#include "Memory/Type.h"
#include "Error.h"

/*
	Magma framework library C entry point.
*/

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef MF_ERROR_OKAY
#define MF_ERROR_OKAY					0x00
#define MF_ERROR_ALREADY_INITIALIZED	0x01
#define MF_ERROR_NOT_INITIALIZED		0x02
#endif

	typedef mfmU32 mfEnum;

	/// <summary>
	///		Inits the magma framework library.
	/// </summary>
	/// <param name="argc">Number of arguments</param>
	/// <param name="argv">Argument array</param>
	/// <returns>
	///		Returns MF_ERROR_OKAY if there was no error.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mfInit(int argc, char** argv);

	/// <summary>
	///		Terminates the magma framework library.
	/// </summary>
	void mfTerminate();

	/// <summary>
	///		Checks if the magma framework library is initialized.
	/// </summary>
	/// <returns>MFM_TRUE if it is, otherwise MFM_FALSE</returns>
	mfmBool mfIsInitialized();

#ifdef __cplusplus
}
#endif
