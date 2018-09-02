#include "FolderArchive.hpp"
#include "FolderArchive.h"
#include "Exception.hpp"
#include "../ErrorString.h"

Magma::Framework::File::Archive Magma::Framework::File::CreateFolderArchive(const mfsUTF8CodeUnit* path, Memory::Allocator allocator)
{
	mffArchive* archive = NULL;
	mfError err;
	err = mffCreateFolderArchive(&archive, allocator.GetNoChecks(), path);
	if (err != MF_ERROR_OKAY)
		throw ArchiveError(mfErrorToString(err));
	return archive;
}