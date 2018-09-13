#include "../../Test.h"

#include <Magma/Framework/String/Conversion.h>

int main()
{
	// U8 conversion tests
	{
		// Valid
		{
			const mfsUTF8CodeUnit* u8s[] =
			{
				u8"23",
				u8"255",
				u8"128",
				u8"127",
				u8"94",
				u8"f0",
				u8"10",
				NULL
			};

			mfmU64 u8b[] =
			{
				10,
				10,
				10,
				10,
				10,
				16,
				2,
			};

			mfmU8 u8e[] =
			{
				23,
				255,
				128,
				127,
				94,
				0xF0,
				0b10,
			};

			const mfsUTF8CodeUnit** s = u8s;
			mfmU64* b = u8b;
			mfmU8* e = u8e;
			while (*s != NULL)
			{
				mfmU8 out = 0;
				TEST_REQUIRE_PASS(mfsParseFromBufferU8(*s, UINT64_MAX, &out, *b, NULL) == MF_ERROR_OKAY);
				TEST_REQUIRE_PASS(out == *e);

				++s;
				++b;
				++e;
			}	
		}

		// Invalid
		{
			const mfsUTF8CodeUnit* u8s[] =
			{
				u8"23",
				u8"",
				u8"128",
				u8"127",
				u8"256",
				u8"fff",
				u8"111111111",
				NULL
			};

			mfmU64 u8b[] =
			{
				3,
				10,
				0,
				1,
				10,
				16,
				2,
			};

			const mfsUTF8CodeUnit** s = u8s;
			mfmU64* b = u8b;
			while (*s != NULL)
			{
				mfmU8 out = 0;
				TEST_REQUIRE_FAIL(mfsParseFromBufferU8(*s, UINT64_MAX, &out, *b, NULL) == MF_ERROR_OKAY);

				++s;
				++b;
			}
		}
	}

	// I8 conversion tests
	{
		// Valid
		{
			const mfsUTF8CodeUnit* i8s[] =
			{
				u8"-23",
				u8"127",
				u8"0f",
				u8"42",
				u8"-127",
				u8"10110",
				u8"-101",
				NULL
			};

			mfmU64 i8b[] =
			{
				10,
				10,
				16,
				10,
				10,
				2,
				2,
			};

			mfmI8 i8e[] =
			{
				-23,
				127,
				0x0f,
				42,
				-127,
				0b10110,
				-0b101,
			};

			const mfsUTF8CodeUnit** s = i8s;
			mfmU64* b = i8b;
			mfmI8* e = i8e;
			while (*s != NULL)
			{
				mfmI8 out = 0;
				TEST_REQUIRE_PASS(mfsParseFromBufferI8(*s, UINT64_MAX, &out, *b, NULL) == MF_ERROR_OKAY);
				TEST_REQUIRE_PASS(out == *e);

				++s;
				++b;
				++e;
			}
		}

		// Invalid
		{
			const mfsUTF8CodeUnit* i8s[] =
			{
				u8"23",
				u8"",
				u8"321",
				u8"-124",
				u8"-128",
				u8"ff",
				u8"11111111",
				NULL
			};

			mfmU64 i8b[] =
			{
				3,
				10,
				0,
				1,
				10,
				16,
				2,
			};

			const mfsUTF8CodeUnit** s = i8s;
			mfmU64* b = i8b;
			while (*s != NULL)
			{
				mfmI8 out = 0;
				TEST_REQUIRE_FAIL(mfsParseFromBufferI8(*s, UINT64_MAX, &out, *b, NULL) == MF_ERROR_OKAY);

				++s;
				++b;
			}
		}
	}

	// U16 conversion tests
	{
		// Valid
		{
			const mfsUTF8CodeUnit* u16s[] =
			{
				u8"23",
				u8"255",
				u8"33000",
				u8"65535",
				u8"94",
				u8"fff0",
				u8"1111111111111111",
				NULL
			};

			mfmU64 u16b[] =
			{
				10,
				10,
				10,
				10,
				10,
				16,
				2,
			};

			mfmU16 u16e[] =
			{
				23,
				255,
				33000,
				65535,
				94,
				0xFFF0,
				0b1111111111111111,
			};

			const mfsUTF8CodeUnit** s = u16s;
			mfmU64* b = u16b;
			mfmU16* e = u16e;
			while (*s != NULL)
			{
				mfmU16 out = 0;
				TEST_REQUIRE_PASS(mfsParseFromBufferU16(*s, UINT64_MAX, &out, *b, NULL) == MF_ERROR_OKAY);
				TEST_REQUIRE_PASS(out == *e);

				++s;
				++b;
				++e;
			}
		}

		// Invalid
		{
			const mfsUTF8CodeUnit* u16s[] =
			{
				u8"2332323",
				u8"",
				u8"128",
				u8"127",
				u8"65536",
				u8"1ffff",
				u8"111111111111111111",
				NULL
			};

			mfmU64 u8b[] =
			{
				3,
				10,
				0,
				1,
				10,
				16,
				2,
			};

			const mfsUTF8CodeUnit** s = u16s;
			mfmU64* b = u8b;
			while (*s != NULL)
			{
				mfmU16 out = 0;
				TEST_REQUIRE_FAIL(mfsParseFromBufferU16(*s, UINT64_MAX, &out, *b, NULL) == MF_ERROR_OKAY);

				++s;
				++b;
			}
		}
	}

	// I16 conversion tests
	{
		// Valid
		{
			const mfsUTF8CodeUnit* i16s[] =
			{
				u8"-23",
				u8"255",
				u8"30000",
				u8"-30000",
				u8"94",
				u8"0fff",
				u8"111111111111111",
				NULL
			};

			mfmU64 i16b[] =
			{
				10,
				10,
				10,
				10,
				10,
				16,
				2,
			};

			mfmI16 i16e[] =
			{
				-23,
				255,
				30000,
				-30000,
				94,
				0x0FFF,
				0b111111111111111,
			};

			const mfsUTF8CodeUnit** s = i16s;
			mfmU64* b = i16b;
			mfmI16* e = i16e;
			while (*s != NULL)
			{
				mfmI16 out = 0;
				TEST_REQUIRE_PASS(mfsParseFromBufferI16(*s, UINT64_MAX, &out, *b, NULL) == MF_ERROR_OKAY);
				TEST_REQUIRE_PASS(out == *e);

				++s;
				++b;
				++e;
			}
		}

		// Invalid
		{
			const mfsUTF8CodeUnit* i16s[] =
			{
				u8"2332323",
				u8"",
				u8"128",
				u8"127",
				u8"65535",
				u8"-1ffff",
				u8"11111111111111111",
				NULL
			};

			mfmU64 u8b[] =
			{
				3,
				10,
				0,
				1,
				10,
				16,
				2,
			};

			const mfsUTF8CodeUnit** s = i16s;
			mfmU64* b = u8b;
			while (*s != NULL)
			{
				mfmI16 out = 0;
				TEST_REQUIRE_FAIL(mfsParseFromBufferI16(*s, UINT64_MAX, &out, *b, NULL) == MF_ERROR_OKAY);

				++s;
				++b;
			}
		}
	}

	// U32 conversion tests
	{
		// Valid
		{
			const mfsUTF8CodeUnit* u32s[] =
			{
				u8"23",
				u8"255",
				u8"33000",
				u8"65536",
				u8"94",
				u8"fffffff0",
				u8"11111111111111111111111111111111",
				NULL
			};

			mfmU64 u32b[] =
			{
				10,
				10,
				10,
				10,
				10,
				16,
				2,
			};

			mfmU32 u32e[] =
			{
				23,
				255,
				33000,
				65536,
				94,
				0xfffffff0,
				0b11111111111111111111111111111111,
			};

			const mfsUTF8CodeUnit** s = u32s;
			mfmU64* b = u32b;
			mfmU32* e = u32e;
			while (*s != NULL)
			{
				mfmU32 out = 0;
				TEST_REQUIRE_PASS(mfsParseFromBufferU32(*s, UINT64_MAX, &out, *b, NULL) == MF_ERROR_OKAY);
				TEST_REQUIRE_PASS(out == *e);

				++s;
				++b;
				++e;
			}
		}

		// Invalid
		{
			const mfsUTF8CodeUnit* u32s[] =
			{
				u8"2332323",
				u8"",
				u8"128",
				u8"127",
				u8"1ffffffff",
				u8"111111111111111111111111111111111",
				NULL
			};

			mfmU64 u8b[] =
			{
				3,
				10,
				0,
				1,
				16,
				2,
			};

			const mfsUTF8CodeUnit** s = u32s;
			mfmU64* b = u8b;
			while (*s != NULL)
			{
				mfmU32 out = 0;
				TEST_REQUIRE_FAIL(mfsParseFromBufferU32(*s, UINT64_MAX, &out, *b, NULL) == MF_ERROR_OKAY);

				++s;
				++b;
			}
		}
	}

	// I32 conversion tests
	{
		// Valid
		{
			const mfsUTF8CodeUnit* i32s[] =
			{
				u8"-23",
				u8"255",
				u8"30000",
				u8"-30000",
				u8"94",
				u8"0fff",
				u8"1111111111111111111111111111111",
				NULL
			};

			mfmU64 i32b[] =
			{
				10,
				10,
				10,
				10,
				10,
				16,
				2,
			};

			mfmI32 i32e[] =
			{
				-23,
				255,
				30000,
				-30000,
				94,
				0x0FFF,
				0b1111111111111111111111111111111,
			};

			const mfsUTF8CodeUnit** s = i32s;
			mfmU64* b = i32b;
			mfmI32* e = i32e;
			while (*s != NULL)
			{
				mfmI32 out = 0;
				TEST_REQUIRE_PASS(mfsParseFromBufferI32(*s, UINT64_MAX, &out, *b, NULL) == MF_ERROR_OKAY);
				TEST_REQUIRE_PASS(out == *e);

				++s;
				++b;
				++e;
			}
		}

		// Invalid
		{
			const mfsUTF8CodeUnit* i32s[] =
			{
				u8"2332323",
				u8"",
				u8"128",
				u8"127",
				u8"2147483649",
				u8"-1ffffffff",
				u8"11111111111111111111111111111111",
				NULL
			};

			mfmU64 u8b[] =
			{
				3,
				10,
				0,
				1,
				10,
				16,
				2,
			};

			const mfsUTF8CodeUnit** s = i32s;
			mfmU64* b = u8b;
			while (*s != NULL)
			{
				mfmI32 out = 0;
				TEST_REQUIRE_FAIL(mfsParseFromBufferI32(*s, UINT64_MAX, &out, *b, NULL) == MF_ERROR_OKAY);

				++s;
				++b;
			}
		}
	}

	// U64 conversion tests
	{
		// Valid
		{
			const mfsUTF8CodeUnit* u64s[] =
			{
				u8"23",
				u8"255",
				u8"33000",
				u8"65536",
				u8"94",
				u8"fffffffffffffff0",
				u8"1111111111111111111111111111111111111111111111111111111111111111",
				NULL
			};

			mfmU64 u64b[] =
			{
				10,
				10,
				10,
				10,
				10,
				16,
				2,
			};

			mfmU64 u64e[] =
			{
				23,
				255,
				33000,
				65536,
				94,
				0xfffffffffffffff0,
				0b1111111111111111111111111111111111111111111111111111111111111111,
			};

			const mfsUTF8CodeUnit** s = u64s;
			mfmU64* b = u64b;
			mfmU64* e = u64e;
			while (*s != NULL)
			{
				mfmU64 out = 0;
				TEST_REQUIRE_PASS(mfsParseFromBufferU64(*s, UINT64_MAX, &out, *b, NULL) == MF_ERROR_OKAY);
				TEST_REQUIRE_PASS(out == *e);

				++s;
				++b;
				++e;
			}
		}

		// Invalid
		{
			const mfsUTF8CodeUnit* u64s[] =
			{
				u8"2332323",
				u8"",
				u8"128",
				u8"127",
				u8"fffffffffffffffff",
				u8"111111111111111111111111111111111111111111111111111111111111111111",
				NULL
			};

			mfmU64 u8b[] =
			{
				3,
				10,
				0,
				1,
				16,
				2,
			};

			const mfsUTF8CodeUnit** s = u64s;
			mfmU64* b = u8b;
			while (*s != NULL)
			{
				mfmU64 out = 0;
				TEST_REQUIRE_FAIL(mfsParseFromBufferU64(*s, UINT64_MAX, &out, *b, NULL) == MF_ERROR_OKAY);

				++s;
				++b;
			}
		}
	}

	// I64 conversion tests
	{
		// Valid
		{
			const mfsUTF8CodeUnit* i64s[] =
			{
				u8"-23",
				u8"255",
				u8"30000",
				u8"-30000",
				u8"94",
				u8"0fff",
				u8"1111111111111111111111111111111",
				NULL
			};

			mfmU64 i64b[] =
			{
				10,
				10,
				10,
				10,
				10,
				16,
				2,
			};

			mfmI64 i64e[] =
			{
				-23,
				255,
				30000,
				-30000,
				94,
				0x0FFF,
				0b1111111111111111111111111111111,
			};

			const mfsUTF8CodeUnit** s = i64s;
			mfmU64* b = i64b;
			mfmI64* e = i64e;
			while (*s != NULL)
			{
				mfmI64 out = 0;
				TEST_REQUIRE_PASS(mfsParseFromBufferI64(*s, UINT64_MAX, &out, *b, NULL) == MF_ERROR_OKAY);
				TEST_REQUIRE_PASS(out == *e);

				++s;
				++b;
				++e;
			}
		}

		// Invalid
		{
			const mfsUTF8CodeUnit* i64s[] =
			{
				u8"2332323",
				u8"",
				u8"128",
				u8"127",
				u8"18446744073709551616",
				u8"-1ffffffffffffffff",
				u8"1111111111111111111111111111111111111111111111111111111111111111",
				NULL
			};

			mfmU64 u8b[] =
			{
				3,
				10,
				0,
				1,
				10,
				16,
				2,
			};

			const mfsUTF8CodeUnit** s = i64s;
			mfmU64* b = u8b;
			while (*s != NULL)
			{
				mfmI64 out = 0;
				TEST_REQUIRE_FAIL(mfsParseFromBufferI64(*s, UINT64_MAX, &out, *b, NULL) == MF_ERROR_OKAY);

				++s;
				++b;
			}
		}
	}

	EXIT_PASS();
}