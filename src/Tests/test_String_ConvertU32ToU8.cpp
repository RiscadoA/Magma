#include "Testing.hpp"

#include "../Magma/Framework/String/Conversion.hpp"

int main()
{
	using namespace Magma::Framework::String;

	UnicodePoint srcStr[] =
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

	UTF8CodeUnit dstStr[256];

	TEST_REQUIRE_PASS(ConvertU32ToU8(srcStr, dstStr, 256) == 11);
	TEST_REQUIRE_PASS(dstStr[0] == (UTF8CodeUnit)0x24);
	TEST_REQUIRE_PASS(dstStr[1] == (UTF8CodeUnit)0xC2);
	TEST_REQUIRE_PASS(dstStr[2] == (UTF8CodeUnit)0xA2);
	TEST_REQUIRE_PASS(dstStr[3] == (UTF8CodeUnit)0xE2);
	TEST_REQUIRE_PASS(dstStr[4] == (UTF8CodeUnit)0x82);
	TEST_REQUIRE_PASS(dstStr[5] == (UTF8CodeUnit)0xAC);
	TEST_REQUIRE_PASS(dstStr[6] == (UTF8CodeUnit)0xF0);
	TEST_REQUIRE_PASS(dstStr[7] == (UTF8CodeUnit)0x90);
	TEST_REQUIRE_PASS(dstStr[8] == (UTF8CodeUnit)0x8D);
	TEST_REQUIRE_PASS(dstStr[9] == (UTF8CodeUnit)0x88);
	TEST_REQUIRE_PASS(dstStr[10] == (UTF8CodeUnit)0);

	TEST_REQUIRE_PASS(ConvertU32ToU8(srcStr, dstStr, 3) == 2);
	TEST_REQUIRE_PASS(dstStr[0] == (UTF8CodeUnit)0x24);
	TEST_REQUIRE_PASS(dstStr[1] == (UTF8CodeUnit)0);
	
	TEST_REQUIRE_PASS(ConvertU32ToU8(srcStr, dstStr, 4) == 4);
	TEST_REQUIRE_PASS(dstStr[0] == (UTF8CodeUnit)0x24);
	TEST_REQUIRE_PASS(dstStr[1] == (UTF8CodeUnit)0xC2);
	TEST_REQUIRE_PASS(dstStr[2] == (UTF8CodeUnit)0xA2);
	TEST_REQUIRE_PASS(dstStr[3] == (UTF8CodeUnit)0);

	EXIT_PASS();
}