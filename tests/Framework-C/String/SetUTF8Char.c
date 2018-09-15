#include "../../Test.h"

#include <Magma/Framework/String/UTF8.h>

int main()
{
	mfsUnicodePoint chrs[] =
	{
		0x24, 1,
		0xA2, 2,
		0x20AC, 3,
		0x10348, 4,
		0x00,
	};

	mfsUTF8CodeUnit expectedChrs[][4] =
	{
		{ 0x24, 0x00, 0x00, 0x00 },
		{ 0xC2, 0xA2, 0x00, 0x00 },
		{ 0xE2, 0x82, 0xAC, 0x00 },
		{ 0xF0, 0x90, 0x8D, 0x88 },
		{ 0x00, 0x00, 0x00, 0x00 },
	};

	mfsUTF8CodeUnit dst[4];
	mfsUnicodePoint* chr = &chrs[0];
	mfsUTF8CodeUnit* exp = &expectedChrs[0];
	while (*chr != 0)
	{
		dst[0] = 0;
		dst[1] = 0;
		dst[2] = 0;
		dst[3] = 0;

		mfmU64 size;
		TEST_REQUIRE_PASS(mfsSetUTF8Char(*chr, dst, &size, sizeof(dst)) == MF_ERROR_OKAY);
		TEST_REQUIRE_PASS(size == chr[1]);

		TEST_REQUIRE_PASS(exp[0] == dst[0]);
		TEST_REQUIRE_PASS(exp[1] == dst[1]);
		TEST_REQUIRE_PASS(exp[2] == dst[2]);
		TEST_REQUIRE_PASS(exp[3] == dst[3]);

		chr += 2;
		exp += 4;
	}

	EXIT_PASS();
}