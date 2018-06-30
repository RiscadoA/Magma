#include "Testing.hpp"

#include "../Magma/Framework/String/Conversion.hpp"

int main()
{
	using namespace Magma::Framework::String;

	U32Char chrs[] =
	{
		0x24, 1,
		0xA2, 2,
		0x20AC, 3,
		0x10348, 4,
		0x00,
	};

	U8Char expectedChrs[][4] =
	{
		{ 0x24, 0x00, 0x00, 0x00 },
		{ 0xC2, 0xA2, 0x00, 0x00 },
		{ 0xE2, 0x82, 0xAC, 0x00 },
		{ 0xF0, 0x90, 0x8D, 0x88 },
		{ 0x00, 0x00, 0x00, 0x00 },
	};

	U8Char dst[4];
	U32Char* chr = &chrs[0];
	auto exp = &expectedChrs[0];
	while (*chr != 0)
	{
		dst[0] = 0;
		dst[1] = 0;
		dst[2] = 0;
		dst[3] = 0;

		TEST_REQUIRE_PASS(SetU8Char(dst, chr[0]) == chr[1]);

		TEST_REQUIRE_PASS((*exp)[0] == dst[0]);
		TEST_REQUIRE_PASS((*exp)[1] == dst[1]);
		TEST_REQUIRE_PASS((*exp)[2] == dst[2]);
		TEST_REQUIRE_PASS((*exp)[3] == dst[3]);

		chr += 2;
		exp += 1;
	}

	EXIT_PASS();
}