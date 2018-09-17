#pragma once

#include "Error.h"
#include "UTF32.h"

/*
	UTF-8 string manipulation functions.
*/

#ifdef __cplusplus
extern "C"
{
#endif
	typedef char mfsUTF8CodeUnit;

	/// <summary>
	///		Checks if a UTF-8 character is valid.
	/// </summary>
	/// <param name="chr">Pointer to UTF-8 character</param>
	/// <returns>MFM_TRUE if valid, otherwise MFM_FALSE</returns>
	mfmBool mfsIsValidUTF8Char(const mfsUTF8CodeUnit* chr);

	/// <summary>
	///		Gets the size of a UTF-8 character.
	/// </summary>
	/// <param name="chr">Pointer to UTF-8 character</param>
	/// <param name="size">Pointer to character size</param>
	/// <returns>
	///		Returns MF_ERROR_OKAY if there were no errors.
	///		Returns MFS_ERROR_INVALID_ARGUMENTS if <paramref name="chr">chr</paramref> or <paramref name="size">size</paramref> are NULL.
	///		Returns MFS_ERROR_INVALID_UTF8 if the character at <paramref name="chr">chr</paramref> is invalid.
	/// </returns>
	mfError mfsGetUTF8CharSize(const mfsUTF8CodeUnit* chr, mfmU64* size);

	/// <summary>
	///		Gets the size of a unicode point value if it was encoded in UTF-8.
	/// </summary>
	/// <param name="up">Character unicode point value</param>
	/// <param name="size">Pointer to character size</param>
	/// <returns>
	///		Returns MF_ERROR_OKAY if there were no errors.
	///		Returns MFS_ERROR_INVALID_ARGUMENTS if <paramref name="size">size</paramref> is NULL.
	///		Returns MFS_ERROR_INVALID_UNICODE if the unicode point value is invalid.
	/// </returns>
	mfError mfsGetSizeAsUTF8(mfsUnicodePoint up, mfmU64* size);

	/// <summary>
	///		Gets the unicode point value of a UTF-8 character.
	/// </summary>
	/// <param name="chr">Pointer to UTF-8 character</param>
	/// <param name="up">Pointer to character unicode point value</param>
	/// <returns>
	///		Returns MF_ERROR_OKAY if there were no errors.
	///		Returns MFS_ERROR_INVALID_ARGUMENTS if <paramref name="chr">chr</paramref> or <paramref name="up">up</paramref> are NULL.
	///		Returns MFS_ERROR_INVALID_UTF8 if the UTF-8 character at <paramref name="chr">chr</paramref> is invalid UTF-8.
	/// </returns>
	mfError mfsGetUTF8Char(const mfsUTF8CodeUnit* chr, mfsUnicodePoint* up);

	/// <summary>
	///		Sets the unicode point value of a UTF-8 character.
	/// </summary>
	/// <param name="up">Character unicode point value</param>
	/// <param name="chr">Pointer to the location where the UTF-8 charactcer will be written to</param>
	/// <param name="chrSize">Pointer to the character size (set to NULL to ignore)</param>
	/// <param name="maxChrSize">The maximum size of the character in bytes</param>
	/// <returns>
	///		Returns MF_ERROR_OKAY if there were no errors.
	///		Returns MFS_ERROR_INVALID_ARGUMENTS if <paramref name="chr">chr</paramref> is NULL.
	///		Returns MFS_ERROR_INVALID_UNICODE if the character unicode point value is invalid.
	///		Returns MFS_ERROR_CHARACTER_TOO_BIG if the character size as UTF-8 is bigger than <paramref name="maxChrSize">maxChrSize</paramref>.
	/// </returns>
	mfError mfsSetUTF8Char(mfsUnicodePoint up, mfsUTF8CodeUnit* chr, mfmU64* chrSize, mfmU64 maxChrSize);

#ifdef __cplusplus
}
#endif
