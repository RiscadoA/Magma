#include "Parser.h"
#include "../../../String/StringStream.h"

#include <string.h>

typedef struct
{
	mfvV1XParserState* state;
	const mfvV1XToken* it;
	const mfvV1XToken* lastToken;
	mfvV1XNode* nodes;
	mfmU64 maxNodeCount;
} mfvV1XParserInternalState;

static mfError mfvV1XGetNode(mfvV1XParserInternalState* state, mfvV1XNode** node)
{
	if (state == NULL || node == NULL)
		return MFV_ERROR_INVALID_ARGUMENTS;
	for (mfmU64 i = 0; i < state->maxNodeCount; ++i)
		if (state->nodes[i].active == MFM_FALSE)
		{
			state->nodes[i].first = NULL;
			state->nodes[i].next = NULL;
			state->nodes[i].info = NULL;
			state->nodes[i].attribute[0] = '\0';
			state->nodes[i].active = MFM_TRUE;
			++state->state->nodeCount;
			*node = &state->nodes[i];
			return MF_ERROR_OKAY;
		}
	return MFV_ERROR_NODES_OVERFLOW;
}

static mfError mfvV1XReleaseNode(mfvV1XParserInternalState* state, mfvV1XNode* node)
{
	if (state == NULL || node == NULL)
		return MFV_ERROR_INVALID_ARGUMENTS;
	node->active = MFM_FALSE;
	--state->state->nodeCount;
	return MF_ERROR_OKAY;
}

static mfError mfvAddToNode(mfvV1XNode* parent, mfvV1XNode* child)
{
	if (parent == NULL || child == NULL)
		return MFV_ERROR_INVALID_ARGUMENTS;

	mfvV1XNode* c = parent->first;
	if (c == NULL)
	{
		parent->first = child;
		child->next = NULL;
	}
	else
	{
		while (c->next != NULL)
			c = c->next;
		c->next = child;
		child->next = NULL;
	}

	return MF_ERROR_OKAY;
}

static mfError mfvExpectTokenType(mfvV1XParserInternalState* state, const mfvV1XTokenInfo* info, const mfvV1XToken** out)
{
	if (state == NULL)
		return MFV_ERROR_INVALID_ARGUMENTS;
	if (state->it > state->lastToken)
	{
		mfsStringStream ss;
		mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFV_MAX_ERROR_MESSAGE_SIZE);
		mfsPrintFormatUTF8(&ss, u8"[mfvExpectTokenType : MFV_ERROR_UNEXPECTED_EOF] Unexpected end of file, expected token type '%s'", info->name);
		mfsDestroyLocalStringStream(&ss);
		return MFV_ERROR_UNEXPECTED_EOF;
	}

	if (state->it->info->type != info->type)
	{
		mfsStringStream ss;
		mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFV_MAX_ERROR_MESSAGE_SIZE);
		mfsPrintFormatUTF8(&ss, u8"[mfvExpectTokenType : MFV_ERROR_UNEXPECTED_TOKEN] Unexpected token type '%s', expected '%s'", state->it->info->name, info->name);
		mfsDestroyLocalStringStream(&ss);
		return MFV_ERROR_UNEXPECTED_TOKEN;
	}

	if (out != NULL)
		*out = state->it;
	++state->it;

	return MF_ERROR_OKAY;
}

static mfError mfvExpectType(mfvV1XParserInternalState* state, const mfvV1XToken** out)
{
	if (state == NULL)
		return MFV_ERROR_INVALID_ARGUMENTS;
	if (state->it > state->lastToken)
	{
		mfsStringStream ss;
		mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFV_MAX_ERROR_MESSAGE_SIZE);
		mfsPrintFormatUTF8(&ss, u8"[mfvExpectType : MFV_ERROR_UNEXPECTED_EOF] Unexpected end of file, expected type");
		mfsDestroyLocalStringStream(&ss);
		return MFV_ERROR_UNEXPECTED_EOF;
	}

	if (state->it->info->isType == MFM_FALSE)
	{
		mfsStringStream ss;
		mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFV_MAX_ERROR_MESSAGE_SIZE);
		mfsPrintFormatUTF8(&ss, u8"[mfvExpectType : MFV_ERROR_UNEXPECTED_TOKEN] Unexpected token type '%s', expected type", state->it->info->name);
		mfsDestroyLocalStringStream(&ss);
		return MFV_ERROR_UNEXPECTED_TOKEN;
	}

	if (out != NULL)
		*out = state->it;
	++state->it;

	return MF_ERROR_OKAY;
}

static mfmBool mfvAcceptTokenType(mfvV1XParserInternalState* state, const mfvV1XTokenInfo* info, const mfvV1XToken** out)
{
	if (state == NULL)
		return MFM_FALSE;
	if (state->it > state->lastToken)
	{
		mfsStringStream ss;
		mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFV_MAX_ERROR_MESSAGE_SIZE);
		mfsPrintFormatUTF8(&ss, u8"[mfvAcceptTokenType : MFV_ERROR_UNEXPECTED_EOF] Unexpected end of file, expected token type '%s'", info->name);
		mfsDestroyLocalStringStream(&ss);
		return MFM_FALSE;
	}

	if (state->it->info->type != info->type)
	{
		if (out != NULL)
			*out = NULL;
		return MFM_FALSE;
	}

	if (out != NULL)
		*out = state->it;
	++state->it;

	return MFM_TRUE;
}

static mfError mfvPeekToken(mfvV1XParserInternalState* state, const mfvV1XToken** out)
{
	if (state == NULL || out == NULL)
		return MFV_ERROR_INVALID_ARGUMENTS;
	if (state->it > state->lastToken)
		*out = NULL;
	else
		*out = state->it;
	return MF_ERROR_OKAY;
}

static mfError mfvNextToken(mfvV1XParserInternalState* state)
{
	if (state == NULL)
		return MFV_ERROR_INVALID_ARGUMENTS;
	if (state->it > state->lastToken)
		return MFV_ERROR_UNEXPECTED_EOF;
	++state->it;
	return MF_ERROR_OKAY;
}

static mfError mfvParseExpression(mfvV1XParserInternalState* state, mfvV1XNode** outNode);

static mfError mfvParseOperatorLast(mfvV1XParserInternalState* state, mfvV1XNode** outNode)
{
	if (state == NULL || outNode == NULL)
		return MFV_ERROR_INVALID_ARGUMENTS;

	mfError err;
	const mfvV1XToken* tok = NULL;

	// ( <exp )
	if (mfvAcceptTokenType(state, &MFV_V1X_TINFO_OPEN_PARENTHESIS, NULL) == MFM_TRUE)
	{
		err = mfvParseExpression(state, outNode);
		if (err != MF_ERROR_OKAY)
			return err;
		err = mfvExpectTokenType(state, &MFV_V1X_TINFO_CLOSE_PARENTHESIS, NULL);
		if (err != MF_ERROR_OKAY)
			return err;
		return MF_ERROR_OKAY;
	}

	// <id> | <call>
	else if (mfvAcceptTokenType(state, &MFV_V1X_TINFO_IDENTIFIER, &tok) == MFM_TRUE)
	{
		// <call>
		if (mfvAcceptTokenType(state, &MFV_V1X_TINFO_OPEN_PARENTHESIS, NULL) == MFM_TRUE)
		{
			// Create identifier node
			mfvV1XNode* idNode = NULL;
			err = mfvV1XGetNode(state, &idNode);
			if (err != MF_ERROR_OKAY)
				return err;
			idNode->info = tok->info;
			strcpy(idNode->attribute, tok->attribute);

			// Create call node
			err = mfvV1XGetNode(state, outNode);
			if (err != MF_ERROR_OKAY)
				return err;
			(*outNode)->info = &MFV_V1X_TINFO_CALL;
			err = mfvAddToNode(*outNode, idNode);
			if (err != MF_ERROR_OKAY)
				return err;

			// Create call params node
			mfvV1XNode* params = NULL;
			err = mfvV1XGetNode(state, &params);
			if (err != MF_ERROR_OKAY)
				return err;
			params->info = &MFV_V1X_TINFO_CALL_PARAMS;
			err = mfvAddToNode(*outNode, params);
			if (err != MF_ERROR_OKAY)
				return err;

			// Add params to call params node
			mfvV1XNode* param = NULL;
			if (mfvAcceptTokenType(state, &MFV_V1X_TINFO_CLOSE_PARENTHESIS, NULL) == MFM_FALSE)
			{
				mfmU64 i = 0;
				for (;;)
				{
					err = mfvParseExpression(state, &param);
					if (err != MF_ERROR_OKAY)
						return err;
					if (param == NULL)
					{
						mfsStringStream ss;
						mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFV_MAX_ERROR_MESSAGE_SIZE);
						mfsPrintFormatUTF8(&ss, u8"[mfvParseOperatorLast] Failed to parse function call param %d", i);
						mfsDestroyLocalStringStream(&ss);
						return MFV_ERROR_FAILED_TO_PARSE;
					}

					err = mfvAddToNode(params, param);
					if (err != MF_ERROR_OKAY)
						return err;

					if (mfvAcceptTokenType(state, &MFV_V1X_TINFO_COMMA, NULL) == MFM_FALSE)
						break;

					++i;
				}


				err = mfvExpectTokenType(state, &MFV_V1X_TINFO_CLOSE_PARENTHESIS, NULL);
				if (err != MF_ERROR_OKAY)
					return err;
			}
			
			return MF_ERROR_OKAY;
		}
		// <id>
		else
		{
			err = mfvV1XGetNode(state, outNode);
			if (err != MF_ERROR_OKAY)
				return err;
			(*outNode)->info = tok->info;
			strcpy((*outNode)->attribute, tok->attribute);
			return MF_ERROR_OKAY;
		}
	}

	// <int-literal> | <float-literal> | <string-literal> | <true> | <false>
	else if (mfvAcceptTokenType(state, &MFV_V1X_TINFO_INT_LITERAL, &tok) == MFM_TRUE ||
			 mfvAcceptTokenType(state, &MFV_V1X_TINFO_FLOAT_LITERAL, &tok) == MFM_TRUE ||
			 mfvAcceptTokenType(state, &MFV_V1X_TINFO_STRING_LITERAL, &tok) == MFM_TRUE ||
			 mfvAcceptTokenType(state, &MFV_V1X_TINFO_TRUE, &tok) == MFM_TRUE ||
			 mfvAcceptTokenType(state, &MFV_V1X_TINFO_FALSE, &tok) == MFM_TRUE)
	{
		err = mfvV1XGetNode(state, outNode);
		if (err != MF_ERROR_OKAY)
			return err;
		(*outNode)->info = tok->info;
		strcpy((*outNode)->attribute, tok->attribute);
		return MF_ERROR_OKAY;
	}

	*outNode = NULL;
	return MF_ERROR_OKAY;
}

static mfError mfvParseOperator5(mfvV1XParserInternalState* state, mfvV1XNode** outNode)
{
	if (state == NULL || outNode == NULL)
		return MFV_ERROR_INVALID_ARGUMENTS;

	mfError err;

	
	mfvV1XToken* tok = NULL;

	// <unary-op>
	if (mfvAcceptTokenType(state, &MFV_V1X_TINFO_ADD, &tok) == MFM_TRUE ||
		mfvAcceptTokenType(state, &MFV_V1X_TINFO_SUBTRACT, &tok) == MFM_TRUE ||
		mfvAcceptTokenType(state, &MFV_V1X_TINFO_NOT, &tok) == MFM_TRUE)
	{
		// Create operator node
		mfvV1XNode* op = NULL;
		err = mfvV1XGetNode(state, &op);
		if (err != MF_ERROR_OKAY)
			return err;
		op->info = tok->info;

		// Next token
		err = mfvNextToken(state);
		if (err != MF_ERROR_OKAY)
			return err;

		// Parse term
		mfvV1XNode* term;
		err = mfvParseOperator5(state, &term);
		if (err != MF_ERROR_OKAY)
			return err;
		if (term == NULL)
		{
			mfsStringStream ss;
			mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFV_MAX_ERROR_MESSAGE_SIZE);
			mfsPrintFormatUTF8(&ss, u8"[mfvParseOperator5] Failed to parse '%s' unary operator term", tok->info->name);
			mfsDestroyLocalStringStream(&ss);
			return MFV_ERROR_FAILED_TO_PARSE;
		}

		// Add term to operator node
		err = mfvAddToNode(op, term);
		if (err != MF_ERROR_OKAY)
			return err;
		*outNode = term;
		return MF_ERROR_OKAY;
	}

	// <member-op>
	// Get first term
	mfvV1XNode* term1 = NULL;
	err = mfvParseOperatorLast(state, &term1);
	if (err != MF_ERROR_OKAY)
		return err;
	if (term1 == NULL)
	{
		// No first term, no expression
		*outNode = NULL;
		return MF_ERROR_OKAY;
	}

	// Get terms
	mfvV1XNode* term2 = NULL;
	for (;;)
	{
		// Check operator token
		err = mfvPeekToken(state, &tok);
		if (err != MF_ERROR_OKAY)
			return err;
		if (tok == NULL ||
			tok->info->type != MFV_V1X_TOKEN_MEMBER)
			break;

		// Create operator node
		mfvV1XNode* op = NULL;
		err = mfvV1XGetNode(state, &op);
		if (err != MF_ERROR_OKAY)
			return err;
		op->info = tok->info;

		// Next token
		err = mfvNextToken(state);
		if (err != MF_ERROR_OKAY)
			return err;

		// Parse second term
		err = mfvParseOperatorLast(state, &term2);
		if (err != MF_ERROR_OKAY)
			return err;
		if (term2 == NULL)
		{
			mfsStringStream ss;
			mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFV_MAX_ERROR_MESSAGE_SIZE);
			mfsPrintFormatUTF8(&ss, u8"[mfvParseOperator5] Failed to parse '%s' operator second term", tok->info->name);
			mfsDestroyLocalStringStream(&ss);
			return MFV_ERROR_FAILED_TO_PARSE;
		}

		// Add terms to operator node
		err = mfvAddToNode(op, term1);
		if (err != MF_ERROR_OKAY)
			return err;
		err = mfvAddToNode(op, term2);
		if (err != MF_ERROR_OKAY)
			return err;
		term1 = op;
	}

	*outNode = term1;
	return MF_ERROR_OKAY;
}


static mfError mfvParseOperator4(mfvV1XParserInternalState* state, mfvV1XNode** outNode)
{
	if (state == NULL || outNode == NULL)
		return MFV_ERROR_INVALID_ARGUMENTS;

	mfError err;

	// Get first term
	mfvV1XNode* term1 = NULL;
	err = mfvParseOperator5(state, &term1);
	if (err != MF_ERROR_OKAY)
		return err;
	if (term1 == NULL)
	{
		// No first term, no expression
		*outNode = NULL;
		return MF_ERROR_OKAY;
	}

	// Get terms
	mfvV1XNode* term2 = NULL;
	mfvV1XToken* tok = NULL;
	for (;;)
	{
		// Check operator token
		err = mfvPeekToken(state, &tok);
		if (err != MF_ERROR_OKAY)
			return err;
		if (tok == NULL || (
			tok->info->type != MFV_V1X_TOKEN_ADD &&
			tok->info->type != MFV_V1X_TOKEN_SUBTRACT))
			break;

		// Create operator node
		mfvV1XNode* op = NULL;
		err = mfvV1XGetNode(state, &op);
		if (err != MF_ERROR_OKAY)
			return err;
		op->info = tok->info;

		// Next token
		err = mfvNextToken(state);
		if (err != MF_ERROR_OKAY)
			return err;

		// Parse second term
		err = mfvParseOperator5(state, &term2);
		if (err != MF_ERROR_OKAY)
			return err;
		if (term2 == NULL)
		{
			mfsStringStream ss;
			mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFV_MAX_ERROR_MESSAGE_SIZE);
			mfsPrintFormatUTF8(&ss, u8"[mfvParseOperator4] Failed to parse '%s' operator second term", tok->info->name);
			mfsDestroyLocalStringStream(&ss);
			return MFV_ERROR_FAILED_TO_PARSE;
		}

		// Add terms to operator node
		err = mfvAddToNode(op, term1);
		if (err != MF_ERROR_OKAY)
			return err;
		err = mfvAddToNode(op, term2);
		if (err != MF_ERROR_OKAY)
			return err;
		term1 = op;
	}

	*outNode = term1;
	return MF_ERROR_OKAY;
}


static mfError mfvParseOperator3(mfvV1XParserInternalState* state, mfvV1XNode** outNode)
{
	if (state == NULL || outNode == NULL)
		return MFV_ERROR_INVALID_ARGUMENTS;

	mfError err;

	// Get first term
	mfvV1XNode* term1 = NULL;
	err = mfvParseOperator4(state, &term1);
	if (err != MF_ERROR_OKAY)
		return err;
	if (term1 == NULL)
	{
		// No first term, no expression
		*outNode = NULL;
		return MF_ERROR_OKAY;
	}

	// Get terms
	mfvV1XNode* term2 = NULL;
	mfvV1XToken* tok = NULL;
	for (;;)
	{
		// Check operator token
		err = mfvPeekToken(state, &tok);
		if (err != MF_ERROR_OKAY)
			return err;
		if (tok == NULL || (
			tok->info->type != MFV_V1X_TOKEN_MULTIPLY &&
			tok->info->type != MFV_V1X_TOKEN_DIVIDE))
			break;

		// Create operator node
		mfvV1XNode* op = NULL;
		err = mfvV1XGetNode(state, &op);
		if (err != MF_ERROR_OKAY)
			return err;
		op->info = tok->info;

		// Next token
		err = mfvNextToken(state);
		if (err != MF_ERROR_OKAY)
			return err;

		// Parse second term
		err = mfvParseOperator4(state, &term2);
		if (err != MF_ERROR_OKAY)
			return err;
		if (term2 == NULL)
		{
			mfsStringStream ss;
			mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFV_MAX_ERROR_MESSAGE_SIZE);
			mfsPrintFormatUTF8(&ss, u8"[mfvParseOperator3] Failed to parse '%s' operator second term", tok->info->name);
			mfsDestroyLocalStringStream(&ss);
			return MFV_ERROR_FAILED_TO_PARSE;
		}

		// Add terms to operator node
		err = mfvAddToNode(op, term1);
		if (err != MF_ERROR_OKAY)
			return err;
		err = mfvAddToNode(op, term2);
		if (err != MF_ERROR_OKAY)
			return err;
		term1 = op;
	}

	*outNode = term1;
	return MF_ERROR_OKAY;
}


static mfError mfvParseOperator2(mfvV1XParserInternalState* state, mfvV1XNode** outNode)
{
	if (state == NULL || outNode == NULL)
		return MFV_ERROR_INVALID_ARGUMENTS;

	mfError err;

	// Get first term
	mfvV1XNode* term1 = NULL;
	err = mfvParseOperator3(state, &term1);
	if (err != MF_ERROR_OKAY)
		return err;
	if (term1 == NULL)
	{
		// No first term, no expression
		*outNode = NULL;
		return MF_ERROR_OKAY;
	}

	// Get terms
	mfvV1XNode* term2 = NULL;
	mfvV1XToken* tok = NULL;
	for (;;)
	{
		// Check operator token
		err = mfvPeekToken(state, &tok);
		if (err != MF_ERROR_OKAY)
			return err;
		if (tok == NULL || (
			tok->info->type != MFV_V1X_TOKEN_EQUAL &&
			tok->info->type != MFV_V1X_TOKEN_DIFFERENT &&
			tok->info->type != MFV_V1X_TOKEN_GREATER &&
			tok->info->type != MFV_V1X_TOKEN_LESS &&
			tok->info->type != MFV_V1X_TOKEN_GEQUAL &&
			tok->info->type != MFV_V1X_TOKEN_LEQUAL))
			break;

		// Create operator node
		mfvV1XNode* op = NULL;
		err = mfvV1XGetNode(state, &op);
		if (err != MF_ERROR_OKAY)
			return err;
		op->info = tok->info;

		// Next token
		err = mfvNextToken(state);
		if (err != MF_ERROR_OKAY)
			return err;

		// Parse second term
		err = mfvParseOperator3(state, &term2);
		if (err != MF_ERROR_OKAY)
			return err;
		if (term2 == NULL)
		{
			mfsStringStream ss;
			mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFV_MAX_ERROR_MESSAGE_SIZE);
			mfsPrintFormatUTF8(&ss, u8"[mfvParseOperator2] Failed to parse '%s' operator second term", tok->info->name);
			mfsDestroyLocalStringStream(&ss);
			return MFV_ERROR_FAILED_TO_PARSE;
		}

		// Add terms to operator node
		err = mfvAddToNode(op, term1);
		if (err != MF_ERROR_OKAY)
			return err;
		err = mfvAddToNode(op, term2);
		if (err != MF_ERROR_OKAY)
			return err;
		term1 = op;
	}

	*outNode = term1;
	return MF_ERROR_OKAY;
}


static mfError mfvParseOperator1(mfvV1XParserInternalState* state, mfvV1XNode** outNode)
{
	if (state == NULL || outNode == NULL)
		return MFV_ERROR_INVALID_ARGUMENTS;

	mfError err;

	// Get first term
	mfvV1XNode* term1 = NULL;
	err = mfvParseOperator2(state, &term1);
	if (err != MF_ERROR_OKAY)
		return err;
	if (term1 == NULL)
	{
		// No first term, no expression
		*outNode = NULL;
		return MF_ERROR_OKAY;
	}

	// Get terms
	mfvV1XNode* term2 = NULL;
	mfvV1XToken* tok = NULL;
	for (;;)
	{
		// Check operator token
		err = mfvPeekToken(state, &tok);
		if (err != MF_ERROR_OKAY)
			return err;
		if (tok == NULL || (
			tok->info->type != MFV_V1X_TOKEN_AND &&
			tok->info->type != MFV_V1X_TOKEN_OR))
			break;

		// Create operator node
		mfvV1XNode* op = NULL;
		err = mfvV1XGetNode(state, &op);
		if (err != MF_ERROR_OKAY)
			return err;
		op->info = tok->info;

		// Next token
		err = mfvNextToken(state);
		if (err != MF_ERROR_OKAY)
			return err;

		// Parse second term
		err = mfvParseOperator2(state, &term2);
		if (err != MF_ERROR_OKAY)
			return err;
		if (term2 == NULL)
		{
			mfsStringStream ss;
			mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFV_MAX_ERROR_MESSAGE_SIZE);
			mfsPrintFormatUTF8(&ss, u8"[mfvParseOperator1] Failed to parse '%s' operator second term", tok->info->name);
			mfsDestroyLocalStringStream(&ss);
			return MFV_ERROR_FAILED_TO_PARSE;
		}

		// Add terms to operator node
		err = mfvAddToNode(op, term1);
		if (err != MF_ERROR_OKAY)
			return err;
		err = mfvAddToNode(op, term2);
		if (err != MF_ERROR_OKAY)
			return err;
		term1 = op;
	}

	*outNode = term1;
	return MF_ERROR_OKAY;
}

static mfError mfvParseExpression(mfvV1XParserInternalState* state, mfvV1XNode** outNode)
{
	if (state == NULL || outNode == NULL)
		return MFV_ERROR_INVALID_ARGUMENTS;

	mfError err;

	// Get first term
	mfvV1XNode* term1 = NULL;
	err = mfvParseOperator1(state, &term1);
	if (err != MF_ERROR_OKAY)
		return err;
	if (term1 == NULL)
	{
		// No first term, no expression
		*outNode = NULL;
		return MF_ERROR_OKAY;
	}

	// Get terms
	mfvV1XNode* term2 = NULL;
	mfvV1XToken* tok = NULL;
	for (;;)
	{
		// Check operator token
		err = mfvPeekToken(state, &tok);
		if (err != MF_ERROR_OKAY)
			return err;
		if (tok == NULL || (
			tok->info->type != MFV_V1X_TOKEN_ASSIGN))
			break;

		// Create operator node
		mfvV1XNode* op = NULL;
		err = mfvV1XGetNode(state, &op);
		if (err != MF_ERROR_OKAY)
			return err;
		op->info = tok->info;

		// Next token
		err = mfvNextToken(state);
		if (err != MF_ERROR_OKAY)
			return err;

		// Parse second term
		err = mfvParseOperator1(state, &term2);
		if (err != MF_ERROR_OKAY)
			return err;
		if (term2 == NULL)
		{
			mfsStringStream ss;
			mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFV_MAX_ERROR_MESSAGE_SIZE);
			mfsPrintFormatUTF8(&ss, u8"[mfvParseOperator1] Failed to parse '%s' operator second term", tok->info->name);
			mfsDestroyLocalStringStream(&ss);
			return MFV_ERROR_FAILED_TO_PARSE;
		}

		// Add terms to operator node
		err = mfvAddToNode(op, term1);
		if (err != MF_ERROR_OKAY)
			return err;
		err = mfvAddToNode(op, term2);
		if (err != MF_ERROR_OKAY)
			return err;
		term1 = op;
	}

	*outNode = term1;
	return MF_ERROR_OKAY;
}

static mfError mfvParseStatement(mfvV1XParserInternalState* state, mfvV1XNode** outNode);

static mfError mfvParseExpressionStatement(mfvV1XParserInternalState* state, mfvV1XNode** outNode)
{
	if (state == NULL || outNode == NULL)
		return MFV_ERROR_INVALID_ARGUMENTS;

	mfError err;

	err = mfvParseExpression(state, outNode);
	if (err != MF_ERROR_OKAY)
		return err;
	if (*outNode == NULL)
		return MF_ERROR_OKAY;

	err = mfvExpectTokenType(state, &MFV_V1X_TINFO_SEMICOLON, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

static mfError mfvParseCompoundStatement(mfvV1XParserInternalState* state, mfvV1XNode** outNode)
{
	if (state == NULL || outNode == NULL)
		return MFV_ERROR_INVALID_ARGUMENTS;

	if (mfvAcceptTokenType(state, &MFV_V1X_TINFO_OPEN_BRACES, NULL) == MFM_FALSE)
	{
		*outNode = NULL;
		return MF_ERROR_OKAY;
	}

	mfError err = mfvV1XGetNode(state, outNode);
	if (err != MF_ERROR_OKAY)
		return err;
	(*outNode)->info = &MFV_V1X_TINFO_COMPOUND_STATEMENT;

	// Get statements
	mfvV1XNode* statement = NULL;
	for (;;)
	{
		err = mfvParseStatement(state, &statement);
		if (err != MF_ERROR_OKAY)
			return err;
		if (statement == NULL)
			break;
		err = mfvAddToNode(*outNode, statement);
		if (err != MF_ERROR_OKAY)
			return err;
	}

	err = mfvExpectTokenType(state, &MFV_V1X_TINFO_CLOSE_BRACES, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

static mfError mfvParseThrowStatement(mfvV1XParserInternalState* state, mfvV1XNode** outNode)
{
	if (state == NULL || outNode == NULL)
		return MFV_ERROR_INVALID_ARGUMENTS;

	mfError err;

	if (mfvAcceptTokenType(state, &MFV_V1X_TINFO_THROW, NULL) == MFM_FALSE)
	{
		*outNode = NULL;
		return MF_ERROR_OKAY;
	}

	err = mfvV1XGetNode(state, outNode);
	if (err != MF_ERROR_OKAY)
		return err;
	(*outNode)->info = &MFV_V1X_TINFO_THROW;

	mfvV1XToken* tok = NULL;
	if (mfvAcceptTokenType(state, &MFV_V1X_TINFO_WARNING, &tok) == MFM_TRUE ||
		mfvAcceptTokenType(state, &MFV_V1X_TINFO_ERROR, &tok) == MFM_TRUE)
	{
		mfvV1XNode* type = NULL;
		err = mfvV1XGetNode(state, &type);
		if (err != MF_ERROR_OKAY)
			return err;
		type->info = tok->info;
		err = mfvAddToNode(*outNode, type);
	}
	else
	{
		mfvV1XToken* tok;
		err = mfvPeekToken(state, &tok);
		if (err != MF_ERROR_OKAY)
			return err;

		mfsStringStream ss;
		mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFV_MAX_ERROR_MESSAGE_SIZE);
		mfsPrintFormatUTF8(&ss, u8"[mfvParseThrowStatement] Failed to parse throw statement, unexpected throw type '%s'", tok->info->name);
		mfsDestroyLocalStringStream(&ss);
		return MFV_ERROR_FAILED_TO_PARSE;
	}

	err = mfvExpectTokenType(state, &MFV_V1X_TINFO_OPEN_PARENTHESIS, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	mfvV1XNode* exp = NULL;
	err = mfvParseExpression(state, &exp);
	if (err != MF_ERROR_OKAY)
		return err;
	if (exp != NULL)
	{
		err = mfvAddToNode(*outNode, exp);
		if (err != MF_ERROR_OKAY)
			return err;
	}

	err = mfvExpectTokenType(state, &MFV_V1X_TINFO_CLOSE_PARENTHESIS, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	err = mfvExpectTokenType(state, &MFV_V1X_TINFO_SEMICOLON, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

static mfError mfvParseReturnStatement(mfvV1XParserInternalState* state, mfvV1XNode** outNode)
{
	if (state == NULL || outNode == NULL)
		return MFV_ERROR_INVALID_ARGUMENTS;

	mfError err;

	if (mfvAcceptTokenType(state, &MFV_V1X_TINFO_RETURN, NULL) == MFM_FALSE)
	{
		*outNode = NULL;
		return MF_ERROR_OKAY;
	}

	err = mfvV1XGetNode(state, outNode);
	if (err != MF_ERROR_OKAY)
		return err;
	(*outNode)->info = &MFV_V1X_TINFO_RETURN;

	mfvV1XNode* exp = NULL;
	err = mfvParseExpression(state, &exp);
	if (err != MF_ERROR_OKAY)
		return err;
	if (exp != NULL)
	{
		err = mfvAddToNode(*outNode, exp);
		if (err != MF_ERROR_OKAY)
			return err;
	}

	err = mfvExpectTokenType(state, &MFV_V1X_TINFO_SEMICOLON, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

static mfError mfvParseEndStatement(mfvV1XParserInternalState* state, mfvV1XNode** outNode)
{
	if (state == NULL || outNode == NULL)
		return MFV_ERROR_INVALID_ARGUMENTS;

	mfError err;

	if (mfvAcceptTokenType(state, &MFV_V1X_TINFO_END, NULL) == MFM_FALSE)
	{
		*outNode = NULL;
		return MF_ERROR_OKAY;
	}

	err = mfvV1XGetNode(state, outNode);
	if (err != MF_ERROR_OKAY)
		return err;
	(*outNode)->info = &MFV_V1X_TINFO_END;

	err = mfvExpectTokenType(state, &MFV_V1X_TINFO_SEMICOLON, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

static mfError mfvParseYieldStatement(mfvV1XParserInternalState* state, mfvV1XNode** outNode)
{
	if (state == NULL || outNode == NULL)
		return MFV_ERROR_INVALID_ARGUMENTS;

	mfError err;

	if (mfvAcceptTokenType(state, &MFV_V1X_TINFO_YIELD, NULL) == MFM_FALSE)
	{
		*outNode = NULL;
		return MF_ERROR_OKAY;
	}

	err = mfvV1XGetNode(state, outNode);
	if (err != MF_ERROR_OKAY)
		return err;
	(*outNode)->info = &MFV_V1X_TINFO_YIELD;

	err = mfvExpectTokenType(state, &MFV_V1X_TINFO_SEMICOLON, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

static mfError mfvParseStatement(mfvV1XParserInternalState* state, mfvV1XNode** outNode)
{
	if (state == NULL || outNode == NULL)
		return MFV_ERROR_INVALID_ARGUMENTS;

	mfError err;
	
	// Check if it is a compound statement
	err = mfvParseCompoundStatement(state, outNode);
	if (err != MF_ERROR_OKAY)
		return err;
	if (*outNode != NULL)
		return MF_ERROR_OKAY;

	// Check if it is an expression statement
	err = mfvParseExpressionStatement(state, outNode);
	if (err != MF_ERROR_OKAY)
		return err;
	if (*outNode != NULL)
		return MF_ERROR_OKAY;

	// Check if it is a throw statement
	err = mfvParseThrowStatement(state, outNode);
	if (err != MF_ERROR_OKAY)
		return err;
	if (*outNode != NULL)
		return MF_ERROR_OKAY;

	// Check if it is a return statement
	err = mfvParseReturnStatement(state, outNode);
	if (err != MF_ERROR_OKAY)
		return err;
	if (*outNode != NULL)
		return MF_ERROR_OKAY;

	// Check if it is a end statement
	err = mfvParseEndStatement(state, outNode);
	if (err != MF_ERROR_OKAY)
		return err;
	if (*outNode != NULL)
		return MF_ERROR_OKAY;

	// Check if it is a yield statement
	err = mfvParseYieldStatement(state, outNode);
	if (err != MF_ERROR_OKAY)
		return err;
	if (*outNode != NULL)
		return MF_ERROR_OKAY;

	// Not a statement
	*outNode = NULL;
	return MF_ERROR_OKAY;
}

static mfError mfvParseFunction(mfvV1XParserInternalState* state, mfvV1XNode** outNode)
{
	if (state == NULL || outNode == NULL)
		return MFV_ERROR_INVALID_ARGUMENTS;

	mfError err;
	err = mfvV1XGetNode(state, outNode);
	if (err != MF_ERROR_OKAY)
		return err;
	(*outNode)->info = &MFV_V1X_TINFO_FUNCTION;

	// Get function type
	{
		mfvV1XToken* tok;
		err = mfvExpectType(state, &tok);
		if (err != MF_ERROR_OKAY)
			return err;
		mfvV1XNode* node;
		err = mfvV1XGetNode(state, &node);
		if (err != MF_ERROR_OKAY)
			return err;
		node->info = tok->info;
		strcpy(node->attribute, tok->attribute);
		err = mfvAddToNode(*outNode, node);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	
	// Set function name
	{
		mfvV1XToken* tok;
		err = mfvExpectTokenType(state, &MFV_V1X_TINFO_IDENTIFIER, &tok);
		if (err != MF_ERROR_OKAY)
			return err;
		mfvV1XNode* node;
		err = mfvV1XGetNode(state, &node);
		if (err != MF_ERROR_OKAY)
			return err;
		node->info = tok->info;
		strcpy(node->attribute, tok->attribute);
		err = mfvAddToNode(*outNode, node);
		if (err != MF_ERROR_OKAY)
			return err;
	}

	// Get params
	{
		err = mfvExpectTokenType(state, &MFV_V1X_TINFO_OPEN_PARENTHESIS, NULL);
		if (err != MF_ERROR_OKAY)
			return err;

		mfvV1XNode* params;
		err = mfvV1XGetNode(state, &params);
		if (err != MF_ERROR_OKAY)
			return err;
		params->info = &MFV_V1X_TINFO_PARAMS_DECL;

		// Get params
		mfvV1XToken* tok;
		err = mfvPeekToken(state, &tok);
		if (err != MF_ERROR_OKAY)
			return err;

		while (tok->info->type != MFV_V1X_TOKEN_CLOSE_PARENTHESIS)
		{
			// Param node
			mfvV1XNode* paramNode;
			err = mfvV1XGetNode(state, &paramNode);
			if (err != MF_ERROR_OKAY)
				return err;

			// Get param type
			err = mfvExpectType(state, &tok);
			if (err != MF_ERROR_OKAY)
				return err;
			paramNode->info = tok->info;

			// Get param ID
			err = mfvExpectTokenType(state, &MFV_V1X_TINFO_IDENTIFIER, &tok);
			if (err != MF_ERROR_OKAY)
				return err;

			// Param ID node
			mfvV1XNode* idNode;
			err = mfvV1XGetNode(state, &idNode);
			if (err != MF_ERROR_OKAY)
				return err;
			idNode->info = tok->info;
			strcpy(idNode->attribute, tok->attribute);
			err = mfvAddToNode(paramNode, idNode);
			if (err != MF_ERROR_OKAY)
				return err;

			// Add node to params
			err = mfvAddToNode(params, paramNode);
			if (err != MF_ERROR_OKAY)
				return err;

			// Check if next token is comma
			if (mfvAcceptTokenType(state, &MFV_V1X_TINFO_COMMA, NULL) == MFM_FALSE)
				break;
		}

		err = mfvExpectTokenType(state, &MFV_V1X_TINFO_CLOSE_PARENTHESIS, NULL);
		if (err != MF_ERROR_OKAY)
			return err;

		err = mfvAddToNode(*outNode, params);
		if (err != MF_ERROR_OKAY)
			return err;
	}

	// Get function body
	{
		mfvV1XNode* node;
		err = mfvParseCompoundStatement(state, &node);
		if (err != MF_ERROR_OKAY)
			return err;
		if (node == NULL)
		{
			mfsStringStream ss;
			mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFV_MAX_ERROR_MESSAGE_SIZE);
			mfsPrintFormatUTF8(&ss, u8"[mfvParseFunction] Failed to parse function compound statement");
			mfsDestroyLocalStringStream(&ss);
		}
		err = mfvAddToNode(*outNode, node);
		if (err != MF_ERROR_OKAY)
			return err;
	}

	return MF_ERROR_OKAY;
}

static mfError mfvParseProgram(mfvV1XParserInternalState* state)
{
	if (state == NULL)
		return MFV_ERROR_INVALID_ARGUMENTS;

	mfError err;

	mfvV1XNode* root;
	err = mfvV1XGetNode(state, &root);
	if (err != MF_ERROR_OKAY)
		return err;
	root->info = NULL; // Root has no type

	// Parse program
	mfvV1XNode* node = NULL;
	for (;;)
	{
		mfvV1XToken* tok;

		err = mfvPeekToken(state, &tok);
		if (err != MF_ERROR_OKAY)
			return err;
		if (tok == NULL)
			break;

		// Check if it is a function
		if (tok->info->isType == MFM_TRUE)
		{
			err = mfvParseFunction(state, &node);
			if (err != MF_ERROR_OKAY)
				return err;
			err = mfvAddToNode(&state->nodes[0], node);
			if (err != MF_ERROR_OKAY)
				return err;
		}
	}

	return MF_ERROR_OKAY;
}

mfError mfvV1XRunMVLParser(const mfvV1XToken * tokens, mfvV1XNode * nodeArray, mfmU64 maxNodeCount, const mfvV1XLexerState * lexerState, mfvV1XParserState * state)
{
	if (tokens == NULL || tokens == NULL || maxNodeCount == 0 || lexerState == NULL || state == NULL)
		return MFV_ERROR_INVALID_ARGUMENTS;

	mfError err;

	mfvV1XParserInternalState internalState;
	internalState.state = state;
	internalState.it = tokens;
	internalState.lastToken = tokens + lexerState->tokenCount - 1;
	internalState.nodes = nodeArray;
	internalState.maxNodeCount = maxNodeCount;
	internalState.state = state;

	state->errorMsg[0] = '\0';
	state->nodeCount = 0;
	for (mfmU64 i = 0; i < maxNodeCount; ++i)
		nodeArray[i].active = MFM_FALSE;

	err = mfvParseProgram(&internalState);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

mfError mfvV1XPrintNode(mfsStream * stream, mfvV1XNode * node, mfmU64 indentation)
{
	if (stream == NULL || node == NULL)
		return MFV_ERROR_INVALID_ARGUMENTS;
	if (node->active == MFM_FALSE)
		return MFV_ERROR_INACTIVE_NODE;

	mfError err = MF_ERROR_OKAY;

	for (mfmU64 i = 0; i < indentation; ++i)
	{
		err = mfsPutString(stream, u8"  ");
		if (err != MF_ERROR_OKAY)
			return err;
	}

	if (node->info == NULL)
	{
		err = mfsPrintFormatUTF8(stream, u8"- 'ROOT'\n");
		if (err != MF_ERROR_OKAY)
			return err;
	}
	else
	{
		if (node->attribute[0] == '\0')
		{
			err = mfsPrintFormatUTF8(stream, u8"- '%s'\n", node->info->name);
			if (err != MF_ERROR_OKAY)
				return err;
		}
		else
		{
			err = mfsPrintFormatUTF8(stream, u8"- '%s' \"%s\"\n", node->info->name, node->attribute);
			if (err != MF_ERROR_OKAY)
				return err;
		}
	}

	mfvV1XNode* c = node->first;
	while (c != NULL)
	{
		err = mfvV1XPrintNode(stream, c, indentation + 1);
		if (err != MF_ERROR_OKAY)
			return err;
		c = c->next;
	}

	return MF_ERROR_OKAY;
}
