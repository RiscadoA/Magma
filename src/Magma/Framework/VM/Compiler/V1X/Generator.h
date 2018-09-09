#pragma once

#include "Parser.h"
#include "../../Bytecode.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct
	{
		mfsUTF8CodeUnit errorMsg[MFV_MAX_ERROR_MESSAGE_SIZE];
		mfmU64 bytecodeSize;
	} mfvV1XGeneratorState;

#define MFV_V1X_TOKEN_REFERENCE										0x0200
	static const mfvV1XTokenInfo MFG_VVX_TINFO_REFERENCE			= { MFV_V1X_TOKEN_REFERENCE, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"reference" };
#define MFV_V1X_TOKEN_ARRAY_REFERENCE								0x0202
	static const mfvV1XTokenInfo MFV_V1X_TINFO_ARRAY_REFERENCE		= { MFV_V1X_TOKEN_ARRAY_REFERENCE, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"array-reference" };

	mfError mfvV1XRunMVLGenerator(const mfvV1XNode* root, mfmU8* bytecode, mfmU64 maxBytecodeSize, mfvV1XGeneratorState* state);

#ifdef __cplusplus
}
#endif
