#pragma once

#include "../Error.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define MFT_ERROR_INVALID_ARGUMENTS		0x0301
#define MFT_ERROR_INTERNAL				0x0302
#define MFT_ERROR_TIMEOUT				0x0303
#define MFT_ERROR_STILL_RUNNING			0x0304
#define MFT_ERROR_MUTEX_LOCKED			0x0305

#ifdef __cplusplus
}
#endif
