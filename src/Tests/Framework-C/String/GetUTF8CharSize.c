#include "../../Test.h"

#include <Magma/Framework/String/UTF8.h>

int main()
{
	mfsUTF8CodeUnit chars[][5] =
	{
		{ 0x24, 0x00, 0x00, 0x00, 1 },
		{ 0xC2, 0xA2, 0x00, 0x00, 2 },
		{ 0xE2, 0x82, 0xAC, 0x00, 3 },
		{ 0xF0, 0x90, 0x8D, 0x88, 4 },

		{ 0x00, 0x00, 0x00, 0x00 }
	};

	for (int i = 0; chars[i][0] != 0x00; ++i)
	{
		mfmU64 size;
		TEST_REQUIRE_PASS(mfsGetUTF8CharSize(chars[i], &size) == MF_ERROR_OKAY);
		TEST_REQUIRE_PASS(size == chars[i][4]);
	}

	EXIT_PASS();
}