#include "Testing.hpp"

#include "../Magma/Framework/String/Conversion.hpp"

int main()
{
	using namespace Magma::Framework::String;

	U32Char srcStr[] =
	{
		0xdeadbeef,
		0xefbeadde,
		0x00,
	};
	U32Char dstStr[256];

	TEST_REQUIRE_PASS(U32FromLE(srcStr, dstStr, 0) == 0);
	TEST_REQUIRE_PASS(U32FromLE(srcStr, dstStr, 1) == 1);
	TEST_REQUIRE_PASS(U32FromLE(srcStr, dstStr, 2) == 2);
	TEST_REQUIRE_PASS(U32FromLE(srcStr, dstStr, 3) == 3);
	TEST_REQUIRE_PASS(U32FromLE(srcStr, dstStr, 4) == 3);

	TEST_REQUIRE_PASS(U32FromBE(srcStr, dstStr, 0) == 0);
	TEST_REQUIRE_PASS(U32FromBE(srcStr, dstStr, 1) == 1);
	TEST_REQUIRE_PASS(U32FromBE(srcStr, dstStr, 2) == 2);
	TEST_REQUIRE_PASS(U32FromBE(srcStr, dstStr, 3) == 3);
	TEST_REQUIRE_PASS(U32FromBE(srcStr, dstStr, 4) == 3);

	TEST_REQUIRE_PASS(U32ToLE(srcStr, dstStr, 0) == 0);
	TEST_REQUIRE_PASS(U32ToLE(srcStr, dstStr, 1) == 1);
	TEST_REQUIRE_PASS(U32ToLE(srcStr, dstStr, 2) == 2);
	TEST_REQUIRE_PASS(U32ToLE(srcStr, dstStr, 3) == 3);
	TEST_REQUIRE_PASS(U32ToLE(srcStr, dstStr, 4) == 3);

	TEST_REQUIRE_PASS(U32ToBE(srcStr, dstStr, 0) == 0);
	TEST_REQUIRE_PASS(U32ToBE(srcStr, dstStr, 1) == 1);
	TEST_REQUIRE_PASS(U32ToBE(srcStr, dstStr, 2) == 2);
	TEST_REQUIRE_PASS(U32ToBE(srcStr, dstStr, 3) == 3);
	TEST_REQUIRE_PASS(U32ToBE(srcStr, dstStr, 4) == 3);

	EXIT_PASS();
}