#include <Magma/Framework/Entry.h>
#include <Magma/Framework/String/Stream.h>
#include <Magma/Framework/VM/Compiler/V1X/Lexer.h>
#include <Magma/Framework/VM/Compiler/V1X/Parser.h>

#include <stdlib.h>

int main(int argc, const char** argv)
{
	if (mfInit(argc, argv) != MF_ERROR_OKAY)
		abort();

	const mfsUTF8CodeUnit* src =
		u8"void func(i32 x, i32 y)\n"
		u8"{\n"
		u8"\tMagma.Print(\"%d\", x + y);"
		u8"}\n"
		u8"void entry(i32 x, f32 y)\n"
		u8"{\n"
		u8"\tfunc(x, y);\n"
		u8"}"
		;

	mfvV1XToken tokens[2048];
	mfvV1XLexerState lexerState;
	if (mfvV1XRunMVLLexer(src, tokens, 2048, &lexerState) != MF_ERROR_OKAY)
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

	mfvV1XNode nodes[2048];
	mfvV1XParserState parserState;
	if (mfvV1XRunMVLParser(tokens, nodes, 2048, &lexerState, &parserState) != MF_ERROR_OKAY)
	{
		mfsPutString(mfsErrStream, parserState.errorMsg);
		abort();
	}

	mfvV1XPrintNode(mfsOutStream, &nodes[0], 0);

	mfTerminate();

	for (;;);
	return 0;
}