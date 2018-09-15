#include "../../Test.h"

#include <Magma/Framework/String/UTF8.h>

int main()
{
	mfsUTF8CodeUnit chrs[][4] =
	{
		{ 0x24, 0x00, 0x00, 0x00 },
		{ 0xC2, 0xA2, 0x00, 0x00 },
		{ 0xE2, 0x82, 0xAC, 0x00 },
		{ 0xF0, 0x90, 0x8D, 0x88 },
		{ 0x00, 0x00, 0x00, 0x00 },
	};

	mfsUnicodePoint expectedChrs[] =
	{
		0x24,
		0xA2,
		0x20AC,
		0x10348,
		0x00,
	};

	mfsUTF8CodeUnit dst[4];
	mfsUTF8CodeUnit* chr = chrs[0];
	mfsUnicodePoint* exp = &expectedChrs[0];
	while (*chr != 0)
	{
		dst[0] = 0;
		dst[1] = 0;
		dst[2] = 0;
		dst[3] = 0;

		mfsUnicodePoint up;
		TEST_REQUIRE_PASS(mfsGetUTF8Char(chr, &up) == MF_ERROR_OKAY);
		TEST_REQUIRE_PASS(*exp == up);

		chr += 4;
		++exp;
	}

	EXIT_PASS();
}