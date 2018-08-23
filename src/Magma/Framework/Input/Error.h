#pragma once

#include "../Memory/Type.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define MFI_ERROR_OKAY					0x0200
#define MFI_ERROR_ALLOCATION_FAILED		0x0201
#define MFI_ERROR_INVALID_ARGUMENTS		0x0202
#define MFI_ERROR_INTERNAL				0x0203
#define MFI_ERROR_ALREADY_INITIALIZED	0x0204
#define MFI_ERROR_NOT_SUPPORTED			0x0205

	typedef mfmU32 mfError;

#ifdef __cplusplus
}
#endif
