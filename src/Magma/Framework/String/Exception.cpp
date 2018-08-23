#include "Exception.hpp"

const mfsUTF8CodeUnit * Magma::Framework::String::ErrorToString(mfError err)
{
	switch (err)
	{
		case MFS_ERROR_OKAY:
			return u8"No error";
		case MFS_ERROR_INVALID_ARGUMENTS:
			return u8"Invalid arguments";
		case MFS_ERROR_INVALID_UTF8:
			return u8"Invalid UTF-8";
		case MFS_ERROR_INVALID_UTF16:
			return u8"Invalid UTF-16";
		case MFS_ERROR_INVALID_UNICODE:
			return u8"Invalid unicode";
		case MFS_ERROR_CHARACTER_TOO_BIG:
			return u8"Character too big";
		case MFS_ERROR_EOF:
			return u8"End of file reached";
		case MFS_ERROR_INTERNAL:
			return u8"Internal error";
		case MFS_ERROR_FAILED_TO_READ_ALL:
			return u8"Failed to read all";
		case MFS_ERROR_FAILED_TO_WRITE_ALL:
			return u8"Failed to write all";
		default:
			return u8"Unknown error";
	}
}