#include "Testing.hpp"

#include "../Magma/Framework/String/UTF32.hpp"

int main()
{
	using namespace Magma::Framework::String;

	// Valid chars
	{
		UnicodePoint valid[] =
		{
			0xE1,
			0x1374,
			0x10ffff,
			0x0
		};

		for (int i = 0; valid[i] != 0x00; ++i)
			TEST_REQUIRE_PASS(IsValidUnicode(valid[i]));
	}

	// Invalid chars
	{
		UnicodePoint invalid[] =
		{
			0x110000,
			0xD800,
			0xDFFF,
			0x0
		};

		for (int i = 0; invalid[i] != 0x00; ++i)
			TEST_REQUIRE_FAIL(IsValidUnicode(invalid[i]));
	}

	EXIT_PASS();
}