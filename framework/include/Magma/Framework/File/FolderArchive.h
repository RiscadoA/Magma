#pragma once

#include "FileSystem.h"

#ifdef __cplusplus
extern "C"
{
#endif

	mfError mffCreateFolderArchive(mffArchive** outArchive, void* allocator, const mfsUTF8CodeUnit* path);

	void mffDestroyFolderArchive(void* archive);

#ifdef __cplusplus
}
#endif
