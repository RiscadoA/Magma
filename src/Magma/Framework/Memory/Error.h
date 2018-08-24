#pragma once

#include "../Error.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define MFM_ERROR_ALLOCATION_FAILED		0x0101
#define MFM_ERROR_ALLOCATOR_OVERFLOW	0x0102
#define MFM_ERROR_ALLOCATION_TOO_BIG	0x0103
#define MFM_ERROR_OUT_OF_BOUNDS			0x0104
#define MFM_ERROR_INVALID_ARGUMENTS		0x0105
#define MFM_ERROR_UNSUPPORTED_FUNCTION	0x0106
#define MFM_ERROR_STILL_HAS_REFERENCES	0x0107

#ifdef __cplusplus
}
#endif
