#pragma once

#include "Error.h"

/*
	Atomic data and operations.
*/

#ifdef __cplusplus
extern "C"
{
#endif

	/// <summary>
	///		Gets an atomic 8 bit variable value.
	/// </summary>
	/// <param name="atomic">Pointer to 8 bit atomic variable</param>
	/// <param name="out">Output value</param>
	/// <returns>
	///		Returns MFT_ERROR_OKAY if there were no errors.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mftAtomic8Load(const volatile mfmI8* atomic, mfmI8* out);

	/// <summary>
	///		Sets an atomic 8 bit variable value.
	/// </summary>
	/// <param name="atomic">Pointer to 8 bit atomic variable</param>
	/// <param name="value">New value</param>
	/// <returns>
	///		Returns MFT_ERROR_OKAY if there were no errors.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mftAtomic8Store(volatile mfmI8* atomic, mfmI8 value);

	/// <summary>
	///		Gets an atomic 16 bit variable value.
	/// </summary>
	/// <param name="atomic">Pointer to 16 bit atomic variable</param>
	/// <param name="out">Output value</param>
	/// <returns>
	///		Returns MFT_ERROR_OKAY if there were no errors.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mftAtomic16Load(const volatile mfmI16* atomic, mfmI16* out);

	/// <summary>
	///		Sets an atomic 16 bit variable value.
	/// </summary>
	/// <param name="atomic">Pointer to 16 bit atomic variable</param>
	/// <param name="value">New value</param>
	/// <returns>
	///		Returns MFT_ERROR_OKAY if there were no errors.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mftAtomic16Store(volatile mfmI16* atomic, mfmI16 value);

	/// <summary>
	///		Gets an atomic 32 bit variable value.
	/// </summary>
	/// <param name="atomic">Pointer to 32 bit atomic variable</param>
	/// <param name="out">Output value</param>
	/// <returns>
	///		Returns MFT_ERROR_OKAY if there were no errors.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mftAtomic32Load(const volatile mfmI32* atomic, mfmI32* out);

	/// <summary>
	///		Sets an atomic 32 bit variable value.
	/// </summary>
	/// <param name="atomic">Pointer to 32 bit atomic variable</param>
	/// <param name="value">New value</param>
	/// <returns>
	///		Returns MFT_ERROR_OKAY if there were no errors.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mftAtomic32Store(volatile mfmI32* atomic, mfmI32 value);

	/// <summary>
	///		Adds a value to an atomic 32 bit variable.
	/// </summary>
	/// <param name="atomic">Pointer to 32 bit atomic variable</param>
	/// <param name="value">Value to add to atomic variable</param>
	/// <returns>
	///		Returns MFT_ERROR_OKAY if there were no errors.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mftAtomic32Add(volatile mfmI32* atomic, mfmI32 value);

#ifdef __cplusplus
}
#endif
