#include "PNGLoader.h"

#include "../../String/Stream.h"

typedef struct
{
	mfmU32 length;
	mfsUTF8CodeUnit type[4];
} mfgPNGChunkHeader;

static mfError mfgLoadPNGHeader(void * stream)
{
	mfmU8 header[8];
	mfmU64 readSize = 0;

	// Read header
	mfError err = mfsRead(stream, header, 8, &readSize);
	if (err != MF_ERROR_OKAY)
		return err;
	if (readSize != 8)
		return MFG_ERROR_UNEXPECTED_EOF;

	// Check header
	if (header[0] != 0x89)
		return MFG_ERROR_INVALID_DATA;
	if (header[1] != 0x50 || header[2] != 0x4E || header[3] != 0x47)
		return MFG_ERROR_INVALID_DATA;
	if (header[4] != 0x0D || header[5] != 0x0A)
		return MFG_ERROR_INVALID_DATA;
	if (header[6] != 0x1A)
		return MFG_ERROR_INVALID_DATA;
	if (header[7] != 0x0A)
		return MFG_ERROR_INVALID_DATA;

	return MF_ERROR_OKAY;
}

static mfError mfgLoadPNGChunkHeader(void * stream)
{
	mfmU8 header[8];
	mfmU64 readSize = 0;

	// Read header
	mfError err = mfsRead(stream, header, 8, &readSize);
	if (err != MF_ERROR_OKAY)
		return err;
	if (readSize != 8)
		return MFG_ERROR_UNEXPECTED_EOF;

	// Check header
	if (header[0] != 0x89)
		return MFG_ERROR_INVALID_DATA;
	if (header[1] != 0x50 || header[2] != 0x4E || header[3] != 0x47)
		return MFG_ERROR_INVALID_DATA;
	if (header[4] != 0x0D || header[5] != 0x0A)
		return MFG_ERROR_INVALID_DATA;
	if (header[6] != 0x1A)
		return MFG_ERROR_INVALID_DATA;
	if (header[7] != 0x0A)
		return MFG_ERROR_INVALID_DATA;

	return MF_ERROR_OKAY;
}

mfError mfgLoadPNG(void * stream, mfgPNGTextureData ** textureData, mfgEnum format, void * allocator)
{
	mfError err;

	// Load header
	err = mfgLoadPNGHeader(stream);
	if (err != MF_ERROR_OKAY)
		return err;

	

	return MF_ERROR_OKAY;
}

void mfgUnloadPNG(void * textureData)
{

}
