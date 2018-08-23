#pragma once

#include "../Memory/Type.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define MFT_ERROR_OKAY					0x0400
#define MFT_ERROR_INVALID_ARGUMENTS		0x0401
#define MFT_ERROR_INTERNAL				0x0402
#define MFT_ERROR_TIMEOUT				0x0403
#define MFT_ERROR_STILL_RUNNING			0x0404

	typedef mfmU32 mfError;

#ifdef __cplusplus
}
#endif
