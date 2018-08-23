#include "Exception.hpp"

const mfsUTF8CodeUnit * Magma::Framework::Memory::ErrorToString(mfError err)
{
	switch (err)
	{
		case MFM_ERROR_OKAY:
			return u8"No error";
		case MFM_ERROR_ALLOCATION_FAILED:
			return u8"Allocation failed";
		case MFM_ERROR_ALLOCATOR_OVERFLOW:
			return u8"Allocator overflow";
		case MFM_ERROR_ALLOCATION_TOO_BIG:
			return u8"Allocation too big";
		case MFM_ERROR_OUT_OF_BOUNDS:
			return u8"Out of bounds";
		case MFM_ERROR_INVALID_ARGUMENTS:
			return u8"Invalid arguments";
		case MFM_ERROR_UNSUPPORTED_FUNCTION:
			return u8"Unsupported function";
		case MFM_ERROR_STILL_HAS_REFERENCES:
			return u8"Still has references";
		default:
			return u8"Unknown error";
	}
}