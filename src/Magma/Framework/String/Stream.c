#include "Stream.h"
#include "../Memory/Allocator.h"

#include <stdio.h>
#include <stdarg.h>

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
	if (mfmDestroyObject(stream) != MF_ERROR_OKAY)
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

	return stream;
}

mfError mfsInitStream()
{
	mfError err = MF_ERROR_OKAY;

	// Initialize mfsInStream stream
	mfsInStream = mfsCreateFileStream(stdin, mfsInDefaultBuffer, sizeof(mfsInDefaultBuffer));
	err = mfmIncObjectRef(&mfsInStream->object);
	if (err != MF_ERROR_OKAY)
		return err;
	
	// Initialize mfsOutStream stream
	mfsOutStream = mfsCreateFileStream(stdout, mfsOutDefaultBuffer, sizeof(mfsOutDefaultBuffer));
	err = mfmIncObjectRef(&mfsOutStream->object);
	if (err != MF_ERROR_OKAY)
		return err;

	// Initialize mfsErrStream stream
	mfsErrStream = mfsCreateFileStream(stderr, mfsErrDefaultBuffer, sizeof(mfsErrDefaultBuffer));
	err = mfmIncObjectRef(&mfsErrStream->object);
	if (err != MF_ERROR_OKAY)
		return err;

	return err;
}

mfError mfsTerminateStream()
{
	mfError err = MF_ERROR_OKAY;

	if (mfsInStream != NULL)
	{
		err = mfmDecObjectRef(&mfsInStream->object);
		if (err != MF_ERROR_OKAY)
			return err;
	}

	if (mfsOutStream != NULL)
	{
		err = mfmDecObjectRef(&mfsOutStream->object);
		if (err != MF_ERROR_OKAY)
			return err;
	}

	if (mfsErrStream != NULL)
	{
		err = mfmDecObjectRef(&mfsErrStream->object);
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

mfError mfsPrintFormat(mfsStream * stream, const mfsUTF8CodeUnit * format, ...)
{
	va_list args;
	va_start(args, format);

	mfsUTF8CodeUnit tempBuf[256];

	mfmBool escape = MFM_FALSE;
	while (*format != '\0')
	{
		if (escape == MFM_TRUE)
		{
			mfError err = mfsPutByte(stream, *format);
			escape = MFM_FALSE;
			if (err != MF_ERROR_OKAY)
			{
				va_end(args);
				return err;
			}
		}
		else
		{
			if (*format == '\\')
				escape = MFM_TRUE;
			else if (*format == '%')
			{
				++format;
				if (*format == 'd')
				{
					int ret = snprintf(tempBuf, sizeof(tempBuf), "%d", va_arg(args, mfmI32));
					if (ret < 0)
						return MFS_ERROR_INTERNAL;
					else
						for (int i = 0; i < ret; ++i)
						{
							mfError err = mfsPutByte(stream, tempBuf[i]);
							if (err != MF_ERROR_OKAY)
							{
								va_end(args);
								return err;
							}
						}
				}
				else if (*format == 'x')
				{
					int ret = snprintf(tempBuf, sizeof(tempBuf), "%x", va_arg(args, mfmI32));
					if (ret < 0)
						return MFS_ERROR_INTERNAL;
					else
						for (int i = 0; i < ret; ++i)
						{
							mfError err = mfsPutByte(stream, tempBuf[i]);
							if (err != MF_ERROR_OKAY)
							{
								va_end(args);
								return err;
							}
						}
				}
				else if (*format == 'f')
				{
					int ret = snprintf(tempBuf, sizeof(tempBuf), "%f", va_arg(args, mfmF64));
					if (ret < 0)
						return MFS_ERROR_INTERNAL;
					else
						for (int i = 0; i < ret; ++i)
						{
							mfError err = mfsPutByte(stream, tempBuf[i]);
							if (err != MF_ERROR_OKAY)
							{
								va_end(args);
								return err;
							}
						}
				}
				else if (*format == 'c')
				{
					int ret = snprintf(tempBuf, sizeof(tempBuf), "%c", va_arg(args, mfsUnicodePoint));
					if (ret < 0)
						return MFS_ERROR_INTERNAL;
					else
						for (int i = 0; i < ret; ++i)
						{
							mfError err = mfsPutByte(stream, tempBuf[i]);
							if (err != MF_ERROR_OKAY)
							{
								va_end(args);
								return err;
							}
						}
				}
				else if (*format == 's')
				{
					mfError err = mfsPutString(stream, va_arg(args, const mfsUTF8CodeUnit*));
					if (err != MF_ERROR_OKAY)
					{
						va_end(args);
						return err;
					}
				}
				else
				{
					va_end(args);
					return MFS_ERROR_INVALID_ARGUMENTS;
				}
			}
			else
			{
				mfError err = mfsPutByte(stream, *format);
				escape = MFM_FALSE;
				if (err != MF_ERROR_OKAY)
				{
					va_end(args);
					return err;
				}
			}
		}

		++format;
	}
	
	va_end(args);

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
