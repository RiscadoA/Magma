#pragma once

#include "Type.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define MFM_ERROR_OKAY					0x0000
#define MFM_ERROR_ALLOCATION_FAILED		0X0001
#define MFM_ERROR_ALLOCATOR_OVERFLOW	0x0002
#define MFM_ERROR_ALLOCATION_TOO_BIG	0x0003
#define MFM_ERROR_OUT_OF_BOUNDS			0x0004
#define MFM_ERROR_INVALID_ARGUMENTS		0x0005
#define MFM_ERROR_UNSUPPORTED_FUNCTION	0x0006
#define MFM_ERROR_STILL_HAS_REFERENCES	0x0007

typedef mfmU32 mfError;

#ifdef __cplusplus
}
#endif
