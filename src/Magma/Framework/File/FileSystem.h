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

	/// <summary>
	///		Inits the file system library.
	/// </summary>
	/// <returns>Error code</returns>
	mfError mffInitFileSystem(void);

	/// <summary>
	///		Terminates the file system library.
	/// </summary>
	/// <returns>Error code</returns>
	mfError mffTerminateFileSystem(void);

	/// <summary>
	///		Registers an archive in the file system.
	/// </summary>
	/// <param name="archive">Archive to register</param>
	/// <param name="name">Archive name</param>
	/// <returns>Error code</returns>
	mfError mffRegisterArchive(mffArchive* archive, const mfsUTF8CodeUnit* name);

	/// <summary>
	///		Unregisters an archive from the file system.
	/// </summary>
	/// <param name="archive">Archive to unregister</param>
	/// <returns>Error code</returns>
	mfError mffUnregisterArchive(mffArchive* archive);

	/// <summary>
	///		Gets a file from the file system.
	/// </summary>
	/// <param name="outFile">Out file handle</param>
	/// <param name="path">File path</param>
	/// <returns>Error code</returns>
	mfError mffGetFile(mffFile** outFile, const mfsUTF8CodeUnit* path);
	
	/// <summary>
	///		Gets the type of a file handle.
	/// </summary>
	/// <param name="file">File handle</param>
	/// <param name="outType">Out file type</param>
	/// <returns>Error code</returns>
	mfError mffGetFileType(mffFile* file, mffEnum* outType);

	/// <summary>
	///		Gets the first file in a directory.
	/// </summary>
	/// <param name="outFile">Out file handle</param>
	/// <param name="directory">Directory handle</param>
	/// <returns>Error code</returns>
	mfError mffGetFirstFile(mffFile** outFile, mffDirectory* directory);

	/// <summary>
	///		Gets the next file in a directory.
	/// </summary>
	/// <param name="outFile">Out file handle</param>
	/// <param name="file">Previous file in directory</param>
	/// <returns>Error code</returns>
	mfError mffGetNextFile(mffFile** outFile, mffFile* file);

	/// <summary>
	///		Gets the parent directory of a file handle.
	/// </summary>
	/// <param name="outParent">Out parent directory handle</param>
	/// <param name="file">File handle</param> 
	/// <returns>Error code</returns>
	mfError mffGetParent(mffDirectory** outParent, mffFile* file);

	/// <summary>
	///		Creates a new directory.
	/// </summary>
	/// <param name="outDir">Out directory handle</param>
	/// <param name="path">Directory path</param>
	/// <returns>Error code</returns>
	mfError mffCreateDirectory(mffDirectory** outDir, const mfsUTF8CodeUnit* path);

	/// <summary>
	///		Deletes a directory (must be emptied first).
	/// </summary>
	/// <param name="dir">Directory handle</param>
	/// <returns>Error code</returns>
	mfError mffDeleteDirectory(mffDirectory* dir);

	/// <summary>
	///		Creates a new file.
	/// </summary>
	/// <param name="outFile">Out file handle</param>
	/// <param name="path">File path</param>
	/// <returns>Error code</returns>
	mfError mffCreateFile(mffFile** outFile, const mfsUTF8CodeUnit* path);

	/// <summary>
	///		Deletes a file.
	/// </summary>
	/// <param name="file">File handle</param>
	/// <returns>Error code</returns>
	mfError mffDeleteFile(mffFile* file);

	/// <summary>
	///		Opens a file stream.
	/// </summary>
	/// <param name="outStream">Out file stream handle</param>
	/// <param name="file">File handle</param>
	/// <param name="mode">Open mode (MFF_FILE_WRITE or MFF_FILE_READ)</param>
	mfError mffOpenFile(mfsStream** outStream, mffFile* file, mffEnum mode);

	/// <summary>
	///		Closes a file stream.
	/// </summary>
	/// <param name="stream">Stream handle</param>
	/// <returns>Error code</returns>
	mfError mffCloseFile(mfsStream* stream);

#ifdef __cplusplus
}
#endif
