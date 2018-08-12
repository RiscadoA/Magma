#pragma once

#include "../Memory/Type.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define MFS_ERROR_OKAY					0x00
#define MFS_ERROR_INVALID_ARGUMENTS		0x01
#define MFS_ERROR_INVALID_UTF8			0x02
#define MFS_ERROR_INVALID_UTF16			0x03
#define MFS_ERROR_INVALID_UNICODE		0x04
#define MFS_ERROR_CHARACTER_TOO_BIG		0x05
#define MFS_ERROR_EOF					0x06
#define MFS_ERROR_INTERNAL				0x07
#define MFS_ERROR_FAILED_TO_READ_ALL	0x08
#define MFS_ERROR_FAILED_TO_WRITE_ALL	0x09

	typedef mfmI32 mfsError;

#ifdef __cplusplus
}
#endif
