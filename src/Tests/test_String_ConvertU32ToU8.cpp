#include "Testing.hpp"

#include "../Magma/Framework/String/Conversion.hpp"

int main()
{
	using namespace Magma::Framework::String;

	U32Char srcStr[] =
	{
		0x24,
		0xA2,
		0x20AC,
		0x10348,
		0x00,
	};

	/*
		Expected UTF-8:

		0x24,
		0xC2, 0xA2,
		0xE2, 0x82, 0xAC,
		0xF0, 0x90, 0x8D, 0x88,
		0x00
	*/

	U8Char dstStr[256];

	TEST_REQUIRE_PASS(ConvertU32ToU8(srcStr, dstStr, 256) == 11);
	TEST_REQUIRE_PASS(dstStr[0] == (char)0x24);
	TEST_REQUIRE_PASS(dstStr[1] == (char)0xC2);
	TEST_REQUIRE_PASS(dstStr[2] == (char)0xA2);
	TEST_REQUIRE_PASS(dstStr[3] == (char)0xE2);
	TEST_REQUIRE_PASS(dstStr[4] == (char)0x82);
	TEST_REQUIRE_PASS(dstStr[5] == (char)0xAC);
	TEST_REQUIRE_PASS(dstStr[6] == (char)0xF0);
	TEST_REQUIRE_PASS(dstStr[7] == (char)0x90);
	TEST_REQUIRE_PASS(dstStr[8] == (char)0x8D);
	TEST_REQUIRE_PASS(dstStr[9] == (char)0x88);
	TEST_REQUIRE_PASS(dstStr[10] == (char)0);

	TEST_REQUIRE_PASS(ConvertU32ToU8(srcStr, dstStr, 3) == 2);
	TEST_REQUIRE_PASS(dstStr[0] == (char)0x24);
	TEST_REQUIRE_PASS(dstStr[1] == (char)0);
	
	TEST_REQUIRE_PASS(ConvertU32ToU8(srcStr, dstStr, 4) == 4);
	TEST_REQUIRE_PASS(dstStr[0] == (char)0x24);
	TEST_REQUIRE_PASS(dstStr[1] == (char)0xC2);
	TEST_REQUIRE_PASS(dstStr[2] == (char)0xA2);
	TEST_REQUIRE_PASS(dstStr[3] == (char)0);

	EXIT_PASS();
}