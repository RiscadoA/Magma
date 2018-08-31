#pragma once

#include "Archive.h"

#ifdef __cplusplus
extern "C"
{
#endif

	mfError mffCreateDefaultArchive(mffArchive** archive, const mfsUTF8CodeUnit* path, void* allocator);

	void mffDestroyDefaultArchive(void* archive);

#ifdef __cplusplus
}
#endif
