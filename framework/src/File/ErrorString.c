#include <Magma/Framework/File/ErrorString.h>

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
		case MFF_ERROR_INTERNAL:
			return u8"[MFF_ERROR_INTERNAL_ERROR] Internal error";
		case MFF_ERROR_NOT_SUPPORTED:
			return u8"[MFF_ERROR_NOT_SUPPORTED] Unsupported feature";
		case MFF_ERROR_NO_REGISTER_ENTRIES:
			return u8"[MFF_ERROR_NO_REGISTER_ENTRIES] No register entries available";
		case MFF_ERROR_TYPE_NOT_REGISTERED:
			return u8"[MFF_ERROR_TYPE_NOT_REGISTERED] Type not registered";
		case MFF_ERROR_PATH_TOO_BIG:
			return u8"[MFF_ERROR_PATH_TOO_BIG] Path too big";
		case MFF_ERROR_ARCHIVE_NOT_FOUND:
			return u8"[MFF_ERROR_ARCHIVE_NOT_FOUND] Archive not found";
		case MFF_ERROR_NOT_A_FILE:
			return u8"[MFF_ERROR_NOT_A_FILE] Path doesn't point to a file";
		case MFF_ERROR_NOT_A_DIRECTORY:
			return u8"[MFF_ERROR_NOT_A_DIRECTORY] Path doesn't point to a directory";
		case MFF_ERROR_MUST_BE_EMPTY:
			return u8"[MFF_ERROR_MUST_BE_EMPTY] Directory must be empty";
		case MFF_ERROR_INVALID_MODE:
			return u8"[MFF_ERROR_INVALID_MODE] Invalid file open mode";
		case MFF_ERROR_ALREADY_EXISTS:
			return u8"[MFF_ERROR_ALREADY_EXISTS] File/directory already exists";
		default:
			return NULL;
	}
}
