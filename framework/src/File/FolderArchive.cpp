#include <Magma/Framework/File/FolderArchive.hpp>
#include <Magma/Framework/File/FolderArchive.h>
#include <Magma/Framework/File/Exception.hpp>
#include <Magma/Framework/ErrorString.h>

Magma::Framework::File::HArchive Magma::Framework::File::CreateFolderArchive(const mfsUTF8CodeUnit* path, Memory::HAllocator allocator)
{
	mffArchive* archive = NULL;
	mfError err;
	err = mffCreateFolderArchive(&archive, allocator.GetNoChecks(), path);
	if (err != MF_ERROR_OKAY)
		throw ArchiveError(mfErrorToString(err));
	return archive;
}