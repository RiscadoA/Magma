#include <Magma/Framework/Memory/Deflate.h>
#include <Magma/Framework/Memory/Inflate.h>
#include <Magma/Framework/Entry.h>

#include <stdlib.h>

int main(int argc, const char** argv)
{
	if (mfInit(argc, argv) != MF_ERROR_OKAY)
		abort();

	mfError err = MF_ERROR_OKAY;

	mfmU8 buffer[256];
	mfsStream* stream;

	err = mfmCreateDeflateStream(&stream, buffer, sizeof(buffer), MFM_FAST_COMPRESSION, NULL);
	if (err != MF_ERROR_OKAY)
		abort();

	mfsUTF8CodeUnit uncompressed[] = u8"Hello hello hello hello hello hello hello hello hello hello hello hello";

	err = mfsWrite(stream, uncompressed, sizeof(uncompressed), NULL);
	if (err != MF_ERROR_OKAY)
		abort();

	mfmU8 compressed[65536];
	mfmU64 compressedSize = 0;

	err = mfsRead(stream, compressed, sizeof(compressed), &compressedSize);
	if (err != MF_ERROR_OKAY)
		abort();

	mfmDestroyDeflateStream(stream);	

	mfsPrintFormat(mfsOutStream, u8"Uncompressed:\n%s\nSize: %d\nCompressed size: %d\n", uncompressed, sizeof(uncompressed), compressedSize);

	mfmU8 decompressed[65536];
	mfmU64 decompressedSize = 0;

	err = mfmCreateInflateStream(&stream, buffer, sizeof(buffer), NULL);
	if (err != MF_ERROR_OKAY)
		abort();

	err = mfsWrite(stream, compressed, compressedSize, NULL);
	if (err != MF_ERROR_OKAY)
		abort();

	err = mfsRead(stream, decompressed, sizeof(decompressed), &decompressedSize);
	if (err != MF_ERROR_OKAY)
		abort();

	mfmDestroyInflateStream(stream);

	mfsPrintFormat(mfsOutStream, u8"Decompressed:\n%s\nSize: %d\n", decompressed, decompressedSize);

	mfsGetByte(mfsInStream, NULL);

	mfTerminate();
	return 0;
}