#include <Magma/Framework/Graphics/2.X/MSL/Lexer.h>
#include <Magma/Framework/String/StringStream.h>
#include "Config.h"

typedef struct
{
	mfgV2XLexerState* state;
	const mfsUTF8CodeUnit * it;
	mfgV2XToken* tokens;
	mfmU64 maxTokenCount;
	mfmBool finished;
} mfgV2XLexerInternalState;

static mfmBool mfgV2XIsWhiteSpace(mfsUTF8CodeUnit chr)
{
	if (chr == ' ' || chr == '\n' || chr == '\t' || chr == '\0')
		return MFM_TRUE;
}

static mfmBool mfgV2XIsAlpha(mfsUTF8CodeUnit chr)
{
	if ((chr >= 'a' && chr <= 'z') || (chr >= 'A' && chr <= 'Z'))
		return MFM_TRUE;
	return MFM_FALSE;
}

static mfmBool mfgV2XIsNumeric(mfsUTF8CodeUnit chr)
{
	if (chr >= '0' && chr <= '9')
		return MFM_TRUE;
	return MFM_FALSE;
}

static mfError mfgV2XPutToken(mfgV2XLexerInternalState * state, const mfgV2XToken* token)
{
	if (state == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;
	else if (token == NULL)
	{
		mfsStringStream ss;
		mfsCreateLocalStringStream(&ss, (mfmU8*)state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
		mfsPutString((mfsStream*)&ss, u8"[mfgV2XPutToken : MFG_ERROR_INVALID_ARGUMENTS] Token is NULL");
		mfsDestroyLocalStringStream(&ss);
		return MFG_ERROR_INVALID_ARGUMENTS;
	}
	else if (state->state->tokenCount + 1 >= state->maxTokenCount)
	{
		mfsStringStream ss;
		mfsCreateLocalStringStream(&ss, (mfmU8*)state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
		mfsPutString((mfsStream*)&ss, u8"[mfgV2XPutToken : MFG_ERROR_TOKENS_OVERFLOW] Tokens array is already full (try increasing its size)");
		mfsDestroyLocalStringStream(&ss);
		return MFG_ERROR_TOKENS_OVERFLOW;
	}

	state->tokens[state->state->tokenCount] = *token;
	++state->state->tokenCount;
	return MF_ERROR_OKAY;
}

static mfError mfgV2XReadToken(mfgV2XLexerInternalState* state)
{
	if (state == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err = MF_ERROR_OKAY;

	mfgV2XToken tok;
	tok.info = NULL;
	tok.attribute[0] = '\0';

	mfmU64 attrIt = 0;

#define SINGLE_CHAR_TOK(chr, tokInfo) else if (*state->it == chr) { tok.info = &tokInfo; err = mfgV2XPutToken(state, &tok); if (err != MF_ERROR_OKAY) return err; ++state->it; return MF_ERROR_OKAY; }

#define MULTI_CHAR_2_TOK(chr, tokInfo) else if (state->it[0] == chr[0] && state->it[1] == chr[1]) { tok.info = &tokInfo; err = mfgV2XPutToken(state, &tok); if (err != MF_ERROR_OKAY) return err; state->it += 2; return MF_ERROR_OKAY; }

#define KEYWORD_TOK(str, tokInfo) if (state->it[0] == str[0]) { \
		attrIt = 0;\
		while (1)\
		{\
			if (str[attrIt] == '\0')\
			{\
				if (mfgV2XIsAlpha(state->it[attrIt]) == MFM_TRUE ||\
					mfgV2XIsNumeric(state->it[attrIt]) == MFM_TRUE)\
					break;\
				tok.info = &tokInfo;\
				err = mfgV2XPutToken(state, &tok);\
				if (err != MF_ERROR_OKAY)\
					return err;\
				state->it += attrIt;\
				return MF_ERROR_OKAY;\
			}\
			else if (state->it[attrIt] != str[attrIt])\
				break;\
			++attrIt;\
		}\
	}

	if (mfgV2XIsWhiteSpace(*state->it) == MFM_TRUE)
	{
		if (*state->it == '\0')
			state->finished = MFM_TRUE;
		else
			++state->it;
		return MF_ERROR_OKAY;
	}

	// Keywords
	KEYWORD_TOK(u8"void", MFG_V2X_TINFO_VOID)
	KEYWORD_TOK(u8"int2x2", MFG_V2X_TINFO_INT22)
	KEYWORD_TOK(u8"int3x3", MFG_V2X_TINFO_INT33)
	KEYWORD_TOK(u8"int4x4", MFG_V2X_TINFO_INT44)
	KEYWORD_TOK(u8"int1", MFG_V2X_TINFO_INT1)
	KEYWORD_TOK(u8"int2", MFG_V2X_TINFO_INT2)
	KEYWORD_TOK(u8"int3", MFG_V2X_TINFO_INT3)
	KEYWORD_TOK(u8"int4", MFG_V2X_TINFO_INT4)
	KEYWORD_TOK(u8"int", MFG_V2X_TINFO_INT1)
	KEYWORD_TOK(u8"float2x2", MFG_V2X_TINFO_FLOAT22)
	KEYWORD_TOK(u8"float3x3", MFG_V2X_TINFO_FLOAT33)
	KEYWORD_TOK(u8"float4x4", MFG_V2X_TINFO_FLOAT44)
	KEYWORD_TOK(u8"float1", MFG_V2X_TINFO_FLOAT1)
	KEYWORD_TOK(u8"float2", MFG_V2X_TINFO_FLOAT2)
	KEYWORD_TOK(u8"float3", MFG_V2X_TINFO_FLOAT3)
	KEYWORD_TOK(u8"float4", MFG_V2X_TINFO_FLOAT4)
	KEYWORD_TOK(u8"float", MFG_V2X_TINFO_FLOAT1)
	KEYWORD_TOK(u8"bool", MFG_V2X_TINFO_BOOL)
	KEYWORD_TOK(u8"if", MFG_V2X_TINFO_IF)
	KEYWORD_TOK(u8"else", MFG_V2X_TINFO_ELSE)
	KEYWORD_TOK(u8"while", MFG_V2X_TINFO_WHILE)
	KEYWORD_TOK(u8"for", MFG_V2X_TINFO_FOR)
	KEYWORD_TOK(u8"return", MFG_V2X_TINFO_RETURN)
	KEYWORD_TOK(u8"discard", MFG_V2X_TINFO_DISCARD)
	KEYWORD_TOK(u8"true", MFG_V2X_TINFO_TRUE)
	KEYWORD_TOK(u8"false", MFG_V2X_TINFO_FALSE)
	KEYWORD_TOK(u8"Input", MFG_V2X_TINFO_INPUT)
	KEYWORD_TOK(u8"Output", MFG_V2X_TINFO_OUTPUT)
	KEYWORD_TOK(u8"ConstantBuffer", MFG_V2X_TINFO_CONSTANT_BUFFER)
	KEYWORD_TOK(u8"Texture1D", MFG_V2X_TINFO_TEXTURE_1D)
	KEYWORD_TOK(u8"Texture2D", MFG_V2X_TINFO_TEXTURE_2D)
	KEYWORD_TOK(u8"Texture3D", MFG_V2X_TINFO_TEXTURE_3D)

	// Operators
	SINGLE_CHAR_TOK('+', MFG_V2X_TINFO_ADD)
	SINGLE_CHAR_TOK('-', MFG_V2X_TINFO_SUBTRACT)
	SINGLE_CHAR_TOK('*', MFG_V2X_TINFO_MULTIPLY)
	SINGLE_CHAR_TOK('/', MFG_V2X_TINFO_DIVIDE)
	MULTI_CHAR_2_TOK("==", MFG_V2X_TINFO_EQUAL)
	MULTI_CHAR_2_TOK("!=", MFG_V2X_TINFO_DIFFERENT)
	MULTI_CHAR_2_TOK(">=", MFG_V2X_TINFO_GEQUAL)
	MULTI_CHAR_2_TOK("<=", MFG_V2X_TINFO_LEQUAL)
	SINGLE_CHAR_TOK('>', MFG_V2X_TINFO_GREATER)
	SINGLE_CHAR_TOK('<', MFG_V2X_TINFO_LESS)
	MULTI_CHAR_2_TOK("&&", MFG_V2X_TINFO_AND)
	MULTI_CHAR_2_TOK("||", MFG_V2X_TINFO_OR)
	SINGLE_CHAR_TOK('!', MFG_V2X_TINFO_NOT)
	SINGLE_CHAR_TOK('=', MFG_V2X_TINFO_ASSIGN)
	SINGLE_CHAR_TOK('.', MFG_V2X_TINFO_MEMBER)

	// Punctuation
	SINGLE_CHAR_TOK('{', MFG_V2X_TINFO_OPEN_BRACES)
	SINGLE_CHAR_TOK('}', MFG_V2X_TINFO_CLOSE_BRACES)
	SINGLE_CHAR_TOK('[', MFG_V2X_TINFO_OPEN_BRACKETS)
	SINGLE_CHAR_TOK(']', MFG_V2X_TINFO_CLOSE_BRACKETS)
	SINGLE_CHAR_TOK('(', MFG_V2X_TINFO_OPEN_PARENTHESIS)
	SINGLE_CHAR_TOK(')', MFG_V2X_TINFO_CLOSE_PARENTHESIS)
	SINGLE_CHAR_TOK(';', MFG_V2X_TINFO_SEMICOLON)
	SINGLE_CHAR_TOK(':', MFG_V2X_TINFO_COLON)
	SINGLE_CHAR_TOK(',', MFG_V2X_TINFO_COMMA)

	// Float literal
	if (mfgV2XIsNumeric(*state->it) == MFM_TRUE)
	{
		mfmBool foundDot = MFM_FALSE;
		attrIt = 0;
		while (1)
		{
			tok.attribute[attrIt] = state->it[attrIt];
			++attrIt;
			if (state->it[attrIt] == '.')
			{
				if (foundDot == MFM_FALSE)
					foundDot = MFM_TRUE;
				else
					break;
			}
			else if (mfgV2XIsWhiteSpace(state->it[attrIt]) == MFM_TRUE ||
				(mfgV2XIsNumeric(state->it[attrIt]) == MFM_FALSE &&
					mfgV2XIsAlpha(state->it[attrIt]) == MFM_FALSE) ||
					 state->it[attrIt] == 'f')
			{
				if (foundDot == MFM_FALSE)
					break;
				tok.attribute[attrIt] = '\0';
				if (state->it[attrIt] == 'f')
					++attrIt;
				tok.info = &MFG_V2X_TINFO_FLOAT_LITERAL;
				err = mfgV2XPutToken(state, &tok);
				if (err != MF_ERROR_OKAY)
					return err;
				state->it += attrIt;
				return MF_ERROR_OKAY;
			}
			else if (mfgV2XIsNumeric(state->it[attrIt]) == MFM_FALSE)
				break;
			else
			{
				if (attrIt >= MFG_V2X_TOKEN_ATTRIBUTE_SIZE - 1)
				{
					mfsStringStream ss;
					mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
					mfsPrintFormat(&ss, u8"[mfgV2XReadToken : MFG_ERROR_TOKEN_ATTRIBUTE_TOO_BIG] Float literal token is too big:\n\"(%s)\"", state->it);
					mfsDestroyLocalStringStream(&ss);
					return MFG_ERROR_TOKEN_ATTRIBUTE_TOO_BIG;
				}
			}
		}

		tok.attribute[0] = '\0';
	}

	// Integer literal
	if (mfgV2XIsNumeric(*state->it) == MFM_TRUE)
	{
		attrIt = 0;
		while (1)
		{
			tok.attribute[attrIt] = state->it[attrIt];
			++attrIt;
			if (mfgV2XIsWhiteSpace(state->it[attrIt]) == MFM_TRUE ||
				(mfgV2XIsNumeric(state->it[attrIt]) == MFM_FALSE &&
				 mfgV2XIsAlpha(state->it[attrIt]) == MFM_FALSE))
			{
				tok.attribute[attrIt] = '\0';
				tok.info = &MFG_V2X_TINFO_INT_LITERAL;
				err = mfgV2XPutToken(state, &tok);
				if (err != MF_ERROR_OKAY)
					return err;
				state->it += attrIt;
				return MF_ERROR_OKAY;
			}
			else if (mfgV2XIsNumeric(state->it[attrIt]) == MFM_FALSE)
				break;
			else
			{
				if (attrIt >= MFG_V2X_TOKEN_ATTRIBUTE_SIZE - 1)
				{
					mfsStringStream ss;
					mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
					mfsPrintFormat(&ss, u8"[mfgV2XReadToken : MFG_ERROR_TOKEN_ATTRIBUTE_TOO_BIG] Int literal token is too big:\n\"(%s)\"", state->it);
					mfsDestroyLocalStringStream(&ss);
					return MFG_ERROR_TOKEN_ATTRIBUTE_TOO_BIG;
				}
			}
		}

		tok.attribute[0] = '\0';
	}

	// Identifier
	if (mfgV2XIsAlpha(*state->it) == MFM_TRUE || *state->it == '_')
	{
		attrIt = 0;
		while (1)
		{
			tok.attribute[attrIt] = state->it[attrIt];
			++attrIt;
			if (mfgV2XIsWhiteSpace(state->it[attrIt]) == MFM_TRUE ||
				(mfgV2XIsNumeric(state->it[attrIt]) == MFM_FALSE &&
				 mfgV2XIsAlpha(state->it[attrIt]) == MFM_FALSE &&
				 state->it[attrIt] != '_'))
			{
				tok.attribute[attrIt] = '\0';
				tok.info = &MFG_V2X_TINFO_IDENTIFIER;
				err = mfgV2XPutToken(state, &tok);
				if (err != MF_ERROR_OKAY)
					return err;
				state->it += attrIt;
				return MF_ERROR_OKAY;
			}
			else if (mfgV2XIsAlpha(state->it[attrIt]) == MFM_FALSE &&
					 mfgV2XIsNumeric(state->it[attrIt]) == MFM_FALSE &&
					 state->it[attrIt] != '_')
				break;
			else
			{
				if (attrIt >= MFG_V2X_TOKEN_ATTRIBUTE_SIZE - 1)
				{
					mfsStringStream ss;
					mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
					mfsPrintFormat(&ss, u8"[mfgV2XReadToken : MFG_ERROR_TOKEN_ATTRIBUTE_TOO_BIG] Identifier token is too big:\n\"(%s)\"", state->it);
					mfsDestroyLocalStringStream(&ss);
					return MFG_ERROR_TOKEN_ATTRIBUTE_TOO_BIG;
				}
			}
		}

		tok.attribute[0] = '\0';
	}

	// Unknown token
	{
		mfsStringStream ss;
		mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
		mfsPrintFormat(&ss, u8"[mfgV2XReadToken : MFG_ERROR_UNKNOWN_TOKEN] Unknown token found on:\n<<%s>>", state->it);
		mfsDestroyLocalStringStream(&ss);
		return MFG_ERROR_UNKNOWN_TOKEN;
	}

#undef SINGLE_CHAR_TOK
#undef MULTI_CHAR_2_TOK
#undef KEYWORD_TOK
}

mfError mfgV2XRunMVLLexer(const mfsUTF8CodeUnit * source, mfgV2XToken * tokens, mfmU64 maxTokenCount, mfgV2XLexerState * state)
{
	if (source == NULL || tokens == NULL || maxTokenCount == 0 || state == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;

	mfgV2XLexerInternalState internalState;
	internalState.state = state;
	internalState.it = source;
	internalState.tokens = tokens;
	internalState.maxTokenCount = maxTokenCount;
	internalState.state = state;
	internalState.finished = MFM_FALSE;

	state->tokenCount = 0;

	while (internalState.finished == MFM_FALSE)
	{
		err = mfgV2XReadToken(&internalState);
		if (err != MF_ERROR_OKAY)
			return err;
	}

	return MF_ERROR_OKAY;
}
