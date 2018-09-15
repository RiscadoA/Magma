#include <Magma/Framework/String/UTF32.h>

mfmBool mfsIsValidUTF32Char(mfsUnicodePoint up)
{
	if (up > 0x10FFFF)
		return MFM_FALSE;
	if (up >= 0xD800 && up <= 0xDFFF)
		return MFM_FALSE;
	return MFM_TRUE;
}
