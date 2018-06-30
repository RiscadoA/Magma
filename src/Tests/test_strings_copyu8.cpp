#include "Testing.hpp"

#include "../Magma/Framework/String/UTF8.hpp"

int main()
{
	using namespace Magma::Framework::String;

	U8Char srcStr[] =
	{
		0x24,
		0xC2, 0xA2,
		0xE2, 0x82, 0xAC,
		0xF0, 0x90, 0x8D, 0x88,
		0x00,
	};

	U8Char dstStr[256];

	TEST_REQUIRE_PASS(CopyU8(srcStr, dstStr, 256) == 11);
	for (size_t i = 0; i < 11; ++i)
		TEST_REQUIRE_PASS(srcStr[i] == dstStr[i]);
	TEST_REQUIRE_PASS(dstStr[10] == 0);

	TEST_REQUIRE_PASS(CopyU8(srcStr, dstStr, 4) == 4);
	for (size_t i = 0; i < 3; ++i)
		TEST_REQUIRE_PASS(srcStr[i] == dstStr[i]);
	TEST_REQUIRE_PASS(dstStr[3] == 0);

	EXIT_PASS();
}