#pragma once

#include "../Memory/Type.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define MFG_ERROR_OKAY					0x00
#define MFG_ERROR_INVALID_ARGUMENTS		0x01
#define MFG_ERROR_ALLOCATION_FAILED		0X02
#define MFG_ERROR_NOT_FOUND				0x03
#define MFG_ERROR_INVALID_DATA			0x04
#define MFG_ERROR_UNSUPPORTED_MAJOR_VER	0x05
#define MFG_ERROR_UNSUPPORTED_MINOR_VER	0x06
#define MFG_ERROR_FAILED_TO_WRITE		0x07
#define MFG_ERROR_NOT_SUPPORTED			0x08
#define MFG_ERROR_NO_EXTENSION			0x09
#define MFG_ERROR_INTERNAL				0x0A

typedef mfmI32 mfgError;

#ifdef __cplusplus
}
#endif
