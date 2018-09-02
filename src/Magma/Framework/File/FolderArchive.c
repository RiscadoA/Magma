#include "FolderArchive.h"
#include "Path.h"
#include "Config.h"

#include "../Memory/Allocator.h"
#include "../String/StringStream.h"
#include "../Thread/Mutex.h"

#include <stdlib.h>
#include <string.h>

#if defined(MAGMA_FRAMEWORK_USE_WINDOWS_FILESYSTEM)
#include <Windows.h>
#else
#error MAGMA_FRAMEWORK_USE_WINDOWS_FILESYSTEM must be defined. File system library still only supports Windows.
#endif

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

static void mffDestroyFile(void* file)
{
	if (file == NULL)
		abort();

	mfError err;
	mffFolderFile* folderFile = file;

	err = mfmDestroyObject(&folderFile->base.object);
	if (err != MF_ERROR_OKAY)
		abort();

	err = mfmDeallocate(((mffFolderArchive*)folderFile->base.archive)->allocator, folderFile);
	if (err != MF_ERROR_OKAY)
		abort();
}

static mfError mffArchiveGetFileUnsafe(mffArchive* archive, mffFile** outFile, const mfsUTF8CodeUnit* path)
{
	mffFolderArchive* folderArchive = archive;
	mfError err;

	if (strcmp(path, u8"/") == 0)
	{
		*outFile = NULL;
		return MF_ERROR_OKAY;
	}

	mffFolderFile* currentFile = folderArchive->firstFile;
	while (currentFile != NULL)
	{
		if (strcmp(currentFile->path, path) == 0)
		{
			*outFile = currentFile;
			return MF_ERROR_OKAY;
		}

		// Next file
		if (currentFile->first != NULL)
			currentFile = currentFile->first;
		else
		{
			if (currentFile->next == NULL)
			{
				if (currentFile->parent == NULL)
					break;
				mffFolderFile* nextParent = currentFile;
				do
				{
					nextParent = nextParent->parent;
					if (nextParent == NULL)
						break;
				} while (nextParent->next == NULL);
				if (nextParent == NULL)
					currentFile = NULL;
				else
					currentFile = nextParent->next;
			}
			else
				currentFile = currentFile->next;
		}
	}

	return MFF_ERROR_FILE_NOT_FOUND;
}

static mfError mffArchiveGetFile(mffArchive* archive, mffFile** outFile, const mfsUTF8CodeUnit* path)
{
	mffFolderArchive* folderArchive = archive;
	mfError err;

	err = mftLockMutex(folderArchive->mutex, 0);
	if (err != MF_ERROR_OKAY)
		return err;

	err = mffArchiveGetFileUnsafe(archive, outFile, path);
	if (err != MF_ERROR_OKAY)
	{
		mftUnlockMutex(folderArchive->mutex);
		return err;
	}

	err = mftUnlockMutex(folderArchive->mutex);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
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
	if (directory == NULL)
		*outFile = ((mffFolderArchive*)archive)->firstFile;
	else
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

static mfError mffArchiveCreateFileUnsafe(mffArchive* archive, mffFile** outFile, const mfsUTF8CodeUnit* path)
{
	mfError err;
	mffFolderArchive* folderArchive = archive;

	// Get parent path
	// Example: /test.txt -> / ; /Test/test.txt -> /Test/

	// Get last '/'
	const mfsUTF8CodeUnit* lastSep = path;
	const mfsUTF8CodeUnit* it = path;
	while (*it != '\0')
	{
		if (*it == '/')
			lastSep = it;
		++it;
	}

	// Get separator offset
	mfmU64 lastSepOff = lastSep - path;
	if (lastSepOff >= MFF_MAX_FILE_PATH_SIZE - 1)
		return MFF_ERROR_PATH_TOO_BIG;

	// Get parent file
	mffFolderFile* parentFile;
	{
		mfsUTF8CodeUnit parentPath[MFF_MAX_FILE_PATH_SIZE];
		memcpy(parentPath, path, lastSepOff);
		parentPath[lastSepOff] = '\0';

		if (lastSepOff == 0)
			parentFile = NULL;
		else
		{
			err = mffArchiveGetFileUnsafe(archive, &parentFile, parentPath);
			if (err != MF_ERROR_OKAY)
				return err;
		}
	}

	mffFolderFile* file;

	// Create real file
	{
		mfsUTF8CodeUnit realPath[256];
		{
			mfsStringStream ss;
			err = mfsCreateLocalStringStream(&ss, realPath, sizeof(realPath));
			if (err != MF_ERROR_OKAY)
				return err;
			err = mfsPutString(&ss, folderArchive->path);
			if (err != MF_ERROR_OKAY)
				return err;
			err = mfsPutString(&ss, path);
			if (err != MF_ERROR_OKAY)
				return err;
			mfsDestroyLocalStringStream(&ss);
		}

		HANDLE f = CreateFile(realPath, 0, 0, NULL, CREATE_NEW, 0, NULL);
		if (f == INVALID_HANDLE_VALUE)
			return MFF_ERROR_INTERNAL_ERROR;
		GetLastError();
		CloseHandle(f);
	}

	// Create file
	{
		err = mfmAllocate(folderArchive->allocator, &file, sizeof(mffFolderFile));
		if (err != MF_ERROR_OKAY)
			return err;

		err = mfmInitObject(&file->base.object);
		if (err != MF_ERROR_OKAY)
			return err;

		file->base.object.destructorFunc = &mffDestroyFile;
		file->base.archive = archive;
		file->first = NULL;
		file->next = NULL;
		file->parent = parentFile;
		file->type = MFF_FILE;

		mfsStringStream ss;
		err = mfsCreateLocalStringStream(&ss, file->path, sizeof(file->path));
		if (err != MF_ERROR_OKAY)
			return err;
		err = mfsPrintFormat(&ss, u8"%s", path);
		if (err != MF_ERROR_OKAY)
			return err;
		mfsDestroyLocalStringStream(&ss);
	}

	// Add new file to parent file
	if (parentFile == NULL)
	{
		if (folderArchive->firstFile == NULL)
			folderArchive->firstFile = file;
		else
		{
			mffFolderFile* prevFile = folderArchive->firstFile;
			while (prevFile->next != NULL)
				prevFile = prevFile->next;
			prevFile->next = file;
		}
	}
	else
	{
		if (parentFile->first == NULL)
			parentFile->first = file;
		else
		{
			mffFolderFile* prevFile = parentFile->first;
			while (prevFile->next != NULL)
				prevFile = prevFile->next;
			prevFile->next = file;
		}
	}

	return MF_ERROR_OKAY;
}

static mfError mffArchiveCreateFile(mffArchive* archive, mffFile** outFile, const mfsUTF8CodeUnit* path)
{
	mffFolderArchive* folderArchive = archive;
	mfError err;

	err = mftLockMutex(folderArchive->mutex, 0);
	if (err != MF_ERROR_OKAY)
		return err;

	err = mffArchiveCreateFileUnsafe(archive, outFile, path);
	if (err != MF_ERROR_OKAY)
	{
		mftUnlockMutex(folderArchive->mutex);
		return err;
	}

	err = mftUnlockMutex(folderArchive->mutex);
	if (err != MF_ERROR_OKAY)
		return err;

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

#if defined(MAGMA_FRAMEWORK_USE_WINDOWS_FILESYSTEM)
static mfError mffFillWindowsFiles(mffFolderArchive * archive, void* allocator, const mfsUTF8CodeUnit* path, const mfsUTF8CodeUnit* internalPath, mffFolderFile* parent, mffFolderFile** first)
{
	

	mfError err;
	WIN32_FIND_DATA data;
	HANDLE hFind;

	{
		mfsUTF8CodeUnit searchPath[MFF_PATH_MAX_SIZE];

		mfsStringStream ss;
		err = mfsCreateLocalStringStream(&ss, searchPath, sizeof(searchPath));
		if (err != MF_ERROR_OKAY)
			return err;
		err = mfsPrintFormat(&ss, u8"%s/*.*", path);
		if (err != MF_ERROR_OKAY)
			return err;
		mfsDestroyLocalStringStream(&ss);

		hFind = FindFirstFile(searchPath, &data);
	}


	if (hFind != INVALID_HANDLE_VALUE)
	{
		// Iterate over files in directory
		do
		{
			if (strcmp(data.cFileName, u8".") == 0 ||
				strcmp(data.cFileName, u8"..") == 0)
				continue;

			// If is directory
			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				mffFolderFile* file;

				err = mfmAllocate(allocator, &file, sizeof(mffFolderFile));
				if (err != MF_ERROR_OKAY)
					return err;

				err = mfmInitObject(&file->base.object);
				if (err != MF_ERROR_OKAY)
					return err;

				file->base.object.destructorFunc = &mffDestroyFile;
				file->base.archive = archive;
				file->first = NULL;
				file->next = NULL;
				file->parent = parent;
				file->type = MFF_DIRECTORY;

				mfsStringStream ss;
				
				// Get internal path
				err = mfsCreateLocalStringStream(&ss, file->path, sizeof(file->path));
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfsPrintFormat(&ss, u8"%s/%s", internalPath, data.cFileName);
				if (err != MF_ERROR_OKAY)
					return err;
				mfsDestroyLocalStringStream(&ss);

				// Get real path
				mfsUTF8CodeUnit realPath[MFF_MAX_FILE_PATH_SIZE];
				err = mfsCreateLocalStringStream(&ss, realPath, sizeof(realPath));
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfsPrintFormat(&ss, u8"%s/%s", path, data.cFileName);
				if (err != MF_ERROR_OKAY)
					return err;
				mfsDestroyLocalStringStream(&ss);

				*first = file;
				first = &(*first)->next;

				err = mffFillWindowsFiles(archive, allocator, realPath, file->path, file, &file->first);
				if (err != MF_ERROR_OKAY)
					return err;
			}
			// If is file
			else
			{
				mffFolderFile* file;

				err = mfmAllocate(allocator, &file, sizeof(mffFolderFile));
				if (err != MF_ERROR_OKAY)
					return err;

				err = mfmInitObject(&file->base.object);
				if (err != MF_ERROR_OKAY)
					return err;

				file->base.object.destructorFunc = &mffDestroyFile;
				file->base.archive = archive;
				file->first = NULL;
				file->next = NULL;
				file->parent = parent;
				file->type = MFF_FILE;

				mfsStringStream ss;
				err = mfsCreateLocalStringStream(&ss, file->path, sizeof(file->path));
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfsPrintFormat(&ss, u8"%s/%s", internalPath, data.cFileName);
				if (err != MF_ERROR_OKAY)
					return err;
				mfsDestroyLocalStringStream(&ss);

				*first = file;
				first = &(*first)->next;
			}
		}
		while (FindNextFile(hFind, &data));
	}
}
#endif

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
#if defined(MAGMA_FRAMEWORK_USE_WINDOWS_FILESYSTEM)
	err = mffFillWindowsFiles(folderArchive, allocator, path, u8"", NULL, &folderArchive->firstFile);
	if (err != MF_ERROR_OKAY)
		return err;
#endif

	*outArchive = folderArchive;
	return MF_ERROR_OKAY;
}

void mffDestroyFolderArchive(void * archive)
{
	if (archive == NULL)
		abort();

	mfError err;
	mffFolderArchive* folderArchive = archive;

	err = mftDestroyMutex(folderArchive->mutex);
	if (err != MF_ERROR_OKAY)
		abort();

	err = mfmDestroyObject(&folderArchive->base.object);
	if (err != MF_ERROR_OKAY)
		abort();
	
	err = mfmDeallocate(folderArchive->allocator, folderArchive);
	if (err != MF_ERROR_OKAY)
		abort();
}
