#include <Magma/Framework/Memory/Allocator.h>
#include <Magma/Framework/Entry.h>
#include <Magma/Framework/String/Stream.h>

#include <stdlib.h>

int main(int argc, const char** argv)
{
	if (mfInit(argc, argv) != MF_ERROR_OKAY)
		abort();

	mfError err = MF_ERROR_OKAY;
	
	void* mem;
	err = mfmAllocateAligned(NULL, &mem, 256, 256);
	if (err != MF_ERROR_OKAY)
		abort();

	err = mfsPrintFormat(mfsOutStream, u8"0x%x\n", mem);
	if (err != MF_ERROR_OKAY)
		abort();

	err = mfmDeallocateAligned(NULL, mem);
	if (err != MF_ERROR_OKAY)
		abort();

	mfTerminate();
	return 0;
}