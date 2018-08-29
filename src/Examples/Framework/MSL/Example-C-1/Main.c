#include <Magma/Framework/Entry.h>
#include <Magma/Framework/String/Stream.h>
#include <Magma/Framework/Graphics/2.X/MSL/Lexer.h>
#include <Magma/Framework/Graphics/2.X/MSL/Parser.h>
#include <Magma/Framework/Graphics/2.X/MSL/Generator.h>
#include <Magma/Framework/Graphics/2.X/Bytecode.h>
#include <Magma/Framework/Graphics/2.X/OGL4Assembler.h>
#include <Magma/Framework/Graphics/2.X/D3D11Assembler.h>

#include <stdlib.h>

int main(int argc, const char** argv)
{
	if (mfInit(argc, argv) != MF_ERROR_OKAY)
		abort();

	const mfsUTF8CodeUnit* src =
		u8"Input { float4 position : position; int instanceID : _instanceID; };"
		u8"Output { float4 position : _position; };"
		u8"ConstantBuffer buffer : buffer { float4x4 transforms[256]; };"
		u8"Texture1D tex1d : texture1D;"
		u8"Texture2D tex2d : texture2D;"
		u8"Texture3D tex3d : texture3D;"

		u8"void main()"
		u8"{"
		u8"		Output.position = mulvec(buffer.transforms[Input.instanceID], Input.position);"
		u8"}"
		;

	mfgV2XToken tokens[2048];
	mfgV2XLexerState lexerState;
	if (mfgV2XRunMVLLexer(src, tokens, 2048, &lexerState) != MF_ERROR_OKAY)
	{
		mfsPutString(mfsErrStream, lexerState.errorMsg);
		abort();
	}

	for (mfmU64 i = 0; i < lexerState.tokenCount; ++i)
	{
		if (tokens[i].attribute[0] != '\0')
			mfsPrintFormatUTF8(mfsOutStream, u8"'%s' \"%s\"\n", tokens[i].info->name, tokens[i].attribute);
		else
			mfsPrintFormatUTF8(mfsOutStream, u8"'%s'\n", tokens[i].info->name);
	}

	mfgV2XNode nodes[2048];
	mfgV2XParserState parserState;
	mfgV2XCompilerState compilerState;
	if (mfgV2XRunMVLParser(tokens, nodes, 2048, &lexerState, &parserState, &compilerState) != MF_ERROR_OKAY)
	{
		mfsPutString(mfsErrStream, parserState.errorMsg);
		abort();
	}

	mfgV2XPrintNode(mfsOutStream, &nodes[0], 0);

	mfmU8 bytecode[4096];
	mfmU8 metaData[4096];
	mfgV2XGeneratorState generatorState;
	
	if (mfgV2XRunMVLGenerator(&nodes[0], bytecode, sizeof(bytecode), metaData, sizeof(metaData), &generatorState, &compilerState, MFG_VERTEX_SHADER) != MF_ERROR_OKAY)
	{
		mfsPutString(mfsErrStream, generatorState.errorMsg);
		abort();
	}

	mfgMetaData* md;
	if (mfgLoadMetaData(metaData, generatorState.metaDataSize, &md, NULL) != MF_ERROR_OKAY)
		abort();

	if(mfgV2XOGL4Assemble(bytecode, generatorState.bytecodeSize, md, mfsOutStream) != MF_ERROR_OKAY)
		abort();

	if (mfgV2XD3D11Assemble(bytecode, generatorState.bytecodeSize, md, mfsOutStream) != MF_ERROR_OKAY)
		abort();

	mfgUnloadMetaData(md);

	mfTerminate();

	for (;;);
	return 0;
}