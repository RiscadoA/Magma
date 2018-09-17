#include <Magma/Framework/Graphics/ErrorString.h>

const mfsUTF8CodeUnit * mfgErrorToString(mfError err)
{
	switch (err)
	{
		case MF_ERROR_OKAY:
			return u8"[MF_ERROR_OKAY] No error";
		case MFG_ERROR_INVALID_ARGUMENTS:
			return u8"[MFG_ERROR_INVALID_ARGUMENTS] Invalid arguments";
		case MFG_ERROR_ALLOCATION_FAILED:
			return u8"[MFG_ERROR_ALLOCATION_FAILED] Allocation failed";
		case MFG_ERROR_NOT_FOUND:
			return u8"[MFG_ERROR_NOT_FOUND] Couldn't find something";
		case MFG_ERROR_INVALID_DATA:
			return u8"[MFG_ERROR_INVALID_DATA] Invalid data";
		case MFG_ERROR_UNSUPPORTED_MAJOR_VER:
			return u8"[MFG_ERROR_UNSUPPORTED_MAJOR_VER] Unsupported major version";
		case MFG_ERROR_UNSUPPORTED_MINOR_VER:
			return u8"[MFG_ERROR_UNSUPPORTED_MINOR_VER] Unsupported minor version";
		case MFG_ERROR_FAILED_TO_WRITE:
			return u8"[MFG_ERROR_FAILED_TO_WRITE] Failed to write";
		case MFG_ERROR_NOT_SUPPORTED:
			return u8"[MFG_ERROR_NOT_SUPPORTED] Unsupported feature";
		case MFG_ERROR_NO_EXTENSION:
			return u8"[MFG_ERROR_NO_EXTENSION] Couldn't get extension";
		case MFG_ERROR_INTERNAL:
			return u8"[MFG_ERROR_INTERNAL] Internal error";
		case MFG_ERROR_NO_REGISTER_ENTRIES:
			return u8"[MFG_ERROR_NO_REGISTER_ENTRIES] No register entries available";
		case MFG_ERROR_TYPE_NOT_REGISTERED:
			return u8"[MFG_ERROR_TYPE_NOT_REGISTERED] Type not registered";

		default:
			return NULL;
	}
}
