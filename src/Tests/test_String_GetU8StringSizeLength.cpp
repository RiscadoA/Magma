#include "Testing.hpp"

#include "../Magma/Framework/String/UTF8.hpp"

int main()
{
	using namespace Magma::Framework::String;

	U8CodePoint str[] =
	{
		0x24,
		0xC2, 0xA2,
		0xE2, 0x82, 0xAC,
		0xF0, 0x90, 0x8D, 0x88,
		0x00,
	};

	TEST_REQUIRE_PASS(GetU8StringSize(str) == 11);
	TEST_REQUIRE_PASS(GetU8StringLength(str) == 4);

	EXIT_PASS();
}