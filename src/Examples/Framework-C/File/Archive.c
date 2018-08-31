#include <Magma/Framework/Entry.h>
#include <Magma/Framework/File/Archive.h>
#include <Magma/Framework/String/Stream.h>
#include <Magma/Framework/String/StringStream.h>

#include <stdlib.h>

int main(int argc, const char** argv)
{
	if (mfInit(argc, argv) != MF_ERROR_OKAY)
		abort();

	mfError err;

	mfsUTF8CodeUnit archivePath[256];
	{
		mfsStringStream ss;

		err = mfsCreateLocalStringStream(&ss, archivePath, sizeof(archivePath));
		if (err != MF_ERROR_OKAY)
			abort();
		err = mfsPutString(&ss, mffMagmaRootDirectory);
		if (err != MF_ERROR_OKAY)
			abort();
		err = mfsPutString(&ss, "resources/");
		if (err != MF_ERROR_OKAY)
			abort();
		mfsDestroyLocalStringStream(&ss);
	}

	mffArchive* archive;
	err = mffCreateArchive(&archive, archivePath, NULL);
	if (err != MF_ERROR_OKAY)
		abort();

	mffFile* file;
	err = mffGetFile(archive, &file, u8"Shaders/GUI/box.mni");
	if (err != MF_ERROR_OKAY)
		abort();

	mffDestroyArchive(archive);

	mfTerminate();
	return 0;
}
