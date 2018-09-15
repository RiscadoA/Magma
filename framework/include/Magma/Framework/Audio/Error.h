#pragma once

#include "../Error.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define MFA_ERROR_INVALID_ARGUMENTS					0x0801
#define MFA_ERROR_INTERNAL							0x0802
#define MFA_ERROR_NO_REGISTER_ENTRIES				0x0803
#define MFA_ERROR_TYPE_NOT_REGISTERED				0x0804
#define MFA_ERROR_NOT_SUPPORTED						0x0805
#define MFA_ERROR_TOO_MANY_QUEUED_BUFFERS			0x0806
#define MFA_ERROR_NO_QUEUED_BUFFERS					0x0807
#define MFA_ERROR_INVALID_DATA						0x0808

#ifdef __cplusplus
}
#endif
