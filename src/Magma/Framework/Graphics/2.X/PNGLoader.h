#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "../Error.h"
#include "../../Memory/Type.h"

	typedef struct
	{
		mfmU32 width;
		mfmU32 height;
		mfmU8* data;
		void* allocator;
	} mfgPNGTextureData;

	mfError mfgLoadPNG(const mfmU8* data, mfmU64 size, mfgPNGTextureData** textureData, void* allocator);

	mfError mfgUnloadPNG(mfgPNGTextureData* textureData);

#ifdef __cplusplus
}
#endif
