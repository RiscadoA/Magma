#pragma once

#include "Internal.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct
	{
		mfmU64 bytecodeSize;
		mfmU64 metaDataSize;
		mfsUTF8CodeUnit errorMsg[MFG_V2X_MAX_ERROR_MESSAGE_SIZE];
	} mfgV2XMVLCompilerInfo;

	/// <summary>
	///		Compiles MSL code into MSL bytecode.
	/// </summary>
	/// <param name="msl">MSL source code</param>
	/// <param name="bytecode">Out bytecode array</param>
	/// <param name="maxBytecodeSize">Bytecode array size</param>
	/// <param name="metaData">Out meta data array</param>
	/// <param name="maxMetaDataSize">Meta data array size</param>
	/// <param name="shaderType">Shader type</param>
	/// <param name="info">Compiler info</param>
	mfError mfgV2XRunMSLCompiler(const mfsUTF8CodeUnit* msl, mfmU8* bytecode, mfmU64 maxBytecodeSize, mfmU8* metaData, mfmU64 maxMetaDataSize, mfgV2XEnum shaderType, mfgV2XMVLCompilerInfo* info);

#ifdef __cplusplus
}
#endif
