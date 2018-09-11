#include "Conversion.h"

mfError mfsPrintToBufferU8(mfsUTF8CodeUnit * buffer, mfmU64 bufferSize, mfmU8 value, mfmU64 base, mfmU64 * outSize)
{
	if (buffer == NULL || bufferSize == 0 || base <= 1 || base > 16)
		return MFS_ERROR_INVALID_ARGUMENTS;

	const mfsUTF8CodeUnit characters[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

	for (mfmU64 i = 0;; ++i)
	{
		// Buffer end reached
		if (i == bufferSize)
		{
			if (outSize != NULL)
				*outSize = i;
			return MFS_ERROR_EOF;
		}

		buffer[i] = characters[value % base];
		value /= base;

		// Finished
		if (value <= 0)
		{
			// Reverse string
			for (mfmU64 j = 0; j < i + 1; ++j)
			{
				if (j >= i - j)
					break;
				mfsUTF8CodeUnit temp = buffer[j];
				buffer[j] = buffer[i - j];
				buffer[i - j] = temp;
			}

			if (outSize != NULL)
				*outSize = i + 1;
			return MF_ERROR_OKAY;
		}
	}
}

mfError mfsPrintToBufferI8(mfsUTF8CodeUnit * buffer, mfmU64 bufferSize, mfmI8 value, mfmU64 base, mfmU64 * outSize)
{
	if (buffer == NULL || bufferSize == 0 || base <= 1 || base > 16)
		return MFS_ERROR_INVALID_ARGUMENTS;

	const mfsUTF8CodeUnit characters[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

	mfmBool negative = (value < 0) ? MFM_TRUE : MFM_FALSE;

	for (mfmU64 i = 0;; ++i)
	{
		if (i == 0 && negative != MFM_FALSE)
		{
			buffer[0] = '-';
			value = -value;
			continue;
		}

		// Buffer end reached
		if (i == bufferSize)
		{
			if (outSize != NULL)
				*outSize = i;
			return MFS_ERROR_EOF;
		}

		buffer[i] = characters[value % base];
		value /= base;

		// Finished
		if (value <= 0)
		{
			// Reverse string
			if (negative == MFM_FALSE)
				for (mfmU64 j = 0; j < i + 1; ++j)
				{
					if (j >= i - j)
						break;
					mfsUTF8CodeUnit temp = buffer[j];
					buffer[j] = buffer[i - j];
					buffer[i - j] = temp;
				}
			else
				for (mfmU64 j = 1; j < i + 2; ++j)
				{
					if (j >= i - j + 1)
						break;
					mfsUTF8CodeUnit temp = buffer[j];
					buffer[j] = buffer[i - j + 1];
					buffer[i - j + 1] = temp;
				}

			if (outSize != NULL)
				*outSize = i + 1;
			return MF_ERROR_OKAY;
		}
	}
}

mfError mfsPrintToBufferU16(mfsUTF8CodeUnit * buffer, mfmU64 bufferSize, mfmU16 value, mfmU64 base, mfmU64 * outSize)
{
	if (buffer == NULL || bufferSize == 0 || base <= 1 || base > 16)
		return MFS_ERROR_INVALID_ARGUMENTS;

	const mfsUTF8CodeUnit characters[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

	for (mfmU64 i = 0;; ++i)
	{
		// Buffer end reached
		if (i == bufferSize)
		{
			if (outSize != NULL)
				*outSize = i;
			return MFS_ERROR_EOF;
		}

		buffer[i] = characters[value % base];
		value /= base;

		// Finished
		if (value <= 0)
		{
			// Reverse string
			for (mfmU64 j = 0; j < i + 1; ++j)
			{
				if (j >= i - j)
					break;
				mfsUTF8CodeUnit temp = buffer[j];
				buffer[j] = buffer[i - j];
				buffer[i - j] = temp;
			}

			if (outSize != NULL)
				*outSize = i + 1;
			return MF_ERROR_OKAY;
		}
	}
}

mfError mfsPrintToBufferI16(mfsUTF8CodeUnit * buffer, mfmU64 bufferSize, mfmI16 value, mfmU64 base, mfmU64 * outSize)
{
	if (buffer == NULL || bufferSize == 0 || base <= 1 || base > 16)
		return MFS_ERROR_INVALID_ARGUMENTS;

	const mfsUTF8CodeUnit characters[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

	mfmBool negative = (value < 0) ? MFM_TRUE : MFM_FALSE;

	for (mfmU64 i = 0;; ++i)
	{
		if (i == 0 && negative != MFM_FALSE)
		{
			buffer[0] = '-';
			value = -value;
			continue;
		}

		// Buffer end reached
		if (i == bufferSize)
		{
			if (outSize != NULL)
				*outSize = i;
			return MFS_ERROR_EOF;
		}

		buffer[i] = characters[value % base];
		value /= base;

		// Finished
		if (value <= 0)
		{
			// Reverse string
			if (negative == MFM_FALSE)
				for (mfmU64 j = 0; j < i + 1; ++j)
				{
					if (j >= i - j)
						break;
					mfsUTF8CodeUnit temp = buffer[j];
					buffer[j] = buffer[i - j];
					buffer[i - j] = temp;
				}
			else
				for (mfmU64 j = 1; j < i + 2; ++j)
				{
					if (j >= i - j + 1)
						break;
					mfsUTF8CodeUnit temp = buffer[j];
					buffer[j] = buffer[i - j + 1];
					buffer[i - j + 1] = temp;
				}

			if (outSize != NULL)
				*outSize = i + 1;
			return MF_ERROR_OKAY;
		}
	}
}

mfError mfsPrintToBufferU32(mfsUTF8CodeUnit * buffer, mfmU64 bufferSize, mfmU32 value, mfmU64 base, mfmU64 * outSize)
{
	if (buffer == NULL || bufferSize == 0 || base <= 1 || base > 16)
		return MFS_ERROR_INVALID_ARGUMENTS;

	const mfsUTF8CodeUnit characters[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

	for (mfmU64 i = 0;; ++i)
	{
		// Buffer end reached
		if (i == bufferSize)
		{
			if (outSize != NULL)
				*outSize = i;
			return MFS_ERROR_EOF;
		}

		buffer[i] = characters[value % base];
		value /= base;

		// Finished
		if (value <= 0)
		{
			// Reverse string
			for (mfmU64 j = 0; j < i + 1; ++j)
			{
				if (j >= i - j)
					break;
				mfsUTF8CodeUnit temp = buffer[j];
				buffer[j] = buffer[i - j];
				buffer[i - j] = temp;
			}

			if (outSize != NULL)
				*outSize = i + 1;
			return MF_ERROR_OKAY;
		}
	}
}

mfError mfsPrintToBufferI32(mfsUTF8CodeUnit * buffer, mfmU64 bufferSize, mfmI32 value, mfmU64 base, mfmU64 * outSize)
{
	if (buffer == NULL || bufferSize == 0 || base <= 1 || base > 16)
		return MFS_ERROR_INVALID_ARGUMENTS;

	const mfsUTF8CodeUnit characters[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

	mfmBool negative = (value < 0) ? MFM_TRUE : MFM_FALSE;

	for (mfmU64 i = 0;; ++i)
	{
		if (i == 0 && negative != MFM_FALSE)
		{
			buffer[0] = '-';
			value = -value;
			continue;
		}

		// Buffer end reached
		if (i == bufferSize)
		{
			if (outSize != NULL)
				*outSize = i;
			return MFS_ERROR_EOF;
		}

		buffer[i] = characters[value % base];
		value /= base;

		// Finished
		if (value <= 0)
		{
			// Reverse string
			if (negative == MFM_FALSE)
				for (mfmU64 j = 0; j < i + 1; ++j)
				{
					if (j >= i - j)
						break;
					mfsUTF8CodeUnit temp = buffer[j];
					buffer[j] = buffer[i - j];
					buffer[i - j] = temp;
				}
			else
				for (mfmU64 j = 1; j < i + 2; ++j)
				{
					if (j >= i - j + 1)
						break;
					mfsUTF8CodeUnit temp = buffer[j];
					buffer[j] = buffer[i - j + 1];
					buffer[i - j + 1] = temp;
				}

			if (outSize != NULL)
				*outSize = i + 1;
			return MF_ERROR_OKAY;
		}
	}
}

mfError mfsPrintToBufferU64(mfsUTF8CodeUnit * buffer, mfmU64 bufferSize, mfmU64 value, mfmU64 base, mfmU64 * outSize)
{
	if (buffer == NULL || bufferSize == 0 || base <= 1 || base > 16)
		return MFS_ERROR_INVALID_ARGUMENTS;

	const mfsUTF8CodeUnit characters[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

	for (mfmU64 i = 0;; ++i)
	{
		// Buffer end reached
		if (i == bufferSize)
		{
			if (outSize != NULL)
				*outSize = i;
			return MFS_ERROR_EOF;
		}

		buffer[i] = characters[value % base];
		value /= base;

		// Finished
		if (value <= 0)
		{
			// Reverse string
			for (mfmU64 j = 0; j < i + 1; ++j)
			{
				if (j >= i - j)
					break;
				mfsUTF8CodeUnit temp = buffer[j];
				buffer[j] = buffer[i - j];
				buffer[i - j] = temp;
			}

			if (outSize != NULL)
				*outSize = i + 1;
			return MF_ERROR_OKAY;
		}
	}
}

mfError mfsPrintToBufferI64(mfsUTF8CodeUnit * buffer, mfmU64 bufferSize, mfmI64 value, mfmU64 base, mfmU64 * outSize)
{
	if (buffer == NULL || bufferSize == 0 || base <= 1 || base > 16)
		return MFS_ERROR_INVALID_ARGUMENTS;

	const mfsUTF8CodeUnit characters[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

	mfmBool negative = (value < 0) ? MFM_TRUE : MFM_FALSE;

	for (mfmU64 i = 0;; ++i)
	{
		if (i == 0 && negative != MFM_FALSE)
		{
			buffer[0] = '-';
			value = -value;
			continue;
		}

		// Buffer end reached
		if (i == bufferSize)
		{
			if (outSize != NULL)
				*outSize = i;
			return MFS_ERROR_EOF;
		}

		buffer[i] = characters[value % base];
		value /= base;

		// Finished
		if (value <= 0)
		{
			// Reverse string
			if (negative == MFM_FALSE)
				for (mfmU64 j = 0; j < i + 1; ++j)
				{
					if (j >= i - j)
						break;
					mfsUTF8CodeUnit temp = buffer[j];
					buffer[j] = buffer[i - j];
					buffer[i - j] = temp;
				}
			else
				for (mfmU64 j = 1; j < i + 2; ++j)
				{
					if (j >= i - j + 1)
						break;
					mfsUTF8CodeUnit temp = buffer[j];
					buffer[j] = buffer[i - j + 1];
					buffer[i - j + 1] = temp;
				}

			if (outSize != NULL)
				*outSize = i + 1;
			return MF_ERROR_OKAY;
		}
	}
}

mfError mfsParseFromBufferU8(const mfsUTF8CodeUnit * buffer, mfmU64 bufferSize, mfmU8 * value, mfmU64 base, mfmU64 * outSize)
{
	if (buffer == NULL || bufferSize == 0 || base <= 1 || base > 16)
		return MFS_ERROR_INVALID_ARGUMENTS;

	mfmU64 index = 0;

	// Skip whitespace
	while (buffer[index] == ' ' || buffer[index] == '\n' || buffer[index] == '\t')
	{
		++index;
		if (index >= bufferSize)
			return MFS_ERROR_FAILED_TO_PARSE;
	}

	const mfsUTF8CodeUnit characters[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
	mfmU8 out = 0;

	for (;; ++index)
	{
		if (buffer[index] == ' ' || buffer[index] == '\n' || buffer[index] == '\t' || buffer[index] == '\0')
		{
			if (outSize != NULL)
				*outSize = index;
			break;
		}
		else if (index >= bufferSize)
		{
			if (outSize != NULL)
				*outSize = bufferSize;
			break;
		}

		if (out <= UINT8_MAX / base)
			out *= base;
		else
			return MFS_ERROR_NUMBER_TOO_BIG;

		for (mfmU64 i = 0; i < base + 1; ++i)
		{
			if (i == base)
				return MFS_ERROR_FAILED_TO_PARSE;
			if (characters[i] == buffer[index])
			{
				out += i;
				break;
			}
		}
	}

	if (value != NULL)
		*value = out;

	return MF_ERROR_OKAY;
}

mfError mfsParseFromBufferI8(const mfsUTF8CodeUnit * buffer, mfmU64 bufferSize, mfmI8 * value, mfmU64 base, mfmU64 * outSize)
{
	if (buffer == NULL || bufferSize == 0 || base <= 1 || base > 16)
		return MFS_ERROR_INVALID_ARGUMENTS;

	mfmU64 index = 0;

	// Skip whitespace
	while (buffer[index] == ' ' || buffer[index] == '\n' || buffer[index] == '\t')
	{
		++index;
		if (index >= bufferSize)
			return MFS_ERROR_FAILED_TO_PARSE;
	}

	mfmBool negative = MFM_FALSE;
	// Check if negative
	if (buffer[index] == '-')
	{
		negative = MFM_TRUE;
		++index;
		if (index >= bufferSize)
			return MFS_ERROR_FAILED_TO_PARSE;
	}

	const mfsUTF8CodeUnit characters[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
	mfmI8 out = 0;

	for (;; ++index)
	{
		if (buffer[index] == ' ' || buffer[index] == '\n' || buffer[index] == '\t' || buffer[index] == '\0')
		{
			if (outSize != NULL)
				*outSize = index;
			break;
		}
		else if (index >= bufferSize)
		{
			if (outSize != NULL)
				*outSize = bufferSize;
			break;
		}

		if (out <= INT8_MAX / base)
			out *= base;
		else
			return MFS_ERROR_NUMBER_TOO_BIG;

		for (mfmU64 i = 0; i < base + 1; ++i)
		{
			if (i == base)
				return MFS_ERROR_FAILED_TO_PARSE;
			if (characters[i] == buffer[index])
			{
				out += i;
				break;
			}
		}
	}

	if (negative != MFM_FALSE)
		out = -out;

	if (value != NULL)
		*value = out;

	return MF_ERROR_OKAY;
}

mfError mfsParseFromBufferU16(const mfsUTF8CodeUnit * buffer, mfmU64 bufferSize, mfmU16 * value, mfmU64 base, mfmU64 * outSize)
{
	if (buffer == NULL || bufferSize == 0 || base <= 1 || base > 16)
		return MFS_ERROR_INVALID_ARGUMENTS;

	mfmU64 index = 0;

	// Skip whitespace
	while (buffer[index] == ' ' || buffer[index] == '\n' || buffer[index] == '\t')
	{
		++index;
		if (index >= bufferSize)
			return MFS_ERROR_FAILED_TO_PARSE;
	}

	const mfsUTF8CodeUnit characters[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
	mfmU16 out = 0;

	for (;; ++index)
	{
		if (buffer[index] == ' ' || buffer[index] == '\n' || buffer[index] == '\t' || buffer[index] == '\0')
		{
			if (outSize != NULL)
				*outSize = index;
			break;
		}
		else if (index >= bufferSize)
		{
			if (outSize != NULL)
				*outSize = bufferSize;
			break;
		}

		if (out <= UINT16_MAX / base)
			out *= base;
		else
			return MFS_ERROR_NUMBER_TOO_BIG;

		for (mfmU64 i = 0; i < base + 1; ++i)
		{
			if (i == base)
				return MFS_ERROR_FAILED_TO_PARSE;
			if (characters[i] == buffer[index])
			{
				out += i;
				break;
			}
		}
	}

	if (value != NULL)
		*value = out;

	return MF_ERROR_OKAY;
}

mfError mfsParseFromBufferI16(const mfsUTF8CodeUnit * buffer, mfmU64 bufferSize, mfmI16 * value, mfmU64 base, mfmU64 * outSize)
{
	if (buffer == NULL || bufferSize == 0 || base <= 1 || base > 16)
		return MFS_ERROR_INVALID_ARGUMENTS;

	mfmU64 index = 0;

	// Skip whitespace
	while (buffer[index] == ' ' || buffer[index] == '\n' || buffer[index] == '\t')
	{
		++index;
		if (index >= bufferSize)
			return MFS_ERROR_FAILED_TO_PARSE;
	}

	mfmBool negative = MFM_FALSE;
	// Check if negative
	if (buffer[index] == '-')
	{
		negative = MFM_TRUE;
		++index;
		if (index >= bufferSize)
			return MFS_ERROR_FAILED_TO_PARSE;
	}

	const mfsUTF8CodeUnit characters[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
	mfmI16 out = 0;

	for (;; ++index)
	{
		if (buffer[index] == ' ' || buffer[index] == '\n' || buffer[index] == '\t' || buffer[index] == '\0')
		{
			if (outSize != NULL)
				*outSize = index;
			break;
		}
		else if (index >= bufferSize)
		{
			if (outSize != NULL)
				*outSize = bufferSize;
			break;
		}

		if (out <= INT16_MAX / base)
			out *= base;
		else
			return MFS_ERROR_NUMBER_TOO_BIG;

		for (mfmU64 i = 0; i < base + 1; ++i)
		{
			if (i == base)
				return MFS_ERROR_FAILED_TO_PARSE;
			if (characters[i] == buffer[index])
			{
				out += i;
				break;
			}
		}
	}

	if (negative != MFM_FALSE)
		out = -out;

	if (value != NULL)
		*value = out;

	return MF_ERROR_OKAY;
}

mfError mfsParseFromBufferU32(const mfsUTF8CodeUnit * buffer, mfmU64 bufferSize, mfmU32 * value, mfmU64 base, mfmU64 * outSize)
{
	if (buffer == NULL || bufferSize == 0 || base <= 1 || base > 16)
		return MFS_ERROR_INVALID_ARGUMENTS;

	mfmU64 index = 0;

	// Skip whitespace
	while (buffer[index] == ' ' || buffer[index] == '\n' || buffer[index] == '\t')
	{
		++index;
		if (index >= bufferSize)
			return MFS_ERROR_FAILED_TO_PARSE;
	}

	const mfsUTF8CodeUnit characters[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
	mfmU32 out = 0;

	for (;; ++index)
	{
		if (buffer[index] == ' ' || buffer[index] == '\n' || buffer[index] == '\t' || buffer[index] == '\0')
		{
			if (outSize != NULL)
				*outSize = index;
			break;
		}
		else if (index >= bufferSize)
		{
			if (outSize != NULL)
				*outSize = bufferSize;
			break;
		}

		if (out <= UINT32_MAX / base)
			out *= base;
		else
			return MFS_ERROR_NUMBER_TOO_BIG;

		for (mfmU64 i = 0; i < base + 1; ++i)
		{
			if (i == base)
				return MFS_ERROR_FAILED_TO_PARSE;
			if (characters[i] == buffer[index])
			{
				out += i;
				break;
			}
		}
	}

	if (value != NULL)
		*value = out;

	return MF_ERROR_OKAY;
}

mfError mfsParseFromBufferI32(const mfsUTF8CodeUnit * buffer, mfmU64 bufferSize, mfmI32 * value, mfmU64 base, mfmU64 * outSize)
{
	if (buffer == NULL || bufferSize == 0 || base <= 1 || base > 16)
		return MFS_ERROR_INVALID_ARGUMENTS;

	mfmU64 index = 0;

	// Skip whitespace
	while (buffer[index] == ' ' || buffer[index] == '\n' || buffer[index] == '\t')
	{
		++index;
		if (index >= bufferSize)
			return MFS_ERROR_FAILED_TO_PARSE;
	}

	mfmBool negative = MFM_FALSE;
	// Check if negative
	if (buffer[index] == '-')
	{
		negative = MFM_TRUE;
		++index;
		if (index >= bufferSize)
			return MFS_ERROR_FAILED_TO_PARSE;
	}

	const mfsUTF8CodeUnit characters[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
	mfmI32 out = 0;

	for (;; ++index)
	{
		if (buffer[index] == ' ' || buffer[index] == '\n' || buffer[index] == '\t' || buffer[index] == '\0')
		{
			if (outSize != NULL)
				*outSize = index;
			break;
		}
		else if (index >= bufferSize)
		{
			if (outSize != NULL)
				*outSize = bufferSize;
			break;
		}

		if (out <= INT32_MAX / base)
			out *= base;
		else
			return MFS_ERROR_NUMBER_TOO_BIG;

		for (mfmU64 i = 0; i < base + 1; ++i)
		{
			if (i == base)
				return MFS_ERROR_FAILED_TO_PARSE;
			if (characters[i] == buffer[index])
			{
				out += i;
				break;
			}
		}
	}

	if (negative != MFM_FALSE)
		out = -out;

	if (value != NULL)
		*value = out;

	return MF_ERROR_OKAY;
}

mfError mfsParseFromBufferU64(const mfsUTF8CodeUnit * buffer, mfmU64 bufferSize, mfmU64 * value, mfmU64 base, mfmU64 * outSize)
{
	if (buffer == NULL || bufferSize == 0 || base <= 1 || base > 16)
		return MFS_ERROR_INVALID_ARGUMENTS;

	mfmU64 index = 0;

	// Skip whitespace
	while (buffer[index] == ' ' || buffer[index] == '\n' || buffer[index] == '\t')
	{
		++index;
		if (index >= bufferSize)
			return MFS_ERROR_FAILED_TO_PARSE;
	}

	const mfsUTF8CodeUnit characters[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
	mfmU64 out = 0;

	for (;; ++index)
	{
		if (buffer[index] == ' ' || buffer[index] == '\n' || buffer[index] == '\t' || buffer[index] == '\0')
		{
			if (outSize != NULL)
				*outSize = index;
			break;
		}
		else if (index >= bufferSize)
		{
			if (outSize != NULL)
				*outSize = bufferSize;
			break;
		}

		if (out <= UINT64_MAX / base)
			out *= base;
		else
			return MFS_ERROR_NUMBER_TOO_BIG;

		for (mfmU64 i = 0; i < base + 1; ++i)
		{
			if (i == base)
				return MFS_ERROR_FAILED_TO_PARSE;
			if (characters[i] == buffer[index])
			{
				out += i;
				break;
			}
		}
	}

	if (value != NULL)
		*value = out;

	return MF_ERROR_OKAY;
}

mfError mfsParseFromBufferI64(const mfsUTF8CodeUnit * buffer, mfmU64 bufferSize, mfmI64 * value, mfmU64 base, mfmU64 * outSize)
{
	if (buffer == NULL || bufferSize == 0 || base <= 1 || base > 16)
		return MFS_ERROR_INVALID_ARGUMENTS;

	mfmU64 index = 0;

	// Skip whitespace
	while (buffer[index] == ' ' || buffer[index] == '\n' || buffer[index] == '\t')
	{
		++index;
		if (index >= bufferSize)
			return MFS_ERROR_FAILED_TO_PARSE;
	}

	mfmBool negative = MFM_FALSE;
	// Check if negative
	if (buffer[index] == '-')
	{
		negative = MFM_TRUE;
		++index;
		if (index >= bufferSize)
			return MFS_ERROR_FAILED_TO_PARSE;
	}

	const mfsUTF8CodeUnit characters[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
	mfmI64 out = 0;

	for (;; ++index)
	{
		if (buffer[index] == ' ' || buffer[index] == '\n' || buffer[index] == '\t' || buffer[index] == '\0')
		{
			if (outSize != NULL)
				*outSize = index;
			break;
		}
		else if (index >= bufferSize)
		{
			if (outSize != NULL)
				*outSize = bufferSize;
			break;
		}

		if (out <= INT64_MAX / base)
			out *= base;
		else
			return MFS_ERROR_NUMBER_TOO_BIG;

		for (mfmU64 i = 0; i < base + 1; ++i)
		{
			if (i == base)
				return MFS_ERROR_FAILED_TO_PARSE;
			if (characters[i] == buffer[index])
			{
				out += i;
				break;
			}
		}
	}

	if (negative != MFM_FALSE)
		out = -out;

	if (value != NULL)
		*value = out;

	return MF_ERROR_OKAY;
}
