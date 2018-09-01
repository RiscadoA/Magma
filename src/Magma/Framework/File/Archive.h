#pragma once

#include "Error.h"
#include "Path.h"

#include "../Memory/Object.h"
#include "../String/Stream.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define MFF_MAX_ARCHIVE_CREATOR_REGISTER_ENTRIES	16
#define MFF_READ	0x01
#define MFF_WRITE	0x02

	typedef struct mffArchive mffArchive;
	typedef struct mffFile mffFile;

	typedef mfError(*mffArchiveOpenFile)(mfsStream** stream, mffFile* file, mfmU32 mode);
	typedef mfError(*mffArchiveCreateFile)(mffArchive* archive, mffFile** file, const mfsUTF8CodeUnit* name);

	struct mffFile
	{
		// Base object.
		mfmObject object;

		// File parent archive.
		mffArchive* archive;

		// Next file in parent archive.
		mffFile* nextFile;

		// File name (including extension).
		mfsUTF8CodeUnit name[MFF_FILE_NAME_MAX_SIZE];

		// File type (MFF_FILE or MFF_ARCHIVE).
		mffEnum type;

		// Allocator used to allocate this file.
		void* allocator;
	};

	struct mffArchive
	{
		// Base file.
		mffFile base;

		// First file in archive.
		mffFile* firstFile;

		// Opens a file.
		mffArchiveOpenFile openFile;

		// Creates a file.
		mffArchiveCreateFile createFile;

		// Archive path (if any).
		mfsUTF8CodeUnit path[MFF_PATH_MAX_SIZE];
	};

	typedef mfError(*mffArchiveCreatorFunction)(mffArchive** archive, const mfsUTF8CodeUnit* path, void* allocator);

	mfError mffInitArchives(void);
	
	void mffTerminateArchives(void);

	/// <summary>
	///		Registers a archive creator.
	/// </summary>
	/// <param name="type">Archive extension name (with a maximum size of 16 bytes)</param>
	/// <param name="func">Archive creator function</param>
	/// <returns>
	///		MF_ERROR_OKAY if there were no errors.
	///		MFF_ERROR_NO_REGISTER_ENTRIES if there are no more creator slots in the register.
	/// </returns>
	mfError mffRegisterArchiveCreator(const mfsUTF8CodeUnit* extension, mffArchiveCreatorFunction func);

	mfError mffCreateArchive(mffArchive** archive, const mfsUTF8CodeUnit* path, void* allocator);

	void mffDestroyArchive(mffArchive* archive);

	mfError mffGetFile(mffArchive* archive, mffFile** file, const mfsUTF8CodeUnit* path);

	mfError mffOpenFile(mfsStream** stream, mffFile* file, mfmU32 mode);

	mfError mffCloseFile(mfsStream* stream);

	mfError mffCreateFile(mffArchive* archive, mffFile** file, const mfsUTF8CodeUnit* name);

#ifdef __cplusplus
}
#endif
