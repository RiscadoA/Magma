#pragma once

#include "Error.h"

#include "../Memory/Object.h"
#include "../String/Stream.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define MFF_MAX_ARCHIVE_COUNT		16
#define MFF_MAX_FILE_PATH_SIZE		512
#define MFF_MAX_ARCHIVE_NAME_SIZE	32

#define MFF_FILE_WRITE	0x01
#define MFF_FILE_READ	0x02

#define MFF_FILE		0x03
#define MFF_DIRECTORY	0x04
#define MFF_ARCHIVE		0x05

	typedef mfmU32 mffEnum;
	typedef struct mffFile mffFile;
	typedef mffFile mffDirectory;
	typedef struct mffArchive mffArchive;

	typedef mfError(*mffArchiveGetFileFunc)(mffArchive* archive, mffFile** outFile, const mfsUTF8CodeUnit* path);
	typedef mfError(*mffArchiveGetFileTypeFunc)(mffArchive* archive, mffFile* file, mffEnum* outType);
	typedef mfError(*mffArchiveGetFirstFileFunc)(mffArchive* archive, mffFile** outFile, mffDirectory* directory);
	typedef mfError(*mffArchiveGetNextFileFunc)(mffArchive* archive, mffFile** outFile, mffFile* file);
	typedef mfError(*mffArchiveGetParentFunc)(mffArchive* archive, mffDirectory** outParent, mffFile* file);
	typedef mfError(*mffArchiveCreateDirectoryFunc)(mffArchive* archive, mffDirectory** outDir, const mfsUTF8CodeUnit* path);
	typedef mfError(*mffArchiveDeleteDirectoryFunc)(mffArchive* archive, mffDirectory* dir);
	typedef mfError(*mffArchiveCreateFileFunc)(mffArchive* archive, mffFile** outFile, const mfsUTF8CodeUnit* path);
	typedef mfError(*mffArchiveDeleteFileFunc)(mffArchive* archive, mffFile* file);
	typedef mfError(*mffArchiveOpenFileFunc)(mffArchive* archive, mfsStream** outStream, mffFile* file, mffEnum mode);

	struct mffFile
	{
		mfmObject object;
		mffArchive* archive;
	};

	struct mffArchive
	{
		mfmObject object;
		mfsUTF8CodeUnit name[MFF_MAX_ARCHIVE_NAME_SIZE];

		mffArchiveGetFileFunc getFile;
		mffArchiveGetFileTypeFunc getFileType;
		mffArchiveGetFirstFileFunc getFirstFile;
		mffArchiveGetNextFileFunc getNextFile;
		mffArchiveGetParentFunc getParent;
		mffArchiveCreateDirectoryFunc createDirectory;
		mffArchiveDeleteDirectoryFunc deleteDirectory;
		mffArchiveCreateFileFunc createFile;
		mffArchiveDeleteFileFunc deleteFile;
		mffArchiveOpenFileFunc openFile;
	};

	mfError mffInitFileSystem(void);

	mfError mffTerminateFileSystem(void);

	mfError mffRegisterArchive(mffArchive* archive, const mfsUTF8CodeUnit* name);

	mfError mffUnregisterArchive(mffArchive* archive);

	mfError mffGetFile(mffFile** outFile, const mfsUTF8CodeUnit* path);
	
	mfError mffGetFileType(mffFile* file, mffEnum* outType);

	mfError mffGetFirstFile(mffFile** outFile, mffDirectory* directory);

	mfError mffGetNextFile(mffFile** outFile, mffFile* file);

	mfError mffGetParent(mffDirectory** outParent, mffFile* file);

	mfError mffCreateDirectory(mffDirectory** outDir, const mfsUTF8CodeUnit* path);

	mfError mffDeleteDirectory(mffDirectory* dir);

	mfError mffCreateFile(mffFile** outFile, const mfsUTF8CodeUnit* path);

	mfError mffDeleteFile(mffFile* file);

	mfError mffOpenFile(mfsStream** outStream, mffFile* file, mffEnum mode);

	mfError mffCloseFile(mfsStream* stream);

#ifdef __cplusplus
}
#endif
