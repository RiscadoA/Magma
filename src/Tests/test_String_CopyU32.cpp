#include "Testing.hpp"

#include "../Magma/Framework/String/UTF32.hpp"

int main()
{
	using namespace Magma::Framework::String;

	UnicodePoint srcStr[] =
	{
		0x10FFFF,
		0xe1,
		0x416,
		0x1344,
		0x0
	};

	UnicodePoint dstStr[256];

	TEST_REQUIRE_PASS(CopyUTF32(srcStr, dstStr, 256) == 5);
	for (size_t i = 0; i < 5; ++i)
		TEST_REQUIRE_PASS(srcStr[i] == dstStr[i]);
	TEST_REQUIRE_PASS(dstStr[4] == 0);

	TEST_REQUIRE_PASS(CopyUTF32(srcStr, dstStr, 4) == 4);
	for (size_t i = 0; i < 3; ++i)
		TEST_REQUIRE_PASS(srcStr[i] == dstStr[i]);
	TEST_REQUIRE_PASS(dstStr[3] == 0);

	EXIT_PASS();
}