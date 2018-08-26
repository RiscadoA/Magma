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

	mfError mfvV1XRunMVLParser(const mfvV1XToken* tokens, mfvV1XNode* nodeArray, mfmU64 maxNodeCount, const mfvV1XLexerState* lexerState, mfvV1XParserState* state);

	mfError mfvV1XPrintNode(mfsStream* stream, mfvV1XNode* node, mfmU64 indentation);

#ifdef __cplusplus
}
#endif
