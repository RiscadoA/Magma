#include <Magma/Framework/Entry.h>
#include <Magma/Framework/File/Path.h>
#include <Magma/Framework/String/Stream.h>
#include <Magma/Framework/String/StringStream.h>

#include <stdlib.h>

int main(int argc, const char** argv)
{
	if (mfInit(argc, argv) != MF_ERROR_OKAY)
		abort();

	mfError err;

	const mfsUTF8CodeUnit* path1 = u8"/test/text.txt";
	const mfsUTF8CodeUnit* path2 = u8"/test/text/";
	const mfsUTF8CodeUnit* path3 = u8"/test/";
	const mfsUTF8CodeUnit* path4 = u8"/a/";

	err = mfsPrintFormat(mfsOutStream, u8"%s\n", mffGoDownPath(path1));
	if (err != MF_ERROR_OKAY)
		abort();

	err = mfsPrintFormat(mfsOutStream, u8"%s\n", mffGoDownPath(path2));
	if (err != MF_ERROR_OKAY)
		abort();

	err = mfsPrintFormat(mfsOutStream, u8"%s\n", mffGoDownPath(path3));
	if (err != MF_ERROR_OKAY)
		abort();

	err = mfsPrintFormat(mfsOutStream, u8"%s\n", mffGoDownPath(path4));
	if (err != MF_ERROR_OKAY)
		abort();

	mfTerminate();
	return 0;
}
