#pragma once

#include "../Memory/Type.h"

#include <uchar.h>

/*
	UTF-32 string manipulation functions.
*/

#ifdef __cplusplus
extern "C"
{
#endif
	typedef char32_t mfsUTF32CodeUnit;
	typedef char32_t mfsUnicodePoint;

	/// <summary>
	///		Checks if a unicode point value is valid unicode.
	/// </summary>
	/// <param name="up">Unicode point value</param>
	/// <returns>MFM_TRUE if valid, otherwise MFM_FALSE</returns>
	mfmBool mfsIsValidUTF32Char(mfsUnicodePoint up);

#ifdef __cplusplus
}
#endif
