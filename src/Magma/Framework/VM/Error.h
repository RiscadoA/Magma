#pragma once

#include "../Error.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define MFV_ERROR_INVALID_ARGUMENTS			0x0601
#define MFV_ERROR_INVALID_INSTRUCTION		0x0602
#define MFV_ERROR_EXCEPTION_THROWN			0x0603
#define MFV_ERROR_FUNCTION_ALREADY_DEFINED	0x0604
#define MFV_ERROR_FUNCTION_NOT_DEFINED		0x0605
#define MFV_ERROR_STACK_OVERFLOW			0x0606
#define MFV_ERROR_STACK_UNDERFLOW			0x0607
#define MFV_ERROR_NULL_CODE					0x0608

#ifdef __cplusplus
}
#endif
