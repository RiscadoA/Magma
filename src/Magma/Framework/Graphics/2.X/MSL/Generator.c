#include "Generator.h"
#include "../../../Memory/Endianness.h"
#include "../../../String/StringStream.h"

#include <string.h>
#include <stdlib.h>

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

static mfError mfgDeclareType(mfgV2XGeneratorInternalState* state, mfgV2XEnum type, mfmU16 index)
{
	if (state == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;
	mfmU8 u8T = 0;

	switch (type)
	{
		case MFG_V2X_TOKEN_INT1: u8T = MFG_BYTECODE_DECLI1; break;
		case MFG_V2X_TOKEN_INT2: u8T = MFG_BYTECODE_DECLI2; break;
		case MFG_V2X_TOKEN_INT3: u8T = MFG_BYTECODE_DECLI3; break;
		case MFG_V2X_TOKEN_INT4: u8T = MFG_BYTECODE_DECLI4; break;
		case MFG_V2X_TOKEN_INT22: u8T = MFG_BYTECODE_DECLI22; break;
		case MFG_V2X_TOKEN_INT33: u8T = MFG_BYTECODE_DECLI33; break;
		case MFG_V2X_TOKEN_INT44: u8T = MFG_BYTECODE_DECLI44; break;

		case MFG_V2X_TOKEN_FLOAT1: u8T = MFG_BYTECODE_DECLF1; break;
		case MFG_V2X_TOKEN_FLOAT2: u8T = MFG_BYTECODE_DECLF2; break;
		case MFG_V2X_TOKEN_FLOAT3: u8T = MFG_BYTECODE_DECLF3; break;
		case MFG_V2X_TOKEN_FLOAT4: u8T = MFG_BYTECODE_DECLF4; break;
		case MFG_V2X_TOKEN_FLOAT22: u8T = MFG_BYTECODE_DECLF22; break;
		case MFG_V2X_TOKEN_FLOAT33: u8T = MFG_BYTECODE_DECLF33; break;
		case MFG_V2X_TOKEN_FLOAT44: u8T = MFG_BYTECODE_DECLF44; break;

		case MFG_V2X_TOKEN_BOOL: u8T = MFG_BYTECODE_DECLB1; break;

		default:
		{
			mfsStringStream ss;
			mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
			mfsPrintFormatUTF8(&ss, u8"[mfgDeclareType] Unsupported variable type");
			mfsDestroyLocalStringStream(&ss);
			return MFG_ERROR_UNSUPPORTED_TYPE;
		}
	}

	err = mfgBytecodePut8(state, &u8T);
	if (err != MF_ERROR_OKAY)
		return err;

	err = mfgBytecodePut16(state, &index);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

static mfError mfgGetComponent(mfgV2XGeneratorInternalState* state, mfgV2XEnum type, mfmU16 varID, mfmU16 outID, mfmU8 index)
{
	if (state == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;
	mfmU8 u8T = 0;

	switch (type)
	{
		case MFG_V2X_TOKEN_INT2: u8T = MFG_BYTECODE_GET2CMP; break;
		case MFG_V2X_TOKEN_INT3: u8T = MFG_BYTECODE_GET3CMP; break;
		case MFG_V2X_TOKEN_INT4: u8T = MFG_BYTECODE_GET4CMP; break;

		case MFG_V2X_TOKEN_FLOAT2: u8T = MFG_BYTECODE_GET2CMP; break;
		case MFG_V2X_TOKEN_FLOAT3: u8T = MFG_BYTECODE_GET3CMP; break;
		case MFG_V2X_TOKEN_FLOAT4: u8T = MFG_BYTECODE_GET4CMP; break;

		default:
		{
			mfsStringStream ss;
			mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
			mfsPrintFormatUTF8(&ss, u8"[mfgGetComponent] Unsupported variable type");
			mfsDestroyLocalStringStream(&ss);
			return MFG_ERROR_UNSUPPORTED_TYPE;
		}
	}

	err = mfgBytecodePut8(state, &u8T);
	if (err != MF_ERROR_OKAY)
		return err;

	err = mfgBytecodePut16(state, &outID);
	if (err != MF_ERROR_OKAY)
		return err;

	err = mfgBytecodePut16(state, &varID);
	if (err != MF_ERROR_OKAY)
		return err;

	err = mfgBytecodePut8(state, &index);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

static mfError mfgAccessArray(mfgV2XGeneratorInternalState* state, mfmU16 varID, mfmU16 outID, mfmU16 indexID)
{
	if (state == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;

	mfmU8 u8T = MFG_BYTECODE_GETACMP;
	err = mfgBytecodePut8(state, &u8T);
	if (err != MF_ERROR_OKAY)
		return err;

	err = mfgBytecodePut16(state, &outID);
	if (err != MF_ERROR_OKAY)
		return err;

	err = mfgBytecodePut16(state, &varID);
	if (err != MF_ERROR_OKAY)
		return err;

	err = mfgBytecodePut16(state, &indexID);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}

static mfError mfgGenerateExpression(mfgV2XGeneratorInternalState* state, mfgV2XNode* node, mfmU16 outVar, mfmU16* prodVar)
{
	if (state == NULL || node == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;
	mfmU8 u8T = 0;
	mfmU16 u16T = 0;
	mfmU32 u32T = 0;

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
				err = mfgGenerateExpression(state, term2, term1->ref.varID, NULL);
				if (err != MF_ERROR_OKAY)
					return err;

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

					u16T = term1->ref.varID;
					err = mfgBytecodePut16(state, &u16T);
					if (err != MF_ERROR_OKAY)
						return err;
				}
			}
			
			else if (term1->info->type == MFG_V2X_TOKEN_CMP_REFERENCE)
			{
				if (term1->first == NULL)
				{
					u8T = MFG_BYTECODE_OPSCOPE;
					err = mfgBytecodePut8(state, &u8T);
					if (err != MF_ERROR_OKAY)
						return err;
					// Get component reference
					err = mfgGetComponent(state, term1->ref.type, term1->ref.varID, term1->ref.cmpVarID, term1->ref.cmpIndex);
					if (err != MF_ERROR_OKAY)
						return err;
					// Set component value
					err = mfgGenerateExpression(state, term2, term1->ref.cmpVarID, NULL);
					if (err != MF_ERROR_OKAY)
						return err;
					// Set out value
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

						u16T = term1->ref.cmpVarID;
						err = mfgBytecodePut16(state, &u16T);
						if (err != MF_ERROR_OKAY)
							return err;
					}
					u8T = MFG_BYTECODE_CLSCOPE;
					err = mfgBytecodePut8(state, &u8T);
					if (err != MF_ERROR_OKAY)
						return err;
				}
				else
				{
					u8T = MFG_BYTECODE_OPSCOPE;
					err = mfgBytecodePut8(state, &u8T);
					if (err != MF_ERROR_OKAY)
						return err;
					// Get first term value
					mfmU16 term1Temp;
					err = mfgDeclareType(state, term1->first->returnType, term1Temp);
					if (err != MF_ERROR_OKAY)
						return err;
					err = mfgGenerateExpression(state, term1->first, term1Temp, NULL);
					if (err != MF_ERROR_OKAY)
						return err;
					// Get component reference
					err = mfgGetComponent(state, term1->ref.type, term1Temp, term1->ref.cmpVarID, term1->ref.cmpIndex);
					if (err != MF_ERROR_OKAY)
						return err;
					// Set component value
					err = mfgGenerateExpression(state, term2, term1->ref.cmpVarID, NULL);
					if (err != MF_ERROR_OKAY)
						return err;
					// Set out value
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

						u16T = term1->ref.cmpVarID;
						err = mfgBytecodePut16(state, &u16T);
						if (err != MF_ERROR_OKAY)
							return err;
					}
					u8T = MFG_BYTECODE_CLSCOPE;
					err = mfgBytecodePut8(state, &u8T);
					if (err != MF_ERROR_OKAY)
						return err;
				}
			}

			break;
		}

		case MFG_V2X_TOKEN_ADD:
		{
			if (outVar != 0xFFFF)
			{
				if (term2 == NULL)
				{
					err = mfgGenerateExpression(state, term1, outVar, NULL);
					if (err != MF_ERROR_OKAY)
						return err;
				}
				else
				{
					u8T = MFG_BYTECODE_OPSCOPE;
					err = mfgBytecodePut8(state, &u8T);
					if (err != MF_ERROR_OKAY)
						return err;

					// Get first term value
					mfmU16 term1Temp = state->nextVarIndex++;
					err = mfgDeclareType(state, term1->returnType, term1Temp);
					if (err != MF_ERROR_OKAY)
						return err;
					err = mfgGenerateExpression(state, term1, term1Temp, NULL);
					if (err != MF_ERROR_OKAY)
						return err;

					// Get second term value
					mfmU16 term2Temp = state->nextVarIndex++;
					err = mfgDeclareType(state, term2->returnType, term2Temp);
					if (err != MF_ERROR_OKAY)
						return err;
					err = mfgGenerateExpression(state, term2, term2Temp, NULL);
					if (err != MF_ERROR_OKAY)
						return err;

					// Perform operation
					u8T = MFG_BYTECODE_ADD;
					err = mfgBytecodePut8(state, &u8T);
					if (err != MF_ERROR_OKAY)
						return err;

					u16T = term1Temp;
					err = mfgBytecodePut16(state, &u16T);
					if (err != MF_ERROR_OKAY)
						return err;

					u16T = term2Temp;
					err = mfgBytecodePut16(state, &u16T);
					if (err != MF_ERROR_OKAY)
						return err;

					u16T = outVar;
					err = mfgBytecodePut16(state, &u16T);
					if (err != MF_ERROR_OKAY)
						return err;

					u8T = MFG_BYTECODE_CLSCOPE;
					err = mfgBytecodePut8(state, &u8T);
					if (err != MF_ERROR_OKAY)
						return err;
				}
			}
			break;
		}

		case MFG_V2X_TOKEN_SUBTRACT:
		{
			if (outVar != 0xFFFF)
			{
				u8T = MFG_BYTECODE_OPSCOPE;
				err = mfgBytecodePut8(state, &u8T);
				if (err != MF_ERROR_OKAY)
					return err;

				mfmU16 term1Temp = 0;
				mfmU16 term2Temp = 0;

				
				if (term2 != NULL)
				{
					// Get first term value
					term1Temp = state->nextVarIndex++;
					err = mfgDeclareType(state, term1->returnType, term1Temp);
					if (err != MF_ERROR_OKAY)
						return err;
					err = mfgGenerateExpression(state, term1, term1Temp, NULL);
					if (err != MF_ERROR_OKAY)
						return err;

					// Get second term value
					term2Temp = state->nextVarIndex++;
					err = mfgDeclareType(state, term2->returnType, term2Temp);
					if (err != MF_ERROR_OKAY)
						return err;
					err = mfgGenerateExpression(state, term2, term2Temp, NULL);
					if (err != MF_ERROR_OKAY)
						return err;

					// Perform operation
					u8T = MFG_BYTECODE_SUBTRACT;
					err = mfgBytecodePut8(state, &u8T);
					if (err != MF_ERROR_OKAY)
						return err;

					u16T = term1Temp;
					err = mfgBytecodePut16(state, &u16T);
					if (err != MF_ERROR_OKAY)
						return err;

					u16T = term2Temp;
					err = mfgBytecodePut16(state, &u16T);
					if (err != MF_ERROR_OKAY)
						return err;

					u16T = outVar;
					err = mfgBytecodePut16(state, &u16T);
					if (err != MF_ERROR_OKAY)
						return err;
				}
				else
				{
					// Get first term value
					term1Temp = state->nextVarIndex++;
					err = mfgDeclareType(state, term1->returnType, term1Temp);
					if (err != MF_ERROR_OKAY)
						return err;
					err = mfgGenerateExpression(state, term1, term1Temp, NULL);
					if (err != MF_ERROR_OKAY)
						return err;

					// Perform operation
					u8T = MFG_BYTECODE_NEGATE;
					err = mfgBytecodePut8(state, &u8T);
					if (err != MF_ERROR_OKAY)
						return err;

					u16T = term1Temp;
					err = mfgBytecodePut16(state, &u16T);
					if (err != MF_ERROR_OKAY)
						return err;

					u16T = outVar;
					err = mfgBytecodePut16(state, &u16T);
					if (err != MF_ERROR_OKAY)
						return err;
				}			

				u8T = MFG_BYTECODE_CLSCOPE;
				err = mfgBytecodePut8(state, &u8T);
				if (err != MF_ERROR_OKAY)
					return err;
			}
			break;
		}

		case MFG_V2X_TOKEN_MULTIPLY:
		{
			if (outVar != 0xFFFF)
			{
				u8T = MFG_BYTECODE_OPSCOPE;
				err = mfgBytecodePut8(state, &u8T);
				if (err != MF_ERROR_OKAY)
					return err;

				// Get first term value
				mfmU16 term1Temp = state->nextVarIndex++;
				err = mfgDeclareType(state, term1->returnType, term1Temp);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfgGenerateExpression(state, term1, term1Temp, NULL);
				if (err != MF_ERROR_OKAY)
					return err;

				// Get second term value
				mfmU16 term2Temp = state->nextVarIndex++;
				err = mfgDeclareType(state, term2->returnType, term2Temp);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfgGenerateExpression(state, term2, term2Temp, NULL);
				if (err != MF_ERROR_OKAY)
					return err;

				// Perform operation
				u8T = MFG_BYTECODE_MULTIPLY;
				err = mfgBytecodePut8(state, &u8T);
				if (err != MF_ERROR_OKAY)
					return err;

				u16T = term1Temp;
				err = mfgBytecodePut16(state, &u16T);
				if (err != MF_ERROR_OKAY)
					return err;

				u16T = term2Temp;
				err = mfgBytecodePut16(state, &u16T);
				if (err != MF_ERROR_OKAY)
					return err;

				u16T = outVar;
				err = mfgBytecodePut16(state, &u16T);
				if (err != MF_ERROR_OKAY)
					return err;

				u8T = MFG_BYTECODE_CLSCOPE;
				err = mfgBytecodePut8(state, &u8T);
				if (err != MF_ERROR_OKAY)
					return err;
			}
			break;
		}

		case MFG_V2X_TOKEN_DIVIDE:
		{
			if (outVar != 0xFFFF)
			{
				u8T = MFG_BYTECODE_OPSCOPE;
				err = mfgBytecodePut8(state, &u8T);
				if (err != MF_ERROR_OKAY)
					return err;

				// Get first term value
				mfmU16 term1Temp = state->nextVarIndex++;
				err = mfgDeclareType(state, term1->returnType, term1Temp);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfgGenerateExpression(state, term1, term1Temp, NULL);
				if (err != MF_ERROR_OKAY)
					return err;

				// Get second term value
				mfmU16 term2Temp = state->nextVarIndex++;
				err = mfgDeclareType(state, term2->returnType, term2Temp);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfgGenerateExpression(state, term2, term2Temp, NULL);
				if (err != MF_ERROR_OKAY)
					return err;

				// Perform operation
				u8T = MFG_BYTECODE_DIVIDE;
				err = mfgBytecodePut8(state, &u8T);
				if (err != MF_ERROR_OKAY)
					return err;

				u16T = term1Temp;
				err = mfgBytecodePut16(state, &u16T);
				if (err != MF_ERROR_OKAY)
					return err;

				u16T = term2Temp;
				err = mfgBytecodePut16(state, &u16T);
				if (err != MF_ERROR_OKAY)
					return err;

				u16T = outVar;
				err = mfgBytecodePut16(state, &u16T);
				if (err != MF_ERROR_OKAY)
					return err;

				u8T = MFG_BYTECODE_CLSCOPE;
				err = mfgBytecodePut8(state, &u8T);
				if (err != MF_ERROR_OKAY)
					return err;
			}
			break;
		}

		case MFG_V2X_TOKEN_NOT:
		{
			if (outVar != 0xFFFF)
			{
				u8T = MFG_BYTECODE_OPSCOPE;
				err = mfgBytecodePut8(state, &u8T);
				if (err != MF_ERROR_OKAY)
					return err;

				mfmU16 term1Temp = 0;

				// Get term value
				term1Temp = state->nextVarIndex++;
				err = mfgDeclareType(state, term1->returnType, term1Temp);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfgGenerateExpression(state, term1, term1Temp, NULL);
				if (err != MF_ERROR_OKAY)
					return err;

				// Perform operation
				u8T = MFG_BYTECODE_NOT;
				err = mfgBytecodePut8(state, &u8T);
				if (err != MF_ERROR_OKAY)
					return err;

				u16T = term1Temp;
				err = mfgBytecodePut16(state, &u16T);
				if (err != MF_ERROR_OKAY)
					return err;

				u16T = outVar;
				err = mfgBytecodePut16(state, &u16T);
				if (err != MF_ERROR_OKAY)
					return err;

				u8T = MFG_BYTECODE_CLSCOPE;
				err = mfgBytecodePut8(state, &u8T);
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

				u16T = node->ref.varID;
				err = mfgBytecodePut16(state, &u16T);
				if (err != MF_ERROR_OKAY)
					return err;
			}
			break;
		}

		case MFG_V2X_TOKEN_CMP_REFERENCE:
		{
			if (outVar != 0xFFFF)
			{
				if (node->first == NULL)
				{
					// Get component reference
					err = mfgGetComponent(state, node->ref.type, node->ref.varID, node->ref.cmpVarID, node->ref.cmpIndex);
					if (err != MF_ERROR_OKAY)
						return err;

					// Assign output to component
					u8T = MFG_BYTECODE_ASSIGN;
					err = mfgBytecodePut8(state, &u8T);
					if (err != MF_ERROR_OKAY)
						return err;

					u16T = outVar;
					err = mfgBytecodePut16(state, &u16T);
					if (err != MF_ERROR_OKAY)
						return err;

					u16T = node->ref.cmpVarID;
					err = mfgBytecodePut16(state, &u16T);
					if (err != MF_ERROR_OKAY)
						return err;
				}
				else
				{
					// Calculate temp
					mfmU16 temp;
					err = mfgDeclareType(state, node->first->returnType, temp);
					if (err != MF_ERROR_OKAY)
						return err;
					err = mfgGenerateExpression(state, node->first, temp, NULL);
					if (err != MF_ERROR_OKAY)
						return err;

					// Get component reference
					err = mfgGetComponent(state, node->ref.type, temp, node->ref.cmpVarID, node->ref.cmpIndex);
					if (err != MF_ERROR_OKAY)
						return err;

					// Assign output to component
					u8T = MFG_BYTECODE_ASSIGN;
					err = mfgBytecodePut8(state, &u8T);
					if (err != MF_ERROR_OKAY)
						return err;

					u16T = outVar;
					err = mfgBytecodePut16(state, &u16T);
					if (err != MF_ERROR_OKAY)
						return err;

					u16T = node->ref.cmpVarID;
					err = mfgBytecodePut16(state, &u16T);
					if (err != MF_ERROR_OKAY)
						return err;
				}
				
			}
			break;
		}


		case MFG_V2X_TOKEN_ARRAY_REFERENCE:
		{
			if (outVar != 0xFFFF)
			{
				u8T = MFG_BYTECODE_OPSCOPE;
				err = mfgBytecodePut8(state, &u8T);
				if (err != MF_ERROR_OKAY)
					return err;

				// Get index
				mfmU16 index = state->nextVarIndex++;
				err = mfgDeclareType(state, node->first->returnType, index);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfgGenerateExpression(state, node->first, index, NULL);
				if (err != MF_ERROR_OKAY)
					return err;

				// Get array reference
				err = mfgAccessArray(state, node->ref.varID, node->ref.cmpVarID, index);
				if (err != MF_ERROR_OKAY)
					return err;

				// Assign output to component
				u8T = MFG_BYTECODE_ASSIGN;
				err = mfgBytecodePut8(state, &u8T);
				if (err != MF_ERROR_OKAY)
					return err;

				u16T = outVar;
				err = mfgBytecodePut16(state, &u16T);
				if (err != MF_ERROR_OKAY)
					return err;

				u16T = node->ref.cmpVarID;
				err = mfgBytecodePut16(state, &u16T);
				if (err != MF_ERROR_OKAY)
					return err;

				u8T = MFG_BYTECODE_CLSCOPE;
				err = mfgBytecodePut8(state, &u8T);
				if (err != MF_ERROR_OKAY)
					return err;
			}
			break;
		}

		case MFG_V2X_TOKEN_INT_LITERAL:
		{
			if (outVar != 0xFFFF)
			{
				// Assign output to literal
				u8T = MFG_BYTECODE_LITI1;
				err = mfgBytecodePut8(state, &u8T);
				if (err != MF_ERROR_OKAY)
					return err;

				u16T = outVar;
				err = mfgBytecodePut16(state, &u16T);
				if (err != MF_ERROR_OKAY)
					return err;

				u32T = atoi(node->attribute);
				err = mfgBytecodePut32(state, &u32T);
				if (err != MF_ERROR_OKAY)
					return err;
			}
			break;
		}

		case MFG_V2X_TOKEN_FLOAT_LITERAL:
		{
			if (outVar != 0xFFFF)
			{
				// Assign output to literal
				u8T = MFG_BYTECODE_LITF1;
				err = mfgBytecodePut8(state, &u8T);
				if (err != MF_ERROR_OKAY)
					return err;

				u16T = outVar;
				err = mfgBytecodePut16(state, &u16T);
				if (err != MF_ERROR_OKAY)
					return err;

				mfmF32 f32T = atof(node->attribute);
				err = mfgBytecodePut32(state, &f32T);
				if (err != MF_ERROR_OKAY)
					return err;
			}
			break;
		}

		case MFG_V2X_TOKEN_FALSE:
		{
			if (outVar != 0xFFFF)
			{
				// Assign output to literal
				u8T = MFG_BYTECODE_LITB1FALSE;
				err = mfgBytecodePut8(state, &u8T);
				if (err != MF_ERROR_OKAY)
					return err;

				u16T = outVar;
				err = mfgBytecodePut16(state, &u16T);
				if (err != MF_ERROR_OKAY)
					return err;
			}
			break;
		}

		case MFG_V2X_TOKEN_TRUE:
		{
			if (outVar != 0xFFFF)
			{
				// Assign output to literal
				u8T = MFG_BYTECODE_LITB1TRUE;
				err = mfgBytecodePut8(state, &u8T);
				if (err != MF_ERROR_OKAY)
					return err;

				u16T = outVar;
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

		case MFG_V2X_TOKEN_ADD:
		case MFG_V2X_TOKEN_SUBTRACT:
		case MFG_V2X_TOKEN_MULTIPLY:
		case MFG_V2X_TOKEN_DIVIDE:
			err = mfgAnnotateExpression(state, term1);
			if (err != MF_ERROR_OKAY)
				return err;

			if (term2 != NULL)
			{
				err = mfgAnnotateExpression(state, term2);
				if (err != MF_ERROR_OKAY)
					return err;
			}

			node->isConstant = MFM_FALSE;
			node->isLValue = MFM_FALSE;
			node->returnType = term1->returnType;
			return MF_ERROR_OKAY;

		case MFG_V2X_TOKEN_EQUAL:
		case MFG_V2X_TOKEN_DIFFERENT:
		case MFG_V2X_TOKEN_GREATER:
		case MFG_V2X_TOKEN_LESS:
		case MFG_V2X_TOKEN_GEQUAL:
		case MFG_V2X_TOKEN_LEQUAL:
			err = mfgAnnotateExpression(state, term1);
			if (err != MF_ERROR_OKAY)
				return err;

			err = mfgAnnotateExpression(state, term2);
			if (err != MF_ERROR_OKAY)
				return err;

			node->isConstant = MFM_FALSE;
			node->isLValue = MFM_FALSE;
			node->returnType = MFG_V2X_TOKEN_BOOL;
			return MF_ERROR_OKAY;

		case MFG_V2X_TOKEN_AND:
		case MFG_V2X_TOKEN_OR:
			err = mfgAnnotateExpression(state, term1);
			if (err != MF_ERROR_OKAY)
				return err;

			err = mfgAnnotateExpression(state, term2);
			if (err != MF_ERROR_OKAY)
				return err;

			if (term1->returnType != MFG_V2X_TOKEN_BOOL ||
				term2->returnType != MFG_V2X_TOKEN_BOOL)
			{
				mfsStringStream ss;
				mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
				mfsPutString(&ss, u8"[mfgAnnotateExpression] 'and' and 'or' terms must be booleans");
				mfsDestroyLocalStringStream(&ss);
				return MFG_ERROR_FAILED_TO_GENERATE_EXPRESSION;
			}

			node->isConstant = MFM_FALSE;
			node->isLValue = MFM_FALSE;
			node->returnType = MFG_V2X_TOKEN_BOOL;
			return MF_ERROR_OKAY;

		case MFG_V2X_TOKEN_NOT:
			err = mfgAnnotateExpression(state, term1);
			if (err != MF_ERROR_OKAY)
				return err;

			if (term1->returnType != MFG_V2X_TOKEN_BOOL)
			{
				mfsStringStream ss;
				mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
				mfsPutString(&ss, u8"[mfgAnnotateExpression] 'not' term must be a boolean");
				mfsDestroyLocalStringStream(&ss);
				return MFG_ERROR_FAILED_TO_GENERATE_EXPRESSION;
			}

			node->isConstant = MFM_FALSE;
			node->isLValue = MFM_FALSE;
			node->returnType = MFG_V2X_TOKEN_BOOL;
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

		case MFG_V2X_TOKEN_FALSE:
		case MFG_V2X_TOKEN_TRUE:
			node->isLValue = MFM_FALSE;
			node->isConstant = MFM_TRUE;
			node->returnType = MFG_V2X_TOKEN_BOOL;
			return MF_ERROR_OKAY;

		case MFG_V2X_TOKEN_IDENTIFIER:
		{
			node->isLValue = MFM_TRUE;
			node->isConstant = MFM_FALSE;
			node->returnType = 0;

			for (mfmU32 i = 0; i < MFG_V2X_MAX_TEXTURE_1DS; ++i)
				if (strcmp(state->compilerState->texture1Ds[i].id, node->attribute) == 0)
				{
					node->ref.varID = state->compilerState->texture1Ds[i].index;
					node->ref.type = MFG_V2X_TOKEN_TEXTURE_1D;
					node->returnType = MFG_V2X_TOKEN_TEXTURE_1D;
					node->info = &MFG_V2X_TINFO_REFERENCE;
					return MF_ERROR_OKAY;
				}

			for (mfmU32 i = 0; i < MFG_V2X_MAX_TEXTURE_2DS; ++i)
				if (strcmp(state->compilerState->texture2Ds[i].id, node->attribute) == 0)
				{
					node->ref.varID = state->compilerState->texture2Ds[i].index;
					node->ref.type = MFG_V2X_TOKEN_TEXTURE_2D;
					node->returnType = MFG_V2X_TOKEN_TEXTURE_2D;
					node->info = &MFG_V2X_TINFO_REFERENCE;
					return MF_ERROR_OKAY;
				}

			for (mfmU32 i = 0; i < MFG_V2X_MAX_TEXTURE_3DS; ++i)
				if (strcmp(state->compilerState->texture3Ds[i].id, node->attribute) == 0)
				{
					node->ref.varID = state->compilerState->texture3Ds[i].index;
					node->ref.type = MFG_V2X_TOKEN_TEXTURE_3D;
					node->returnType = MFG_V2X_TOKEN_TEXTURE_3D;
					node->info = &MFG_V2X_TINFO_REFERENCE;
					return MF_ERROR_OKAY;
				}

			// TO DO: GET LOCAL VARIABLES

			mfsStringStream ss;
			mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
			mfsPrintFormatUTF8(&ss, u8"[mfgAnnotateExpression] Couldn't find a variable with the ID '%s'", node->attribute);
			mfsDestroyLocalStringStream(&ss);
			return MFG_ERROR_FAILED_TO_GENERATE_EXPRESSION;
		}

		case MFG_V2X_TOKEN_ARRAY_ACCESS:
		{
			err = mfgAnnotateExpression(state, term1);
			if (err != MF_ERROR_OKAY)
				return err;
			err = mfgAnnotateExpression(state, term2);
			if (err != MF_ERROR_OKAY)
				return err;

			if (term1->info->type != MFG_V2X_TOKEN_REFERENCE)
			{
				mfsStringStream ss;
				mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
				mfsPrintFormatUTF8(&ss, u8"[mfgAnnotateExpression] First term in a array access operator must be a reference");
				mfsDestroyLocalStringStream(&ss);
				return MFG_ERROR_FAILED_TO_GENERATE_EXPRESSION;
			}

			node->info = &MFG_V2X_TINFO_ARRAY_REFERENCE;
			node->ref.varID = term1->ref.varID;
			node->ref.type = term1->ref.type;
			node->ref.cmpVarID = state->nextVarIndex++;
			node->first = term2;
			node->returnType = node->ref.type;

			return MF_ERROR_OKAY;
		}

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
						node->first = NULL;
						node->ref.varID = state->compilerState->input.variables[i].index;
						node->ref.type = state->compilerState->input.variables[i].type;
						node->isLValue = MFM_TRUE;
						node->isConstant = MFM_FALSE;
						node->returnType = node->ref.type;
						return MF_ERROR_OKAY;
					}

				mfsStringStream ss;
				mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
				mfsPrintFormatUTF8(&ss, u8"[mfgAnnotateExpression] No input variable was found with the ID '%s'", term2->attribute);
				mfsDestroyLocalStringStream(&ss);
				return MFG_ERROR_FAILED_TO_GENERATE_EXPRESSION;
			}
			// Output struct member
			if (term1->info->type == MFG_V2X_TOKEN_OUTPUT)
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
						node->first = NULL;
						node->ref.varID = state->compilerState->output.variables[i].index;
						node->ref.type = state->compilerState->output.variables[i].type;
						node->isLValue = MFM_TRUE;
						node->isConstant = MFM_FALSE;
						node->returnType = node->ref.type;
						return MF_ERROR_OKAY;
					}

				mfsStringStream ss;
				mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
				mfsPrintFormatUTF8(&ss, u8"[mfgAnnotateExpression] No output variable was found with the ID '%s'", term2->attribute);
				mfsDestroyLocalStringStream(&ss);
				return MFG_ERROR_FAILED_TO_GENERATE_EXPRESSION;
			}
			// Buffer member?
			if (term1->info->type == MFG_V2X_TOKEN_IDENTIFIER)
			{
				for (mfmU32 i = 0; i < MFG_V2X_MAX_CONSTANT_BUFFERS; ++i)
					if (strcmp(state->compilerState->constantBuffers[i].id, term1->attribute) == 0)
					{
						// Search for the corresponding buffer member variable, if any
						for (mfmU32 j = 0; j < MFG_V2X_MAX_CONSTANT_BUFFER_VARS; ++j)
							if (strcmp(state->compilerState->constantBuffers[i].variables[j].id, term2->attribute) == 0)
							{
								node->info = &MFG_V2X_TINFO_REFERENCE;
								node->first = NULL;
								node->ref.varID = state->compilerState->constantBuffers[i].variables[j].index;
								node->ref.type = state->compilerState->constantBuffers[i].variables[j].type;
								node->isLValue = MFM_TRUE;
								node->isConstant = MFM_FALSE;
								node->returnType = node->ref.type;
								return MF_ERROR_OKAY;
							}

						// Couldn't find a matching member variable
						mfsStringStream ss;
						mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
						mfsPrintFormatUTF8(&ss, u8"[mfgAnnotateExpression] The constant buffer '%s' has no member '%s'", term1->attribute, term2->attribute);
						mfsDestroyLocalStringStream(&ss);
						return MFG_ERROR_FAILED_TO_GENERATE_EXPRESSION;
					}
			}
			// Component access
			{
				err = mfgAnnotateExpression(state, term1);
				if (err != MF_ERROR_OKAY)
					return err;

				if (term1->info->type == MFG_V2X_TOKEN_REFERENCE)
				{
					node->info = &MFG_V2X_TINFO_CMP_REFERENCE;
					node->ref.varID = term1->ref.varID;
					node->ref.type = term1->ref.type;
					node->first = NULL;
				}
				else if (term1->info->type == MFG_V2X_TOKEN_CMP_REFERENCE)
				{
					mfsStringStream ss;
					mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
					mfsPrintFormatUTF8(&ss, u8"[mfgAnnotateExpression] A component cannot have its components accessed");
					mfsDestroyLocalStringStream(&ss);
					return MFG_ERROR_FAILED_TO_GENERATE_EXPRESSION;
				}
				else
				{
					node->info = &MFG_V2X_TINFO_CMP_REFERENCE;
					node->ref.varID = 0xFFFF;
					node->ref.type = term1->returnType;
					term1->next = NULL;
				}

				node->ref.cmpVarID = state->nextVarIndex++;

				if (term2->info->type != MFG_V2X_TOKEN_IDENTIFIER)
				{
					mfsStringStream ss;
					mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
					mfsPrintFormatUTF8(&ss, u8"[mfgAnnotateExpression] Member expression second term must be an identifier");
					mfsDestroyLocalStringStream(&ss);
					return MFG_ERROR_FAILED_TO_GENERATE_EXPRESSION;
				}

				if (strcmp(term2->attribute, u8"x") == 0)
					node->ref.cmpIndex = 0;
				else if (strcmp(term2->attribute, u8"y") == 0)
					node->ref.cmpIndex = 1;
				else if (strcmp(term2->attribute, u8"z") == 0)
					node->ref.cmpIndex = 2;
				else if (strcmp(term2->attribute, u8"w") == 0)
					node->ref.cmpIndex = 3;
				else
				{
					mfsStringStream ss;
					mfsCreateLocalStringStream(&ss, state->state->errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
					mfsPrintFormatUTF8(&ss, u8"[mfgAnnotateExpression] Invalid member expression second term");
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
