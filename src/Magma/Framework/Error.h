#pragma once

#include "Memory/Type.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define MF_ERROR_OKAY					0x0000
#define MF_ERROR_ALREADY_INITIALIZED	0x0001
#define MF_ERROR_NOT_INITIALIZED		0x0002

	typedef mfmU32 mfError;

#ifdef __cplusplus
}
#endif
