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

mfError mfsPrintToBufferF32(mfsUTF8CodeUnit* buffer, mfmU64 bufferSize, mfmF32 value, mfmU64 base, mfmU64 decimalPlaces, mfmU64* outSize)
{
	if (buffer == NULL || bufferSize == 0 || base <= 1 || base > 16)
		return MFS_ERROR_INVALID_ARGUMENTS;

	if (value == MFM_F32_INFINITY)
	{
		buffer[0] = 'I';

		if (bufferSize <= 1)
		{
			if (outSize != NULL)
				*outSize = 1;
			return MFS_ERROR_EOF;
		}
		buffer[1] = 'N';

		if (bufferSize <= 2)
		{
			if (outSize != NULL)
				*outSize = 2;
			return MFS_ERROR_EOF;
		}
		buffer[2] = 'F';
	}
	else if (value == -MFM_F32_INFINITY)
	{
		buffer[0] = '-';

		if (bufferSize <= 1)
		{
			if (outSize != NULL)
				*outSize = 1;
			return MFS_ERROR_EOF;
		}
		buffer[1] = 'I';

		if (bufferSize <= 2)
		{
			if (outSize != NULL)
				*outSize = 2;
			return MFS_ERROR_EOF;
		}
		buffer[2] = 'N';

		if (bufferSize <= 3)
		{
			if (outSize != NULL)
				*outSize = 3;
			return MFS_ERROR_EOF;
		}
		buffer[3] = 'F';
	}
	else if (value != value)
	{
		buffer[0] = 'N';

		if (bufferSize <= 1)
		{
			if (outSize != NULL)
				*outSize = 1;
			return MFS_ERROR_EOF;
		}
		buffer[1] = 'A';

		if (bufferSize <= 2)
		{
			if (outSize != NULL)
				*outSize = 2;
			return MFS_ERROR_EOF;
		}
		buffer[2] = 'N';
	}
	else if (value >= 0.0f && value <= 16777217.0f)
	{
		mfmF32 integerPartF = 0.0f;
		mfmF32 decimalPartF = modff(value, &integerPartF);
		mfmU64 integerPart = round(integerPartF);
		mfmU64 decimalPart = round(decimalPartF * pow(10, decimalPlaces));

		mfError err;
		mfmU64 index = 0;
		mfmU64 size = 0;

		// Print integer part
		err = mfsPrintToBufferU64(buffer + index, bufferSize - index, integerPart, base, &size);
		index += size;
		if (err != MF_ERROR_OKAY)
		{
			if (outSize != NULL)
				*outSize = index;
			return err;
		}
	
		if (decimalPlaces > 0)
		{
			// Print dot
			if (bufferSize - index < 1)
			{
				if (outSize != NULL)
					*outSize = index;
				return MFS_ERROR_EOF;
			}
			buffer[index] = '.';
			++index;

			
			// Count leading zeroes of fractional part
			mfmU32 leadingZeroes = 0;
			{
				mfmF32 leadingZeroesProbe = decimalPartF;
				leadingZeroesProbe *= base;
				while (leadingZeroesProbe < 1.0f && leadingZeroesProbe != 0.0f)
				{
					leadingZeroesProbe *= base;
					++leadingZeroes;
				}
			}

			// Print leading zeros
			for (mfmU32 i = 0; i < leadingZeroes; ++i)
			{
				if (bufferSize - index < 1)
				{
					if (outSize != NULL)
						*outSize = index;
					return MFS_ERROR_EOF;
				}
				buffer[index] = '0';
				++index;
			}

			// Print fractional part
			err = mfsPrintToBufferU64(buffer + index, bufferSize - index, decimalPart, base, &size);
			index += size;
			size += leadingZeroes;
			if (err != MF_ERROR_OKAY)
			{
				if (outSize != NULL)
					*outSize = index;
				return err;
			}
			if (size < decimalPlaces)
			{
				while (size != decimalPlaces)
				{
					if (bufferSize - index < 1)
					{
						if (outSize != NULL)
							*outSize = index;
						return MFS_ERROR_EOF;
					}
					buffer[index] = '0';
					++index;
					++size;
				}
			}
		}

		if (outSize != NULL)
			*outSize = index;
	}
	else if (value < 0.0f && -value <= 16777217.0f)
	{
		mfmF32 integerPartF = 0.0f;
		mfmF32 decimalPartF = modff(-value, &integerPartF);
		mfmU64 integerPart = round(integerPartF);
		mfmU64 decimalPart = round(decimalPartF * pow(10, decimalPlaces));

		mfError err;
		mfmU64 index = 0;
		mfmU64 size = 0;

		// Print dash
		if (bufferSize - index < 1)
		{
			if (outSize != NULL)
				*outSize = index;
			return MFS_ERROR_EOF;
		}
		buffer[index] = '-';
		++index;

		// Print integer part
		err = mfsPrintToBufferU64(buffer + index, bufferSize - index, integerPart, base, &size);
		index += size;
		if (err != MF_ERROR_OKAY)
		{
			if (outSize != NULL)
				*outSize = index;
			return err;
		}
	
		if (decimalPlaces > 0)
		{
			// Print dot
			if (bufferSize - index < 1)
			{
				if (outSize != NULL)
					*outSize = index;
				return MFS_ERROR_EOF;
			}
			buffer[index] = '.';
			++index;

			// Count leading zeroes of fractional part
			mfmU32 leadingZeroes = 0;
			{
				mfmF32 leadingZeroesProbe = decimalPartF;
				leadingZeroesProbe *= base;
				while (leadingZeroesProbe < 1.0f && leadingZeroesProbe != 0.0f)
				{
					leadingZeroesProbe *= base;
					++leadingZeroes;
				}
			}

			// Print leading zeros
			for (mfmU32 i = 0; i < leadingZeroes; ++i)
			{
				if (bufferSize - index < 1)
				{
					if (outSize != NULL)
						*outSize = index;
					return MFS_ERROR_EOF;
				}
				buffer[index] = '0';
				++index;
			}

			// Print fractional part
			err = mfsPrintToBufferU64(buffer + index, bufferSize - index, decimalPart, base, &size);
			index += size;
			size += leadingZeroes;
			if (err != MF_ERROR_OKAY)
			{
				if (outSize != NULL)
					*outSize = index;
				return err;
			}
			if (size < decimalPlaces)
			{
				while (size != decimalPlaces)
				{
					if (bufferSize - index < 1)
					{
						if (outSize != NULL)
							*outSize = index;
						return MFS_ERROR_EOF;
					}
					buffer[index] = '0';
					++index;
					++size;
				}
			}
		}

		if (outSize != NULL)
			*outSize = index;
	}
	else
	{
		// Print as exponent
		mfmI32 exp = floor(log10(fabs(value)));
		mfmF32 fractional = value / pow(10, exp);

		if (fractional >= 10.0f)
		{
			++exp;
			fractional /= 10.0f;
		}
		else if (fractional <= 0.1f)
		{
			--exp;
			fractional *= 10.0f;
		}

		mfError err;
		mfmU64 index = 0;
		mfmU64 size = 0;

		// Print fractional part
		err = mfsPrintToBufferF32(buffer + index, bufferSize - index, fractional, base, decimalPlaces, &size);
		index += size;
		if (err != MF_ERROR_OKAY)
		{
			if (outSize != NULL)
				*outSize = index;
			return err;
		}

		// Print exponent
		if (bufferSize - index < 1)
		{
			if (outSize != NULL)
				*outSize = index;
			return MFS_ERROR_EOF;
		}
		buffer[index] = 'E';
		++index;

		if (exp > 0)
		{
			if (bufferSize - index < 1)
			{
				if (outSize != NULL)
					*outSize = index;
				return MFS_ERROR_EOF;
			}
			buffer[index] = '+';
			++index;
		}

		err = mfsPrintToBufferU64(buffer + index, bufferSize - index, exp, base, &size);
		index += size;
		if (err != MF_ERROR_OKAY)
		{
			if (outSize != NULL)
				*outSize = index;
			return err;
		}

		if (outSize != NULL)
			*outSize = index;
	}

	return MF_ERROR_OKAY;
}

mfError mfsPrintToBufferF64(mfsUTF8CodeUnit* buffer, mfmU64 bufferSize, mfmF64 value, mfmU64 base, mfmU64 decimalPlaces, mfmU64* outSize)
{
	if (buffer == NULL || bufferSize == 0 || base <= 1 || base > 16)
		return MFS_ERROR_INVALID_ARGUMENTS;

	if (value == MFM_F64_INFINITY)
	{
		buffer[0] = 'I';

		if (bufferSize <= 1)
		{
			if (outSize != NULL)
				*outSize = 1;
			return MFS_ERROR_EOF;
		}
		buffer[1] = 'N';

		if (bufferSize <= 2)
		{
			if (outSize != NULL)
				*outSize = 2;
			return MFS_ERROR_EOF;
		}
		buffer[2] = 'F';
	}
	else if (value == -MFM_F64_INFINITY)
	{
		buffer[0] = '-';

		if (bufferSize <= 1)
		{
			if (outSize != NULL)
				*outSize = 1;
			return MFS_ERROR_EOF;
		}
		buffer[1] = 'I';

		if (bufferSize <= 2)
		{
			if (outSize != NULL)
				*outSize = 2;
			return MFS_ERROR_EOF;
		}
		buffer[2] = 'N';

		if (bufferSize <= 3)
		{
			if (outSize != NULL)
				*outSize = 3;
			return MFS_ERROR_EOF;
		}
		buffer[3] = 'F';
	}
	else if (value != value)
	{
		buffer[0] = 'N';

		if (bufferSize <= 1)
		{
			if (outSize != NULL)
				*outSize = 1;
			return MFS_ERROR_EOF;
		}
		buffer[1] = 'A';

		if (bufferSize <= 2)
		{
			if (outSize != NULL)
				*outSize = 2;
			return MFS_ERROR_EOF;
		}
		buffer[2] = 'N';
	}
	else if (value >= 0.0 && value <= 16777217.0)
	{
		mfmF64 integerPartF = 0.0f;
		mfmF64 decimalPartF = modf(value, &integerPartF);
		mfmU64 integerPart = round(integerPartF);
		mfmU64 decimalPart = round(decimalPartF * pow(10, decimalPlaces));

		mfError err;
		mfmU64 index = 0;
		mfmU64 size = 0;

		// Print integer part
		err = mfsPrintToBufferU64(buffer + index, bufferSize - index, integerPart, base, &size);
		index += size;
		if (err != MF_ERROR_OKAY)
		{
			if (outSize != NULL)
				*outSize = index;
			return err;
		}
	
		if (decimalPlaces > 0)
		{
			// Print dot
			if (bufferSize - index < 1)
			{
				if (outSize != NULL)
					*outSize = index;
				return MFS_ERROR_EOF;
			}
			buffer[index] = '.';
			++index;

			
			// Count leading zeroes of fractional part
			mfmU32 leadingZeroes = 0;
			{
				mfmF64 leadingZeroesProbe = decimalPartF;
				leadingZeroesProbe *= base;
				while (leadingZeroesProbe < 1.0 && leadingZeroesProbe != 0.0)
				{
					leadingZeroesProbe *= base;
					++leadingZeroes;
				}
			}

			// Print leading zeros
			for (mfmU32 i = 0; i < leadingZeroes; ++i)
			{
				if (bufferSize - index < 1)
				{
					if (outSize != NULL)
						*outSize = index;
					return MFS_ERROR_EOF;
				}
				buffer[index] = '0';
				++index;
			}

			// Print fractional part
			err = mfsPrintToBufferU64(buffer + index, bufferSize - index, decimalPart, base, &size);
			index += size;
			size += leadingZeroes;
			if (err != MF_ERROR_OKAY)
			{
				if (outSize != NULL)
					*outSize = index;
				return err;
			}
			if (size < decimalPlaces)
			{
				while (size != decimalPlaces)
				{
					if (bufferSize - index < 1)
					{
						if (outSize != NULL)
							*outSize = index;
						return MFS_ERROR_EOF;
					}
					buffer[index] = '0';
					++index;
					++size;
				}
			}
		}

		if (outSize != NULL)
			*outSize = index;
	}
	else if (value < 0.0 && -value <= 16777217.0)
	{
		mfmF64 integerPartF = 0.0f;
		mfmF64 decimalPartF = modf(-value, &integerPartF);
		mfmU64 integerPart = round(integerPartF);
		mfmU64 decimalPart = round(decimalPartF * pow(10, decimalPlaces));

		mfError err;
		mfmU64 index = 0;
		mfmU64 size = 0;

		// Print dash
		if (bufferSize - index < 1)
		{
			if (outSize != NULL)
				*outSize = index;
			return MFS_ERROR_EOF;
		}
		buffer[index] = '-';
		++index;

		// Print integer part
		err = mfsPrintToBufferU64(buffer + index, bufferSize - index, integerPart, base, &size);
		index += size;
		if (err != MF_ERROR_OKAY)
		{
			if (outSize != NULL)
				*outSize = index;
			return err;
		}
	
		if (decimalPlaces > 0)
		{
			// Print dot
			if (bufferSize - index < 1)
			{
				if (outSize != NULL)
					*outSize = index;
				return MFS_ERROR_EOF;
			}
			buffer[index] = '.';
			++index;

			// Count leading zeroes of fractional part
			mfmU32 leadingZeroes = 0;
			{
				mfmF64 leadingZeroesProbe = decimalPartF;
				leadingZeroesProbe *= base;
				while (leadingZeroesProbe < 1.0 && leadingZeroesProbe != 0.0)
				{
					leadingZeroesProbe *= base;
					++leadingZeroes;
				}
			}

			// Print leading zeros
			for (mfmU32 i = 0; i < leadingZeroes; ++i)
			{
				if (bufferSize - index < 1)
				{
					if (outSize != NULL)
						*outSize = index;
					return MFS_ERROR_EOF;
				}
				buffer[index] = '0';
				++index;
			}

			// Print fractional part
			err = mfsPrintToBufferU64(buffer + index, bufferSize - index, decimalPart, base, &size);
			index += size;
			size += leadingZeroes;
			if (err != MF_ERROR_OKAY)
			{
				if (outSize != NULL)
					*outSize = index;
				return err;
			}
			if (size < decimalPlaces)
			{
				while (size != decimalPlaces)
				{
					if (bufferSize - index < 1)
					{
						if (outSize != NULL)
							*outSize = index;
						return MFS_ERROR_EOF;
					}
					buffer[index] = '0';
					++index;
					++size;
				}
			}
		}

		if (outSize != NULL)
			*outSize = index;
	}
	else
	{
		// Print as exponent
		mfmI64 exp = floor(log10(fabs(value)));
		mfmF64 fractional = value / pow(10, exp);

		if (fractional >= 10.0)
		{
			++exp;
			fractional /= 10.0;
		}
		else if (fractional <= 0.1)
		{
			--exp;
			fractional *= 10.0f;
		}

		mfError err;
		mfmU64 index = 0;
		mfmU64 size = 0;

		// Print fractional part
		err = mfsPrintToBufferF64(buffer + index, bufferSize - index, fractional, base, decimalPlaces, &size);
		index += size;
		if (err != MF_ERROR_OKAY)
		{
			if (outSize != NULL)
				*outSize = index;
			return err;
		}

		// Print exponent
		if (bufferSize - index < 1)
		{
			if (outSize != NULL)
				*outSize = index;
			return MFS_ERROR_EOF;
		}
		buffer[index] = 'E';
		++index;

		if (exp > 0)
		{
			if (bufferSize - index < 1)
			{
				if (outSize != NULL)
					*outSize = index;
				return MFS_ERROR_EOF;
			}
			buffer[index] = '+';
			++index;
		}

		err = mfsPrintToBufferU64(buffer + index, bufferSize - index, exp, base, &size);
		index += size;
		if (err != MF_ERROR_OKAY)
		{
			if (outSize != NULL)
				*outSize = index;
			return err;
		}

		if (outSize != NULL)
			*outSize = index;
	}

	return MF_ERROR_OKAY;
}

mfError mfsParseFromBufferU8(const mfsUTF8CodeUnit * buffer, mfmU64 bufferSize, mfmU8 * value, mfmU64 base, mfmU64 * outSize)
{
	if (buffer == NULL || bufferSize == 0 || base <= 1 || base > 16)
		return MFS_ERROR_INVALID_ARGUMENTS;

	mfmU64 index = 0;

	// Skip whitespaces
	while (buffer[index] == ' ' || buffer[index] == '\n' || buffer[index] == '\t')
	{
		++index;
		if (index >= bufferSize)
			return MFS_ERROR_FAILED_TO_PARSE;
	}

	if (buffer[index] == '\0')
		return MFS_ERROR_FAILED_TO_PARSE;

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
				if (UINT8_MAX - i < out)
					return MFS_ERROR_NUMBER_TOO_BIG;
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

	if (buffer[index] == '\0')
		return MFS_ERROR_FAILED_TO_PARSE;

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
				if (INT8_MAX - i < out)
					return MFS_ERROR_NUMBER_TOO_BIG;
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

	if (buffer[index] == '\0')
		return MFS_ERROR_FAILED_TO_PARSE;

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
				if (UINT16_MAX - i < out)
					return MFS_ERROR_NUMBER_TOO_BIG;
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

	if (buffer[index] == '\0')
		return MFS_ERROR_FAILED_TO_PARSE;

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
				if (INT16_MAX - i < out)
					return MFS_ERROR_NUMBER_TOO_BIG;
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

	if (buffer[index] == '\0')
		return MFS_ERROR_FAILED_TO_PARSE;

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
				if (UINT32_MAX - i < out)
					return MFS_ERROR_NUMBER_TOO_BIG;
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

	if (buffer[index] == '\0')
		return MFS_ERROR_FAILED_TO_PARSE;

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
				if (INT32_MAX - i < out)
					return MFS_ERROR_NUMBER_TOO_BIG;
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

	if (buffer[index] == '\0')
		return MFS_ERROR_FAILED_TO_PARSE;

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
				if (UINT64_MAX - i < out)
					return MFS_ERROR_NUMBER_TOO_BIG;
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

	if (buffer[index] == '\0')
		return MFS_ERROR_FAILED_TO_PARSE;

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
				if (INT64_MAX - i < out)
					return MFS_ERROR_NUMBER_TOO_BIG;
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

mfError mfsParseFromBufferF32(const mfsUTF8CodeUnit * buffer, mfmU64 bufferSize, mfmF32 * value, mfmU64 base, mfmU64 * outSize)
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

	if (buffer[index] == '\0')
		return MFS_ERROR_FAILED_TO_PARSE;

	const mfsUTF8CodeUnit characters[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
	mfmF32 integral = 0.0f;
	mfmF32 fractional = 0.0f;

	// Integral part
	for (;; ++index)
	{
		if (buffer[index] == ' ' || buffer[index] == '\n' || buffer[index] == '\t' || buffer[index] == '.' || buffer[index] == '\0' || buffer[index] == 'E' || buffer[index] == 'e')
			break;
		else if (index >= bufferSize)
			break;

		if (integral <= MFM_F32_MAX / base)
			integral *= base;
		else
			return MFS_ERROR_NUMBER_TOO_BIG;

		for (mfmU64 i = 0; i < base + 1; ++i)
		{
			if (i == base)
				return MFS_ERROR_FAILED_TO_PARSE;
			if (characters[i] == buffer[index])
			{
				if (MFM_F32_MAX - i < integral)
					return MFS_ERROR_NUMBER_TOO_BIG;
				integral += i;
				break;
			}
		}
	}

	// Fractional part
	if (index < bufferSize && buffer[index] == '.')
	{
		mfmF32 div = 1.0f;
		++index;
		for (;; ++index)
		{
			if (buffer[index] == ' ' || buffer[index] == '\n' || buffer[index] == '\t' || buffer[index] == '\0' || buffer[index] == 'E' || buffer[index] == 'e')
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

			div /= base;

			for (mfmU64 i = 0; i < base + 1; ++i)
			{
				if (i == base)
					return MFS_ERROR_FAILED_TO_PARSE;
				if (characters[i] == buffer[index])
				{
					if (MFM_F32_MAX - i * div < fractional)
						return MFS_ERROR_NUMBER_TOO_BIG;
					fractional += i * div;
					break;
				}
			}			
		}
	}

	// Has exponent?
	if (index < bufferSize && (buffer[index] == 'E' || buffer[index] == 'e'))
	{
		++index;

		mfmI32 exponent = 0;
		mfmBool isNegative = MFM_FALSE;
		if (buffer[index] == '-')
		{
			isNegative = MFM_TRUE;
			++index;
		}

		// Exponent part
		for (;; ++index)
		{
			if (buffer[index] == ' ' || buffer[index] == '\n' || buffer[index] == '\t' || buffer[index] == '\0')
				break;
			else if (index >= bufferSize)
				break;

			if (exponent <= MFM_I32_MAX / base)
				exponent *= base;
			else
				return MFS_ERROR_NUMBER_TOO_BIG;

			for (mfmU64 i = 0; i < base + 1; ++i)
			{
				if (i == base)
					return MFS_ERROR_FAILED_TO_PARSE;
				if (characters[i] == buffer[index])
				{
					if (MFM_I32_MAX - i < exponent)
						return MFS_ERROR_NUMBER_TOO_BIG;
					exponent += i;
					break;
				}
			}
		}

		if (isNegative != MFM_FALSE)
			exponent = -exponent;

		if (outSize != NULL)
			*outSize = index;
		if (value != NULL)
			*value = (integral + fractional) * pow(10, exponent);
	}
	// No exponent
	else
	{
		if (outSize != NULL)
			*outSize = index;
		if (value != NULL)
			*value = integral + fractional;
	}

	return MF_ERROR_OKAY;
}

mfError mfsParseFromBufferF64(const mfsUTF8CodeUnit * buffer, mfmU64 bufferSize, mfmF64 * value, mfmU64 base, mfmU64 * outSize)
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

	if (buffer[index] == '\0')
		return MFS_ERROR_FAILED_TO_PARSE;

	const mfsUTF8CodeUnit characters[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
	mfmF64 integral = 0.0;
	mfmF64 fractional = 0.0;

	// Integral part
	for (;; ++index)
	{
		if (buffer[index] == ' ' || buffer[index] == '\n' || buffer[index] == '\t' || buffer[index] == '.' || buffer[index] == '\0' || buffer[index] == 'E' || buffer[index] == 'e')
			break;
		else if (index >= bufferSize)
			break;

		if (integral <= MFM_F64_MAX / base)
			integral *= base;
		else
			return MFS_ERROR_NUMBER_TOO_BIG;

		for (mfmU64 i = 0; i < base + 1; ++i)
		{
			if (i == base)
				return MFS_ERROR_FAILED_TO_PARSE;
			if (characters[i] == buffer[index])
			{
				if (MFM_F64_MAX - i < integral)
					return MFS_ERROR_NUMBER_TOO_BIG;
				integral += i;
				break;
			}
		}
	}

	// Fractional part
	if (index < bufferSize && buffer[index] == '.')
	{
		mfmF64 div = 1.0f;
		++index;
		for (;; ++index)
		{
			if (buffer[index] == ' ' || buffer[index] == '\n' || buffer[index] == '\t' || buffer[index] == '\0' || buffer[index] == 'E' || buffer[index] == 'e')
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

			div /= base;

			for (mfmU64 i = 0; i < base + 1; ++i)
			{
				if (i == base)
					return MFS_ERROR_FAILED_TO_PARSE;
				if (characters[i] == buffer[index])
				{
					if (MFM_F64_MAX - i * div < fractional)
						return MFS_ERROR_NUMBER_TOO_BIG;
					fractional += i * div;
					break;
				}
			}			
		}
	}

	// Has exponent?
	if (index < bufferSize && (buffer[index] == 'E' || buffer[index] == 'e'))
	{
		++index;

		mfmI64 exponent = 0;
		mfmBool isNegative = MFM_FALSE;
		if (buffer[index] == '-')
		{
			isNegative = MFM_TRUE;
			++index;
		}

		// Exponent part
		for (;; ++index)
		{
			if (buffer[index] == ' ' || buffer[index] == '\n' || buffer[index] == '\t' || buffer[index] == '\0')
				break;
			else if (index >= bufferSize)
				break;

			if (exponent <= MFM_I64_MAX / base)
				exponent *= base;
			else
				return MFS_ERROR_NUMBER_TOO_BIG;

			for (mfmU64 i = 0; i < base + 1; ++i)
			{
				if (i == base)
					return MFS_ERROR_FAILED_TO_PARSE;
				if (characters[i] == buffer[index])
				{
					if (MFM_I64_MAX - i < exponent)
						return MFS_ERROR_NUMBER_TOO_BIG;
					exponent += i;
					break;
				}
			}
		}

		if (isNegative != MFM_FALSE)
			exponent = -exponent;

		if (outSize != NULL)
			*outSize = index;
		if (value != NULL)
			*value = (integral + fractional) * pow(10, exponent);
	}
	// No exponent
	else
	{
		if (outSize != NULL)
			*outSize = index;
		if (value != NULL)
			*value = integral + fractional;
	}

	return MF_ERROR_OKAY;
}