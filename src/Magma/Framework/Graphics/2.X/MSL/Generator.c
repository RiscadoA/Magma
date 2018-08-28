#include "Generator.h"
#include "../../../Memory/Endianness.h"

#include <string.h>

typedef struct
{
	mfgV2XGeneratorState* state;
	mfgV2XCompilerState* compilerState;
	mfmU8* bytecode;
	mfmU8* metaData;
	mfmU64 maxBytecodeSize;
	mfmU64 maxMetaDataSize;
} mfgV2XGeneratorInternalState;

static mfError mfgBytecodePut8(mfgV2XGeneratorInternalState* state, const void* data)
{
	if (state->state->bytecodeSize + 1 >= state->maxBytecodeSize)
		return MFG_ERROR_BYTECODE_OVERFLOW;
	memcpy(state->bytecode + state->state->bytecodeSize, data, 1);
	state->state->bytecodeSize += 1;
	return MF_ERROR_OKAY;
}

static mfError mfgBytecodePut16(mfgV2XGeneratorInternalState* state, const void* data)
{
	if (state->state->bytecodeSize + 2 >= state->maxBytecodeSize)
		return MFG_ERROR_BYTECODE_OVERFLOW;
	mfmToBigEndian2(data, state->bytecode + state->state->bytecodeSize);
	state->state->bytecodeSize += 2;
	return MF_ERROR_OKAY;
}

static mfError mfgBytecodePut32(mfgV2XGeneratorInternalState* state, const void* data)
{
	if (state->state->bytecodeSize + 4 >= state->maxBytecodeSize)
		return MFG_ERROR_BYTECODE_OVERFLOW;
	mfmToBigEndian4(data, state->bytecode + state->state->bytecodeSize);
	state->state->bytecodeSize += 4;
	return MF_ERROR_OKAY;
}

static mfError mfgMetaDataPut8(mfgV2XGeneratorInternalState* state, const void* data)
{
	if (state->state->metaDataSize + 1 >= state->maxMetaDataSize)
		return MFG_ERROR_METADATA_OVERFLOW;
	memcpy(state->metaData + state->state->metaDataSize, data, 1);
	state->state->metaDataSize += 1;
	return MF_ERROR_OKAY;
}

static mfError mfgMetaDataPut16(mfgV2XGeneratorInternalState* state, const void* data)
{
	if (state->state->metaDataSize + 2 >= state->maxMetaDataSize)
		return MFG_ERROR_METADATA_OVERFLOW;
	mfmToBigEndian2(data, state->metaData + state->state->metaDataSize);
	state->state->metaDataSize += 2;
	return MF_ERROR_OKAY;
}

static mfError mfgMetaDataPut32(mfgV2XGeneratorInternalState* state, const void* data)
{
	if (state->state->metaDataSize + 4 >= state->maxMetaDataSize)
		return MFG_ERROR_METADATA_OVERFLOW;
	mfmToBigEndian4(data, state->metaData + state->state->metaDataSize);
	state->state->metaDataSize += 4;
	return MF_ERROR_OKAY;
}

static mfError mfgGenerateMetaData(mfgV2XGeneratorInternalState* state)
{

}

mfError mfgV2XRunMVLGenerator(const mfgV2XNode * root, mfmU8 * bytecode, mfmU64 maxBytecodeSize, mfmU8 * metaData, mfmU64 maxMetaDataSize, mfgV2XGeneratorState* state, const mfgV2XCompilerState * compilerState)
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

	state->bytecodeSize = 0;
	state->metaDataSize = 0;

	err = mfgGenerateMetaData(&internalState);

	return MF_ERROR_OKAY;
}
