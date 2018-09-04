#include "Compiler.h"
#include "Generator.h"

#include <string.h>

mfError mfgV2XRunMSLCompiler(const mfsUTF8CodeUnit* msl, mfmU8* bytecode, mfmU64 maxBytecodeSize, mfmU8* metaData, mfmU64 maxMetaDataSize, mfgV2XEnum shaderType, mfgV2XMVLCompilerInfo* info)
{
	if (msl == NULL || bytecode == NULL || maxBytecodeSize == 0 || metaData == NULL || maxMetaDataSize == 0 || info == NULL)
		return MFG_ERROR_INVALID_ARGUMENTS;

	mfError err;

	mfgV2XToken tokens[4096];
	mfgV2XNode nodes[4096];

	mfgV2XLexerState lexerState;
	mfgV2XParserState parserState;
	mfgV2XGeneratorState generatorState;
	mfgV2XCompilerState compilerState;

	memset(lexerState.errorMsg, 0, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
	memset(parserState.errorMsg, 0, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
	memset(generatorState.errorMsg, 0, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);

	err = mfgV2XRunMVLLexer(msl, tokens, sizeof(tokens) / sizeof(mfgV2XToken), &lexerState);
	if (err != MF_ERROR_OKAY)
	{
		memcpy(info->errorMsg, lexerState.errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
		return err;
	}

	err = mfgV2XRunMVLParser(tokens, nodes, sizeof(nodes) / sizeof(mfgV2XNode), &lexerState, &parserState, &compilerState);
	if (err != MF_ERROR_OKAY)
	{
		memcpy(info->errorMsg, parserState.errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
		return err;
	}

	err = mfgV2XRunMVLGenerator(&nodes[0], bytecode, maxBytecodeSize, metaData, maxMetaDataSize, &generatorState, &compilerState, shaderType);
	if (err != MF_ERROR_OKAY)
	{
		memcpy(info->errorMsg, generatorState.errorMsg, MFG_V2X_MAX_ERROR_MESSAGE_SIZE);
		return err;
	}
	
	info->bytecodeSize = generatorState.bytecodeSize;
	info->metaDataSize = generatorState.metaDataSize;

	return MF_ERROR_OKAY;
}
