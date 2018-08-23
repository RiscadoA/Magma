#pragma once

#include "../../Memory/Type.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define MFG_ERROR_OKAY					0x0300
#define MFG_ERROR_INVALID_ARGUMENTS		0x0301
#define MFG_ERROR_ALLOCATION_FAILED		0X0302
#define MFG_ERROR_NOT_FOUND				0x0303
#define MFG_ERROR_INVALID_DATA			0x0304
#define MFG_ERROR_UNSUPPORTED_MAJOR_VER	0x0305
#define MFG_ERROR_UNSUPPORTED_MINOR_VER	0x0306
#define MFG_ERROR_FAILED_TO_WRITE		0x0307
#define MFG_ERROR_NOT_SUPPORTED			0x0308
#define MFG_ERROR_NO_EXTENSION			0x0309
#define MFG_ERROR_INTERNAL				0x030A

typedef mfmU32 mfError;

#ifdef __cplusplus
}
#endif
