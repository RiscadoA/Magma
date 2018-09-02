#include "ErrorString.h"

const mfsUTF8CodeUnit * mfsErrorToString(mfError err)
{
	switch (err)
	{
		case MF_ERROR_OKAY:
			return u8"[MF_ERROR_OKAY] No error";
		case MFS_ERROR_INVALID_ARGUMENTS:
			return u8"[MFS_ERROR_INVALID_ARGUMENTS] Invalid arguments";
		case MFS_ERROR_INVALID_UTF8:
			return u8"[MFS_ERROR_INVALID_UTF8] Invalid UTF-8";
		case MFS_ERROR_INVALID_UTF16:
			return u8"[MFS_ERROR_INVALID_UTF16] Invalid UTF-16";
		case MFS_ERROR_INVALID_UNICODE:
			return u8"[MFS_ERROR_INVALID_UNICODE] Invalid unicode";
		case MFS_ERROR_CHARACTER_TOO_BIG:
			return u8"[MFS_ERROR_CHARACTER_TOO_BIG] Character is too big";
		case MFS_ERROR_EOF:
			return u8"[MFS_ERROR_EOF] Stream reached end of file";
		case MFS_ERROR_INTERNAL:
			return u8"[MFS_ERROR_INTERNAL] Internal error";
		case MFS_ERROR_FAILED_TO_READ_ALL:
			return u8"[MFS_ERROR_FAILED_TO_READ_ALL] Failed to read all of the data requested from stream";
		case MFS_ERROR_FAILED_TO_WRITE_ALL:
			return u8"[MFS_ERROR_FAILED_TO_WRITE_ALL] Failed to write all of the data requested to stream";

		default:
			return NULL;
	}
}
