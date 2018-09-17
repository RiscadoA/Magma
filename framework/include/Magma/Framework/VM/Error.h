#pragma once

#include "../Error.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define MFV_ERROR_INVALID_ARGUMENTS			0x0601
#define MFV_ERROR_INVALID_INSTRUCTION		0x0602
#define MFV_ERROR_ERROR_THROWN				0x0603
#define MFV_ERROR_WARNING_THROWN			0x0604
#define MFV_ERROR_FUNCTION_ALREADY_DEFINED	0x0605
#define MFV_ERROR_FUNCTION_NOT_DEFINED		0x0606
#define MFV_ERROR_STACK_OVERFLOW			0x0607
#define MFV_ERROR_STACK_UNDERFLOW			0x0608
#define MFV_ERROR_NULL_CODE					0x0609
#define MFV_ERROR_CALL_STACK_OVERFLOW		0x060A
#define MFV_ERROR_CALL_STACK_UNDERFLOW		0x060B
#define MFV_ERROR_REGISTER_OUT_OF_BOUNDS	0x060C
#define MFV_ERROR_TOKENS_OVERFLOW			0x060D
#define MFV_ERROR_UNKNOWN_TOKEN				0x060E
#define MFV_ERROR_TOKEN_ATTRIBUTE_TOO_BIG	0x060F
#define MFV_ERROR_NODES_OVERFLOW			0x0610
#define MFV_ERROR_UNEXPECTED_TOKEN			0x0611
#define MFV_ERROR_UNEXPECTED_EOF			0x0612
#define MFV_ERROR_INACTIVE_NODE				0x0613
#define MFV_ERROR_FAILED_TO_PARSE			0x0614

#ifdef __cplusplus
}
#endif