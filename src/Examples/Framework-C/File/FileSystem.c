#include <Magma/Framework/Entry.h>
#include <Magma/Framework/File/FileSystem.h>
#include <Magma/Framework/File/Path.h>
#include <Magma/Framework/File/FolderArchive.h>
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
		err = mfsPutString(&ss, "/resources/FileSystem-Example");
		if (err != MF_ERROR_OKAY)
			abort();
		mfsDestroyLocalStringStream(&ss);
	}

	mffArchive* archive;
	
	err = mffCreateFolderArchive(&archive, NULL, archivePath);
	if (err != MF_ERROR_OKAY)
		abort();

	err = mffRegisterArchive(archive);
	if (err != MF_ERROR_OKAY)
		abort();

	err = mffCreateFile(NULL, u8"/FileSystem-Example/testFolder/test4/test2/test.txt");
	if (err != MF_ERROR_OKAY)
		abort();

	mffFile* file;
	err = mffGetFile(&file, u8"/FileSystem-Example/testFolder/test4/test2/test.txt");
	if (err != MF_ERROR_OKAY)
		abort();

	err = mffDeleteFile(file);
	if (err != MF_ERROR_OKAY)
		abort();

	err = mffUnregisterArchive(archive);
	if (err != MF_ERROR_OKAY)
		abort();

	mffDestroyFolderArchive(archive);
	mfTerminate();
	return 0;
}
