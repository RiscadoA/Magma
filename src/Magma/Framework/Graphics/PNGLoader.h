#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "Error.h"
#include "../Memory/Type.h"

	typedef struct
	{
		mfmU32 width;
		mfmU32 height;
		mfmU8* data;
		void* allocator;
	} mfgPNGTextureData;

	/// <summary>
	///		Loads a texture from a PNG file.
	/// </summary>
	/// <param name="stream">Input data stream handle</param>
	/// <param name="textureData">Out texture data</param>
	/// <param name="allocator">Allocator to use</param>
	/// <returns>
	///		Returns MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mfgLoadPNG(void* stream, mfgPNGTextureData** textureData, void* allocator);

	/// <summary>
	///		Unloads a texture previously loaded by mfgLoadPNG.
	/// </summary>
	/// <param name="textureData">Texture data pointer</param>
	/// <returns>
	///		Returns MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mfgUnloadPNG(mfgPNGTextureData* textureData);

#ifdef __cplusplus
}
#endif
