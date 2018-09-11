#include <Magma/Framework/Entry.h>
#include <Magma/Framework/String/UTF8.h>
#include <Magma/Framework/String/Conversion.h>
#include <Magma/Framework/String/Stream.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, const char** argv)
{
	if (mfInit(argc, argv) != MF_ERROR_OKAY)
		abort();

	mfError err;

	char buf[256] = { '\0' };

	err = mfsPrintToBufferU64(buf, sizeof(buf), 0xffff'ffff'ffff'ffff, 16, NULL);
	if (err != MF_ERROR_OKAY)
		abort();

	mfmU64 out;
	err = mfsParseFromBufferU64(buf, sizeof(buf), &out, 16, NULL);
	if (err != MF_ERROR_OKAY)
		abort();

	memset(buf, 0, sizeof(buf));

	err = mfsPrintToBufferU64(buf, sizeof(buf), out, 10, NULL);
	if (err != MF_ERROR_OKAY)
		abort();

	err = mfsPutString(mfsOutStream, buf);
	if (err != MF_ERROR_OKAY)
		abort();

	err = mfsReadUntil(mfsInStream, NULL, 0, NULL, u8"\n");
	if (err != MF_ERROR_OKAY)
		abort();

	mfTerminate();
	return 0;
}