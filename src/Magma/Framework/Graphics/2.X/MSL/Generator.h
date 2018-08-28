#pragma once

#include "Parser.h"
#include "../Bytecode.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct
	{
		mfsUTF8CodeUnit errorMsg[MFG_V2X_MAX_ERROR_MESSAGE_SIZE];
		mfmU64 bytecodeSize;
		mfmU64 metaDataSize;
	} mfgV2XGeneratorState;

	mfError mfgV2XRunMVLGenerator(const mfgV2XNode* root, mfmU8* bytecode, mfmU64 maxBytecodeSize, mfmU8* metaData, mfmU64 maxMetaDataSize, mfgV2XGeneratorState* state, const mfgV2XCompilerState* compilerState, mfgV2XEnum shaderType);

#ifdef __cplusplus
}
#endif
