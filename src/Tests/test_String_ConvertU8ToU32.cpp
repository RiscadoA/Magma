#include "Testing.hpp"

#include "../Magma/Framework/String/Conversion.hpp"

int main()
{
	using namespace Magma::Framework::String;

	UTF8CodeUnit srcStr[] =
	{
		0x24,
		0xC2, 0xA2,
		0xE2, 0x82, 0xAC,
		0xF0, 0x90, 0x8D, 0x88,
		0x00,
	};

	UnicodePoint dstStr[256];

	TEST_REQUIRE_PASS(ConvertU8ToU32(srcStr, dstStr, 256) == 5);
	TEST_REQUIRE_PASS(dstStr[0] == 0x24);
	TEST_REQUIRE_PASS(dstStr[1] == 0xA2);
	TEST_REQUIRE_PASS(dstStr[2] == 0x20AC);
	TEST_REQUIRE_PASS(dstStr[3] == 0x10348);
	TEST_REQUIRE_PASS(dstStr[4] == 0);

	TEST_REQUIRE_PASS(ConvertU8ToU32(srcStr, dstStr, 3) == 3);
	TEST_REQUIRE_PASS(dstStr[0] == 0x24);
	TEST_REQUIRE_PASS(dstStr[1] == 0xA2);
	TEST_REQUIRE_PASS(dstStr[2] == 0);

	EXIT_PASS();
}