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

typedef mfmI32 mfgError;

#ifdef __cplusplus
}
#endif
