#pragma once

#include "../Memory/Type.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define MFI_MOUSE_INVALID	0x00

#define MFI_MOUSE_LEFT		0x01
#define MFI_MOUSE_RIGHT		0x02
#define MFI_MOUSE_MIDDLE	0x03
#define MFI_MOUSE_MAX		0x03

	typedef mfmU32 mfiMouseButton;

#ifdef __cplusplus
}
#endif
