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
	mfmBool mfmIsLittleEndian(void);

	/// <summary>
	///		Checks if the system uses big endianness.
	/// </summary>
	/// <returns>MFM_TRUE if yes, otherwise MFM_FALSE</returns>
	mfmBool mfmIsBigEndian(void);

	/// <summary>
	///		Converts a 8 byte value from the system endianness to little endianness.
	/// </summary>
	/// <param name="src">Source value pointer</param>
	/// <param name="dst">Destination value pointer</param>
	void mfmToLittleEndian8(const void* src, void* dst);

	/// <summary>
	///		Converts a 4 byte value from the system endianness to little endianness.
	/// </summary>
	/// <param name="src">Source value pointer</param>
	/// <param name="dst">Destination value pointer</param>
	void mfmToLittleEndian4(const void* src, void* dst);

	/// <summary>
	///		Converts a 2 byte value from the system endianness to little endianness.
	/// </summary>
	/// <param name="src">Source value pointer</param>
	/// <param name="dst">Destination value pointer</param>
	void mfmToLittleEndian2(const void* src, void* dst);

	/// <summary>
	///		Converts a 8 byte value from little endianness to the system endianness.
	/// </summary>
	/// <param name="src">Source value pointer</param>
	/// <param name="dst">Destination value pointer</param>
	void mfmFromLittleEndian8(const void* src, void* dst);

	/// <summary>
	///		Converts a 4 byte value from little endianness to the system endianness.
	/// </summary>
	/// <param name="src">Source value pointer</param>
	/// <param name="dst">Destination value pointer</param>
	void mfmFromLittleEndian4(const void* src, void* dst);

	/// <summary>
	///		Converts a 2 byte value from little endianness to the system endianness.
	/// </summary>
	/// <param name="src">Source value pointer</param>
	/// <param name="dst">Destination value pointer</param>
	void mfmFromLittleEndian2(const void* src, void* dst);

	/// <summary>
	///		Converts a 8 byte value from the system endianness to big endianness.
	/// </summary>
	/// <param name="src">Source value pointer</param>
	/// <param name="dst">Destination value pointer</param>
	void mfmToBigEndian8(const void* src, void* dst);

	/// <summary>
	///		Converts a 4 byte value from the system endianness to big endianness.
	/// </summary>
	/// <param name="src">Source value pointer</param>
	/// <param name="dst">Destination value pointer</param>
	void mfmToBigEndian4(const void* src, void* dst);

	/// <summary>
	///		Converts a 2 byte value from the system endianness to big endianness.
	/// </summary>
	/// <param name="src">Source value pointer</param>
	/// <param name="dst">Destination value pointer</param>
	void mfmToBigEndian2(const void* src, void* dst);

	/// <summary>
	///		Converts a 8 byte value from big endianness to the system endianness.
	/// </summary>
	/// <param name="src">Source value pointer</param>
	/// <param name="dst">Destination value pointer</param>
	void mfmFromBigEndian8(const void* src, void* dst);

	/// <summary>
	///		Converts a 4 byte value from big endianness to the system endianness.
	/// </summary>
	/// <param name="src">Source value pointer</param>
	/// <param name="dst">Destination value pointer</param>
	void mfmFromBigEndian4(const void* src, void* dst);

	/// <summary>
	///		Converts a 2 byte value from big endianness to the system endianness.
	/// </summary>
	/// <param name="src">Source value pointer</param>
	/// <param name="dst">Destination value pointer</param>
	void mfmFromBigEndian2(const void* src, void* dst);

#ifdef __cplusplus
}
#endif
