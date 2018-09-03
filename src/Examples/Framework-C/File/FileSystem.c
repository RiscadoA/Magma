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

	err = mffRegisterArchive(archive, u8"temp");
	if (err != MF_ERROR_OKAY)
		abort();

	mffFile* dir;
	err = mffCreateDirectory(&dir, u8"/temp/testFolder");
	if (err != MF_ERROR_OKAY)
		abort();

	err = mffCreateFile(NULL, u8"/temp/testFolder/test.txt");
	if (err != MF_ERROR_OKAY)
		abort();

	mffFile* file;
	err = mffGetFile(&file, u8"/temp/testFolder/test.txt");
	if (err != MF_ERROR_OKAY)
		abort();

	mfsStream* stream;
	err = mffOpenFile(&stream, file, MFF_FILE_WRITE);
	if (err != MF_ERROR_OKAY)
		abort();

	err = mfsPutString(stream, u8"test\n");
	if (err != MF_ERROR_OKAY)
		abort();

	err = mffCloseFile(stream);
	if (err != MF_ERROR_OKAY)
		abort();

	err = mffDeleteFile(file);
	if (err != MF_ERROR_OKAY)
		abort();

	err = mffDeleteDirectory(dir);
	if (err != MF_ERROR_OKAY)
		abort();

	err = mffUnregisterArchive(archive);
	if (err != MF_ERROR_OKAY)
		abort();

	mffDestroyFolderArchive(archive);
	mfTerminate();
	return 0;
}
