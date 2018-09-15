#include <Magma/Framework/File/FileSystem.hpp>
#include <Magma/Framework/File/Exception.hpp>
#include <Magma/Framework/ErrorString.h>

void Magma::Framework::File::RegisterArchive(HArchive archive, const mfsUTF8CodeUnit* name)
{
	mfError err;
	err = mffRegisterArchive((mffArchive*)&archive.Get(), name);
	if (err != MF_ERROR_OKAY)
		throw FileSystemError(mfErrorToString(err));
}

void Magma::Framework::File::UnregisterArchive(HArchive archive)
{
	mfError err;
	err = mffUnregisterArchive((mffArchive*)&archive.Get());
	if (err != MF_ERROR_OKAY)
		throw FileSystemError(mfErrorToString(err));
}

Magma::Framework::File::FileType Magma::Framework::File::HFile::GetType()
{
	mfError err;
	mffEnum type;
	err = mffGetFileType((mffFile*)&this->Get(), &type);
	if (err != MF_ERROR_OKAY)
		throw FileSystemError(mfErrorToString(err));
	return static_cast<FileType>(type);
}

Magma::Framework::File::HFile Magma::Framework::File::HFile::GetFirst()
{
	mfError err;
	mffFile* file;
	err = mffGetFirstFile(&file, (mffFile*)&this->Get());
	if (err != MF_ERROR_OKAY)
		throw FileSystemError(mfErrorToString(err));
	return file;
}

Magma::Framework::File::HFile Magma::Framework::File::HFile::GetNext()
{
	mfError err;
	mffFile* file;
	err = mffGetNextFile(&file, (mffFile*)&this->Get());
	if (err != MF_ERROR_OKAY)
		throw FileSystemError(mfErrorToString(err));
	return file;
}

Magma::Framework::File::HFile Magma::Framework::File::HFile::GetParent()
{
	mfError err;
	mffFile* file;
	err = mffGetParent(&file, (mffFile*)&this->Get());
	if (err != MF_ERROR_OKAY)
		throw FileSystemError(mfErrorToString(err));
	return file;
}

Magma::Framework::File::HFile Magma::Framework::File::GetFile(const mfsUTF8CodeUnit * path)
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

Magma::Framework::File::HFile Magma::Framework::File::CreateDirectory(const mfsUTF8CodeUnit * path)
{
	mfError err;
	mffDirectory* dir;
	err = mffCreateDirectory(&dir, path);
	if (err != MF_ERROR_OKAY && err != MFF_ERROR_ALREADY_EXISTS)
		throw FileSystemError(mfErrorToString(err));
	return dir;
}

void Magma::Framework::File::DeleteDirectory(HFile & dir)
{
	mfError err;
	mffDirectory* d = (mffDirectory*)dir.GetNoChecks();
	dir.Release();
	err = mffDeleteDirectory(d);
	if (err != MF_ERROR_OKAY)
		throw FileSystemError(mfErrorToString(err));
}

Magma::Framework::File::HFile Magma::Framework::File::CreateFile(const mfsUTF8CodeUnit * path)
{
	mfError err;
	mffFile* f;
	err = mffCreateFile(&f, path);
	if (err != MF_ERROR_OKAY && err != MFF_ERROR_ALREADY_EXISTS)
		throw FileSystemError(mfErrorToString(err));
	return f;
}

void Magma::Framework::File::DeleteFile(HFile & file)
{
	mfError err;
	mffFile* f = (mffFile*)file.GetNoChecks();
	file.Release();
	err = mffDeleteFile(f);
	if (err != MF_ERROR_OKAY)
		throw FileSystemError(mfErrorToString(err));
}

Magma::Framework::String::HStream Magma::Framework::File::OpenFile(HFile file, FileMode mode)
{
	mfError err;
	mfsStream* stream;
	err = mffOpenFile(&stream, (mffFile*)&file.Get(), static_cast<mffEnum>(mode));
	if (err != MF_ERROR_OKAY)
		throw FileSystemError(mfErrorToString(err));
	return stream;
}