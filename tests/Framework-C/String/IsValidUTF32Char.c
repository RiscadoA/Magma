#include "../../Test.h"

#include <Magma/Framework/String/UTF32.h>

int main()
{
	// Valid chars
	{
		mfsUnicodePoint valid[] =
		{
			0xE1,
			0x1374,
			0x10ffff,
			0x0
		};

		for (int i = 0; valid[i] != 0x00; ++i)
			TEST_REQUIRE_PASS(mfsIsValidUTF32Char(valid[i]) == MFM_TRUE);
	}

	// Invalid chars
	{
		mfsUnicodePoint invalid[] =
		{
			0x110000,
			0xD800,
			0xDFFF,
			0x0
		};

		for (int i = 0; invalid[i] != 0x00; ++i)
			TEST_REQUIRE_FAIL(mfsIsValidUTF32Char(invalid[i]) == MFM_TRUE);
	}

	EXIT_PASS();
}