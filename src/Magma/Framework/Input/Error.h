#pragma once

#include "../Memory/Type.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define MFI_ERROR_OKAY					0x00
#define MFI_ERROR_ALLOCATION_FAILED		0x01
#define MFI_ERROR_INVALID_ARGUMENTS		0x02
#define MFI_ERROR_INTERNAL				0x03
#define MFI_ERROR_ALREADY_INITIALIZED	0x04
#define MFI_ERROR_NOT_SUPPORTED			0x05

	typedef mfmI32 mfiError;

#ifdef __cplusplus
}
#endif
