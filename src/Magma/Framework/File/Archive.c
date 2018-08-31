#include "Archive.h"
#include "DefaultArchive.h"
#include "Config.h"

#include "../String/StringStream.h"
#include "../Memory/Allocator.h"

#include <string.h>
#include <stdlib.h>

struct
{
	mfmBool active;
	mffArchiveCreatorFunction func;
	mfsUTF8CodeUnit extension[16];
} mffArchiveCreatorRegisterEntries[MFF_MAX_ARCHIVE_CREATOR_REGISTER_ENTRIES];

mfError mffInitArchives(void)
{
	// Init all entries

	for (mfmU64 i = 0; i < MFF_MAX_ARCHIVE_CREATOR_REGISTER_ENTRIES; ++i)
	{
		mffArchiveCreatorRegisterEntries[i].active = MFM_FALSE;
		mffArchiveCreatorRegisterEntries[i].func = NULL;
	}

	return mffRegisterArchiveCreator(NULL, &mffCreateDefaultArchive);
}

void mffTerminateArchives(void)
{
	// Reset all entries

	for (mfmU64 i = 0; i < MFF_MAX_ARCHIVE_CREATOR_REGISTER_ENTRIES; ++i)
	{
		mffArchiveCreatorRegisterEntries[i].active = MFM_FALSE;
		mffArchiveCreatorRegisterEntries[i].func = NULL;
	}
}

mfError mffRegisterArchiveCreator(const mfsUTF8CodeUnit * extension, mffArchiveCreatorFunction func)
{
	if (func == NULL)
		return MFF_ERROR_INVALID_ARGUMENTS;

	for (mfmU64 i = 0; i < MFF_MAX_ARCHIVE_CREATOR_REGISTER_ENTRIES; ++i)
		if (mffArchiveCreatorRegisterEntries[i].active == MFM_FALSE)
		{
			mffArchiveCreatorRegisterEntries[i].active = MFM_TRUE;
			mffArchiveCreatorRegisterEntries[i].func = func;

			if (extension == NULL)
				mffArchiveCreatorRegisterEntries[i].extension[0] = '\0';
			else
				for (mfmU32 j = 0; j < 16; ++j)
				{
					mffArchiveCreatorRegisterEntries[i].extension[j] = extension[j];
					if (extension[j] == '\0')
						break;
				}

			return MF_ERROR_OKAY;
		}

	return MFF_ERROR_NO_REGISTER_ENTRIES;
}

mfError mffCreateArchive(mffArchive ** archive, const mfsUTF8CodeUnit * path, void* allocator)
{
	if (archive == NULL || path == NULL)
		return MFF_ERROR_INVALID_ARGUMENTS;

	for (mfmU64 i = 0; i < MFF_MAX_ARCHIVE_CREATOR_REGISTER_ENTRIES; ++i)
		if (mffArchiveCreatorRegisterEntries[i].active == MFM_TRUE)
		{
			// Check if extension matches
			const mfsUTF8CodeUnit* ext = mffGetPathExtension(path);
			if (ext == NULL && mffArchiveCreatorRegisterEntries[i].extension[0] != '\0')
				continue;
			else if (ext != NULL && strcmp(mffArchiveCreatorRegisterEntries[i].extension, ext) != 0)
				continue;
			return mffArchiveCreatorRegisterEntries[i].func(archive, path, allocator);
		}
			
	return MFF_ERROR_TYPE_NOT_REGISTERED;
}

void mffDestroyArchive(mffArchive * archive)
{
	((mfmObject*)archive)->destructorFunc(archive);
}

mfError mffGetFile(mffArchive * archive, mffFile ** file, const mfsUTF8CodeUnit * path)
{
	if (archive == NULL || file == NULL || path == NULL)
		return MFF_ERROR_INVALID_ARGUMENTS;

	if (*path == '/')
		++path;

	mffFile* f = archive->firstFile;
	while (f != NULL)
	{
		const mfsUTF8CodeUnit* it1 = path;
		const mfsUTF8CodeUnit* it2 = f->name;
		for (;;)
		{
			if (*it1 == *it2)
			{
				if (*it1 == '\0')
				{
					*file = f;
					return MF_ERROR_OKAY;
				}
			}
			else if (*it1 == '/' || *it2 == '\0')
				return mffGetFile(f, file, mffGoDownPath(path));
			else
				break;

			++it1;
			++it2;
		}
		f = f->nextFile;
	}

	return MFF_ERROR_FILE_NOT_FOUND;
}

mfError mffOpenFile(mfsStream ** stream, mffFile * file, mfmU32 mode)
{
	if (file == NULL)
		return MFF_ERROR_INVALID_ARGUMENTS;
	return file->archive->openFile(stream, file, mode);
}

mfError mffCloseFile(mfsStream * stream)
{
	if (stream == NULL)
		return MFF_ERROR_INVALID_ARGUMENTS;
	stream->object.destructorFunc(stream);
	return MF_ERROR_OKAY;
}

mfError mffCreateFile(mffArchive * archive, mffFile ** file, const mfsUTF8CodeUnit * name)
{
	return archive->createFile(archive, file, name);
}
