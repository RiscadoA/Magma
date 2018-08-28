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

#define MFG_V2X_TOKEN_REFERENCE										0x0200
	static const mfgV2XTokenInfo MFG_V2X_TINFO_REFERENCE			= { MFG_V2X_TOKEN_REFERENCE, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"reference" };
#define MFG_V2X_TOKEN_CMP_REFERENCE									0x0201
	static const mfgV2XTokenInfo MFG_V2X_TINFO_CMP_REFERENCE		= { MFG_V2X_TOKEN_CMP_REFERENCE, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"cmp-reference" };
#define MFG_V2X_TOKEN_ARRAY_REFERENCE								0x0202
	static const mfgV2XTokenInfo MFG_V2X_TINFO_ARRAY_REFERENCE		= { MFG_V2X_TOKEN_ARRAY_REFERENCE, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"array-reference" };

	mfError mfgV2XRunMVLGenerator(const mfgV2XNode* root, mfmU8* bytecode, mfmU64 maxBytecodeSize, mfmU8* metaData, mfmU64 maxMetaDataSize, mfgV2XGeneratorState* state, const mfgV2XCompilerState* compilerState, mfgV2XEnum shaderType);

#ifdef __cplusplus
}
#endif
