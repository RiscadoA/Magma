#include <Magma/Framework/Entry.h>
#include <Magma/Framework/String/Stream.h>
#include <Magma/Framework/VM/Compiler/V1X/Lexer.h>

#include <stdlib.h>

int main(int argc, const char** argv)
{
	if (mfInit(argc, argv) != MF_ERROR_OKAY)
		abort();

	const mfsUTF8CodeUnit* src = u8"bool test = 2 == 9; u32 var = 2 + 9; if (test == false) throw error (\"2 == 9 evaluated to false\");";

	mfvV1XToken tokens[2048];
	mfvV1XLexerState state;
	if (mfvV1XRunMVLLexer(src, tokens, 2048, &state) != MF_ERROR_OKAY)
	{
		mfsPutString(mfsErrStream, state.errorMsg);
		abort();
	}

	for (mfmU64 i = 0; i < state.tokenCount; ++i)
	{
		if (tokens[i].attribute[0] != '\0')
			mfsPrintFormatUTF8(mfsOutStream, u8"'%s' \"%s\"\n", tokens[i].info->name, tokens[i].attribute);
		else
			mfsPrintFormatUTF8(mfsOutStream, u8"'%s'\n", tokens[i].info->name);
	}

	mfTerminate();

	for (;;);
	return 0;
}