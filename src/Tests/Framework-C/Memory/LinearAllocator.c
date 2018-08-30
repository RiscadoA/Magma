#include "../../Test.h"

#include <Magma/Framework/Memory/LinearAllocator.h>
#include <Magma/Framework/Entry.h>

int main(int argc, char** argv)
{
	TEST_REQUIRE_PASS(mfInit(argc, argv) == MF_ERROR_OKAY);

	{
		mfmLinearAllocator* linear = NULL;

		TEST_REQUIRE_PASS(mfmCreateLinearAllocator(&linear, sizeof(mfmU8) * 3) == MF_ERROR_OKAY);

		mfmU8* v0 = NULL;
		mfmU8* v1 = NULL;
		TEST_REQUIRE_PASS(mfmAllocate(linear, &v0, sizeof(mfmU8) * 2) == MF_ERROR_OKAY);
		TEST_REQUIRE_FAIL(mfmAllocate(linear, &v1, sizeof(mfmU8) * 2) == MF_ERROR_OKAY);
		TEST_REQUIRE_PASS(mfmAllocate(linear, &v1, sizeof(mfmU8) * 1) == MF_ERROR_OKAY);
		TEST_REQUIRE_PASS(mfmDeallocate(linear, v1) == MFM_ERROR_UNSUPPORTED_FUNCTION);
		TEST_REQUIRE_PASS(mfmDeallocate(linear, v0) == MFM_ERROR_UNSUPPORTED_FUNCTION);
		TEST_REQUIRE_FAIL(mfmAllocate(linear, &v1, sizeof(mfmU8) * 3) == MF_ERROR_OKAY);
		TEST_REQUIRE_PASS(mfmLinearReset(linear));
		TEST_REQUIRE_PASS(mfmAllocate(linear, &v1, sizeof(mfmU8) * 1) == MF_ERROR_OKAY);

		mfmDestroyLinearAllocator(linear);
	}

	mfTerminate();
	EXIT_PASS();
}