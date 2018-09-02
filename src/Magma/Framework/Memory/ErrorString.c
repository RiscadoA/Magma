#include "ErrorString.h"

const mfsUTF8CodeUnit * mfmErrorToString(mfError err)
{
	switch (err)
	{
		case MF_ERROR_OKAY:
			return u8"[MF_ERROR_OKAY] No error";
		case MFM_ERROR_ALLOCATION_FAILED:
			return u8"[MFM_ERROR_ALLOCATION_FAILED] Allocation failed";
		case MFM_ERROR_ALLOCATOR_OVERFLOW:
			return u8"[MFM_ERROR_ALLOCATOR_OVERFLOW] Allocator overflow";
		case MFM_ERROR_ALLOCATION_TOO_BIG:
			return u8"[MFM_ERROR_ALLOCATION_TOO_BIG] Allocation too big";
		case MFM_ERROR_OUT_OF_BOUNDS:
			return u8"[MFM_ERROR_OUT_OF_BOUNDS] Out of bounds";
		case MFM_ERROR_INVALID_ARGUMENTS:
			return u8"[MFM_ERROR_INVALID_ARGUMENTS] Invalid arguments";
		case MFM_ERROR_UNSUPPORTED_FUNCTION:
			return u8"[MFM_ERROR_UNSUPPORTED_FUNCTION] Unsupported function";
		case MFM_ERROR_STILL_HAS_REFERENCES:
			return u8"[MFM_ERROR_STILL_HAS_REFERENCES] Object still has references to it";

		default:
			return NULL;
	}
}
