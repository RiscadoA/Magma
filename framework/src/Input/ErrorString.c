#include <Magma/Framework/Input/ErrorString.h>

const mfsUTF8CodeUnit * mfiErrorToString(mfError err)
{
	switch (err)
	{
		case MF_ERROR_OKAY:
			return u8"[MF_ERROR_OKAY] No error";
		case MFI_ERROR_ALLOCATION_FAILED:
			return u8"[MFI_ERROR_ALLOCATION_FAILED] Allocation failed";
		case MFI_ERROR_INVALID_ARGUMENTS:
			return u8"[MFI_ERROR_INVALID_ARGUMENTS] Invalid arguments";
		case MFI_ERROR_INTERNAL:
			return u8"[MFI_ERROR_INTERNAL] Internal error";
		case MFI_ERROR_ALREADY_INITIALIZED:
			return u8"[MFI_ERROR_ALREADY_INITIALIZED] Already initialized";
		case MFI_ERROR_NOT_SUPPORTED:
			return u8"[MFI_ERROR_NOT_SUPPORTED] Unsupported feature";
		case MFI_ERROR_NO_REGISTER_ENTRIES:
			return u8"[MFI_ERROR_NO_REGISTER_ENTRIES] No register entries available";
		case MFI_ERROR_TYPE_NOT_REGISTERED:
			return u8"[MFI_ERROR_TYPE_NOT_REGISTERED] Type not registered";
		default:
			return NULL;
	}
}
