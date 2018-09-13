#include <Magma/Framework/Entry.h>
#include <Magma/Framework/String/UTF8.h>
#include <Magma/Framework/String/Stream.h>
#include <stdlib.h>

int main(int argc, const char** argv)
{
	if (mfInit(argc, argv) != MF_ERROR_OKAY)
		abort();

	mfError err;
	
	err = mfsPrintFormat(mfsOutStream, u8"Hello World!\n%d + %d = %f", 2, 3, 2.0f + 3.0f);
	if (err != MF_ERROR_OKAY)
		abort();
	
	err = mfsGetByte(mfsInStream, NULL);
	if (err != MF_ERROR_OKAY)
		abort();

	mfTerminate();
	return 0;
}