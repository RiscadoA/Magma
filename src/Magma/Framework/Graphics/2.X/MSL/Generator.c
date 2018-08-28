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

static mfError mfgMetaDataPutN(mfgV2XGeneratorInternalState* state, const void* data, mfmU64 size)
{
	if (state->state->metaDataSize + size >= state->maxMetaDataSize)
		return MFG_ERROR_METADATA_OVERFLOW;
	memcpy(state->metaData + state->state->metaDataSize, data, size);
	state->state->metaDataSize += size;
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

static mfError mfgGenerateBytecode(mfgV2XGeneratorInternalState* state)
{
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
	err = mfgGenerateBytecode(&internalState);
	if (err != MF_ERROR_OKAY)
		return err;

	return MF_ERROR_OKAY;
}
