#pragma once

#include "Lexer.h"

#include "../../../String/Stream.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct mfgV2XNode mfgV2XNode;


#define MFG_V2X_STACK_FRAME_MAX_VARS 32
#define MFG_V2X_MAX_STACK_FRAMES 64

	typedef struct mfgV2XStackFrame mfgV2XStackFrame;

	struct mfgV2XStackFrame
	{
		mfgV2XStackFrame* parent;
		mfgV2XCompilerMSLVariable localVariables[MFG_V2X_STACK_FRAME_MAX_VARS];
	};

	typedef struct
	{
		mfmU16 varIndex;
		mfmU16 cmpIndex;
		mfgV2XEnum type;
		mfmU16 cmpVarID;
	} mfgV2XReference;
	
	struct mfgV2XNode
	{
		mfgV2XTokenInfo* info;
		mfsUTF8CodeUnit attribute[MFG_V2X_TOKEN_ATTRIBUTE_SIZE];
		mfgV2XNode* first;
		mfgV2XNode* next;
		mfmBool active;
		mfgV2XEnum returnType;
		mfmBool isConstant;
		mfmBool isLValue;
		mfgV2XReference ref;
		mfgV2XStackFrame* stackFrame;
	};

	typedef struct
	{
		mfsUTF8CodeUnit errorMsg[MFG_V2X_MAX_ERROR_MESSAGE_SIZE];
		mfmU64 nodeCount;
	} mfgV2XParserState;

#define MFG_V2X_TOKEN_FUNCTION											0x0100
	static const mfgV2XTokenInfo MFG_V2X_TINFO_FUNCTION					= { MFG_V2X_TOKEN_FUNCTION, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"function" };
#define MFG_V2X_TOKEN_PARAMS_DECL										0x0101
	static const mfgV2XTokenInfo MFG_V2X_TINFO_PARAMS_DECL				= { MFG_V2X_TOKEN_PARAMS_DECL, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"params-declaration" };
#define MFG_V2X_TOKEN_COMPOUND_STATEMENT								0x0102
	static const mfgV2XTokenInfo MFG_V2X_TINFO_COMPOUND_STATEMENT		= { MFG_V2X_TOKEN_COMPOUND_STATEMENT, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"compound-statement" };
#define MFG_V2X_TOKEN_DECLARATION_STATEMENT								0x0103
	static const mfgV2XTokenInfo MFG_V2X_TINFO_DECLARATION_STATEMENT	= { MFG_V2X_TOKEN_DECLARATION_STATEMENT, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"declaration-statement" };
#define MFG_V2X_TOKEN_CALL												0x0104
	static const mfgV2XTokenInfo MFG_V2X_TINFO_CALL						 = { MFG_V2X_TOKEN_CALL, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"call" };
#define MFG_V2X_TOKEN_CALL_PARAMS										0x0105
	static const mfgV2XTokenInfo MFG_V2X_TINFO_CALL_PARAMS				= { MFG_V2X_TOKEN_CALL_PARAMS, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"call-params" };
#define MFG_V2X_TOKEN_ARRAY_ACCESS										0x0106
	static const mfgV2XTokenInfo MFG_V2X_TINFO_ARRAY_ACCESS				= { MFG_V2X_TOKEN_ARRAY_ACCESS, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"array-access" };
#define MFG_V2X_TOKEN_CONSTRUCTOR										0x0107
	static const mfgV2XTokenInfo MFG_V2X_TINFO_CONSTRUCTOR				= { MFG_V2X_TOKEN_CONSTRUCTOR, MFM_FALSE, MFM_FALSE, MFM_FALSE, u8"constructor" };

	mfError mfgV2XRunMVLParser(const mfgV2XToken* tokens, mfgV2XNode* nodeArray, mfmU64 maxNodeCount, const mfgV2XLexerState* lexerState, mfgV2XParserState* state, mfgV2XCompilerState* compilerState);

	mfError mfgV2XPrintNode(mfsStream* stream, mfgV2XNode* node, mfmU64 indentation);

#ifdef __cplusplus
}
#endif
