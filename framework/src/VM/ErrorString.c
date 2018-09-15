#include <Magma/Framework/VM/ErrorString.h>

const mfsUTF8CodeUnit * mfvErrorToString(mfError err)
{
	switch (err)
	{
		case MF_ERROR_OKAY:
			return u8"[MF_ERROR_OKAY] No error";
		case MFV_ERROR_INVALID_ARGUMENTS:
			return u8"[MFV_ERROR_INVALID_ARGUMENTS] Invalid arguments";
		case MFV_ERROR_INVALID_INSTRUCTION:
			return u8"[MFV_ERROR_INVALID_INSTRUCTION] Invalid bytecode instruction";
		case MFV_ERROR_ERROR_THROWN:
			return u8"[MFV_ERROR_ERROR_THROWN] An error was thrown by the program";
		case MFV_ERROR_WARNING_THROWN:
			return u8"[MFV_ERROR_WARNING_THROWN] A warning was thrown by the program";
		case MFV_ERROR_FUNCTION_ALREADY_DEFINED:
			return u8"[MFV_ERROR_FUNCTION_ALREADY_DEFINED] Function is already defined";
		case MFV_ERROR_FUNCTION_NOT_DEFINED:
			return u8"[MFV_ERROR_FUNCTION_NOT_DEFINED] Function is not defined";
		case MFV_ERROR_STACK_OVERFLOW:
			return u8"[MFV_ERROR_STACK_OVERFLOW] Virtual stack overflow";
		case MFV_ERROR_STACK_UNDERFLOW:
			return u8"[MFV_ERROR_STACK_UNDERFLOW] Virtual stack underflow";
		case MFV_ERROR_NULL_CODE:
			return u8"[MFV_ERROR_NULL_CODE] No code in the virtual machine";
		case MFV_ERROR_CALL_STACK_OVERFLOW:
			return u8"[MFV_ERROR_CALL_STACK_OVERFLOW] Virtual call stack overflow";
		case MFV_ERROR_CALL_STACK_UNDERFLOW:
			return u8"[MFV_ERROR_CALL_STACK_UNDERFLOW] Virtual call stack underflow";
		case MFV_ERROR_REGISTER_OUT_OF_BOUNDS:
			return u8"[MFV_ERROR_REGISTER_OUT_OF_BOUNDS] Register out of bounds";
		case MFV_ERROR_TOKENS_OVERFLOW:
			return u8"[MFV_ERROR_TOKENS_OVERFLOW] Tokens overflow";
		case MFV_ERROR_UNKNOWN_TOKEN:
			return u8"[MFV_ERROR_UNKNOWN_TOKEN] Unknown token";
		case MFV_ERROR_TOKEN_ATTRIBUTE_TOO_BIG:
			return u8"[MFV_ERROR_TOKEN_ATTRIBUTE_TOO_BIG] Token attribute too big";
		case MFV_ERROR_NODES_OVERFLOW:
			return u8"[MFV_ERROR_NODES_OVERFLOW] Nodes overflow";
		case MFV_ERROR_UNEXPECTED_TOKEN:
			return u8"[MFV_ERROR_UNEXPECTED_TOKEN] Unexpected token";
		case MFV_ERROR_UNEXPECTED_EOF:
			return u8"[MFV_ERROR_UNEXPECTED_EOF] Unexpected end of file";
		case MFV_ERROR_INACTIVE_NODE:
			return u8"[MFV_ERROR_INACTIVE_NODE] Inactive node";
		case MFV_ERROR_FAILED_TO_PARSE:
			return u8"[MFV_ERROR_FAILED_TO_PARSE] Failed to parse";


		default:
			return NULL;
	}
}
