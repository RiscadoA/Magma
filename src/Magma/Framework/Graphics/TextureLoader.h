#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "2.X/RenderDevice.h"

#include "Error.h"
#include "../Memory/Object.h"

	typedef struct
	{
		mfmObject object;
		mfmU32 width;
		mfmU32 height;
		mfmU8* data;
		void* allocator;
	} mfgTextureData;

	mfError mfgInitTextureLoader(void);

	mfError mfgTerminateTextureLoader(void);

	/// <summary>
	///		Loads a texture from a PNG file.
	///		The texture data array must be freed later with mfmDeallocate.
	/// </summary>
	/// <param name="stream">Input data stream handle</param>
	/// <param name="textureData">Out texture data</param>
	/// <param name="allocator">Allocator to use</param>
	/// <returns>
	///		Returns MFG_ERROR_OKAY if there were no errors.
	///		Otherwise returns an error code.
	/// </returns>
	mfError mfgLoadTexture(void* stream, mfgTextureData* textureData, mfgEnum format, void* allocator);

#ifdef __cplusplus
}
#endif
