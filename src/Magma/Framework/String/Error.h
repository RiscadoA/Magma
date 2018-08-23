#pragma once

#include "../Memory/Type.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define MFS_ERROR_OKAY					0x0100
#define MFS_ERROR_INVALID_ARGUMENTS		0x0101
#define MFS_ERROR_INVALID_UTF8			0x0102
#define MFS_ERROR_INVALID_UTF16			0x0103
#define MFS_ERROR_INVALID_UNICODE		0x0104
#define MFS_ERROR_CHARACTER_TOO_BIG		0x0105
#define MFS_ERROR_EOF					0x0106
#define MFS_ERROR_INTERNAL				0x0107
#define MFS_ERROR_FAILED_TO_READ_ALL	0x0108
#define MFS_ERROR_FAILED_TO_WRITE_ALL	0x0109

	typedef mfmI32 mfsError;

#ifdef __cplusplus
}
#endif
