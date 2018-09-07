#include "ErrorString.h"

const mfsUTF8CodeUnit * mfaErrorToString(mfError err)
{
	switch (err)
	{
		case MF_ERROR_OKAY:
			return u8"[MF_ERROR_OKAY] No error";
		case MFA_ERROR_INVALID_ARGUMENTS:
			return u8"[MFA_ERROR_INVALID_ARGUMENTS] Invalid arguments";
		case MFA_ERROR_INTERNAL:
			return u8"[MFA_ERROR_INTERNAL] Internal error";
		case MFA_ERROR_NO_REGISTER_ENTRIES:
			return u8"[MFA_ERROR_NO_REGISTER_ENTRIES] No register entries available";
		case MFA_ERROR_TYPE_NOT_REGISTERED:
			return u8"[MFA_ERROR_TYPE_NOT_REGISTERED] Type not registered";

		default:
			return NULL;
	}
}
