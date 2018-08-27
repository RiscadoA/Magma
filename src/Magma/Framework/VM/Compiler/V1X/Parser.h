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

#define MFV_V1X_TOKEN_FUNCTION											0x0100
	static const mfvV1XTokenInfo MFV_V1X_TINFO_FUNCTION					= { MFV_V1X_TOKEN_FUNCTION, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"function" };
#define MFV_V1X_TOKEN_PARAMS_DECL										0x0101
	static const mfvV1XTokenInfo MFV_V1X_TINFO_PARAMS_DECL				= { MFV_V1X_TOKEN_PARAMS_DECL, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"params-declaration" };
#define MFV_V1X_TOKEN_COMPOUND_STATEMENT								0x0102
	static const mfvV1XTokenInfo MFV_V1X_TINFO_COMPOUND_STATEMENT		= { MFV_V1X_TOKEN_COMPOUND_STATEMENT, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"compound-statement" };
#define MFV_V1X_TOKEN_DECLARATION_STATEMENT								0x0103
	static const mfvV1XTokenInfo MFV_V1X_TINFO_DECLARATION_STATEMENT	= { MFV_V1X_TOKEN_DECLARATION_STATEMENT, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"declaration-statement" };
#define MFV_V1X_TOKEN_CALL												0x0104
	static const mfvV1XTokenInfo MFV_V1X_TINFO_CALL						 = { MFV_V1X_TOKEN_CALL, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"call" };
#define MFV_V1X_TOKEN_CALL_PARAMS										0x0105
	static const mfvV1XTokenInfo MFV_V1X_TINFO_CALL_PARAMS				= { MFV_V1X_TOKEN_CALL_PARAMS, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"call-params" };

	mfError mfvV1XRunMVLParser(const mfvV1XToken* tokens, mfvV1XNode* nodeArray, mfmU64 maxNodeCount, const mfvV1XLexerState* lexerState, mfvV1XParserState* state);

	mfError mfvV1XPrintNode(mfsStream* stream, mfvV1XNode* node, mfmU64 indentation);

#ifdef __cplusplus
}
#endif
