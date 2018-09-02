#include "ErrorString.h"

const mfsUTF8CodeUnit * mftErrorToString(mfError err)
{
	switch (err)
	{
		case MF_ERROR_OKAY:
			return u8"[MF_ERROR_OKAY] No error";
		case MFT_ERROR_INVALID_ARGUMENTS:
			return u8"[MFT_ERROR_INVALID_ARGUMENTS] Invalid arguments";
		case MFT_ERROR_INTERNAL:
			return u8"[MFT_ERROR_INTERNAL] Internal error";
		case MFT_ERROR_TIMEOUT:
			return u8"[MFT_ERROR_TIMEOUT] Timeout reached";
		case MFT_ERROR_STILL_RUNNING:
			return u8"[MFT_ERROR_STILL_RUNNING] Thread is still running";
		case MFT_ERROR_MUTEX_LOCKED:
			return u8"[MFT_ERROR_MUTEX_LOCKED] Mutex is locked";

		default:
			return NULL;
	}
}
