#include "ErrorString.h"

const mfsUTF8CodeUnit * mffErrorToString(mfError err)
{
	switch (err)
	{
		case MF_ERROR_OKAY:
			return u8"[MF_ERROR_OKAY] No error";
		case MFF_ERROR_INVALID_ARGUMENTS:
			return u8"[MFF_ERROR_INVALID_ARGUMENTS] Invalid arguments";
		case MFF_ERROR_FILE_NOT_FOUND:
			return u8"[MFF_ERROR_FILE_NOT_FOUND] File not found";
		case MFF_ERROR_INTERNAL_ERROR:
			return u8"[MFF_ERROR_INTERNAL_ERROR] Internal error";
		case MFF_ERROR_NOT_SUPPORTED:
			return u8"[MFF_ERROR_NOT_SUPPORTED] Unsupported feature";
		case MFF_ERROR_NO_REGISTER_ENTRIES:
			return u8"[MFF_ERROR_NO_REGISTER_ENTRIES] No register entries available";
		case MFF_ERROR_TYPE_NOT_REGISTERED:
			return u8"[MFF_ERROR_TYPE_NOT_REGISTERED] Type not registered";
		default:
			return NULL;
	}
}
