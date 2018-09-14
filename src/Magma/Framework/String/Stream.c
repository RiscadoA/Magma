#include "Stream.h"
#include "../Memory/Allocator.h"
#include "../String/Conversion.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

mfsStream* mfsInStream = NULL;
mfsStream* mfsOutStream = NULL;
mfsStream* mfsErrStream = NULL;

static mfmU8 mfsInDefaultBuffer[512];
static mfmU8 mfsOutDefaultBuffer[512];
static mfmU8 mfsErrDefaultBuffer[512];

typedef struct
{
	mfsStream base;
	FILE* file;
} mfsFileStream;

static mfError mfsFileWrite(void* stream, const mfmU8* data, mfmU64 dataSize, mfmU64* outSize)
{
	mfsFileStream* fileStream = (mfsFileStream*)stream;

	mfmU64 size = fwrite(data, sizeof(mfmU8), dataSize, fileStream->file);
	if (outSize != NULL)
		*outSize = size;

	return MF_ERROR_OKAY;
}

static mfError mfsFileRead(void* stream, mfmU8* data, mfmU64 dataSize, mfmU64* outSize)
{
	mfsFileStream* fileStream = (mfsFileStream*)stream;

	mfmU64 size = fread(data, sizeof(mfmU8), dataSize, fileStream->file);
	if (outSize != NULL)
		*outSize = size;

	return MF_ERROR_OKAY;
}

static mfError mfsFileFlush(void* stream)
{
	mfsFileStream* fileStream = (mfsFileStream*)stream;
	int ret = fflush(fileStream->file);
	if (ret == EOF)
		return MFS_ERROR_EOF;
	return MF_ERROR_OKAY;
}

static mfError mfsFileSetBuffer(void* stream, mfmU8* buffer, mfmU64 bufferSize)
{
	mfsFileStream* fileStream = (mfsFileStream*)stream;

	if (buffer != NULL)
	{
		int ret = setvbuf(fileStream->file, buffer, _IOFBF, bufferSize);
		if (ret != 0)
			return MFS_ERROR_INTERNAL;
	}
	else
	{
		int ret = setvbuf(fileStream->file, NULL, _IONBF, 0);
		if (ret != 0)
			return MFS_ERROR_INTERNAL;
	}

	return MF_ERROR_OKAY;
}

static void mfsDestroyFileStream(void* stream)
{
	if (mfmDeinitObject(stream) != MF_ERROR_OKAY)
		abort();
	if (mfmDeallocate(NULL, stream) != MF_ERROR_OKAY)
		abort();
}

static mfsStream* mfsCreateFileStream(FILE* file, mfmU8* buffer, mfmU64 bufferSize)
{
	mfsFileStream* stream = NULL;
	if (mfmAllocate(NULL, &stream, sizeof(mfsFileStream)) != MF_ERROR_OKAY)
		abort();

	{
		mfError err = mfmInitObject(&stream->base.object);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	stream->base.object.destructorFunc = &mfsDestroyFileStream;

	stream->base.buffer = buffer;
	stream->base.bufferSize = bufferSize;
	stream->file = file;

	stream->base.read = &mfsFileRead;
	stream->base.write = &mfsFileWrite;
	stream->base.flush = &mfsFileFlush;
	stream->base.setBuffer = &mfsFileSetBuffer;
	stream->base.seekBegin = NULL;
	stream->base.seekEnd = NULL;
	stream->base.seekHead = NULL;
	stream->base.tell = NULL;
	stream->base.eof = NULL;

	return (mfsStream*)stream;
}

mfError mfsInitStream()
{
	mfError err = MF_ERROR_OKAY;

	// Initialize mfsInStream stream
	mfsInStream = mfsCreateFileStream(stdin, mfsInDefaultBuffer, sizeof(mfsInDefaultBuffer));
	err = mfmAcquireObject(&mfsInStream->object);
	if (err != MF_ERROR_OKAY)
		return err;
	
	// Initialize mfsOutStream stream
	mfsOutStream = mfsCreateFileStream(stdout, mfsOutDefaultBuffer, sizeof(mfsOutDefaultBuffer));
	err = mfmAcquireObject(&mfsOutStream->object);
	if (err != MF_ERROR_OKAY)
		return err;

	// Initialize mfsErrStream stream
	mfsErrStream = mfsCreateFileStream(stderr, mfsErrDefaultBuffer, sizeof(mfsErrDefaultBuffer));
	err = mfmAcquireObject(&mfsErrStream->object);
	if (err != MF_ERROR_OKAY)
		return err;

	return err;
}

mfError mfsTerminateStream()
{
	mfError err = MF_ERROR_OKAY;

	if (mfsInStream != NULL)
	{
		err = mfmReleaseObject(&mfsInStream->object);
		if (err != MF_ERROR_OKAY)
			return err;
	}

	if (mfsOutStream != NULL)
	{
		err = mfmReleaseObject(&mfsOutStream->object);
		if (err != MF_ERROR_OKAY)
			return err;
	}

	if (mfsErrStream != NULL)
	{
		err = mfmReleaseObject(&mfsErrStream->object);
		if (err != MF_ERROR_OKAY)
			return err;
	}

	return err;
}

mfError mfsWrite(mfsStream * stream, const mfmU8 * data, mfmU64 dataSize, mfmU64 * outSize)
{
	if (stream == NULL || data == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;
	if (dataSize == 0)
		return MF_ERROR_OKAY;
	if (stream->write == NULL)
		return MFS_ERROR_UNSUPPORTED_FUNCTION;
	return stream->write(stream, data, dataSize, outSize);
}

mfError mfsRead(mfsStream * stream, mfmU8 * data, mfmU64 dataSize, mfmU64 * outSize)
{
	if (stream == NULL || data == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;
	if (dataSize == 0)
		return MF_ERROR_OKAY;
	if (stream->read == NULL)
		return MFS_ERROR_UNSUPPORTED_FUNCTION;
	return stream->read(stream, data, dataSize, outSize);
}

mfError mfsReadUntil(mfsStream * stream, mfmU8 * data, mfmU64 dataSize, mfmU64 * outSize, const mfsUTF8CodeUnit * terminator)
{
	if (stream == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;

	if (data != NULL)
	{
		if (dataSize == 0)
			return 0;
		else if (dataSize == 1)
		{
			data[0] = '\0';
			return 1;
		}
	}

	mfError err = MF_ERROR_OKAY;

	mfmU64 size = 0;
	mfmU64 terminatorIndex = 0;
	while (size < dataSize - 1)
	{
		if (terminator != NULL && terminator[terminatorIndex] == '\0')
			break;

		mfsUTF8CodeUnit chr;
		err = mfsGetByte(stream, (mfmU8*)&chr);

		if (terminator == NULL && (chr == ' ' || chr == '\t' || chr == '\n'))
			break;

		if (err == MF_ERROR_OKAY)
		{
			if (terminator != NULL && chr == terminator[terminatorIndex])
				++terminatorIndex;
			else
			{
				for (mfmU64 i = 0; i < terminatorIndex; ++i)
					if (data != NULL)
					{
						data[size++] = terminator[i];
						if (size >= dataSize - 1)
							break;
					}

				terminatorIndex = 0;

				if (terminator != NULL && chr == terminator[terminatorIndex])
					++terminatorIndex;
				else if (data != NULL)
				{
					if (size >= dataSize - 1)
						break;
					else
						data[size++] = chr;
				}
			}
		}
		else if (err == MFS_ERROR_EOF)
			break;
		else
			return err;
	}

	if (data != NULL)
		data[size] = '\0';
	if (outSize != NULL)
		*outSize = size;
	return MF_ERROR_OKAY;
}

mfError mfsFlush(mfsStream * stream)
{
	if (stream == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;
	if (stream->flush == NULL)
		return MFS_ERROR_UNSUPPORTED_FUNCTION;
	return stream->flush(stream);
}

mfError mfsSetBuffer(mfsStream * stream, mfmU8 * buffer, mfmU64 bufferSize)
{
	if (stream == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;
	if (stream->setBuffer == NULL)
		return MFS_ERROR_UNSUPPORTED_FUNCTION;
	return stream->setBuffer(stream, buffer, bufferSize);
}

mfError mfsGetByte(mfsStream * stream, mfmU8 * byte)
{
	if (stream == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;
	if (stream->read == NULL)
		return MFS_ERROR_UNSUPPORTED_FUNCTION;

	if (byte == NULL)
	{
		mfmU8 temp;
		mfmU64 readSize = 0;
		mfError err = stream->read(stream, &temp, sizeof(mfmU8), &readSize);
		if (err != MF_ERROR_OKAY)
			return err;
		if (readSize != sizeof(mfmU8))
			return MFS_ERROR_FAILED_TO_READ_ALL;
		return MF_ERROR_OKAY;
	}
	else
	{
		mfmU64 readSize = 0;
		mfError err = stream->read(stream, byte, sizeof(mfmU8), &readSize);
		if (err != MF_ERROR_OKAY)
			return err;
		if (readSize != sizeof(mfmU8))
			return MFS_ERROR_FAILED_TO_READ_ALL;
		return MF_ERROR_OKAY;
	}
}

mfError mfsPutByte(mfsStream * stream, mfmU8 byte)
{
	if (stream == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;
	if (stream->write == NULL)
		return MFS_ERROR_UNSUPPORTED_FUNCTION;

	mfmU64 writeSize = 0;
	mfError err = stream->write(stream, &byte, sizeof(mfmU8), &writeSize);
	if (err != MF_ERROR_OKAY)
		return err;
	if (writeSize != sizeof(mfmU8))
		return MFS_ERROR_FAILED_TO_WRITE_ALL;
	return MF_ERROR_OKAY;
}

mfError mfsPutString(mfsStream * stream, const mfsUTF8CodeUnit * str)
{
	while (*str != '\0')
	{
		mfError err = mfsPutByte(stream, *str);
		if (err != MF_ERROR_OKAY)
			return err;
		++str;
	}

	return MF_ERROR_OKAY;
}

mfError mfsPrintU8(mfsStream * stream, mfmU8 value, mfmU64 base)
{
	mfsUTF8CodeUnit buf[256] = { '\0' };

	mfError err = mfsPrintToBufferU8(buf, sizeof(buf), value, base, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	err = mfsPutString(stream, buf);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

mfError mfsPrintU16(mfsStream * stream, mfmU16 value, mfmU64 base)
{
	mfsUTF8CodeUnit buf[256] = { '\0' };

	mfError err = mfsPrintToBufferU16(buf, sizeof(buf), value, base, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	err = mfsPutString(stream, buf);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

mfError mfsPrintU32(mfsStream * stream, mfmU32 value, mfmU64 base)
{
	mfsUTF8CodeUnit buf[256] = { '\0' };

	mfError err = mfsPrintToBufferU32(buf, sizeof(buf), value, base, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	err = mfsPutString(stream, buf);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

mfError mfsPrintU64(mfsStream * stream, mfmU64 value, mfmU64 base)
{
	mfsUTF8CodeUnit buf[256] = { '\0' };

	mfError err = mfsPrintToBufferU64(buf, sizeof(buf), value, base, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	err = mfsPutString(stream, buf);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

mfError mfsPrintI8(mfsStream * stream, mfmI8 value, mfmU64 base)
{
	mfsUTF8CodeUnit buf[256] = { '\0' };

	mfError err = mfsPrintToBufferI8(buf, sizeof(buf), value, base, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	err = mfsPutString(stream, buf);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

mfError mfsPrintI16(mfsStream * stream, mfmI16 value, mfmU64 base)
{
	mfsUTF8CodeUnit buf[256] = { '\0' };

	mfError err = mfsPrintToBufferI16(buf, sizeof(buf), value, base, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	err = mfsPutString(stream, buf);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

mfError mfsPrintI32(mfsStream * stream, mfmI32 value, mfmU64 base)
{
	mfsUTF8CodeUnit buf[256] = { '\0' };

	mfError err = mfsPrintToBufferI32(buf, sizeof(buf), value, base, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	err = mfsPutString(stream, buf);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

mfError mfsPrintI64(mfsStream * stream, mfmI64 value, mfmU64 base)
{
	mfsUTF8CodeUnit buf[256] = { '\0' };

	mfError err = mfsPrintToBufferI64(buf, sizeof(buf), value, base, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	err = mfsPutString(stream, buf);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

mfError mfsPrintF32(mfsStream * stream, mfmF32 value, mfmU64 base, mfmU64 decimalPlaces)
{
	mfsUTF8CodeUnit buf[256] = { '\0' };

	mfError err = mfsPrintToBufferF32(buf, sizeof(buf), value, base, decimalPlaces, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	err = mfsPutString(stream, buf);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

mfError mfsPrintF64(mfsStream * stream, mfmF64 value, mfmU64 base, mfmU64 decimalPlaces)
{
	mfsUTF8CodeUnit buf[256] = { '\0' };

	mfError err = mfsPrintToBufferF64(buf, sizeof(buf), value, base, decimalPlaces, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	err = mfsPutString(stream, buf);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

mfError mfsPrintFormat(mfsStream * stream, const mfsUTF8CodeUnit * format, ...)
{
	va_list args;
	va_start(args, format);
	mfError err = mfsPrintFormatList(stream, format, args);
	va_end(args);
	return err;
}

mfError mfsPrintFormatList(mfsStream * stream, const mfsUTF8CodeUnit * format, va_list args)
{
	mfmBool escape = MFM_FALSE;
	while (*format != '\0')
	{
		if (escape == MFM_TRUE)
		{
			mfError err = mfsPutByte(stream, *format);
			escape = MFM_FALSE;
			if (err != MF_ERROR_OKAY)
				return err;
		}
		else
		{
			if (*format == '\\')
				escape = MFM_TRUE;
			// New formatting
			else if (*format == '%' && *(format + 1) == '%')
			{
				format += 2;
				mfmU64 base = 10;
				mfmU64 decimalPlaces = 4;
				mfError err;

				// Params
				while (*format == '-')
				{
					// Base specifier
					if (*(format + 1) == 'b')
					{
						format += 2;
						switch (*format)
						{
							case 'b': base = 2; break;
							case 'f': base = 4; break;
							case 'o': base = 8; break;
							case 'd': base = 10; break;
							case 'h': base = 16; break;
							default:
								return MFS_ERROR_INVALID_ARGUMENTS;
						}
						++format;
					}

					// Decimal places specifier
					if (*(format + 1) == 'd')
					{
						format += 2;
						switch (*format)
						{
							case '0': decimalPlaces = 0; break;
							case '1': decimalPlaces = 1; break;
							case '2': decimalPlaces = 2; break;
							case '3': decimalPlaces = 3; break;
							case '4': decimalPlaces = 4; break;
							case '5': decimalPlaces = 5; break;
							case '6': decimalPlaces = 6; break;
							case '7': decimalPlaces = 7; break;
							case '8': decimalPlaces = 8; break;
							case '9': decimalPlaces = 9; break;
							default:
								return MFS_ERROR_INVALID_ARGUMENTS;
						}
						++format;
					}
				}

				// Type
				switch (*format)
				{
					case 'u':
						if (*(format + 1) == 'l')
						{
							err = mfsPrintU64(stream, va_arg(args, mfmU64), base, decimalPlaces);
							if (err != MF_ERROR_OKAY)
								return err;
							++format;
						}
						else
						{
							err = mfsPrintU32(stream, va_arg(args, mfmU32), base, decimalPlaces);
							if (err != MF_ERROR_OKAY)
								return err;
						}
						break;

					case 'i':
						if (*(format + 1) == 'l')
						{
							err = mfsPrintI64(stream, va_arg(args, mfmI64), base, decimalPlaces);
							if (err != MF_ERROR_OKAY)
								return err;
							++format;
						}
						else
						{
							err = mfsPrintI32(stream, va_arg(args, mfmI32), base, decimalPlaces);
							if (err != MF_ERROR_OKAY)
								return err;
						}
						break;

					case 'f':
						err = mfsPrintF64(stream, va_arg(args, mfmF64), base, decimalPlaces);
						if (err != MF_ERROR_OKAY)
							return err;
						break;

					case 'c':
						err = mfsPutByte(stream, va_arg(args, mfsUnicodePoint));
						if (err != MF_ERROR_OKAY)
							return err;
						break;

					case 's':
						err = mfsPutString(stream, va_arg(args, const mfsUTF8CodeUnit*));
						if (err != MF_ERROR_OKAY)
							return err;
						break;

					case 'p':
						if (sizeof(void*) == sizeof(mfmU32))
						{
							err = mfsPrintU32(stream, va_arg(args, mfmU32), base, decimalPlaces);
							if (err != MF_ERROR_OKAY)
								return err;
						}
						else if (sizeof(void*) == sizeof(mfmU64))
						{
							err = mfsPrintU32(stream, va_arg(args, mfmU64), base, decimalPlaces);
							if (err != MF_ERROR_OKAY)
								return err;
						}
						else abort();
				}
			}
			// Old formatting
			else if (*format == '%')
			{
				++format;
				if (*format == 'd')
				{
					mfError err = mfsPrintI32(stream, va_arg(args, mfmI32), 10);
					if (err != MF_ERROR_OKAY)
						return err;
				}
				else if (*format == 'x')
				{
					mfError err = mfsPrintI32(stream, va_arg(args, mfmI32), 16);
					if (err != MF_ERROR_OKAY)
						return err;
				}
				else if (*format == 'f')
				{
					mfError err = mfsPrintF64(stream, va_arg(args, mfmF64), 10, 4);
					if (err != MF_ERROR_OKAY)
						return err;
				}
				else if (*format == 'c')
				{
					mfError err = mfsPutByte(stream, va_arg(args, mfsUnicodePoint));
					if (err != MF_ERROR_OKAY)
						return err;
				}
				else if (*format == 's')
				{
					mfError err = mfsPutString(stream, va_arg(args, const mfsUTF8CodeUnit*));
					if (err != MF_ERROR_OKAY)
						return err;
				}
				else
					return MFS_ERROR_INVALID_ARGUMENTS;
			}
			else
			{
				mfError err = mfsPutByte(stream, *format);
				escape = MFM_FALSE;
				if (err != MF_ERROR_OKAY)
					return err;
			}
		}

		++format;
	}

	return MF_ERROR_OKAY;
}

mfError mfsSeekBegin(mfsStream * stream, mfmU64 offset)
{
	if (stream == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;
	if (stream->seekBegin == NULL)
		return MFS_ERROR_UNSUPPORTED_FUNCTION;
	mfError err = stream->seekBegin(stream, offset);
	if (err != MF_ERROR_OKAY)
		return err;
	return MF_ERROR_OKAY;
}

mfError mfsSeekEnd(mfsStream * stream, mfmU64 offset)
{
	if (stream == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;
	if (stream->seekEnd == NULL)
		return MFS_ERROR_UNSUPPORTED_FUNCTION;
	mfError err = stream->seekEnd(stream, offset);
	if (err != MF_ERROR_OKAY)
		return err;
	return MF_ERROR_OKAY;
}

mfError mfsSeekHead(mfsStream * stream, mfmI64 offset)
{
	if (stream == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;
	if (stream->seekHead == NULL)
		return MFS_ERROR_UNSUPPORTED_FUNCTION;
	mfError err = stream->seekHead(stream, offset);
	if (err != MF_ERROR_OKAY)
		return err;
	return MF_ERROR_OKAY;
}

mfError mfsTell(mfsStream * stream, mfmU64 * outPosition)
{
	if (stream == NULL || outPosition == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;
	if (stream->seekHead == NULL)
		return MFS_ERROR_UNSUPPORTED_FUNCTION;
	mfError err = stream->tell(stream, outPosition);
	if (err != MF_ERROR_OKAY)
		return err;
	return MF_ERROR_OKAY;
}

mfError mfsEOF(mfsStream * stream, mfmBool * eof)
{
	if (stream == NULL || eof == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;
	if (stream->eof == NULL)
		return MFS_ERROR_UNSUPPORTED_FUNCTION;
	mfError err = stream->eof(stream, eof);
	if (err != MF_ERROR_OKAY)
		return err;
	return MF_ERROR_OKAY;
}

mfError mfsParseU8(mfsStream * stream, mfmU8 * value, mfmU64 base, const mfsUTF8CodeUnit * sep)
{
	mfsUTF8CodeUnit str[256] = { '\0' };
	mfmU64 readSize = 0;

	mfError err = mfsReadUntil(stream, str, sizeof(str), &readSize, sep);
	if (err != MF_ERROR_OKAY)
		return err;

	err = mfsParseFromBufferU8(str, readSize, value, base, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

mfError mfsParseU16(mfsStream * stream, mfmU16 * value, mfmU64 base, const mfsUTF8CodeUnit * sep)
{
	mfsUTF8CodeUnit str[256] = { '\0' };
	mfmU64 readSize = 0;

	mfError err = mfsReadUntil(stream, str, sizeof(str), &readSize, sep);
	if (err != MF_ERROR_OKAY)
		return err;

	err = mfsParseFromBufferU16(str, readSize, value, base, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

mfError mfsParseU32(mfsStream * stream, mfmU32 * value, mfmU64 base, const mfsUTF8CodeUnit * sep)
{
	mfsUTF8CodeUnit str[256] = { '\0' };
	mfmU64 readSize = 0;

	mfError err = mfsReadUntil(stream, str, sizeof(str), &readSize, sep);
	if (err != MF_ERROR_OKAY)
		return err;

	err = mfsParseFromBufferU32(str, readSize, value, base, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

mfError mfsParseU64(mfsStream * stream, mfmU64 * value, mfmU64 base, const mfsUTF8CodeUnit * sep)
{
	mfsUTF8CodeUnit str[256] = { '\0' };
	mfmU64 readSize = 0;

	mfError err = mfsReadUntil(stream, str, sizeof(str), &readSize, sep);
	if (err != MF_ERROR_OKAY)
		return err;

	err = mfsParseFromBufferU64(str, readSize, value, base, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

mfError mfsParseI8(mfsStream * stream, mfmI8 * value, mfmU64 base, const mfsUTF8CodeUnit * sep)
{
	mfsUTF8CodeUnit str[256] = { '\0' };
	mfmU64 readSize = 0;

	mfError err = mfsReadUntil(stream, str, sizeof(str), &readSize, sep);
	if (err != MF_ERROR_OKAY)
		return err;

	err = mfsParseFromBufferI8(str, readSize, value, base, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

mfError mfsParseI16(mfsStream * stream, mfmI16 * value, mfmU64 base, const mfsUTF8CodeUnit * sep)
{
	mfsUTF8CodeUnit str[256] = { '\0' };
	mfmU64 readSize = 0;

	mfError err = mfsReadUntil(stream, str, sizeof(str), &readSize, sep);
	if (err != MF_ERROR_OKAY)
		return err;

	err = mfsParseFromBufferI16(str, readSize, value, base, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

mfError mfsParseI32(mfsStream * stream, mfmI32 * value, mfmU64 base, const mfsUTF8CodeUnit * sep)
{
	mfsUTF8CodeUnit str[256] = { '\0' };
	mfmU64 readSize = 0;

	mfError err = mfsReadUntil(stream, str, sizeof(str), &readSize, sep);
	if (err != MF_ERROR_OKAY)
		return err;

	err = mfsParseFromBufferI32(str, readSize, value, base, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

mfError mfsParseI64(mfsStream * stream, mfmI64 * value, mfmU64 base, const mfsUTF8CodeUnit * sep)
{
	mfsUTF8CodeUnit str[256] = { '\0' };
	mfmU64 readSize = 0;

	mfError err = mfsReadUntil(stream, str, sizeof(str), &readSize, sep);
	if (err != MF_ERROR_OKAY)
		return err;

	err = mfsParseFromBufferI64(str, readSize, value, base, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

mfError mfsParseF32(mfsStream * stream, mfmF32 * value, mfmU64 base, const mfsUTF8CodeUnit * sep)
{
	mfsUTF8CodeUnit str[256] = { '\0' };
	mfmU64 readSize = 0;

	mfError err = mfsReadUntil(stream, str, sizeof(str), &readSize, sep);
	if (err != MF_ERROR_OKAY)
		return err;

	err = mfsParseFromBufferF32(str, readSize, value, base, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

mfError mfsParseF64(mfsStream * stream, mfmF64 * value, mfmU64 base, const mfsUTF8CodeUnit * sep)
{
	mfsUTF8CodeUnit str[256] = { '\0' };
	mfmU64 readSize = 0;

	mfError err = mfsReadUntil(stream, str, sizeof(str), &readSize, sep);
	if (err != MF_ERROR_OKAY)
		return err;

	err = mfsParseFromBufferF64(str, readSize, value, base, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}
