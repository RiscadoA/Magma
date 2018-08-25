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

#define SINGLE_CHAR_TOK(chr, tokInfo) else if (*state->it == chr) { tok.info = &tokInfo; err = mfvV1XPutToken(state, &tok); if (err != MF_ERROR_OKAY) return err; ++state->it; return MF_ERROR_OKAY; }

	if (*state->it == '\0')
	{
		state->finished = MFM_TRUE;
		return MF_ERROR_OKAY;
	}

	// Operators
	SINGLE_CHAR_TOK('+', MFV_V1X_TINFO_ADD)
	SINGLE_CHAR_TOK('-', MFV_V1X_TINFO_SUBTRACT)
	SINGLE_CHAR_TOK('*', MFV_V1X_TINFO_MULTIPLY)
	SINGLE_CHAR_TOK('/', MFV_V1X_TINFO_DIVIDE)
	SINGLE_CHAR_TOK('>', MFV_V1X_TINFO_GREATER)
	SINGLE_CHAR_TOK('<', MFV_V1X_TINFO_LESS)
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
	else
	{
		mfsStringStream ss;
		mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFV_MAX_ERROR_MESSAGE_SIZE);
		mfsPrintFormatUTF8(&ss, u8"[mfvV1XReadToken : MFV_ERROR_UNKNOWN_TOKEN] Unknown token found on:\n\"(%s)\"", state->it);
		mfsDestroyLocalStringStream(&ss);
		return MFV_ERROR_UNKNOWN_TOKEN;
	}

#undef SINGLE_CHAR_TOK
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
