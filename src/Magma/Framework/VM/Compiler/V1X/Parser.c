#include "Parser.h"
#include "../../../String/StringStream.h"

#include <string.h>

typedef struct
{
	mfvV1XLexerState* state;
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
			++state->state->tokenCount;
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
	--state->state->tokenCount;
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

		err = mfvExpectTokenType(state, &MFV_V1X_TINFO_CLOSE_PARENTHESIS, NULL);
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
		err = mfvPeekToken(state, &node);
		if (err != MF_ERROR_OKAY)
			return err;
		if (node == NULL)
			break;

		// Check if it is a function
		if (node->info->isType == MFM_TRUE)
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
