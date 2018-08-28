#include "Generator.h"
#include "../../../Memory/Endianness.h"
#include "../../../String/StringStream.h"

#include <string.h>

typedef struct
{
	mfgV2XGeneratorState* state;
	mfgV2XCompilerState* compilerState;
	mfmU8* bytecode;
	mfmU8* metaData;
	mfmU64 maxBytecodeSize;
	mfmU64 maxMetaDataSize;
	mfgV2XEnum shaderType;
	mfmU16 nextVarIndex;
	mfmU16 nextBufIndex;
} mfgV2XGeneratorInternalState;

static mfError mfgBytecodePut8(mfgV2XGeneratorInternalState* state, const void* data)
{
	if (state == NULL || data == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	if (state->state->bytecodeSize + 1 >= state->maxBytecodeSize)
		return MFG_ERROR_BYTECODE_OVERFLOW;
	memcpy(state->bytecode + state->state->bytecodeSize, data, 1);
	state->state->bytecodeSize += 1;
	return MF_ERROR_OKAY;
}

static mfError mfgBytecodePut16(mfgV2XGeneratorInternalState* state, const void* data)
{
	if (state == NULL || data == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	if (state->state->bytecodeSize + 2 >= state->maxBytecodeSize)
		return MFG_ERROR_BYTECODE_OVERFLOW;
	mfmToBigEndian2(data, state->bytecode + state->state->bytecodeSize);
	state->state->bytecodeSize += 2;
	return MF_ERROR_OKAY;
}

static mfError mfgBytecodePut32(mfgV2XGeneratorInternalState* state, const void* data)
{
	if (state == NULL || data == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	if (state->state->bytecodeSize + 4 >= state->maxBytecodeSize)
		return MFG_ERROR_BYTECODE_OVERFLOW;
	mfmToBigEndian4(data, state->bytecode + state->state->bytecodeSize);
	state->state->bytecodeSize += 4;
	return MF_ERROR_OKAY;
}

static mfError mfgMetaDataPut8(mfgV2XGeneratorInternalState* state, const void* data)
{
	if (state == NULL || data == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	if (state->state->metaDataSize + 1 >= state->maxMetaDataSize)
		return MFG_ERROR_METADATA_OVERFLOW;
	memcpy(state->metaData + state->state->metaDataSize, data, 1);
	state->state->metaDataSize += 1;
	return MF_ERROR_OKAY;
}

static mfError mfgMetaDataPut16(mfgV2XGeneratorInternalState* state, const void* data)
{
	if (state == NULL || data == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	if (state->state->metaDataSize + 2 >= state->maxMetaDataSize)
		return MFG_ERROR_METADATA_OVERFLOW;
	mfmToBigEndian2(data, state->metaData + state->state->metaDataSize);
	state->state->metaDataSize += 2;
	return MF_ERROR_OKAY;
}

static mfError mfgMetaDataPut32(mfgV2XGeneratorInternalState* state, const void* data)
{
	if (state == NULL || data == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	if (state->state->metaDataSize + 4 >= state->maxMetaDataSize)
		return MFG_ERROR_METADATA_OVERFLOW;
	mfmToBigEndian4(data, state->metaData + state->state->metaDataSize);
	state->state->metaDataSize += 4;
	return MF_ERROR_OKAY;
}

static mfError mfgMetaDataPutN(mfgV2XGeneratorInternalState* state, const void* data, mfmU64 size)
{
	if (state == NULL || data == NULL || size == 0)
		return MFG_ERROR_INVALID_ARGUMENTS;

	if (state->state->metaDataSize + size >= state->maxMetaDataSize)
		return MFG_ERROR_METADATA_OVERFLOW;
	memcpy(state->metaData + state->state->metaDataSize, data, size);
	state->state->metaDataSize += size;
	return MF_ERROR_OKAY;
}

static mfError mfgGenerateExpression(mfgV2XGeneratorInternalState* state, mfgV2XNode* node, mfmU16 outVar, mfmU16* prodVar)
{
	if (state == NULL || node == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;
	mfmU8 u8T = 0;
	mfmU16 u16T = 0;

	u8T = MFG_BYTECODE_OPSCOPE;
	err = mfgBytecodePut8(state, &u8T);
	if (err != MF_ERROR_OKAY)
		return err;

	mfmU16 prevIndex = state->nextVarIndex;

	mfgV2XNode* term1 = node->first;
	mfgV2XNode* term2 = NULL;
	if (term1 != NULL)
		term2 = term1->next;

	switch (node->info->type)
	{
		case MFG_V2X_TOKEN_ASSIGN:
		{
			if (term1->info->type == MFG_V2X_TOKEN_REFERENCE)
			{
				err = mfgGenerateExpression(state, term2, term1->refIndex, NULL);
				if (err != MF_ERROR_OKAY)
					return err;
			}

			if (outVar != 0xFFFF)
			{
				u8T = MFG_BYTECODE_ASSIGN;
				err = mfgBytecodePut8(state, &u8T);
				if (err != MF_ERROR_OKAY)
					return err;

				u16T = outVar;
				err = mfgBytecodePut16(state, &u16T);
				if (err != MF_ERROR_OKAY)
					return err;

				u16T = term1->refIndex;
				err = mfgBytecodePut16(state, &u16T);
				if (err != MF_ERROR_OKAY)
					return err;
			}
			break;
		}

		case MFG_V2X_TOKEN_REFERENCE:
		{
			if (outVar != 0xFFFF)
			{
				u8T = MFG_BYTECODE_ASSIGN;
				err = mfgBytecodePut8(state, &u8T);
				if (err != MF_ERROR_OKAY)
					return err;

				u16T = outVar;
				err = mfgBytecodePut16(state, &u16T);
				if (err != MF_ERROR_OKAY)
					return err;

				u16T = node->refIndex;
				err = mfgBytecodePut16(state, &u16T);
				if (err != MF_ERROR_OKAY)
					return err;
			}
			break;
		}

		default:
		{
			mfsStringStream ss;
			mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
			mfsPrintFormatUTF8(&ss, u8"[mfgGenerateExpression] Failed to generate expression, unsupported operator type '%s'", node->info->name);
			mfsDestroyLocalStringStream(&ss);
			return MFG_ERROR_FAILED_TO_GENERATE_EXPRESSION;
		}
	}

	state->nextVarIndex = prevIndex;

	u8T = MFG_BYTECODE_CLSCOPE;
	err = mfgBytecodePut8(state, &u8T);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

static mfError mfgGenerateStatement(mfgV2XGeneratorInternalState* state, mfgV2XNode* node);

static mfError mfgGenerateCompoundStatement(mfgV2XGeneratorInternalState* state, mfgV2XNode* node)
{
	if (state == NULL || node == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;

	mfgV2XNode* statement = node->first;
	while (statement != NULL)
	{
		err = mfgGenerateStatement(state, statement);
		if (err != MF_ERROR_OKAY)
			return err;
		statement = statement->next;
	}

	return MF_ERROR_OKAY;
}

static mfError mfgGenerateStatement(mfgV2XGeneratorInternalState* state, mfgV2XNode* node)
{
	if (state == NULL || node == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;

	if (node->info->isOperator == MFM_TRUE)
	{
		err = mfgGenerateExpression(state, node, 0xFFFF, NULL);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	else if (node->info->type == MFG_V2X_TOKEN_COMPOUND_STATEMENT)
	{
		err = mfgGenerateCompoundStatement(state, node, 0xFFFF, NULL);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	else
	{
		mfsStringStream ss;
		mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
		mfsPrintFormatUTF8(&ss, u8"[mfgGenerateStatement] Failed to generate statement, unsupported node type '%s'", node->info->name);
		mfsDestroyLocalStringStream(&ss);
		return MFG_ERROR_FAILED_TO_GENERATE_STATEMENT;
	}

	return MF_ERROR_OKAY;
}

static mfError mfgGenerateFunction(mfgV2XGeneratorInternalState* state, mfgV2XNode* node)
{
	if (state == NULL || node == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;

	mfgV2XNode* type = node->first;
	if (type->info->type != MFG_V2X_TOKEN_VOID)
	{
		mfsStringStream ss;
		mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
		mfsPrintFormatUTF8(&ss, u8"[mfgGenerateFunction] Returning functions are unsupported for now (only 'void' functions are allowed)");
		mfsDestroyLocalStringStream(&ss);
		return MFG_ERROR_UNSUPPORTED_FEATURE;
	}

	mfgV2XNode* compoundStatement = node->first->next->next->next; // Ignore name, not neeeded for now
	err = mfgGenerateCompoundStatement(state, compoundStatement);
	if (err != MF_ERROR_OKAY)
		return err;
	return MF_ERROR_OKAY;
}

static mfError mfgGenerateBytecode(mfgV2XGeneratorInternalState* state, const mfgV2XNode* root)
{
	if (state == NULL || root == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;

	mfmU8 u8T = 0;
	mfmU16 u16T = 0;
	mfmU32 u32T = 0;

	// Write bytecode marker
	u8T = MFG_BYTECODE_HEADER_MARKER_0;
	err = mfgBytecodePut8(state, &u8T);
	if (err != MF_ERROR_OKAY)
		return err;
	u8T = MFG_BYTECODE_HEADER_MARKER_1;
	err = mfgBytecodePut8(state, &u8T);
	if (err != MF_ERROR_OKAY)
		return err;
	u8T = MFG_BYTECODE_HEADER_MARKER_2;
	err = mfgBytecodePut8(state, &u8T);
	if (err != MF_ERROR_OKAY)
		return err;
	u8T = MFG_BYTECODE_HEADER_MARKER_3;
	err = mfgBytecodePut8(state, &u8T);
	if (err != MF_ERROR_OKAY)
		return err;

	// Write bytecode version
	u8T = 2;
	err = mfgBytecodePut8(state, &u8T);
	if (err != MF_ERROR_OKAY)
		return err;
	u8T = 1;
	err = mfgBytecodePut8(state, &u8T);
	if (err != MF_ERROR_OKAY)
		return err;

	// Write entry function
	mfgV2XNode* node = root->first;
	while (node != NULL)
	{
		// Generate only main function (for now)
		if (node->info->type == MFG_V2X_TOKEN_FUNCTION)
			if (strcmp(node->first->next->attribute, u8"main") == 0)
			{
				err = mfgGenerateFunction(state, node);
				if (err != MF_ERROR_OKAY)
					return err;
			}
		node = node->next;
	}

	return MF_ERROR_OKAY;
}

static mfError mfgAnnotateStatement(mfgV2XGeneratorInternalState* state, mfgV2XNode* node);

static mfError mfgAnnotateExpression(mfgV2XGeneratorInternalState* state, mfgV2XNode* node)
{
	if (state == NULL || node == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;

	mfgV2XNode* term1 = node->first;
	mfgV2XNode* term2 = NULL;
	if (node->first != NULL)
		term2 = node->first->next;

	switch (node->info->type)
	{
		case MFG_V2X_TOKEN_ASSIGN:
			err = mfgAnnotateExpression(state, term1);
			if (err != MF_ERROR_OKAY)
				return err;
			err = mfgAnnotateExpression(state, term2);
			if (err != MF_ERROR_OKAY)
				return err;

			if (term1->isLValue == MFM_FALSE)
			{
				mfsStringStream ss;
				mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
				mfsPrintFormatUTF8(&ss, u8"[mfgAnnotateExpression] Assign expression first term must be an lvalue");
				mfsDestroyLocalStringStream(&ss);
				return MFG_ERROR_FAILED_TO_GENERATE_EXPRESSION;
			}

			return MF_ERROR_OKAY;

		case MFG_V2X_TOKEN_INT_LITERAL:
			node->isLValue = MFM_FALSE;
			node->isConstant = MFM_TRUE;
			node->returnType = MFG_V2X_TOKEN_INT1;
			return MF_ERROR_OKAY;

		case MFG_V2X_TOKEN_FLOAT_LITERAL:
			node->isLValue = MFM_FALSE;
			node->isConstant = MFM_TRUE;
			node->returnType = MFG_V2X_TOKEN_FLOAT1;
			return MF_ERROR_OKAY;

		case MFG_V2X_TOKEN_IDENTIFIER:
			node->isLValue = MFM_TRUE;
			node->isConstant = MFM_FALSE;
			node->returnType = 0;
			return MF_ERROR_OKAY;

		case MFG_V2X_TOKEN_MEMBER:
		{
			// Input struct member
			if (term1->info->type == MFG_V2X_TOKEN_INPUT)
			{
				node->info = &MFG_V2X_TINFO_REFERENCE;
				if (term2->info->type != MFG_V2X_TOKEN_IDENTIFIER)
				{
					mfsStringStream ss;
					mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
					mfsPrintFormatUTF8(&ss, u8"[mfgAnnotateExpression] Input struct member second term must be an identifier");
					mfsDestroyLocalStringStream(&ss);
					return MFG_ERROR_FAILED_TO_GENERATE_EXPRESSION;
				}

				for (mfmU32 i = 0; i < MFG_V2X_MAX_INPUT_VARS; ++i)
					if (strcmp(term2->attribute, state->compilerState->input.variables[i].id) == 0)
					{
						node->refIndex = state->compilerState->input.variables[i].index;
						node->isLValue = MFM_TRUE;
						node->isConstant = MFM_FALSE;
						node->returnType = state->compilerState->input.variables[i].type;
						return MF_ERROR_OKAY;
					}

				mfsStringStream ss;
				mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
				mfsPrintFormatUTF8(&ss, u8"[mfgAnnotateExpression] No input variable was found with the ID '%s'", term2->attribute);
				mfsDestroyLocalStringStream(&ss);
				return MFG_ERROR_FAILED_TO_GENERATE_EXPRESSION;
			}
			// Output struct member
			else if (term1->info->type == MFG_V2X_TOKEN_OUTPUT)
			{
				node->info = &MFG_V2X_TINFO_REFERENCE;
				if (term2->info->type != MFG_V2X_TOKEN_IDENTIFIER)
				{
					mfsStringStream ss;
					mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
					mfsPrintFormatUTF8(&ss, u8"[mfgAnnotateExpression] Output struct member second term must be an identifier");
					mfsDestroyLocalStringStream(&ss);
					return MFG_ERROR_FAILED_TO_GENERATE_EXPRESSION;
				}

				for (mfmU32 i = 0; i < MFG_V2X_MAX_OUTPUT_VARS; ++i)
					if (strcmp(term2->attribute, state->compilerState->output.variables[i].id) == 0)
					{
						node->refIndex = state->compilerState->output.variables[i].index;
						node->isLValue = MFM_TRUE;
						node->isConstant = MFM_FALSE;
						node->returnType = state->compilerState->output.variables[i].type;
						return MF_ERROR_OKAY;
					}

				mfsStringStream ss;
				mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
				mfsPrintFormatUTF8(&ss, u8"[mfgAnnotateExpression] No output variable was found with the ID '%s'", term2->attribute);
				mfsDestroyLocalStringStream(&ss);
				return MFG_ERROR_FAILED_TO_GENERATE_EXPRESSION;
			}
			else
			{
				err = mfgAnnotateExpression(state, term1);
				if (err != MF_ERROR_OKAY)
					return err;

				if (term1->info->type != MFG_V2X_TOKEN_REFERENCE)
				{
					mfsStringStream ss;
					mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
					mfsPrintFormatUTF8(&ss, u8"[mfgAnnotateExpression] Member expression first term must be a reference");
					mfsDestroyLocalStringStream(&ss);
					return MFG_ERROR_FAILED_TO_GENERATE_EXPRESSION;
				}

				err = mfgAnnotateExpression(state, term2);
				if (err != MF_ERROR_OKAY)
					return err;

				if (term2->info->type != MFG_V2X_TOKEN_IDENTIFIER)
				{
					mfsStringStream ss;
					mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
					mfsPrintFormatUTF8(&ss, u8"[mfgAnnotateExpression] Member expression second term must be an identifier");
					mfsDestroyLocalStringStream(&ss);
					return MFG_ERROR_FAILED_TO_GENERATE_EXPRESSION;
				}

				return MF_ERROR_OKAY;
			}
		}
	}
}

static mfError mfgAnnotateCompoundStatement(mfgV2XGeneratorInternalState* state, mfgV2XNode* node)
{
	if (state == NULL || node == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;

	node->isConstant = MFM_FALSE;
	node->isLValue = MFM_FALSE;
	node->returnType = 0;

	mfgV2XNode* statement = node->first;
	while (statement != NULL)
	{
		err = mfgAnnotateStatement(state, statement);
		if (err != MF_ERROR_OKAY)
			return err;
		statement = statement->next;
	}

	return MF_ERROR_OKAY;
}

static mfError mfgAnnotateStatement(mfgV2XGeneratorInternalState* state, mfgV2XNode* node)
{
	if (state == NULL || node == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;

	if (node->info->isOperator == MFM_TRUE)
	{
		err = mfgAnnotateExpression(state, node);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	else if (node->info->type == MFG_V2X_TOKEN_COMPOUND_STATEMENT)
	{
		err = mfgAnnotateCompoundStatement(state, node);
		if (err != MF_ERROR_OKAY)
			return err;
	}

	return MF_ERROR_OKAY;
}

static mfError mfgAnnotateFunction(mfgV2XGeneratorInternalState* state, mfgV2XNode* node)
{
	if (state == NULL || node == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;

	node->isConstant = MFM_FALSE;
	node->isLValue = MFM_FALSE;
	node->returnType = 0;

	err = mfgAnnotateCompoundStatement(state, node->first->next->next->next);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

static mfError mfgAnnotateProgram(mfgV2XGeneratorInternalState* state, const mfgV2XNode* root)
{
	if (state == NULL || root == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;

	// Annotate functions
	mfgV2XNode* node = root->first;
	while (node != NULL)
	{
		if (node->info->type == MFG_V2X_TOKEN_FUNCTION)
		{
			err = mfgAnnotateFunction(state, node);
			if (err != MF_ERROR_OKAY)
				return err;
		}
		node = node->next;
	}

	return MF_ERROR_OKAY;
}

static mfError mfgGenerateMetaData(mfgV2XGeneratorInternalState* state)
{
	mfError err;

	mfmU8 u8T = 0;
	mfmU16 u16T = 0;
	mfmU32 u32T = 0;

	// Write meta data marker
	u8T = MFG_METADATA_HEADER_MARKER_0;
	err = mfgMetaDataPut8(state, &u8T);
	if (err != MF_ERROR_OKAY)
		return err;
	u8T = MFG_METADATA_HEADER_MARKER_1;
	err = mfgMetaDataPut8(state, &u8T);
	if (err != MF_ERROR_OKAY)
		return err;
	u8T = MFG_METADATA_HEADER_MARKER_2;
	err = mfgMetaDataPut8(state, &u8T);
	if (err != MF_ERROR_OKAY)
		return err;
	u8T = MFG_METADATA_HEADER_MARKER_3;
	err = mfgMetaDataPut8(state, &u8T);
	if (err != MF_ERROR_OKAY)
		return err;

	// Write shader type
	u8T = state->shaderType;
	err = mfgMetaDataPut8(state, &u8T);
	if (err != MF_ERROR_OKAY)
		return err;

	// Write input variable count
	{
		u8T = 0;
		for (mfmU32 i = 0; i < MFG_V2X_MAX_INPUT_VARS; ++i)
			if (state->compilerState->input.variables[i].active == MFM_TRUE)
				++u8T;
		err = mfgMetaDataPut8(state, &u8T);
		if (err != MF_ERROR_OKAY)
			return err;
	}

	// Write output variable count
	{
		u8T = 0;
		for (mfmU32 i = 0; i < MFG_V2X_MAX_OUTPUT_VARS; ++i)
			if (state->compilerState->output.variables[i].active == MFM_TRUE)
				++u8T;
		err = mfgMetaDataPut8(state, &u8T);
		if (err != MF_ERROR_OKAY)
			return err;
	}

	// Write binding point count
	{
		u8T = 0;
		for (mfmU32 i = 0; i < MFG_V2X_MAX_CONSTANT_BUFFERS; ++i)
			if (state->compilerState->constantBuffers[i].active == MFM_TRUE)
				++u8T;
		for (mfmU32 i = 0; i < MFG_V2X_MAX_TEXTURE_1DS; ++i)
			if (state->compilerState->texture1Ds[i].active == MFM_TRUE)
				++u8T;
		for (mfmU32 i = 0; i < MFG_V2X_MAX_TEXTURE_2DS; ++i)
			if (state->compilerState->texture2Ds[i].active == MFM_TRUE)
				++u8T;
		for (mfmU32 i = 0; i < MFG_V2X_MAX_TEXTURE_3DS; ++i)
			if (state->compilerState->texture3Ds[i].active == MFM_TRUE)
				++u8T;
		err = mfgMetaDataPut8(state, &u8T);
		if (err != MF_ERROR_OKAY)
			return err;
	}

	// Write input variables
	{
		for (mfmU32 i = 0; i < MFG_V2X_MAX_INPUT_VARS; ++i)
			if (state->compilerState->input.variables[i].active == MFM_TRUE)
			{
				// Write variable name
				err = mfgMetaDataPutN(state, state->compilerState->input.variables[i].name, 15);
				if (err != MF_ERROR_OKAY)
					return err;
				u8T = '\0';
				err = mfgMetaDataPut8(state, &u8T);
				if (err != MF_ERROR_OKAY)
					return err;

				// Get and write variable index
				state->compilerState->input.variables[i].index = state->nextVarIndex++;
				err = mfgMetaDataPut16(state, &state->compilerState->input.variables[i].index);
				if (err != MF_ERROR_OKAY)
					return err;

				// Write variable type
				switch (state->compilerState->input.variables[i].type)
				{
					case MFG_V2X_TOKEN_INT1: u8T = MFG_INT1; break;
					case MFG_V2X_TOKEN_INT2: u8T = MFG_INT2; break;
					case MFG_V2X_TOKEN_INT3: u8T = MFG_INT3; break;
					case MFG_V2X_TOKEN_INT4: u8T = MFG_INT4; break;
					case MFG_V2X_TOKEN_INT22: u8T = MFG_INT22; break;
					case MFG_V2X_TOKEN_INT33: u8T = MFG_INT33; break;
					case MFG_V2X_TOKEN_INT44: u8T = MFG_INT44; break;

					case MFG_V2X_TOKEN_FLOAT1: u8T = MFG_FLOAT1; break;
					case MFG_V2X_TOKEN_FLOAT2: u8T = MFG_FLOAT2; break;
					case MFG_V2X_TOKEN_FLOAT3: u8T = MFG_FLOAT3; break;
					case MFG_V2X_TOKEN_FLOAT4: u8T = MFG_FLOAT4; break;
					case MFG_V2X_TOKEN_FLOAT22: u8T = MFG_FLOAT22; break;
					case MFG_V2X_TOKEN_FLOAT33: u8T = MFG_FLOAT33; break;
					case MFG_V2X_TOKEN_FLOAT44: u8T = MFG_FLOAT44; break;

					default:
					{
						mfsStringStream ss;
						mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
						mfsPrintFormatUTF8(&ss, u8"[mfgGenerateMetaData] Failed to generate meta data, unsupported input variable type");
						mfsDestroyLocalStringStream(&ss);
						return MFG_ERROR_TOO_MANY_VARIABLES;
						break;
					}
				}

				err = mfgMetaDataPut8(state, &u8T);
				if (err != MF_ERROR_OKAY)
					return err;
			}
	}

	// Write output variables
	{
		for (mfmU32 i = 0; i < MFG_V2X_MAX_OUTPUT_VARS; ++i)
			if (state->compilerState->output.variables[i].active == MFM_TRUE)
			{
				// Write variable name
				err = mfgMetaDataPutN(state, state->compilerState->output.variables[i].name, 15);
				if (err != MF_ERROR_OKAY)
					return err;
				u8T = '\0';
				err = mfgMetaDataPut8(state, &u8T);
				if (err != MF_ERROR_OKAY)
					return err;

				// Get and write variable index
				state->compilerState->output.variables[i].index = state->nextVarIndex++;
				err = mfgMetaDataPut16(state, &state->compilerState->output.variables[i].index);
				if (err != MF_ERROR_OKAY)
					return err;

				// Write variable type
				switch (state->compilerState->output.variables[i].type)
				{
					case MFG_V2X_TOKEN_INT1: u8T = MFG_INT1; break;
					case MFG_V2X_TOKEN_INT2: u8T = MFG_INT2; break;
					case MFG_V2X_TOKEN_INT3: u8T = MFG_INT3; break;
					case MFG_V2X_TOKEN_INT4: u8T = MFG_INT4; break;
					case MFG_V2X_TOKEN_INT22: u8T = MFG_INT22; break;
					case MFG_V2X_TOKEN_INT33: u8T = MFG_INT33; break;
					case MFG_V2X_TOKEN_INT44: u8T = MFG_INT44; break;

					case MFG_V2X_TOKEN_FLOAT1: u8T = MFG_FLOAT1; break;
					case MFG_V2X_TOKEN_FLOAT2: u8T = MFG_FLOAT2; break;
					case MFG_V2X_TOKEN_FLOAT3: u8T = MFG_FLOAT3; break;
					case MFG_V2X_TOKEN_FLOAT4: u8T = MFG_FLOAT4; break;
					case MFG_V2X_TOKEN_FLOAT22: u8T = MFG_FLOAT22; break;
					case MFG_V2X_TOKEN_FLOAT33: u8T = MFG_FLOAT33; break;
					case MFG_V2X_TOKEN_FLOAT44: u8T = MFG_FLOAT44; break;

					default:
					{
						mfsStringStream ss;
						mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
						mfsPrintFormatUTF8(&ss, u8"[mfgGenerateMetaData] Failed to generate meta data, unsupported output variable type");
						mfsDestroyLocalStringStream(&ss);
						return MFG_ERROR_TOO_MANY_VARIABLES;
						break;
					}
				}

				err = mfgMetaDataPut8(state, &u8T);
				if (err != MF_ERROR_OKAY)
					return err;
			}
	}


	// Write texture 1Ds
	{
		for (mfmU32 i = 0; i < MFG_V2X_MAX_TEXTURE_1DS; ++i)
			if (state->compilerState->texture1Ds[i].active == MFM_TRUE)
			{
				// Write texture name
				err = mfgMetaDataPutN(state, state->compilerState->texture1Ds[i].name, 15);
				if (err != MF_ERROR_OKAY)
					return err;
				u8T = '\0';
				err = mfgMetaDataPut8(state, &u8T);
				if (err != MF_ERROR_OKAY)
					return err;

				// Write binding point type
				u8T = MFG_TEXTURE_1D;
				err = mfgMetaDataPut8(state, &u8T);
				if (err != MF_ERROR_OKAY)
					return err;

				// Get and write texture index
				state->compilerState->texture1Ds[i].index = state->nextVarIndex++;
				err = mfgMetaDataPut16(state, &state->compilerState->texture1Ds[i].index);
				if (err != MF_ERROR_OKAY)
					return err;
			}
	}

	// Write texture 2Ds
	{
		for (mfmU32 i = 0; i < MFG_V2X_MAX_TEXTURE_2DS; ++i)
			if (state->compilerState->texture2Ds[i].active == MFM_TRUE)
			{
				// Write texture name
				err = mfgMetaDataPutN(state, state->compilerState->texture2Ds[i].name, 15);
				if (err != MF_ERROR_OKAY)
					return err;
				u8T = '\0';
				err = mfgMetaDataPut8(state, &u8T);
				if (err != MF_ERROR_OKAY)
					return err;

				// Write binding point type
				u8T = MFG_TEXTURE_2D;
				err = mfgMetaDataPut8(state, &u8T);
				if (err != MF_ERROR_OKAY)
					return err;

				// Get and write texture index
				state->compilerState->texture2Ds[i].index = state->nextVarIndex++;
				err = mfgMetaDataPut16(state, &state->compilerState->texture2Ds[i].index);
				if (err != MF_ERROR_OKAY)
					return err;
			}
	}

	// Write texture 3Ds
	{
		for (mfmU32 i = 0; i < MFG_V2X_MAX_TEXTURE_3DS; ++i)
			if (state->compilerState->texture3Ds[i].active == MFM_TRUE)
			{
				// Write texture name
				err = mfgMetaDataPutN(state, state->compilerState->texture3Ds[i].name, 15);
				if (err != MF_ERROR_OKAY)
					return err;
				u8T = '\0';
				err = mfgMetaDataPut8(state, &u8T);
				if (err != MF_ERROR_OKAY)
					return err;

				// Write binding point type
				u8T = MFG_TEXTURE_3D;
				err = mfgMetaDataPut8(state, &u8T);
				if (err != MF_ERROR_OKAY)
					return err;

				// Get and write texture index
				state->compilerState->texture3Ds[i].index = state->nextVarIndex++;
				err = mfgMetaDataPut16(state, &state->compilerState->texture3Ds[i].index);
				if (err != MF_ERROR_OKAY)
					return err;
			}
	}

	// Write constant buffers
	{
		for (mfmU32 i = 0; i < MFG_V2X_MAX_CONSTANT_BUFFERS; ++i)
			if (state->compilerState->constantBuffers[i].active == MFM_TRUE)
			{
				// Write constant buffer name
				err = mfgMetaDataPutN(state, state->compilerState->constantBuffers[i].name, 15);
				if (err != MF_ERROR_OKAY)
					return err;
				u8T = '\0';
				err = mfgMetaDataPut8(state, &u8T);
				if (err != MF_ERROR_OKAY)
					return err;

				// Write binding point type
				u8T = MFG_CONSTANT_BUFFER;
				err = mfgMetaDataPut8(state, &u8T);
				if (err != MF_ERROR_OKAY)
					return err;

				// Get and write buffer index
				state->compilerState->constantBuffers[i].index = state->nextBufIndex++;
				err = mfgMetaDataPut16(state, &state->compilerState->constantBuffers[i].index);
				if (err != MF_ERROR_OKAY)
					return err;

				// Write variable count
				u16T = 0;
				for (mfmU32 j = 0; j < MFG_V2X_MAX_CONSTANT_BUFFER_VARS; ++j)
					if (state->compilerState->constantBuffers[i].variables[j].active == MFM_TRUE)
						++u16T;
				err = mfgMetaDataPut16(state, &u16T);
				if (err != MF_ERROR_OKAY)
					return err;

				// Write variables
				for (mfmU32 j = 0; j < MFG_V2X_MAX_CONSTANT_BUFFER_VARS; ++j)
					if (state->compilerState->constantBuffers[i].variables[j].active == MFM_TRUE)
					{
						// Write variable index
						state->compilerState->constantBuffers[i].variables[j].index = state->nextVarIndex++;
						err = mfgMetaDataPut16(state, &state->compilerState->constantBuffers[i].variables[j].index);
						if (err != MF_ERROR_OKAY)
							return err;

						// Write array size
						u16T = state->compilerState->constantBuffers[i].variables[j].arraySize;
						err = mfgMetaDataPut16(state, &u16T);
						if (err != MF_ERROR_OKAY)
							return err;

						// Write variable type
						switch (state->compilerState->output.variables[i].type)
						{
							case MFG_V2X_TOKEN_INT1: u8T = MFG_INT1; break;
							case MFG_V2X_TOKEN_INT2: u8T = MFG_INT2; break;
							case MFG_V2X_TOKEN_INT3: u8T = MFG_INT3; break;
							case MFG_V2X_TOKEN_INT4: u8T = MFG_INT4; break;
							case MFG_V2X_TOKEN_INT22: u8T = MFG_INT22; break;
							case MFG_V2X_TOKEN_INT33: u8T = MFG_INT33; break;
							case MFG_V2X_TOKEN_INT44: u8T = MFG_INT44; break;

							case MFG_V2X_TOKEN_FLOAT1: u8T = MFG_FLOAT1; break;
							case MFG_V2X_TOKEN_FLOAT2: u8T = MFG_FLOAT2; break;
							case MFG_V2X_TOKEN_FLOAT3: u8T = MFG_FLOAT3; break;
							case MFG_V2X_TOKEN_FLOAT4: u8T = MFG_FLOAT4; break;
							case MFG_V2X_TOKEN_FLOAT22: u8T = MFG_FLOAT22; break;
							case MFG_V2X_TOKEN_FLOAT33: u8T = MFG_FLOAT33; break;
							case MFG_V2X_TOKEN_FLOAT44: u8T = MFG_FLOAT44; break;

							default:
							{
								mfsStringStream ss;
								mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
								mfsPrintFormatUTF8(&ss, u8"[mfgGenerateMetaData] Failed to generate meta data, unsupported constant buffer variable type");
								mfsDestroyLocalStringStream(&ss);
								return MFG_ERROR_TOO_MANY_VARIABLES;
								break;
							}
						}

						err = mfgMetaDataPut8(state, &u8T);
						if (err != MF_ERROR_OKAY)
							return err;
					}
			}
	}

	return MF_ERROR_OKAY;
}

mfError mfgV2XRunMVLGenerator(const mfgV2XNode * root, mfmU8 * bytecode, mfmU64 maxBytecodeSize, mfmU8 * metaData, mfmU64 maxMetaDataSize, mfgV2XGeneratorState* state, const mfgV2XCompilerState * compilerState, mfgV2XEnum shaderType)
{
	if (root == NULL || bytecode == NULL || maxBytecodeSize == 0 || metaData == NULL || maxMetaDataSize == 0 || compilerState == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;

	mfgV2XGeneratorInternalState internalState;
	internalState.state = state;
	internalState.compilerState = compilerState;
	internalState.bytecode = bytecode;
	internalState.metaData = metaData;
	internalState.maxBytecodeSize = maxBytecodeSize;
	internalState.maxMetaDataSize = maxMetaDataSize;
	internalState.shaderType = shaderType;
	internalState.nextVarIndex = 0;
	internalState.nextBufIndex = 0;

	state->bytecodeSize = 0;
	state->metaDataSize = 0;

	err = mfgGenerateMetaData(&internalState);
	if (err != MF_ERROR_OKAY)
		return err;
	err = mfgAnnotateProgram(&internalState, root);
	if (err != MF_ERROR_OKAY)
		return err;
	mfgV2XPrintNode(mfsOutStream, root, 0);
	err = mfgGenerateBytecode(&internalState, root);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}
