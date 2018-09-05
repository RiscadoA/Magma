#pragma once

#include "../Error.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define MFS_ERROR_INVALID_ARGUMENTS		0x0201
#define MFS_ERROR_INVALID_UTF8			0x0202
#define MFS_ERROR_INVALID_UTF16			0x0203
#define MFS_ERROR_INVALID_UNICODE		0x0204
#define MFS_ERROR_CHARACTER_TOO_BIG		0x0205
#define MFS_ERROR_EOF					0x0206
#define MFS_ERROR_INTERNAL				0x0207
#define MFS_ERROR_FAILED_TO_READ_ALL	0x0208
#define MFS_ERROR_FAILED_TO_WRITE_ALL	0x0209
#define MFS_ERROR_UNSUPPORTED_FUNCTION	0x020A

#ifdef __cplusplus
}
#endif
