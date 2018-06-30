#include "Testing.hpp"

#include "../Magma/Framework/String/UTF32.hpp"

int main()
{
	using namespace Magma::Framework::String;

	// Valid chars
	{
		U32Char valid[] =
		{
			0xE1,
			0x1374,
			0x10ffff,
			0x0
		};

		for (int i = 0; valid[i] != 0x00; ++i)
			TEST_REQUIRE_PASS(IsValidU32Char(valid[i]));
	}

	// Invalid chars
	{
		U32Char invalid[] =
		{
			0x110000,
			0xD800,
			0xDFFF,
			0x0
		};

		for (int i = 0; invalid[i] != 0x00; ++i)
			TEST_REQUIRE_FAIL(IsValidU32Char(invalid[i]));
	}

	EXIT_PASS();
}