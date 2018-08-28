#include "Parser.h"
#include "../../../String/StringStream.h"

#include <string.h>
#include <stdlib.h>

typedef struct
{
	mfgV2XParserState* state;
	mfgV2XCompilerState* compilerState;
	const mfgV2XToken* it;
	const mfgV2XToken* lastToken;
	mfgV2XNode* nodes;
	mfmU64 maxNodeCount;
} mfgV2XParserInternalState;

static mfError mfgV2XGetNode(mfgV2XParserInternalState* state, mfgV2XNode** node)
{
	if (state == NULL || node == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;
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
	return MFG_ERROR_NODES_OVERFLOW;
}

static mfError mfgV2XReleaseNode(mfgV2XParserInternalState* state, mfgV2XNode* node)
{
	if (state == NULL || node == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;
	node->active = MFM_FALSE;
	--state->state->nodeCount;
	return MF_ERROR_OKAY;
}

static mfError mfgAddToNode(mfgV2XNode* parent, mfgV2XNode* child)
{
	if (parent == NULL || child == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfgV2XNode* c = parent->first;
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

static mfError mfgExpectTokenType(mfgV2XParserInternalState* state, const mfgV2XTokenInfo* info, const mfgV2XToken** out)
{
	if (state == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;
	if (state->it > state->lastToken)
	{
		mfsStringStream ss;
		mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
		mfsPrintFormatUTF8(&ss, u8"[mfgExpectTokenType : MFG_ERROR_UNEXPECTED_EOF] Unexpected end of file, expected token type '%s'", info->name);
		mfsDestroyLocalStringStream(&ss);
		return MFG_ERROR_UNEXPECTED_EOF;
	}

	if (state->it->info->type != info->type)
	{
		mfsStringStream ss;
		mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
		mfsPrintFormatUTF8(&ss, u8"[mfgExpectTokenType : MFG_ERROR_UNEXPECTED_TOKEN] Unexpected token type '%s', expected '%s'", state->it->info->name, info->name);
		mfsDestroyLocalStringStream(&ss);
		return MFG_ERROR_UNEXPECTED_TOKEN;
	}

	if (out != NULL)
		*out = state->it;
	++state->it;

	return MF_ERROR_OKAY;
}

static mfError mfgExpectType(mfgV2XParserInternalState* state, const mfgV2XToken** out)
{
	if (state == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;
	if (state->it > state->lastToken)
	{
		mfsStringStream ss;
		mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
		mfsPrintFormatUTF8(&ss, u8"[mfgExpectType : MFG_ERROR_UNEXPECTED_EOF] Unexpected end of file, expected type");
		mfsDestroyLocalStringStream(&ss);
		return MFG_ERROR_UNEXPECTED_EOF;
	}

	if (state->it->info->isType == MFM_FALSE)
	{
		mfsStringStream ss;
		mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
		mfsPrintFormatUTF8(&ss, u8"[mfgExpectType : MFG_ERROR_UNEXPECTED_TOKEN] Unexpected token type '%s', expected type", state->it->info->name);
		mfsDestroyLocalStringStream(&ss);
		return MFG_ERROR_UNEXPECTED_TOKEN;
	}

	if (out != NULL)
		*out = state->it;
	++state->it;

	return MF_ERROR_OKAY;
}

static mfmBool mfgAcceptTokenType(mfgV2XParserInternalState* state, const mfgV2XTokenInfo* info, const mfgV2XToken** out)
{
	if (state == NULL)
		return MFM_FALSE;
	if (state->it > state->lastToken)
	{
		mfsStringStream ss;
		mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
		mfsPrintFormatUTF8(&ss, u8"[mfgAcceptTokenType : MFG_ERROR_UNEXPECTED_EOF] Unexpected end of file, expected token type '%s'", info->name);
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

static mfError mfgPeekToken(mfgV2XParserInternalState* state, const mfgV2XToken** out)
{
	if (state == NULL || out == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;
	if (state->it > state->lastToken)
		*out = NULL;
	else
		*out = state->it;
	return MF_ERROR_OKAY;
}

static mfError mfgNextToken(mfgV2XParserInternalState* state)
{
	if (state == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;
	if (state->it > state->lastToken)
		return MFG_ERROR_UNEXPECTED_EOF;
	++state->it;
	return MF_ERROR_OKAY;
}

static mfError mfgParseExpression(mfgV2XParserInternalState* state, mfgV2XNode** outNode);

static mfError mfgParseOperatorLast(mfgV2XParserInternalState* state, mfgV2XNode** outNode)
{
	if (state == NULL || outNode == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;
	const mfgV2XToken* tok = NULL;

	// ( <exp )
	if (mfgAcceptTokenType(state, &MFG_V2X_TINFO_OPEN_PARENTHESIS, NULL) == MFM_TRUE)
	{
		err = mfgParseExpression(state, outNode);
		if (err != MF_ERROR_OKAY)
			return err;
		err = mfgExpectTokenType(state, &MFG_V2X_TINFO_CLOSE_PARENTHESIS, NULL);
		if (err != MF_ERROR_OKAY)
			return err;
		return MF_ERROR_OKAY;
	}

	// <id> | <call>
	else if (mfgAcceptTokenType(state, &MFG_V2X_TINFO_IDENTIFIER, &tok) == MFM_TRUE)
	{
		// <call>
		if (mfgAcceptTokenType(state, &MFG_V2X_TINFO_OPEN_PARENTHESIS, NULL) == MFM_TRUE)
		{
			// Create identifier node
			mfgV2XNode* idNode = NULL;
			err = mfgV2XGetNode(state, &idNode);
			if (err != MF_ERROR_OKAY)
				return err;
			idNode->info = tok->info;
			strcpy(idNode->attribute, tok->attribute);

			// Create call node
			err = mfgV2XGetNode(state, outNode);
			if (err != MF_ERROR_OKAY)
				return err;
			(*outNode)->info = &MFG_V2X_TINFO_CALL;
			err = mfgAddToNode(*outNode, idNode);
			if (err != MF_ERROR_OKAY)
				return err;

			// Create call params node
			mfgV2XNode* params = NULL;
			err = mfgV2XGetNode(state, &params);
			if (err != MF_ERROR_OKAY)
				return err;
			params->info = &MFG_V2X_TINFO_CALL_PARAMS;
			err = mfgAddToNode(*outNode, params);
			if (err != MF_ERROR_OKAY)
				return err;

			// Add params to call params node
			mfgV2XNode* param = NULL;
			if (mfgAcceptTokenType(state, &MFG_V2X_TINFO_CLOSE_PARENTHESIS, NULL) == MFM_FALSE)
			{
				mfmU64 i = 0;
				for (;;)
				{
					err = mfgParseExpression(state, &param);
					if (err != MF_ERROR_OKAY)
						return err;
					if (param == NULL)
					{
						mfsStringStream ss;
						mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
						mfsPrintFormatUTF8(&ss, u8"[mfgParseOperatorLast] Failed to parse function call param %d", i);
						mfsDestroyLocalStringStream(&ss);
						return MFG_ERROR_FAILED_TO_PARSE;
					}

					err = mfgAddToNode(params, param);
					if (err != MF_ERROR_OKAY)
						return err;

					if (mfgAcceptTokenType(state, &MFG_V2X_TINFO_COMMA, NULL) == MFM_FALSE)
						break;

					++i;
				}


				err = mfgExpectTokenType(state, &MFG_V2X_TINFO_CLOSE_PARENTHESIS, NULL);
				if (err != MF_ERROR_OKAY)
					return err;
			}
			
			return MF_ERROR_OKAY;
		}
		// <id>
		else
		{
			err = mfgV2XGetNode(state, outNode);
			if (err != MF_ERROR_OKAY)
				return err;
			(*outNode)->info = tok->info;
			strcpy((*outNode)->attribute, tok->attribute);
			return MF_ERROR_OKAY;
		}
	}

	// <input> | <output>
	else if (mfgAcceptTokenType(state, &MFG_V2X_TINFO_INPUT, &tok) == MFM_TRUE ||
			 mfgAcceptTokenType(state, &MFG_V2X_TINFO_OUTPUT, &tok) == MFM_TRUE)
	{
		err = mfgV2XGetNode(state, outNode);
		if (err != MF_ERROR_OKAY)
			return err;
		(*outNode)->info = tok->info;
		return MF_ERROR_OKAY;
	}

	// <int-literal> | <float-literal> | <true> | <false>
	else if (mfgAcceptTokenType(state, &MFG_V2X_TINFO_INT_LITERAL, &tok) == MFM_TRUE ||
			 mfgAcceptTokenType(state, &MFG_V2X_TINFO_FLOAT_LITERAL, &tok) == MFM_TRUE ||
			 mfgAcceptTokenType(state, &MFG_V2X_TINFO_TRUE, &tok) == MFM_TRUE ||
			 mfgAcceptTokenType(state, &MFG_V2X_TINFO_FALSE, &tok) == MFM_TRUE)
	{
		err = mfgV2XGetNode(state, outNode);
		if (err != MF_ERROR_OKAY)
			return err;
		(*outNode)->info = tok->info;
		strcpy((*outNode)->attribute, tok->attribute);
		return MF_ERROR_OKAY;
	}

	*outNode = NULL;
	return MF_ERROR_OKAY;
}

static mfError mfgParseOperator6(mfgV2XParserInternalState* state, mfgV2XNode** outNode)
{
	if (state == NULL || outNode == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;

	mfgV2XToken* tok = NULL;

	// <array-access-op>
	// Get first term
	mfgV2XNode* term1 = NULL;
	err = mfgParseOperatorLast(state, &term1);
	if (err != MF_ERROR_OKAY)
		return err;
	if (term1 == NULL)
	{
		// No first term, no expression
		*outNode = NULL;
		return MF_ERROR_OKAY;
	}

	// Get terms
	mfgV2XNode* term2 = NULL;
	for (;;)
	{
		// Check operator token
		err = mfgPeekToken(state, &tok);
		if (err != MF_ERROR_OKAY)
			return err;
		if (tok == NULL ||
			tok->info->type != MFG_V2X_TOKEN_OPEN_BRACKETS)
			break;

		// Create operator node
		mfgV2XNode* op = NULL;
		err = mfgV2XGetNode(state, &op);
		if (err != MF_ERROR_OKAY)
			return err;
		op->info = &MFG_V2X_TINFO_ARRAY_ACCESS;

		// Next token
		err = mfgNextToken(state);
		if (err != MF_ERROR_OKAY)
			return err;

		// Parse second term
		err = mfgParseExpression(state, &term2);
		if (err != MF_ERROR_OKAY)
			return err;
		if (term2 == NULL)
		{
			mfsStringStream ss;
			mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
			mfsPrintFormatUTF8(&ss, u8"[mfgParseOperator6] Failed to parse '%s' operator second term", tok->info->name);
			mfsDestroyLocalStringStream(&ss);
			return MFG_ERROR_FAILED_TO_PARSE;
		}

		err = mfgExpectTokenType(state, &MFG_V2X_TINFO_CLOSE_BRACKETS, NULL);
		if (err != MF_ERROR_OKAY)
			return err;

		// Add terms to operator node
		err = mfgAddToNode(op, term1);
		if (err != MF_ERROR_OKAY)
			return err;
		err = mfgAddToNode(op, term2);
		if (err != MF_ERROR_OKAY)
			return err;
		term1 = op;
	}

	*outNode = term1;
	return MF_ERROR_OKAY;
}

static mfError mfgParseOperator5(mfgV2XParserInternalState* state, mfgV2XNode** outNode)
{
	if (state == NULL || outNode == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;
	
	mfgV2XToken* tok = NULL;

	// <unary-op>
	if (mfgAcceptTokenType(state, &MFG_V2X_TINFO_ADD, &tok) == MFM_TRUE ||
		mfgAcceptTokenType(state, &MFG_V2X_TINFO_SUBTRACT, &tok) == MFM_TRUE ||
		mfgAcceptTokenType(state, &MFG_V2X_TINFO_NOT, &tok) == MFM_TRUE)
	{
		// Create operator node
		mfgV2XNode* op = NULL;
		err = mfgV2XGetNode(state, &op);
		if (err != MF_ERROR_OKAY)
			return err;
		op->info = tok->info;

		// Next token
		err = mfgNextToken(state);
		if (err != MF_ERROR_OKAY)
			return err;

		// Parse term
		mfgV2XNode* term;
		err = mfgParseOperator5(state, &term);
		if (err != MF_ERROR_OKAY)
			return err;
		if (term == NULL)
		{
			mfsStringStream ss;
			mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
			mfsPrintFormatUTF8(&ss, u8"[mfgParseOperator5] Failed to parse '%s' unary operator term", tok->info->name);
			mfsDestroyLocalStringStream(&ss);
			return MFG_ERROR_FAILED_TO_PARSE;
		}

		// Add term to operator node
		err = mfgAddToNode(op, term);
		if (err != MF_ERROR_OKAY)
			return err;
		*outNode = term;
		return MF_ERROR_OKAY;
	}

	// <member-op>
	// Get first term
	mfgV2XNode* term1 = NULL;
	err = mfgParseOperator6(state, &term1);
	if (err != MF_ERROR_OKAY)
		return err;
	if (term1 == NULL)
	{
		// No first term, no expression
		*outNode = NULL;
		return MF_ERROR_OKAY;
	}

	// Get terms
	mfgV2XNode* term2 = NULL;
	for (;;)
	{
		// Check operator token
		err = mfgPeekToken(state, &tok);
		if (err != MF_ERROR_OKAY)
			return err;
		if (tok == NULL ||
			tok->info->type != MFG_V2X_TOKEN_MEMBER)
			break;

		// Create operator node
		mfgV2XNode* op = NULL;
		err = mfgV2XGetNode(state, &op);
		if (err != MF_ERROR_OKAY)
			return err;
		op->info = tok->info;

		// Next token
		err = mfgNextToken(state);
		if (err != MF_ERROR_OKAY)
			return err;

		// Parse second term
		err = mfgParseOperator6(state, &term2);
		if (err != MF_ERROR_OKAY)
			return err;
		if (term2 == NULL)
		{
			mfsStringStream ss;
			mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
			mfsPrintFormatUTF8(&ss, u8"[mfgParseOperator5] Failed to parse '%s' operator second term", tok->info->name);
			mfsDestroyLocalStringStream(&ss);
			return MFG_ERROR_FAILED_TO_PARSE;
		}

		// Add terms to operator node
		err = mfgAddToNode(op, term1);
		if (err != MF_ERROR_OKAY)
			return err;
		err = mfgAddToNode(op, term2);
		if (err != MF_ERROR_OKAY)
			return err;
		term1 = op;
	}

	*outNode = term1;
	return MF_ERROR_OKAY;
}


static mfError mfgParseOperator4(mfgV2XParserInternalState* state, mfgV2XNode** outNode)
{
	if (state == NULL || outNode == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;

	// Get first term
	mfgV2XNode* term1 = NULL;
	err = mfgParseOperator5(state, &term1);
	if (err != MF_ERROR_OKAY)
		return err;
	if (term1 == NULL)
	{
		// No first term, no expression
		*outNode = NULL;
		return MF_ERROR_OKAY;
	}

	// Get terms
	mfgV2XNode* term2 = NULL;
	mfgV2XToken* tok = NULL;
	for (;;)
	{
		// Check operator token
		err = mfgPeekToken(state, &tok);
		if (err != MF_ERROR_OKAY)
			return err;
		if (tok == NULL || (
			tok->info->type != MFG_V2X_TOKEN_ADD &&
			tok->info->type != MFG_V2X_TOKEN_SUBTRACT))
			break;

		// Create operator node
		mfgV2XNode* op = NULL;
		err = mfgV2XGetNode(state, &op);
		if (err != MF_ERROR_OKAY)
			return err;
		op->info = tok->info;

		// Next token
		err = mfgNextToken(state);
		if (err != MF_ERROR_OKAY)
			return err;

		// Parse second term
		err = mfgParseOperator5(state, &term2);
		if (err != MF_ERROR_OKAY)
			return err;
		if (term2 == NULL)
		{
			mfsStringStream ss;
			mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
			mfsPrintFormatUTF8(&ss, u8"[mfgParseOperator4] Failed to parse '%s' operator second term", tok->info->name);
			mfsDestroyLocalStringStream(&ss);
			return MFG_ERROR_FAILED_TO_PARSE;
		}

		// Add terms to operator node
		err = mfgAddToNode(op, term1);
		if (err != MF_ERROR_OKAY)
			return err;
		err = mfgAddToNode(op, term2);
		if (err != MF_ERROR_OKAY)
			return err;
		term1 = op;
	}

	*outNode = term1;
	return MF_ERROR_OKAY;
}


static mfError mfgParseOperator3(mfgV2XParserInternalState* state, mfgV2XNode** outNode)
{
	if (state == NULL || outNode == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;

	// Get first term
	mfgV2XNode* term1 = NULL;
	err = mfgParseOperator4(state, &term1);
	if (err != MF_ERROR_OKAY)
		return err;
	if (term1 == NULL)
	{
		// No first term, no expression
		*outNode = NULL;
		return MF_ERROR_OKAY;
	}

	// Get terms
	mfgV2XNode* term2 = NULL;
	mfgV2XToken* tok = NULL;
	for (;;)
	{
		// Check operator token
		err = mfgPeekToken(state, &tok);
		if (err != MF_ERROR_OKAY)
			return err;
		if (tok == NULL || (
			tok->info->type != MFG_V2X_TOKEN_MULTIPLY &&
			tok->info->type != MFG_V2X_TOKEN_DIVIDE))
			break;

		// Create operator node
		mfgV2XNode* op = NULL;
		err = mfgV2XGetNode(state, &op);
		if (err != MF_ERROR_OKAY)
			return err;
		op->info = tok->info;

		// Next token
		err = mfgNextToken(state);
		if (err != MF_ERROR_OKAY)
			return err;

		// Parse second term
		err = mfgParseOperator4(state, &term2);
		if (err != MF_ERROR_OKAY)
			return err;
		if (term2 == NULL)
		{
			mfsStringStream ss;
			mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
			mfsPrintFormatUTF8(&ss, u8"[mfgParseOperator3] Failed to parse '%s' operator second term", tok->info->name);
			mfsDestroyLocalStringStream(&ss);
			return MFG_ERROR_FAILED_TO_PARSE;
		}

		// Add terms to operator node
		err = mfgAddToNode(op, term1);
		if (err != MF_ERROR_OKAY)
			return err;
		err = mfgAddToNode(op, term2);
		if (err != MF_ERROR_OKAY)
			return err;
		term1 = op;
	}

	*outNode = term1;
	return MF_ERROR_OKAY;
}


static mfError mfgParseOperator2(mfgV2XParserInternalState* state, mfgV2XNode** outNode)
{
	if (state == NULL || outNode == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;

	// Get first term
	mfgV2XNode* term1 = NULL;
	err = mfgParseOperator3(state, &term1);
	if (err != MF_ERROR_OKAY)
		return err;
	if (term1 == NULL)
	{
		// No first term, no expression
		*outNode = NULL;
		return MF_ERROR_OKAY;
	}

	// Get terms
	mfgV2XNode* term2 = NULL;
	mfgV2XToken* tok = NULL;
	for (;;)
	{
		// Check operator token
		err = mfgPeekToken(state, &tok);
		if (err != MF_ERROR_OKAY)
			return err;
		if (tok == NULL || (
			tok->info->type != MFG_V2X_TOKEN_EQUAL &&
			tok->info->type != MFG_V2X_TOKEN_DIFFERENT &&
			tok->info->type != MFG_V2X_TOKEN_GREATER &&
			tok->info->type != MFG_V2X_TOKEN_LESS &&
			tok->info->type != MFG_V2X_TOKEN_GEQUAL &&
			tok->info->type != MFG_V2X_TOKEN_LEQUAL))
			break;

		// Create operator node
		mfgV2XNode* op = NULL;
		err = mfgV2XGetNode(state, &op);
		if (err != MF_ERROR_OKAY)
			return err;
		op->info = tok->info;

		// Next token
		err = mfgNextToken(state);
		if (err != MF_ERROR_OKAY)
			return err;

		// Parse second term
		err = mfgParseOperator3(state, &term2);
		if (err != MF_ERROR_OKAY)
			return err;
		if (term2 == NULL)
		{
			mfsStringStream ss;
			mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
			mfsPrintFormatUTF8(&ss, u8"[mfgParseOperator2] Failed to parse '%s' operator second term", tok->info->name);
			mfsDestroyLocalStringStream(&ss);
			return MFG_ERROR_FAILED_TO_PARSE;
		}

		// Add terms to operator node
		err = mfgAddToNode(op, term1);
		if (err != MF_ERROR_OKAY)
			return err;
		err = mfgAddToNode(op, term2);
		if (err != MF_ERROR_OKAY)
			return err;
		term1 = op;
	}

	*outNode = term1;
	return MF_ERROR_OKAY;
}


static mfError mfgParseOperator1(mfgV2XParserInternalState* state, mfgV2XNode** outNode)
{
	if (state == NULL || outNode == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;

	// Get first term
	mfgV2XNode* term1 = NULL;
	err = mfgParseOperator2(state, &term1);
	if (err != MF_ERROR_OKAY)
		return err;
	if (term1 == NULL)
	{
		// No first term, no expression
		*outNode = NULL;
		return MF_ERROR_OKAY;
	}

	// Get terms
	mfgV2XNode* term2 = NULL;
	mfgV2XToken* tok = NULL;
	for (;;)
	{
		// Check operator token
		err = mfgPeekToken(state, &tok);
		if (err != MF_ERROR_OKAY)
			return err;
		if (tok == NULL || (
			tok->info->type != MFG_V2X_TOKEN_AND &&
			tok->info->type != MFG_V2X_TOKEN_OR))
			break;

		// Create operator node
		mfgV2XNode* op = NULL;
		err = mfgV2XGetNode(state, &op);
		if (err != MF_ERROR_OKAY)
			return err;
		op->info = tok->info;

		// Next token
		err = mfgNextToken(state);
		if (err != MF_ERROR_OKAY)
			return err;

		// Parse second term
		err = mfgParseOperator2(state, &term2);
		if (err != MF_ERROR_OKAY)
			return err;
		if (term2 == NULL)
		{
			mfsStringStream ss;
			mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
			mfsPrintFormatUTF8(&ss, u8"[mfgParseOperator1] Failed to parse '%s' operator second term", tok->info->name);
			mfsDestroyLocalStringStream(&ss);
			return MFG_ERROR_FAILED_TO_PARSE;
		}

		// Add terms to operator node
		err = mfgAddToNode(op, term1);
		if (err != MF_ERROR_OKAY)
			return err;
		err = mfgAddToNode(op, term2);
		if (err != MF_ERROR_OKAY)
			return err;
		term1 = op;
	}

	*outNode = term1;
	return MF_ERROR_OKAY;
}

static mfError mfgParseExpression(mfgV2XParserInternalState* state, mfgV2XNode** outNode)
{
	if (state == NULL || outNode == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;

	// Get first term
	mfgV2XNode* term1 = NULL;
	err = mfgParseOperator1(state, &term1);
	if (err != MF_ERROR_OKAY)
		return err;
	if (term1 == NULL)
	{
		// No first term, no expression
		*outNode = NULL;
		return MF_ERROR_OKAY;
	}

	// Get terms
	mfgV2XNode* term2 = NULL;
	mfgV2XToken* tok = NULL;
	for (;;)
	{
		// Check operator token
		err = mfgPeekToken(state, &tok);
		if (err != MF_ERROR_OKAY)
			return err;
		if (tok == NULL || (
			tok->info->type != MFG_V2X_TOKEN_ASSIGN))
			break;

		// Create operator node
		mfgV2XNode* op = NULL;
		err = mfgV2XGetNode(state, &op);
		if (err != MF_ERROR_OKAY)
			return err;
		op->info = tok->info;

		// Next token
		err = mfgNextToken(state);
		if (err != MF_ERROR_OKAY)
			return err;

		// Parse second term
		err = mfgParseOperator1(state, &term2);
		if (err != MF_ERROR_OKAY)
			return err;
		if (term2 == NULL)
		{
			mfsStringStream ss;
			mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
			mfsPrintFormatUTF8(&ss, u8"[mfgParseOperator1] Failed to parse '%s' operator second term", tok->info->name);
			mfsDestroyLocalStringStream(&ss);
			return MFG_ERROR_FAILED_TO_PARSE;
		}

		// Add terms to operator node
		err = mfgAddToNode(op, term1);
		if (err != MF_ERROR_OKAY)
			return err;
		err = mfgAddToNode(op, term2);
		if (err != MF_ERROR_OKAY)
			return err;
		term1 = op;
	}

	*outNode = term1;
	return MF_ERROR_OKAY;
}

static mfError mfgParseStatement(mfgV2XParserInternalState* state, mfgV2XNode** outNode);

static mfError mfgParseExpressionStatement(mfgV2XParserInternalState* state, mfgV2XNode** outNode)
{
	if (state == NULL || outNode == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;

	err = mfgParseExpression(state, outNode);
	if (err != MF_ERROR_OKAY)
		return err;
	if (*outNode == NULL)
		return MF_ERROR_OKAY;

	err = mfgExpectTokenType(state, &MFG_V2X_TINFO_SEMICOLON, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

static mfError mfgParseCompoundStatement(mfgV2XParserInternalState* state, mfgV2XNode** outNode)
{
	if (state == NULL || outNode == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	if (mfgAcceptTokenType(state, &MFG_V2X_TINFO_OPEN_BRACES, NULL) == MFM_FALSE)
	{
		*outNode = NULL;
		return MF_ERROR_OKAY;
	}

	mfError err = mfgV2XGetNode(state, outNode);
	if (err != MF_ERROR_OKAY)
		return err;
	(*outNode)->info = &MFG_V2X_TINFO_COMPOUND_STATEMENT;

	// Get statements
	mfgV2XNode* statement = NULL;
	for (;;)
	{
		err = mfgParseStatement(state, &statement);
		if (err != MF_ERROR_OKAY)
			return err;
		if (statement == NULL)
			break;
		err = mfgAddToNode(*outNode, statement);
		if (err != MF_ERROR_OKAY)
			return err;
	}

	err = mfgExpectTokenType(state, &MFG_V2X_TINFO_CLOSE_BRACES, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

static mfError mfgParseReturnStatement(mfgV2XParserInternalState* state, mfgV2XNode** outNode)
{
	if (state == NULL || outNode == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;

	if (mfgAcceptTokenType(state, &MFG_V2X_TINFO_RETURN, NULL) == MFM_FALSE)
	{
		*outNode = NULL;
		return MF_ERROR_OKAY;
	}

	err = mfgV2XGetNode(state, outNode);
	if (err != MF_ERROR_OKAY)
		return err;
	(*outNode)->info = &MFG_V2X_TINFO_RETURN;

	mfgV2XNode* exp = NULL;
	err = mfgParseExpression(state, &exp);
	if (err != MF_ERROR_OKAY)
		return err;
	if (exp != NULL)
	{
		err = mfgAddToNode(*outNode, exp);
		if (err != MF_ERROR_OKAY)
			return err;
	}

	err = mfgExpectTokenType(state, &MFG_V2X_TINFO_SEMICOLON, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

static mfError mfgParseDiscardStatement(mfgV2XParserInternalState* state, mfgV2XNode** outNode)
{
	if (state == NULL || outNode == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;

	if (mfgAcceptTokenType(state, &MFG_V2X_TINFO_DISCARD, NULL) == MFM_FALSE)
	{
		*outNode = NULL;
		return MF_ERROR_OKAY;
	}

	err = mfgV2XGetNode(state, outNode);
	if (err != MF_ERROR_OKAY)
		return err;
	(*outNode)->info = &MFG_V2X_TINFO_DISCARD;

	err = mfgExpectTokenType(state, &MFG_V2X_TINFO_SEMICOLON, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

static mfError mfgParseStatement(mfgV2XParserInternalState* state, mfgV2XNode** outNode)
{
	if (state == NULL || outNode == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;
	
	// Check if it is a compound statement
	err = mfgParseCompoundStatement(state, outNode);
	if (err != MF_ERROR_OKAY)
		return err;
	if (*outNode != NULL)
		return MF_ERROR_OKAY;

	// Check if it is an expression statement
	err = mfgParseExpressionStatement(state, outNode);
	if (err != MF_ERROR_OKAY)
		return err;
	if (*outNode != NULL)
		return MF_ERROR_OKAY;

	// Check if it is a return statement
	err = mfgParseReturnStatement(state, outNode);
	if (err != MF_ERROR_OKAY)
		return err;
	if (*outNode != NULL)
		return MF_ERROR_OKAY;

	// Check if it is a discard statement
	err = mfgParseDiscardStatement(state, outNode);
	if (err != MF_ERROR_OKAY)
		return err;
	if (*outNode != NULL)
		return MF_ERROR_OKAY;

	// Not a statement
	*outNode = NULL;
	return MF_ERROR_OKAY;
}

static mfError mfgParseFunction(mfgV2XParserInternalState* state, mfgV2XNode** outNode)
{
	if (state == NULL || outNode == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;
	err = mfgV2XGetNode(state, outNode);
	if (err != MF_ERROR_OKAY)
		return err;
	(*outNode)->info = &MFG_V2X_TINFO_FUNCTION;

	// Get function type
	{
		mfgV2XToken* tok;
		err = mfgExpectType(state, &tok);
		if (err != MF_ERROR_OKAY)
			return err;
		mfgV2XNode* node;
		err = mfgV2XGetNode(state, &node);
		if (err != MF_ERROR_OKAY)
			return err;
		node->info = tok->info;
		strcpy(node->attribute, tok->attribute);
		err = mfgAddToNode(*outNode, node);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	
	// Set function name
	{
		mfgV2XToken* tok;
		err = mfgExpectTokenType(state, &MFG_V2X_TINFO_IDENTIFIER, &tok);
		if (err != MF_ERROR_OKAY)
			return err;
		mfgV2XNode* node;
		err = mfgV2XGetNode(state, &node);
		if (err != MF_ERROR_OKAY)
			return err;
		node->info = tok->info;
		strcpy(node->attribute, tok->attribute);
		err = mfgAddToNode(*outNode, node);
		if (err != MF_ERROR_OKAY)
			return err;
	}

	// Get params
	{
		err = mfgExpectTokenType(state, &MFG_V2X_TINFO_OPEN_PARENTHESIS, NULL);
		if (err != MF_ERROR_OKAY)
			return err;

		mfgV2XNode* params;
		err = mfgV2XGetNode(state, &params);
		if (err != MF_ERROR_OKAY)
			return err;
		params->info = &MFG_V2X_TINFO_PARAMS_DECL;

		// Get params
		mfgV2XToken* tok;
		err = mfgPeekToken(state, &tok);
		if (err != MF_ERROR_OKAY)
			return err;

		while (tok->info->type != MFG_V2X_TOKEN_CLOSE_PARENTHESIS)
		{
			// Param node
			mfgV2XNode* paramNode;
			err = mfgV2XGetNode(state, &paramNode);
			if (err != MF_ERROR_OKAY)
				return err;

			// Get param type
			err = mfgExpectType(state, &tok);
			if (err != MF_ERROR_OKAY)
				return err;
			paramNode->info = tok->info;

			// Get param ID
			err = mfgExpectTokenType(state, &MFG_V2X_TINFO_IDENTIFIER, &tok);
			if (err != MF_ERROR_OKAY)
				return err;

			// Param ID node
			mfgV2XNode* idNode;
			err = mfgV2XGetNode(state, &idNode);
			if (err != MF_ERROR_OKAY)
				return err;
			idNode->info = tok->info;
			strcpy(idNode->attribute, tok->attribute);
			err = mfgAddToNode(paramNode, idNode);
			if (err != MF_ERROR_OKAY)
				return err;

			// Add node to params
			err = mfgAddToNode(params, paramNode);
			if (err != MF_ERROR_OKAY)
				return err;

			// Check if next token is comma
			if (mfgAcceptTokenType(state, &MFG_V2X_TINFO_COMMA, NULL) == MFM_FALSE)
				break;
		}

		err = mfgExpectTokenType(state, &MFG_V2X_TINFO_CLOSE_PARENTHESIS, NULL);
		if (err != MF_ERROR_OKAY)
			return err;

		err = mfgAddToNode(*outNode, params);
		if (err != MF_ERROR_OKAY)
			return err;
	}

	// Get function body
	{
		mfgV2XNode* node;
		err = mfgParseCompoundStatement(state, &node);
		if (err != MF_ERROR_OKAY)
			return err;
		if (node == NULL)
		{
			mfsStringStream ss;
			mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
			mfsPrintFormatUTF8(&ss, u8"[mfgParseFunction] Failed to parse function compound statement");
			mfsDestroyLocalStringStream(&ss);
		}
		err = mfgAddToNode(*outNode, node);
		if (err != MF_ERROR_OKAY)
			return err;
	}

	return MF_ERROR_OKAY;
}

static mfError mfgParseInput(mfgV2XParserInternalState* state)
{
	if (state == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;

	err = mfgExpectTokenType(state, &MFG_V2X_TINFO_INPUT, NULL);
	if (err != MF_ERROR_OKAY)
		return err;
	err = mfgExpectTokenType(state, &MFG_V2X_TINFO_OPEN_BRACES, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	// Extract input variables
	mfmU64 nextVar = 0;
	while (mfgAcceptTokenType(state, &MFG_V2X_TINFO_CLOSE_BRACES, NULL) == MFM_FALSE)
	{
		mfgV2XToken* tok = NULL;
		
		// Get type
		err = mfgExpectType(state, &tok);
		if (err != MF_ERROR_OKAY)
			return err;

		if (nextVar >= MFG_V2X_MAX_INPUT_VARS)
		{
			mfsStringStream ss;
			mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
			mfsPrintFormatUTF8(&ss, u8"[mfgParseInput] Failed to parse input variable, input variable limit (%d) reached", MFG_V2X_MAX_INPUT_VARS);
			mfsDestroyLocalStringStream(&ss);
			return MFG_ERROR_TOO_MANY_VARIABLES;
		}

		state->compilerState->input.variables[nextVar].type = tok->info->type;
		state->compilerState->input.variables[nextVar].arraySize = 0;

		// Get ID
		err = mfgExpectTokenType(state, &MFG_V2X_TINFO_IDENTIFIER, &tok);
		if (err != MF_ERROR_OKAY)
			return err;
		strcpy(state->compilerState->input.variables[nextVar].id, tok->attribute);

		err = mfgExpectTokenType(state, &MFG_V2X_TINFO_COLON, &tok);
		if (err != MF_ERROR_OKAY)
			return err;

		// Get name
		err = mfgExpectTokenType(state, &MFG_V2X_TINFO_IDENTIFIER, &tok);
		if (err != MF_ERROR_OKAY)
			return err;
		strcpy(state->compilerState->input.variables[nextVar].name, tok->attribute);

		err = mfgExpectTokenType(state, &MFG_V2X_TINFO_SEMICOLON, &tok);
		if (err != MF_ERROR_OKAY)
			return err;

		state->compilerState->input.variables[nextVar].active = MFM_TRUE;
		++nextVar;
	}

	err = mfgExpectTokenType(state, &MFG_V2X_TINFO_SEMICOLON, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

static mfError mfgParseOutput(mfgV2XParserInternalState* state)
{
	if (state == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;

	err = mfgExpectTokenType(state, &MFG_V2X_TINFO_OUTPUT, NULL);
	if (err != MF_ERROR_OKAY)
		return err;
	err = mfgExpectTokenType(state, &MFG_V2X_TINFO_OPEN_BRACES, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	// Extract output variables
	mfmU64 nextVar = 0;
	while (mfgAcceptTokenType(state, &MFG_V2X_TINFO_CLOSE_BRACES, NULL) == MFM_FALSE)
	{
		mfgV2XToken* tok = NULL;

		// Get type
		err = mfgExpectType(state, &tok);
		if (err != MF_ERROR_OKAY)
			return err;

		if (nextVar >= MFG_V2X_MAX_OUTPUT_VARS)
		{
			mfsStringStream ss;
			mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
			mfsPrintFormatUTF8(&ss, u8"[mfgParseOutput] Failed to parse output variable, output variable limit (%d) reached", MFG_V2X_MAX_OUTPUT_VARS);
			mfsDestroyLocalStringStream(&ss);
			return MFG_ERROR_TOO_MANY_VARIABLES;
		}

		state->compilerState->output.variables[nextVar].type = tok->info->type;
		state->compilerState->output.variables[nextVar].arraySize = 0;

		// Get ID
		err = mfgExpectTokenType(state, &MFG_V2X_TINFO_IDENTIFIER, &tok);
		if (err != MF_ERROR_OKAY)
			return err;
		strcpy(state->compilerState->output.variables[nextVar].id, tok->attribute);

		err = mfgExpectTokenType(state, &MFG_V2X_TINFO_COLON, &tok);
		if (err != MF_ERROR_OKAY)
			return err;

		// Get name
		err = mfgExpectTokenType(state, &MFG_V2X_TINFO_IDENTIFIER, &tok);
		if (err != MF_ERROR_OKAY)
			return err;
		strcpy(state->compilerState->output.variables[nextVar].name, tok->attribute);

		err = mfgExpectTokenType(state, &MFG_V2X_TINFO_SEMICOLON, &tok);
		if (err != MF_ERROR_OKAY)
			return err;

		state->compilerState->output.variables[nextVar].active = MFM_TRUE;
		++nextVar;
	}

	err = mfgExpectTokenType(state, &MFG_V2X_TINFO_SEMICOLON, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

static mfError mfgParseConstantBuffer(mfgV2XParserInternalState* state)
{
	if (state == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;
	mfgV2XToken* tok = NULL;

	// Get buffer ID
	err = mfgExpectTokenType(state, &MFG_V2X_TINFO_CONSTANT_BUFFER, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	mfmU64 bufferID = 0;
	for (bufferID = 0; bufferID < MFG_V2X_MAX_CONSTANT_BUFFERS; ++bufferID)
		if (state->compilerState->constantBuffers[bufferID].active == MFM_FALSE)
		{
			state->compilerState->constantBuffers[bufferID].active = MFM_TRUE;
			break;
		}
	if (bufferID == MFG_V2X_MAX_CONSTANT_BUFFERS)
	{
		mfsStringStream ss;
		mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
		mfsPrintFormatUTF8(&ss, u8"[mfgParseConstantBuffer] Failed to parse constant buffer, constant buffer limit (%d) reached", MFG_V2X_MAX_CONSTANT_BUFFERS);
		mfsDestroyLocalStringStream(&ss);
		return MFG_ERROR_TOO_MANY_VARIABLES;
	}

	// Get ID
	err = mfgExpectTokenType(state, &MFG_V2X_TINFO_IDENTIFIER, &tok);
	if (err != MF_ERROR_OKAY)
		return err;
	strcpy(state->compilerState->constantBuffers[bufferID].id, tok->attribute);

	err = mfgExpectTokenType(state, &MFG_V2X_TINFO_COLON, &tok);
	if (err != MF_ERROR_OKAY)
		return err;

	// Get name
	err = mfgExpectTokenType(state, &MFG_V2X_TINFO_IDENTIFIER, &tok);
	if (err != MF_ERROR_OKAY)
		return err;
	strcpy(state->compilerState->constantBuffers[bufferID].name, tok->attribute);

	err = mfgExpectTokenType(state, &MFG_V2X_TINFO_OPEN_BRACES, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	// Extract constant buffer variables
	mfmU64 nextVar = 0;
	while (mfgAcceptTokenType(state, &MFG_V2X_TINFO_CLOSE_BRACES, NULL) == MFM_FALSE)
	{
		// Get type
		err = mfgExpectType(state, &tok);
		if (err != MF_ERROR_OKAY)
			return err;

		if (nextVar >= MFG_V2X_MAX_CONSTANT_BUFFER_VARS)
		{
			mfsStringStream ss;
			mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
			mfsPrintFormatUTF8(&ss, u8"[mfgParseInput] Failed to parse constant buffer variable, constant buffer variable limit (%d) reached", MFG_V2X_MAX_CONSTANT_BUFFER_VARS);
			mfsDestroyLocalStringStream(&ss);
			return MFG_ERROR_TOO_MANY_VARIABLES;
		}

		state->compilerState->constantBuffers[bufferID].variables[nextVar].type = tok->info->type;
		
		// Get ID
		err = mfgExpectTokenType(state, &MFG_V2X_TINFO_IDENTIFIER, &tok);
		if (err != MF_ERROR_OKAY)
			return err;
		strcpy(state->compilerState->constantBuffers[bufferID].variables[nextVar].id, tok->attribute);

		// Check if the variable is an array
		if (mfgAcceptTokenType(state, &MFG_V2X_TINFO_OPEN_BRACKETS, NULL) == MFM_TRUE)
		{
			err = mfgExpectTokenType(state, &MFG_V2X_TINFO_INT_LITERAL, &tok);
			if (err != MF_ERROR_OKAY)
				return err;
			state->compilerState->constantBuffers[bufferID].variables[nextVar].arraySize = atoll(tok->attribute);
			err = mfgExpectTokenType(state, &MFG_V2X_TINFO_CLOSE_BRACKETS, NULL);
			if (err != MF_ERROR_OKAY)
				return err;
		}
		else
			state->compilerState->constantBuffers[bufferID].variables[nextVar].arraySize = 0;

		err = mfgExpectTokenType(state, &MFG_V2X_TINFO_SEMICOLON, &tok);
		if (err != MF_ERROR_OKAY)
			return err;

		state->compilerState->constantBuffers[bufferID].variables[nextVar].active = MFM_TRUE;
		++nextVar;
	}

	err = mfgExpectTokenType(state, &MFG_V2X_TINFO_SEMICOLON, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

static mfError mfgParseTexture1D(mfgV2XParserInternalState* state)
{
	if (state == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;
	mfgV2XToken* tok = NULL;

	// Get texture ID
	err = mfgExpectTokenType(state, &MFG_V2X_TINFO_TEXTURE_1D, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	mfmU64 texID = 0;
	for (texID = 0; texID < MFG_V2X_MAX_TEXTURE_1DS; ++texID)
		if (state->compilerState->texture1Ds[texID].active == MFM_FALSE)
		{
			state->compilerState->texture1Ds[texID].active = MFM_TRUE;
			break;
		}
	if (texID == MFG_V2X_MAX_TEXTURE_1DS)
	{
		mfsStringStream ss;
		mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
		mfsPrintFormatUTF8(&ss, u8"[mfgParseTexture1D] Failed to parse texture 1D, texture 1D limit (%d) reached", MFG_V2X_MAX_TEXTURE_1DS);
		mfsDestroyLocalStringStream(&ss);
		return MFG_ERROR_TOO_MANY_VARIABLES;
	}

	// Get ID
	err = mfgExpectTokenType(state, &MFG_V2X_TINFO_IDENTIFIER, &tok);
	if (err != MF_ERROR_OKAY)
		return err;
	strcpy(state->compilerState->texture1Ds[texID].id, tok->attribute);

	err = mfgExpectTokenType(state, &MFG_V2X_TINFO_COLON, &tok);
	if (err != MF_ERROR_OKAY)
		return err;

	// Get name
	err = mfgExpectTokenType(state, &MFG_V2X_TINFO_IDENTIFIER, &tok);
	if (err != MF_ERROR_OKAY)
		return err;
	strcpy(state->compilerState->texture1Ds[texID].name, tok->attribute);

	err = mfgExpectTokenType(state, &MFG_V2X_TINFO_SEMICOLON, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

static mfError mfgParseTexture2D(mfgV2XParserInternalState* state)
{
	if (state == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;
	mfgV2XToken* tok = NULL;

	// Get texture ID
	err = mfgExpectTokenType(state, &MFG_V2X_TINFO_TEXTURE_2D, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	mfmU64 texID = 0;
	for (texID = 0; texID < MFG_V2X_MAX_TEXTURE_2DS; ++texID)
		if (state->compilerState->texture2Ds[texID].active == MFM_FALSE)
		{
			state->compilerState->texture2Ds[texID].active = MFM_TRUE;
			break;
		}
	if (texID == MFG_V2X_MAX_TEXTURE_2DS)
	{
		mfsStringStream ss;
		mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
		mfsPrintFormatUTF8(&ss, u8"[mfgParseTexture2D] Failed to parse texture 2D, texture 2D limit (%d) reached", MFG_V2X_MAX_TEXTURE_2DS);
		mfsDestroyLocalStringStream(&ss);
		return MFG_ERROR_TOO_MANY_VARIABLES;
	}

	// Get ID
	err = mfgExpectTokenType(state, &MFG_V2X_TINFO_IDENTIFIER, &tok);
	if (err != MF_ERROR_OKAY)
		return err;
	strcpy(state->compilerState->texture2Ds[texID].id, tok->attribute);

	err = mfgExpectTokenType(state, &MFG_V2X_TINFO_COLON, &tok);
	if (err != MF_ERROR_OKAY)
		return err;

	// Get name
	err = mfgExpectTokenType(state, &MFG_V2X_TINFO_IDENTIFIER, &tok);
	if (err != MF_ERROR_OKAY)
		return err;
	strcpy(state->compilerState->texture2Ds[texID].name, tok->attribute);

	err = mfgExpectTokenType(state, &MFG_V2X_TINFO_SEMICOLON, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

static mfError mfgParseTexture3D(mfgV2XParserInternalState* state)
{
	if (state == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;
	mfgV2XToken* tok = NULL;

	// Get texture ID
	err = mfgExpectTokenType(state, &MFG_V2X_TINFO_TEXTURE_3D, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	mfmU64 texID = 0;
	for (texID = 0; texID < MFG_V2X_MAX_TEXTURE_3DS; ++texID)
		if (state->compilerState->texture3Ds[texID].active == MFM_FALSE)
		{
			state->compilerState->texture3Ds[texID].active = MFM_TRUE;
			break;
		}
	if (texID == MFG_V2X_MAX_TEXTURE_3DS)
	{
		mfsStringStream ss;
		mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
		mfsPrintFormatUTF8(&ss, u8"[mfgParseTexture3D] Failed to parse texture 3D, texture 3D limit (%d) reached", MFG_V2X_MAX_TEXTURE_3DS);
		mfsDestroyLocalStringStream(&ss);
		return MFG_ERROR_TOO_MANY_VARIABLES;
	}

	// Get ID
	err = mfgExpectTokenType(state, &MFG_V2X_TINFO_IDENTIFIER, &tok);
	if (err != MF_ERROR_OKAY)
		return err;
	strcpy(state->compilerState->texture3Ds[texID].id, tok->attribute);

	err = mfgExpectTokenType(state, &MFG_V2X_TINFO_COLON, &tok);
	if (err != MF_ERROR_OKAY)
		return err;

	// Get name
	err = mfgExpectTokenType(state, &MFG_V2X_TINFO_IDENTIFIER, &tok);
	if (err != MF_ERROR_OKAY)
		return err;
	strcpy(state->compilerState->texture3Ds[texID].name, tok->attribute);

	err = mfgExpectTokenType(state, &MFG_V2X_TINFO_SEMICOLON, NULL);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

static mfError mfgParseProgram(mfgV2XParserInternalState* state)
{
	if (state == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;

	mfgV2XNode* root;
	err = mfgV2XGetNode(state, &root);
	if (err != MF_ERROR_OKAY)
		return err;
	root->info = NULL; // Root has no type

	// Parse program
	mfgV2XNode* node = NULL;
	for (;;)
	{
		mfgV2XToken* tok;

		err = mfgPeekToken(state, &tok);
		if (err != MF_ERROR_OKAY)
			return err;
		if (tok == NULL)
			break;

		// Check if it is the Input struct
		if (tok->info->type == MFG_V2X_TOKEN_INPUT)
		{
			err = mfgParseInput(state);
			if (err != MF_ERROR_OKAY)
				return err;
		}
		// Check if it is the Output struct
		else if (tok->info->type == MFG_V2X_TOKEN_OUTPUT)
		{
			err = mfgParseOutput(state);
			if (err != MF_ERROR_OKAY)
				return err;
		}
		// Check if it is a ConstantBuffer struct
		else if (tok->info->type == MFG_V2X_TOKEN_CONSTANT_BUFFER)
		{
			err = mfgParseConstantBuffer(state);
			if (err != MF_ERROR_OKAY)
				return err;
		}
		// Check if it is a Texture1D
		else if (tok->info->type == MFG_V2X_TOKEN_TEXTURE_1D)
		{
			err = mfgParseTexture1D(state);
			if (err != MF_ERROR_OKAY)
				return err;
		}
		// Check if it is a Texture2D
		else if (tok->info->type == MFG_V2X_TOKEN_TEXTURE_2D)
		{
			err = mfgParseTexture2D(state);
			if (err != MF_ERROR_OKAY)
				return err;
		}
		// Check if it is a Texture3D
		else if (tok->info->type == MFG_V2X_TOKEN_TEXTURE_3D)
		{
			err = mfgParseTexture3D(state);
			if (err != MF_ERROR_OKAY)
				return err;
		}
		// Check if it is a function
		else if (tok->info->isType == MFM_TRUE)
		{
			err = mfgParseFunction(state, &node);
			if (err != MF_ERROR_OKAY)
				return err;
			err = mfgAddToNode(&state->nodes[0], node);
			if (err != MF_ERROR_OKAY)
				return err;
		}
		// Else error
		else
		{
			mfsStringStream ss;
			mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
			mfsPrintFormatUTF8(&ss, u8"[mfgParseProgram] Failed to parse program, unexpected token '%s' on the global scope", tok->info->name);
			mfsDestroyLocalStringStream(&ss);
			return MFG_ERROR_TOO_MANY_VARIABLES;
		}
	}

	return MF_ERROR_OKAY;
}

mfError mfgV2XRunMVLParser(const mfgV2XToken * tokens, mfgV2XNode * nodeArray, mfmU64 maxNodeCount, const mfgV2XLexerState * lexerState, mfgV2XParserState * state, mfgV2XCompilerState* compilerState)
{
	if (tokens == NULL || tokens == NULL || maxNodeCount == 0 || lexerState == NULL || state == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;

	mfgV2XParserInternalState internalState;
	internalState.state = state;
	internalState.compilerState = compilerState;
	internalState.it = tokens;
	internalState.lastToken = tokens + lexerState->tokenCount - 1;
	internalState.nodes = nodeArray;
	internalState.maxNodeCount = maxNodeCount;
	internalState.state = state;

	state->errorMsg[0] = '\0';
	state->nodeCount = 0;
	for (mfmU64 i = 0; i < maxNodeCount; ++i)
		nodeArray[i].active = MFM_FALSE;

	for (mfmU64 i = 0; i < MFG_V2X_MAX_INPUT_VARS; ++i)
		compilerState->input.variables[i].active = MFM_FALSE;
	for (mfmU64 i = 0; i < MFG_V2X_MAX_OUTPUT_VARS; ++i)
		compilerState->output.variables[i].active = MFM_FALSE;
	for (mfmU64 i = 0; i < MFG_V2X_MAX_CONSTANT_BUFFERS; ++i)
	{
		compilerState->constantBuffers[i].active = MFM_FALSE;
		for (mfmU64 j = 0; j < MFG_V2X_MAX_CONSTANT_BUFFER_VARS; ++j)
			compilerState->constantBuffers[i].variables[j].active = MFM_FALSE;
	}
	for (mfmU64 i = 0; i < MFG_V2X_MAX_TEXTURE_1DS; ++i)
		compilerState->texture1Ds[i].active = MFM_FALSE;
	for (mfmU64 i = 0; i < MFG_V2X_MAX_TEXTURE_2DS; ++i)
		compilerState->texture2Ds[i].active = MFM_FALSE;
	for (mfmU64 i = 0; i < MFG_V2X_MAX_TEXTURE_3DS; ++i)
		compilerState->texture3Ds[i].active = MFM_FALSE;

	err = mfgParseProgram(&internalState);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

mfError mfgV2XPrintNode(mfsStream * stream, mfgV2XNode * node, mfmU64 indentation)
{
	if (stream == NULL || node == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;
	if (node->active == MFM_FALSE)
		return MFG_ERROR_INACTIVE_NODE;

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

	mfgV2XNode* c = node->first;
	while (c != NULL)
	{
		err = mfgV2XPrintNode(stream, c, indentation + 1);
		if (err != MF_ERROR_OKAY)
			return err;
		c = c->next;
	}

	return MF_ERROR_OKAY;
}
