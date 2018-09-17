#include <Magma/Framework/Entry.h>
#include <Magma/Framework/String/UTF8.h>
#include <Magma/Framework/String/Stream.h>
#include <stdlib.h>

int main(int argc, const char** argv)
{
	if (mfInit(argc, argv) != MF_ERROR_OKAY)
		abort();

	mfError err;
	
	err = mfsPrintFormat(mfsOutStream, u8"Hello World!\n0x%%-bhi + %%u = %%-d1f\n", 'a', 16u, 32.0f + 16.0f);
	if (err != MF_ERROR_OKAY)
		abort();
	
	mfmF32 f32 = 0.0f;
	err = mfsParseF32(mfsInStream, &f32, 10, u8"\n");
	if (err != MF_ERROR_OKAY)
		abort();

	err = mfsPrintFormat(mfsOutStream, u8"Parsed float from input: %%f\n", f32);
	if (err != MF_ERROR_OKAY)
		abort();

	err = mfsReadUntil(mfsInStream, NULL, 0, NULL, u8"\n");
	if (err != MF_ERROR_OKAY)
		abort();

	mfTerminate();
	return 0;
}