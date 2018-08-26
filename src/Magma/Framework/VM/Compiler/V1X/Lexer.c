#include "Lexer.h"
#include "Config.h"

#include "../../../String/StringStream.h"

typedef struct
{
	mfvV1XLexerState* state;
	const mfsUTF8CodeUnit * it;
	mfvV1XToken* tokens;
	mfmU64 maxTokenCount;
	mfmBool finished;
} mfvV1XLexerInternalState;

static mfmBool mfvV1XIsWhiteSpace(mfsUTF8CodeUnit chr)
{
	if (chr == ' ' || chr == '\n' || chr == '\t' || chr == '\0')
		return MFM_TRUE;
}

static mfmBool mfvV1XIsAlpha(mfsUTF8CodeUnit chr)
{
	if ((chr >= 'a' && chr <= 'z') || (chr >= 'A' && chr <= 'A'))
		return MFM_TRUE;
	return MFM_FALSE;
}

static mfmBool mfvV1XIsNumeric(mfsUTF8CodeUnit chr)
{
	if (chr >= '0' && chr <= '9')
		return MFM_TRUE;
	return MFM_FALSE;
}

static mfError mfvV1XPutToken(mfvV1XLexerInternalState * state, const mfvV1XToken* token)
{
	if (state == NULL)
		return MFV_ERROR_INVALID_ARGUMENTS;
	else if (token == NULL)
	{
		mfsStringStream ss;
		mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFV_MAX_ERROR_MESSAGE_SIZE);
		mfsPutString(&ss, u8"[mfvV1XPutToken : MFV_ERROR_INVALID_ARGUMENTS] Token is NULL");
		mfsDestroyLocalStringStream(&ss);
		return MFV_ERROR_INVALID_ARGUMENTS;
	}
	else if (state->state->tokenCount + 1 >= state->maxTokenCount)
	{
		mfsStringStream ss;
		mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFV_MAX_ERROR_MESSAGE_SIZE);
		mfsPutString(&ss, u8"[mfvV1XPutToken : MFV_ERROR_TOKENS_OVERFLOW] Tokens array is already full (try increasing its size)");
		mfsDestroyLocalStringStream(&ss);
		return MFV_ERROR_TOKENS_OVERFLOW;
	}

	state->tokens[state->state->tokenCount] = *token;
	++state->state->tokenCount;
	return MF_ERROR_OKAY;
}

static mfError mfvV1XReadToken(mfvV1XLexerInternalState* state)
{
	if (state == NULL)
		return MFV_ERROR_INVALID_ARGUMENTS;

	mfError err = MF_ERROR_OKAY;

	mfvV1XToken tok;
	tok.info = NULL;
	tok.attribute[0] = '\0';

	mfmU64 attrIt = 0;

#define SINGLE_CHAR_TOK(chr, tokInfo) else if (*state->it == chr) { tok.info = &tokInfo; err = mfvV1XPutToken(state, &tok); if (err != MF_ERROR_OKAY) return err; ++state->it; return MF_ERROR_OKAY; }

#define MULTI_CHAR_2_TOK(chr, tokInfo) else if (state->it[0] == chr[0] && state->it[1] == chr[1]) { tok.info = &tokInfo; err = mfvV1XPutToken(state, &tok); if (err != MF_ERROR_OKAY) return err; state->it += 2; return MF_ERROR_OKAY; }

#define KEYWORD_TOK(str, tokInfo) if (state->it[0] == str[0]) { \
		attrIt = 0;\
		while (1)\
		{\
			if (str[attrIt] == '\0')\
			{\
				if (mfvV1XIsAlpha(state->it[attrIt]) == MFM_TRUE ||\
					mfvV1XIsNumeric(state->it[attrIt]) == MFM_TRUE)\
					break;\
				tok.info = &tokInfo;\
				err = mfvV1XPutToken(state, &tok);\
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

	if (mfvV1XIsWhiteSpace(*state->it) == MFM_TRUE)
	{
		if (*state->it == '\0')
			state->finished = MFM_TRUE;
		else
			++state->it;
		return MF_ERROR_OKAY;
	}

	// Keywords
	KEYWORD_TOK(u8"void", MFV_V1X_TINFO_VOID)
	KEYWORD_TOK(u8"i8", MFV_V1X_TINFO_I8)
	KEYWORD_TOK(u8"i16", MFV_V1X_TINFO_I16)
	KEYWORD_TOK(u8"i32", MFV_V1X_TINFO_I32)
	KEYWORD_TOK(u8"u8", MFV_V1X_TINFO_U8)
	KEYWORD_TOK(u8"u16", MFV_V1X_TINFO_U16)
	KEYWORD_TOK(u8"u32", MFV_V1X_TINFO_U32)
	KEYWORD_TOK(u8"f32", MFV_V1X_TINFO_F32)
	KEYWORD_TOK(u8"bool", MFV_V1X_TINFO_BOOL)
	KEYWORD_TOK(u8"if", MFV_V1X_TINFO_IF)
	KEYWORD_TOK(u8"else", MFV_V1X_TINFO_ELSE)
	KEYWORD_TOK(u8"while", MFV_V1X_TINFO_WHILE)
	KEYWORD_TOK(u8"for", MFV_V1X_TINFO_FOR)
	KEYWORD_TOK(u8"return", MFV_V1X_TINFO_RETURN)
	KEYWORD_TOK(u8"end", MFV_V1X_TINFO_END)
	KEYWORD_TOK(u8"throw", MFV_V1X_TINFO_THROW)
	KEYWORD_TOK(u8"true", MFV_V1X_TINFO_TRUE)
	KEYWORD_TOK(u8"false", MFV_V1X_TINFO_FALSE)
	KEYWORD_TOK(u8"warning", MFV_V1X_TINFO_WARNING)
	KEYWORD_TOK(u8"error", MFV_V1X_TINFO_ERROR)

	// Operators
	SINGLE_CHAR_TOK('+', MFV_V1X_TINFO_ADD)
	SINGLE_CHAR_TOK('-', MFV_V1X_TINFO_SUBTRACT)
	SINGLE_CHAR_TOK('*', MFV_V1X_TINFO_MULTIPLY)
	SINGLE_CHAR_TOK('/', MFV_V1X_TINFO_DIVIDE)
	MULTI_CHAR_2_TOK("==", MFV_V1X_TINFO_EQUAL)
	MULTI_CHAR_2_TOK("!=", MFV_V1X_TINFO_DIFFERENT)
	MULTI_CHAR_2_TOK(">=", MFV_V1X_TINFO_GEQUAL)
	MULTI_CHAR_2_TOK("<=", MFV_V1X_TINFO_LEQUAL)
	SINGLE_CHAR_TOK('>', MFV_V1X_TINFO_GREATER)
	SINGLE_CHAR_TOK('<', MFV_V1X_TINFO_LESS)
	MULTI_CHAR_2_TOK("<=", MFV_V1X_TINFO_AND)
	MULTI_CHAR_2_TOK("<=", MFV_V1X_TINFO_OR)
	SINGLE_CHAR_TOK('!', MFV_V1X_TINFO_NOT)
	SINGLE_CHAR_TOK('=', MFV_V1X_TINFO_ASSIGN)
	SINGLE_CHAR_TOK('.', MFV_V1X_TINFO_MEMBER)

	// Punctuation
	SINGLE_CHAR_TOK('{', MFV_V1X_TINFO_OPEN_BRACES)
	SINGLE_CHAR_TOK('}', MFV_V1X_TINFO_CLOSE_BRACES)
	SINGLE_CHAR_TOK('[', MFV_V1X_TINFO_OPEN_BRACKETS)
	SINGLE_CHAR_TOK(']', MFV_V1X_TINFO_CLOSE_BRACKETS)
	SINGLE_CHAR_TOK('(', MFV_V1X_TINFO_OPEN_PARENTHESIS)
	SINGLE_CHAR_TOK(')', MFV_V1X_TINFO_CLOSE_PARENTHESIS)
	SINGLE_CHAR_TOK(';', MFV_V1X_TINFO_SEMICOLON)
	SINGLE_CHAR_TOK(':', MFV_V1X_TINFO_COLON)
	SINGLE_CHAR_TOK(',', MFV_V1X_TINFO_COMMA)

	// Float literal
	if (mfvV1XIsNumeric(*state->it) == MFM_TRUE)
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
			else if (mfvV1XIsWhiteSpace(state->it[attrIt]) == MFM_TRUE ||
				(mfvV1XIsNumeric(state->it[attrIt]) == MFM_FALSE &&
					mfvV1XIsAlpha(state->it[attrIt]) == MFM_FALSE))
			{
				if (foundDot == MFM_FALSE)
					break;
				tok.attribute[attrIt] = '\0';
				tok.info = &MFV_V1X_TINFO_FLOAT_LITERAL;
				err = mfvV1XPutToken(state, &tok);
				if (err != MF_ERROR_OKAY)
					return err;
				state->it += attrIt;
				return MF_ERROR_OKAY;
			}
			else if (mfvV1XIsNumeric(state->it[attrIt]) == MFM_FALSE)
				break;
			else
			{
				if (attrIt >= MFV_TOKEN_ATTRIBUTE_SIZE - 1)
				{
					mfsStringStream ss;
					mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFV_MAX_ERROR_MESSAGE_SIZE);
					mfsPrintFormatUTF8(&ss, u8"[mfvV1XReadToken : MFV_ERROR_TOKEN_ATTRIBUTE_TOO_BIG] Float literal token is too big:\n\"(%s)\"", state->it);
					mfsDestroyLocalStringStream(&ss);
					return MFV_ERROR_TOKEN_ATTRIBUTE_TOO_BIG;
				}
			}
		}

		tok.attribute[0] = '\0';
	}

	// Integer literal
	if (mfvV1XIsNumeric(*state->it) == MFM_TRUE)
	{
		attrIt = 0;
		while (1)
		{
			tok.attribute[attrIt] = state->it[attrIt];
			++attrIt;
			if (mfvV1XIsWhiteSpace(state->it[attrIt]) == MFM_TRUE ||
				(mfvV1XIsNumeric(state->it[attrIt]) == MFM_FALSE &&
				 mfvV1XIsAlpha(state->it[attrIt]) == MFM_FALSE))
			{
				tok.attribute[attrIt] = '\0';
				tok.info = &MFV_V1X_TINFO_INT_LITERAL;
				err = mfvV1XPutToken(state, &tok);
				if (err != MF_ERROR_OKAY)
					return err;
				state->it += attrIt;
				return MF_ERROR_OKAY;
			}
			else if (mfvV1XIsNumeric(state->it[attrIt]) == MFM_FALSE)
				break;
			else
			{
				if (attrIt >= MFV_TOKEN_ATTRIBUTE_SIZE - 1)
				{
					mfsStringStream ss;
					mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFV_MAX_ERROR_MESSAGE_SIZE);
					mfsPrintFormatUTF8(&ss, u8"[mfvV1XReadToken : MFV_ERROR_TOKEN_ATTRIBUTE_TOO_BIG] Int literal token is too big:\n\"(%s)\"", state->it);
					mfsDestroyLocalStringStream(&ss);
					return MFV_ERROR_TOKEN_ATTRIBUTE_TOO_BIG;
				}
			}
		}

		tok.attribute[0] = '\0';
	}

	// Identifier
	if (mfvV1XIsAlpha(*state->it) == MFM_TRUE || *state->it == '_')
	{
		attrIt = 0;
		while (1)
		{
			tok.attribute[attrIt] = state->it[attrIt];
			++attrIt;
			if (mfvV1XIsWhiteSpace(state->it[attrIt]) == MFM_TRUE ||
				(mfvV1XIsNumeric(state->it[attrIt]) == MFM_FALSE &&
				 mfvV1XIsAlpha(state->it[attrIt]) == MFM_FALSE &&
				 *state->it != '_'))
			{
				tok.attribute[attrIt] = '\0';
				tok.info = &MFV_V1X_TINFO_IDENTIFIER;
				err = mfvV1XPutToken(state, &tok);
				if (err != MF_ERROR_OKAY)
					return err;
				state->it += attrIt;
				return MF_ERROR_OKAY;
			}
			else if (mfvV1XIsAlpha(state->it[attrIt]) == MFM_FALSE &&
					 mfvV1XIsNumeric(state->it[attrIt]) == MFM_FALSE &&
					 *state->it != '_')
				break;
			else
			{
				if (attrIt >= MFV_TOKEN_ATTRIBUTE_SIZE - 1)
				{
					mfsStringStream ss;
					mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFV_MAX_ERROR_MESSAGE_SIZE);
					mfsPrintFormatUTF8(&ss, u8"[mfvV1XReadToken : MFV_ERROR_TOKEN_ATTRIBUTE_TOO_BIG] Identifier token is too big:\n\"(%s)\"", state->it);
					mfsDestroyLocalStringStream(&ss);
					return MFV_ERROR_TOKEN_ATTRIBUTE_TOO_BIG;
				}
			}
		}

		tok.attribute[0] = '\0';
	}

	// String literal
	if (*state->it == '"')
	{
		attrIt = 1;
		while (1)
		{
			tok.attribute[attrIt - 1] = state->it[attrIt];
			++attrIt;
			if (state->it[attrIt] == '"')
			{
				tok.attribute[attrIt - 1] = '\0';
				tok.info = &MFV_V1X_TINFO_STRING_LITERAL;
				err = mfvV1XPutToken(state, &tok);
				if (err != MF_ERROR_OKAY)
					return err;
				state->it += attrIt + 1;
				return MF_ERROR_OKAY;
			}
			else if (state->it[attrIt] == '\n')
				break;
			else if (state->it[attrIt] == '\\')
			{
				++attrIt;
				tok.attribute[attrIt] = state->it[attrIt];
				++attrIt;
			}
			else
			{
				if (attrIt >= MFV_TOKEN_ATTRIBUTE_SIZE - 1)
				{
					mfsStringStream ss;
					mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFV_MAX_ERROR_MESSAGE_SIZE);
					mfsPrintFormatUTF8(&ss, u8"[mfvV1XReadToken : MFV_ERROR_TOKEN_ATTRIBUTE_TOO_BIG] String literal token is too big:\n\"(%s)\"", state->it);
					mfsDestroyLocalStringStream(&ss);
					return MFV_ERROR_TOKEN_ATTRIBUTE_TOO_BIG;
				}
			}
		}

		tok.attribute[0] = '\0';
	}

	// Unknown token
	{
		mfsStringStream ss;
		mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFV_MAX_ERROR_MESSAGE_SIZE);
		mfsPrintFormatUTF8(&ss, u8"[mfvV1XReadToken : MFV_ERROR_UNKNOWN_TOKEN] Unknown token found on:\n<<%s>>", state->it);
		mfsDestroyLocalStringStream(&ss);
		return MFV_ERROR_UNKNOWN_TOKEN;
	}

#undef SINGLE_CHAR_TOK
#undef MULTI_CHAR_2_TOK
#undef KEYWORD_TOK
}

mfError mfvV1XRunMVLLexer(const mfsUTF8CodeUnit * source, mfvV1XToken * tokens, mfmU64 maxTokenCount, mfvV1XLexerState * state)
{
	if (source == NULL || tokens == NULL || maxTokenCount == 0 || state == NULL)
		return MFV_ERROR_INVALID_ARGUMENTS;

	mfError err;

	mfvV1XLexerInternalState internalState;
	internalState.state = state;
	internalState.it = source;
	internalState.tokens = tokens;
	internalState.maxTokenCount = maxTokenCount;
	internalState.state = state;
	internalState.finished = MFM_FALSE;

	state->tokenCount = 0;

	while (internalState.finished == MFM_FALSE)
	{
		err = mfvV1XReadToken(&internalState);
		if (err != MF_ERROR_OKAY)
			return err;
	}

	return MF_ERROR_OKAY;
}
