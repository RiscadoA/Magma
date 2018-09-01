#include "FolderArchive.h"
#include "Path.h"

#include "../Memory/Allocator.h"
#include "../Thread/Mutex.h"

#include <stdlib.h>
#include <string.h>

typedef struct mffFolderFile mffFolderFile;

struct mffFolderFile
{
	mffFile base;
	mfsUTF8CodeUnit path[MFF_MAX_FILE_PATH_SIZE];
	mffFolderFile* parent;
	mffFolderFile* next;
	mffFolderFile* first;
	mffEnum type;
};

typedef struct
{
	mffArchive base;
	mfsUTF8CodeUnit path[MFF_MAX_FILE_PATH_SIZE];
	void* allocator;
	mffFolderFile* firstFile;
	mftMutex* mutex;
} mffFolderArchive;

static mfError mffArchiveGetFile(mffArchive* archive, mffFile** outFile, const mfsUTF8CodeUnit* path)
{
	mffFolderArchive* folderArchive = archive;
	mfError err;

	err = mftLockMutex(folderArchive->mutex, 0);
	if (err != MF_ERROR_OKAY)
		return err;

	mffFolderFile* currentFile = folderArchive->firstFile;
	while (currentFile != NULL)
	{
		if (strcmp(currentFile->path, path) == 0)
		{
			*outFile = currentFile;

			err = mftUnlockMutex(folderArchive->mutex);
			if (err != MF_ERROR_OKAY)
				return err;

			return MF_ERROR_OKAY;
		}

		// Next file
		if (currentFile->next == NULL)
			currentFile = currentFile->parent;
		else
			currentFile = currentFile->next;
	}

	mftUnlockMutex(folderArchive->mutex);
	return MFF_ERROR_FILE_NOT_FOUND;
}

static mfError mffArchiveGetFileType(mffArchive* archive, mffFile* file, mffEnum* outType)
{
	mffFolderFile* folderFile = file;
	*outType = folderFile->type;
	return MF_ERROR_OKAY;
}

static mfError mffArchiveGetFirstFile(mffArchive* archive, mffFile** outFile, mffDirectory* directory)
{
	mffFolderFile* folderFile = directory;
	*outFile = folderFile->first;
	return MF_ERROR_OKAY;
}

static mfError mffArchiveGetNextFile(mffArchive* archive, mffFile** outFile, mffFile* file)
{
	mffFolderFile* folderFile = file;
	*outFile = folderFile->next;
	return MF_ERROR_OKAY;
}

static mfError mffArchiveGetParent(mffArchive* archive, mffDirectory** outParent, mffFile* file)
{
	mffFolderFile* folderFile = file;
	*outParent = folderFile->parent;
	return MF_ERROR_OKAY;
}

static mfError mffArchiveCreateDirectory(mffArchive* archive, mffDirectory** outDir, const mfsUTF8CodeUnit* path)
{
	// TO DO
	return MF_ERROR_OKAY;
}

static mfError mffArchiveDeleteDirectory(mffArchive* archive, mffDirectory* dir)
{
	// TO DO
	return MF_ERROR_OKAY;
}

static mfError mffArchiveCreateFile(mffArchive* archive, mffFile** outFile, const mfsUTF8CodeUnit* path)
{
	// TO DO
	return MF_ERROR_OKAY;
}

static mfError mffArchiveDeleteFile(mffArchive* archive, mffFile* file)
{
	// TO DO
	return MF_ERROR_OKAY;
}

static void mffArchiveCloseFile(void* stream)
{
	// TO DO
	return MF_ERROR_OKAY;
}

static mfError mffArchiveOpenFile(mffArchive* archive, mfsStream** outStream, mffFile* file, mffEnum mode)
{
	// TO DO
	return MF_ERROR_OKAY;
}

mfError mffCreateFolderArchive(mffArchive ** outArchive, void* allocator, const mfsUTF8CodeUnit* path)
{
	mfError err;
	mffFolderArchive* folderArchive;

	err = mfmAllocate(allocator, &folderArchive, sizeof(mffFolderArchive));
	if (err != MF_ERROR_OKAY)
		return err;

	err = mfmInitObject(&folderArchive->base.object);
	if (err != MF_ERROR_OKAY)
		return err;
	folderArchive->base.object.destructorFunc = &mffDestroyFolderArchive;
	folderArchive->allocator = allocator;
	strcpy(folderArchive->path, path);
	strcpy(folderArchive->base.name, mffGetPathName(path));

	folderArchive->base.getFile = &mffArchiveGetFile;
	folderArchive->base.getFileType = &mffArchiveGetFileType;
	folderArchive->base.getFirstFile = &mffArchiveGetFirstFile;
	folderArchive->base.getNextFile = &mffArchiveGetNextFile;
	folderArchive->base.getParent = &mffArchiveGetParent;
	folderArchive->base.createDirectory = &mffArchiveCreateDirectory;
	folderArchive->base.deleteDirectory = &mffArchiveDeleteDirectory;
	folderArchive->base.createFile = &mffArchiveCreateFile;
	folderArchive->base.deleteFile = &mffArchiveDeleteFile;
	folderArchive->base.openFile = &mffArchiveOpenFile;

	folderArchive->firstFile = NULL;

	// Create mutex
	err = mftCreateMutex(&folderArchive->mutex, allocator);
	if (err != MF_ERROR_OKAY)
		return err;

	// Get files in folder


	*outArchive = folderArchive;
	return MF_ERROR_OKAY;
}

void mffDestroyFolderArchive(void * archive)
{
	if (archive == NULL)
		abort();

	mfError err;
	mffFolderArchive* folderArchive = archive;

	err = mftDestroyMutex(&folderArchive->mutex);
	if (err != MF_ERROR_OKAY)
		abort();

	err = mfmDestroyObject(&folderArchive->base.object);
	if (err != MF_ERROR_OKAY)
		abort();
	
	err = mfmDeallocate(folderArchive->allocator, folderArchive);
	if (err != MF_ERROR_OKAY)
		abort();

	return MF_ERROR_OKAY;
}
