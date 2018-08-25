#include <Magma/Framework/Entry.h>

int main(int argc, const char** argv)
{
	if (mfInit(argc, argv) != MF_ERROR_OKAY)
		abort();

	mfTerminate();
	return 0;
}