#include "FileSystem.hpp"
#include "Exception.hpp"
#include "../ErrorString.h"

void Magma::Framework::File::RegisterArchive(Archive archive, const mfsUTF8CodeUnit* name)
{
	mfError err;
	err = mffRegisterArchive((mffArchive*)&archive.Get(), name);
	if (err != MF_ERROR_OKAY)
		throw FileSystemError(mfErrorToString(err));
}

void Magma::Framework::File::UnregisterArchive(Archive archive)
{
	mfError err;
	err = mffUnregisterArchive((mffArchive*)&archive.Get());
	if (err != MF_ERROR_OKAY)
		throw FileSystemError(mfErrorToString(err));
}

Magma::Framework::File::File Magma::Framework::File::GetFile(const mfsUTF8CodeUnit * path)
{
	mfError err;
	mffFile* file;
	err = mffGetFile(&file, path);
	if (err == MFF_ERROR_FILE_NOT_FOUND)
		return NULL;
	if (err != MF_ERROR_OKAY)
		throw FileSystemError(mfErrorToString(err));
	return file;
}

Magma::Framework::File::File Magma::Framework::File::CreateDirectory(const mfsUTF8CodeUnit * path)
{
	mfError err;
	mffDirectory* dir;
	err = mffCreateDirectory(&dir, path);
	if (err != MF_ERROR_OKAY)
		throw FileSystemError(mfErrorToString(err));
	return dir;
}

void Magma::Framework::File::DeleteDirectory(File & dir)
{
	mfError err;
	mffDirectory* d = (mffDirectory*)dir.GetNoChecks();
	err = mfmIncObjectRef((mfmObject*)d);
	if (err != MF_ERROR_OKAY)
		throw FileSystemError(mfErrorToString(err));
	dir.Release();
	err = mfmDecObjectRef((mfmObject*)d);
	if (err != MF_ERROR_OKAY)
		throw FileSystemError(mfErrorToString(err));
	err = mffDeleteDirectory(d);
	if (err != MF_ERROR_OKAY)
		throw FileSystemError(mfErrorToString(err));
}