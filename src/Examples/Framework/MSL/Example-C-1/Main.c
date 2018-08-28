#include <Magma/Framework/Entry.h>
#include <Magma/Framework/String/Stream.h>
#include <Magma/Framework/Graphics/2.X/MSL/Lexer.h>
#include <Magma/Framework/Graphics/2.X/MSL/Parser.h>

#include <stdlib.h>

int main(int argc, const char** argv)
{
	if (mfInit(argc, argv) != MF_ERROR_OKAY)
		abort();

	const mfsUTF8CodeUnit* src =
		u8"Input { float4 position : POSITION; };"
		u8"Output { float4 position : _POSITION; };"
		u8"ConstantBuffer buffer : BUFFER { float4x4 transforms[256]; };"
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

	mfTerminate();

	for (;;);
	return 0;
}