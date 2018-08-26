#pragma once

#include "Lexer.h"
#include "../../../String/Stream.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct mfvV1XNode mfvV1XNode;

	struct mfvV1XNode
	{
		mfvV1XTokenInfo* info;
		mfsUTF8CodeUnit attribute[MFV_TOKEN_ATTRIBUTE_SIZE];
		mfvV1XNode* first;
		mfvV1XNode* next;
		mfmBool active;
	};

	typedef struct
	{
		mfsUTF8CodeUnit errorMsg[MFV_MAX_ERROR_MESSAGE_SIZE];
		mfmU64 nodeCount;
	} mfvV1XParserState;

#define MFV_V1X_TOKEN_FUNCTION									0x0100
	static const mfvV1XTokenInfo MFV_V1X_TINFO_FUNCTION			= { MFV_V1X_TOKEN_FUNCTION, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"function" };

	mfError mfvV1XRunMVLParser(const mfvV1XToken* tokens, mfvV1XNode* nodeArray, mfmU64 maxNodeCount, const mfvV1XLexerState* lexerState, mfvV1XParserState* state);

	mfError mfvV1XPrintNode(mfsStream* stream, mfvV1XNode* node, mfmU64 indentation);

#ifdef __cplusplus
}
#endif
