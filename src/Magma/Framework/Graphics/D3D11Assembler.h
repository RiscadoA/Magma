#pragma once

#include "../String/Stream.h"
#include "Bytecode.h"
#include "Error.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/// <summary>
	/// 	Assembles binary bytecode and meta data into a HLSL shader.
	/// </summary>
	mfgError mfgD3D11Assemble(const mfmU8* bytecode, mfmU64 bytecodeSize, const mfgMetaData* metaData, mfsStream* outputStream);

#ifdef __cplusplus
}
#endif
