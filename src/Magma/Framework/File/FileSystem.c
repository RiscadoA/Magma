#include "FileSystem.h"
#include "Path.h"
#include "Config.h"

#include "../Thread/Mutex.h"

#include <string.h>

struct
{
	mffArchive* archives[MFF_MAX_ARCHIVE_COUNT];
	mftMutex* mutex;
} mffFileSystem;

mfError mffInitFileSystem(void)
{
	mfError err;

	// Init archives
	for (mfmU64 i = 0; i < MFF_MAX_ARCHIVE_COUNT; ++i)
		mffFileSystem.archives[i] = NULL;
	
	// Create mutex
	err = mftCreateMutex(&mffFileSystem.mutex, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

mfError mffTerminateFileSystem(void)
{
	mfError err;
	
	// Release archives
	err = mftLockMutex(mffFileSystem.mutex, 0);
	if (err != MF_ERROR_OKAY)
		return err;	
	for (mfmU64 i = 0; i < MFF_MAX_ARCHIVE_COUNT; ++i)
	{
		if (mffFileSystem.archives[i] == NULL)
			continue;

		err = mfmDecObjectRef(&mffFileSystem.archives[i]->object);
		if (err != MF_ERROR_OKAY)
		{
			mftUnlockMutex(mffFileSystem.mutex);
			return err;
		}
		mffFileSystem.archives[i] = NULL;
	}
	err = mftUnlockMutex(mffFileSystem.mutex);
	if (err != MF_ERROR_OKAY)
		return err;

	// Destroy mutex
	err = mftDestroyMutex(mffFileSystem.mutex);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

mfError mffRegisterArchive(mffArchive * archive)
{
	mfError err;
	
	// Register archive
	err = mftLockMutex(mffFileSystem.mutex, 0);
	if (err != MF_ERROR_OKAY)
		return err;
	for (mfmU64 i = 0; i < MFF_MAX_ARCHIVE_COUNT; ++i)
	{
		if (mffFileSystem.archives[i] != NULL)
			continue;

		mffFileSystem.archives[i] = archive;
		err = mfmIncObjectRef(&mffFileSystem.archives[i]->object);
		if (err != MF_ERROR_OKAY)
		{
			mftUnlockMutex(mffFileSystem.mutex);
			return err;
		}

		err = mftUnlockMutex(mffFileSystem.mutex);
		if (err != MF_ERROR_OKAY)
			return err;

		return MF_ERROR_OKAY;
	}

	err = mftUnlockMutex(mffFileSystem.mutex);
	if (err != MF_ERROR_OKAY)
		return err;

	return MFF_ERROR_ARCHIVE_NOT_FOUND;
}

mfError mffUnregisterArchive(mffArchive * archive)
{
	mfError err;
	
	// Unregister archive
	err = mftLockMutex(mffFileSystem.mutex, 0);
	if (err != MF_ERROR_OKAY)
		return err;
	for (mfmU64 i = 0; i < MFF_MAX_ARCHIVE_COUNT; ++i)
	{
		if (mffFileSystem.archives[i] != archive)
			continue;

		err = mfmDecObjectRef(&mffFileSystem.archives[i]->object);
		mffFileSystem.archives[i] = NULL;
		if (err != MF_ERROR_OKAY)
		{
			mftUnlockMutex(mffFileSystem.mutex);
			return err;
		}

		err = mftUnlockMutex(mffFileSystem.mutex);
		if (err != MF_ERROR_OKAY)
			return err;

		return MF_ERROR_OKAY;
	}

	err = mftUnlockMutex(mffFileSystem.mutex);
	if (err != MF_ERROR_OKAY)
		return err;

	return MFF_ERROR_ARCHIVE_NOT_FOUND;
}

static mfError mffGetPathArchive(mffArchive** outArchive, const mfsUTF8CodeUnit * path)
{
	mfError err;
	mfsUTF8CodeUnit archiveName[MFF_MAX_ARCHIVE_NAME_SIZE] = { '\0' };

	const mfsUTF8CodeUnit* it = path;
	if (*it == '/')
		++it;

	// Get archive name from path
	for (mfmU64 i = 0; *it != '/' && *it != '\0'; ++i, ++it)
	{
		if (i >= MFF_MAX_ARCHIVE_NAME_SIZE - 1)
			return MFF_ERROR_ARCHIVE_NOT_FOUND;
		archiveName[i] = *it;	
	}

	// Search for archive
	err = mftLockMutex(mffFileSystem.mutex, 0);
	if (err != MF_ERROR_OKAY)
		return err;

	for (mfmU64 i = 0; i < MFF_MAX_ARCHIVE_COUNT; ++i)
	{
		if (mffFileSystem.archives[i] == NULL)
			continue;
		if (strcmp(archiveName, mffFileSystem.archives[i]->name) != 0)
			continue;

		// Found archive
		*outArchive = mffFileSystem.archives[i];
		err = mftUnlockMutex(mffFileSystem.mutex);
		if (err != MF_ERROR_OKAY)
			return err;

		return MF_ERROR_OKAY;

	}

	mftUnlockMutex(mffFileSystem.mutex);
	return MFF_ERROR_ARCHIVE_NOT_FOUND;
}

mfError mffGetFile(mffFile ** outFile, const mfsUTF8CodeUnit * path)
{
	if (outFile == NULL || path == NULL)
		return MFF_ERROR_INVALID_ARGUMENTS;

	mfError err;

	mffArchive* archive;
	err = mffGetPathArchive(&archive, path);
	if (err != MF_ERROR_OKAY)
		return err;

	return archive->getFile(archive, outFile, mffGoDownPath(path));
}

mfError mffGetFileType(mffFile * file, mffEnum * outType)
{
	if (file == NULL || outType == NULL)
		return MFF_ERROR_INVALID_ARGUMENTS;

	return file->archive->getFileType(file->archive, file, outType);
}

mfError mffGetFirstFile(mffFile ** outFile, mffDirectory * directory)
{
	if (outFile == NULL || directory == NULL)
		return MFF_ERROR_INVALID_ARGUMENTS;

	return directory->archive->getFirstFile(directory->archive, outFile, directory);
}

mfError mffGetNextFile(mffFile ** outFile, mffFile * file)
{
	if (outFile == NULL || file == NULL)
		return MFF_ERROR_INVALID_ARGUMENTS;

	return file->archive->getNextFile(file->archive, outFile, file);
}

mfError mffGetParent(mffDirectory ** outParent, mffFile * file)
{
	if (outParent == NULL || file == NULL)
		return MFF_ERROR_INVALID_ARGUMENTS;

	return file->archive->getParent(file->archive, outParent, file);
}

mfError mffCreateDirectory(mffDirectory ** outDir, const mfsUTF8CodeUnit * path)
{
	if (outDir == NULL || path == NULL)
		return MFF_ERROR_INVALID_ARGUMENTS;

	mfError err;

	mffArchive* archive;
	err = mffGetPathArchive(&archive, path);
	if (err != MF_ERROR_OKAY)
		return err;

	return archive->createDirectory(archive, outDir, mffGoDownPath(path));
}

mfError mffDeleteDirectory(mffDirectory * dir)
{
	if (dir == NULL)
		return MFF_ERROR_INVALID_ARGUMENTS;

	return dir->archive->deleteDirectory(dir->archive, dir);
}

mfError mffCreateFile(mffFile ** outFile, const mfsUTF8CodeUnit * path)
{
	if (path == NULL)
		return MFF_ERROR_INVALID_ARGUMENTS;

	mfError err;

	mffArchive* archive;
	err = mffGetPathArchive(&archive, path);
	if (err != MF_ERROR_OKAY)
		return err;

	return archive->createFile(archive, outFile, mffGoDownPath(path));
}

mfError mffDeleteFile(mffFile * file)
{
	if (file == NULL)
		return MFF_ERROR_INVALID_ARGUMENTS;

	return file->archive->deleteFile(file->archive, file);
}

mfError mffOpenFile(mfsStream ** outStream, mffFile * file, mffEnum mode)
{
	if (outStream == NULL || file == NULL)
		return MFF_ERROR_INVALID_ARGUMENTS;
	return file->archive->openFile(file->archive, outStream, file, mode);
}

mfError mffCloseFile(mfsStream * stream)
{
	if (stream == NULL)
		return MFF_ERROR_INVALID_ARGUMENTS;
	stream->object.destructorFunc(stream);
	return MF_ERROR_OKAY;
}