#pragma once

#include "Type.h"

#ifdef __cplusplus
extern "C"
{
#endif
	/// <summary>
	///		Checks if the system uses little endianness.
	/// </summary>
	/// <returns>MFM_TRUE if yes, otherwise MFM_FALSE</returns>
	mfmBool mfmIsLittleEndian();

	/// <summary>
	///		Checks if the system uses big endianness.
	/// </summary>
	/// <returns>MFM_TRUE if yes, otherwise MFM_FALSE</returns>
	mfmBool mfmIsBigEndian();

	/// <summary>
	///		Converts a 8 byte value from the system endianness to little endianness.
	/// </summary>
	/// <param name="src">Source value pointer</param>
	/// <param name="dst">Destination value pointer</param>
	void mfmToLittleEndian8(void* src, void* dst);

	/// <summary>
	///		Converts a 4 byte value from the system endianness to little endianness.
	/// </summary>
	/// <param name="src">Source value pointer</param>
	/// <param name="dst">Destination value pointer</param>
	void mfmToLittleEndian4(void* src, void* dst);

	/// <summary>
	///		Converts a 2 byte value from the system endianness to little endianness.
	/// </summary>
	/// <param name="src">Source value pointer</param>
	/// <param name="dst">Destination value pointer</param>
	void mfmToLittleEndian2(void* src, void* dst);

	/// <summary>
	///		Converts a 8 byte value from little endianness to the system endianness.
	/// </summary>
	/// <param name="src">Source value pointer</param>
	/// <param name="dst">Destination value pointer</param>
	void mfmFromLittleEndian8(void* src, void* dst);

	/// <summary>
	///		Converts a 4 byte value from little endianness to the system endianness.
	/// </summary>
	/// <param name="src">Source value pointer</param>
	/// <param name="dst">Destination value pointer</param>
	void mfmFromLittleEndian4(void* src, void* dst);

	/// <summary>
	///		Converts a 2 byte value from little endianness to the system endianness.
	/// </summary>
	/// <param name="src">Source value pointer</param>
	/// <param name="dst">Destination value pointer</param>
	void mfmFromLittleEndian2(void* src, void* dst);

	/// <summary>
	///		Converts a 8 byte value from the system endianness to big endianness.
	/// </summary>
	/// <param name="src">Source value pointer</param>
	/// <param name="dst">Destination value pointer</param>
	void mfmToBigEndian8(void* src, void* dst);

	/// <summary>
	///		Converts a 4 byte value from the system endianness to big endianness.
	/// </summary>
	/// <param name="src">Source value pointer</param>
	/// <param name="dst">Destination value pointer</param>
	void mfmToBigEndian4(void* src, void* dst);

	/// <summary>
	///		Converts a 2 byte value from the system endianness to big endianness.
	/// </summary>
	/// <param name="src">Source value pointer</param>
	/// <param name="dst">Destination value pointer</param>
	void mfmToBigEndian2(void* src, void* dst);

	/// <summary>
	///		Converts a 8 byte value from big endianness to the system endianness.
	/// </summary>
	/// <param name="src">Source value pointer</param>
	/// <param name="dst">Destination value pointer</param>
	void mfmFromBigEndian8(void* src, void* dst);

	/// <summary>
	///		Converts a 4 byte value from big endianness to the system endianness.
	/// </summary>
	/// <param name="src">Source value pointer</param>
	/// <param name="dst">Destination value pointer</param>
	void mfmFromBigEndian4(void* src, void* dst);

	/// <summary>
	///		Converts a 2 byte value from big endianness to the system endianness.
	/// </summary>
	/// <param name="src">Source value pointer</param>
	/// <param name="dst">Destination value pointer</param>
	void mfmFromBigEndian2(void* src, void* dst);

#ifdef __cplusplus
}
#endif
