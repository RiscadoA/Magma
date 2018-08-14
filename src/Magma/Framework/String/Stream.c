#include "Stream.h"

#include <stdio.h>
#include <stdarg.h>

mfsStream* mfsIn = NULL;
mfsStream* mfsOut = NULL;

static mfmU8 mfsInDefaultBuffer[512];
static mfmU8 mfsOutDefaultBuffer[512];

typedef struct
{
	mfsStream base;
	FILE* file;
} mfsFileStream;

mfsError mfsFileWrite(void* stream, const mfmU8* data, mfmU64 dataSize, mfmU64* outSize)
{
	mfsFileStream* fileStream = (mfsFileStream*)stream;

	mfmU64 size = fwrite(data, sizeof(mfmU8), dataSize, fileStream->file);
	if (outSize != NULL)
		*outSize = size;

	return MFS_ERROR_OKAY;
}

mfsError mfsFileRead(void* stream, mfmU8* data, mfmU64 dataSize, mfmU64* outSize)
{
	mfsFileStream* fileStream = (mfsFileStream*)stream;

	mfmU64 size = fread(data, sizeof(mfmU8), dataSize, fileStream->file);
	if (outSize != NULL)
		*outSize = size;

	return MFS_ERROR_OKAY;
}

mfsError mfsFileFlush(void* stream)
{
	mfsFileStream* fileStream = (mfsFileStream*)stream;
	int ret = fflush(fileStream->file);
	if (ret == EOF)
		return MFS_ERROR_EOF;
	return MFS_ERROR_OKAY;
}

mfsError mfsFileSetBuffer(void* stream, mfmU8* buffer, mfmU64 bufferSize)
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

	return MFS_ERROR_OKAY;
}

void mfsDestroyFileStream(void* stream)
{
	free(stream);
}

mfsStream* mfsCreateFileStream(FILE* file, mfmU8* buffer, mfmU64 bufferSize)
{
	mfsFileStream* stream = (mfsFileStream*)malloc(sizeof(mfsFileStream));

	stream->base.object.destructorFunc = &mfsDestroyFileStream;
	stream->base.object.referenceCount = 0;

	stream->base.buffer = buffer;
	stream->base.bufferSize = bufferSize;
	stream->base.bufferHead = 0;
	stream->file = file;

	stream->base.read = &mfsFileRead;
	stream->base.write = &mfsFileWrite;
	stream->base.flush = &mfsFileFlush;
	stream->base.setBuffer = &mfsFileSetBuffer;
}

void mfsInitStream()
{
	// Initialize mfsIn stream
	mfsIn = mfsCreateFileStream(stdin, mfsInDefaultBuffer, sizeof(mfsInDefaultBuffer));
	
	// Initialize mfsOut stream
	mfsOut = mfsCreateFileStream(stdout, mfsOutDefaultBuffer, sizeof(mfsOutDefaultBuffer));
}

void mfsTerminateStream()
{
	if (mfsIn != NULL)
		mfsIn->object.destructorFunc(mfsIn);
	if (mfsOut != NULL)
		mfsOut->object.destructorFunc(mfsOut);
}

mfsError mfsWrite(mfsStream * stream, const mfmU8 * data, mfmU64 dataSize, mfmU64 * outSize)
{
	if (stream == NULL || data == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;
	if (dataSize == 0)
		return MFS_ERROR_OKAY;
	return stream->write(stream, data, dataSize, outSize);
}

mfsError mfsRead(mfsStream * stream, mfmU8 * data, mfmU64 dataSize, mfmU64 * outSize)
{
	if (stream == NULL || data == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;
	if (dataSize == 0)
		return MFS_ERROR_OKAY;
	return stream->read(stream, data, dataSize, outSize);
}

mfsError mfsFlush(mfsStream * stream)
{
	if (stream == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;
	return stream->flush(stream);
}

mfsError mfsSetBuffer(mfsStream * stream, mfmU8 * buffer, mfmU64 bufferSize)
{
	if (stream == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;

	if (stream->setBuffer == NULL)
	{
		if (stream->buffer != NULL)
		{
			if (bufferSize == 0)
				return MFS_ERROR_INVALID_ARGUMENTS;

			mfsError err = stream->flush(stream);
			if (err != MFS_ERROR_OKAY)
				return err;
		}

		stream->buffer = buffer;
		stream->bufferSize = bufferSize;
		return MFS_ERROR_OKAY;
	}
	else
		return stream->setBuffer(stream, buffer, bufferSize);
}

mfsError mfsOpenFile(mfsStream ** stream, mfmU32 mode, const mfsUTF8CodeUnit * path)
{
	if (stream == NULL || path == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;

	FILE* file;
	errno_t err;
	if (mode == MFS_FILE_READ)
		err = fopen_s(&file, path, "rb");
	else if (mode == MFS_FILE_WRITE)
		err = fopen_s(&file, path, "wb");
	else
		return MFS_ERROR_INVALID_ARGUMENTS;

	if (err != 0)
		return MFS_ERROR_INTERNAL;

	*stream = mfsCreateFileStream(file, NULL, 0);

	return MFS_ERROR_OKAY;
}

void mfsCloseFile(mfsStream * stream)
{
	fclose(((mfsFileStream*)stream)->file);
	mfsDestroyFileStream(stream);
}

mfsError mfsGetByte(mfsStream * stream, mfmU8 * byte)
{
	if (stream == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;

	if (byte == NULL)
	{
		mfmU8 temp;
		mfmU64 readSize = 0;
		mfsError err = stream->read(stream, &temp, sizeof(mfmU8), &readSize);
		if (err != MFS_ERROR_OKAY)
			return err;
		if (readSize != sizeof(mfmU8))
			return MFS_ERROR_FAILED_TO_READ_ALL;
		return MFS_ERROR_OKAY;
	}
	else
	{
		mfmU64 readSize = 0;
		mfsError err = stream->read(stream, byte, sizeof(mfmU8), &readSize);
		if (err != MFS_ERROR_OKAY)
			return err;
		if (readSize != sizeof(mfmU8))
			return MFS_ERROR_FAILED_TO_READ_ALL;
		return MFS_ERROR_OKAY;
	}
}

mfsError mfsPutByte(mfsStream * stream, mfmU8 byte)
{
	if (stream == NULL)
		return MFS_ERROR_INVALID_ARGUMENTS;

	mfmU64 writeSize = 0;
	mfsError err = stream->write(stream, &byte, sizeof(mfmU8), &writeSize);
	if (err != MFS_ERROR_OKAY)
		return err;
	if (writeSize != sizeof(mfmU8))
		return MFS_ERROR_FAILED_TO_WRITE_ALL;
	return MFS_ERROR_OKAY;
}

mfsError mfsPrintFormatUTF8(mfsStream * stream, const mfsUTF8CodeUnit * format, ...)
{
	va_list args;
	va_start(args, format);

	mfsUTF8CodeUnit tempBuf[256];

	mfmBool escape = MFM_FALSE;
	while (*format != '\0')
	{
		if (escape == MFM_TRUE)
		{
			mfsError err = mfsPutByte(stream, *format);
			escape = MFM_FALSE;
			if (err != MFS_ERROR_OKAY)
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
							mfsError err = mfsPutByte(stream, tempBuf[i]);
							escape = MFM_FALSE;
							if (err != MFS_ERROR_OKAY)
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
							mfsError err = mfsPutByte(stream, tempBuf[i]);
							escape = MFM_FALSE;
							if (err != MFS_ERROR_OKAY)
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
							mfsError err = mfsPutByte(stream, tempBuf[i]);
							escape = MFM_FALSE;
							if (err != MFS_ERROR_OKAY)
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
							mfsError err = mfsPutByte(stream, tempBuf[i]);
							escape = MFM_FALSE;
							if (err != MFS_ERROR_OKAY)
							{
								va_end(args);
								return err;
							}
						}
				}
				else if (*format == 's')
				{
					int ret = snprintf(tempBuf, sizeof(tempBuf), "%s", va_arg(args, const mfsUTF8CodeUnit*));
					if (ret < 0)
						return MFS_ERROR_INTERNAL;
					else
						for (int i = 0; i < ret; ++i)
						{
							mfsError err = mfsPutByte(stream, tempBuf[i]);
							escape = MFM_FALSE;
							if (err != MFS_ERROR_OKAY)
							{
								va_end(args);
								return err;
							}
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
				mfsError err = mfsPutByte(stream, *format);
				escape = MFM_FALSE;
				if (err != MFS_ERROR_OKAY)
				{
					va_end(args);
					return err;
				}
			}
		}

		++format;
	}
	
	va_end(args);

	return MFS_ERROR_OKAY;
}
