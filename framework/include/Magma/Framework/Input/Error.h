#pragma once

#include "../Error.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define MFI_ERROR_ALLOCATION_FAILED		0x0401
#define MFI_ERROR_INVALID_ARGUMENTS		0x0402
#define MFI_ERROR_INTERNAL				0x0403
#define MFI_ERROR_ALREADY_INITIALIZED	0x0404
#define MFI_ERROR_NOT_SUPPORTED			0x0405
#define MFI_ERROR_NO_REGISTER_ENTRIES	0x0406
#define MFI_ERROR_TYPE_NOT_REGISTERED	0x0407

#ifdef __cplusplus
}
#endif
