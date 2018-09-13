#include "../../Test.h"

#include <Magma/Framework/Memory/StackAllocator.h>
#include <Magma/Framework/Entry.h>

int main(int argc, char** argv)
{
	TEST_REQUIRE_PASS(mfInit(argc, argv) == MF_ERROR_OKAY);

	{
		mfmStackAllocator* stack = NULL;

		TEST_REQUIRE_PASS(mfmCreateStackAllocator(&stack, sizeof(mfmU8) * 3) == MF_ERROR_OKAY);

		mfmU8* v0 = NULL;
		mfmU8* v1 = NULL;
		mfmU8* v2 = NULL;
		TEST_REQUIRE_PASS(mfmAllocate(stack, &v0, sizeof(mfmU8) * 2) == MF_ERROR_OKAY);
		TEST_REQUIRE_FAIL(mfmAllocate(stack, &v1, sizeof(mfmU8) * 2) == MF_ERROR_OKAY);
		TEST_REQUIRE_PASS(mfmAllocate(stack, &v2, sizeof(mfmU8) * 1) == MF_ERROR_OKAY);
		TEST_REQUIRE_FAIL(mfmDeallocate(stack, v1) == MF_ERROR_OKAY);
		TEST_REQUIRE_PASS(mfmStackSetHead(stack, v0) == MF_ERROR_OKAY);
		TEST_REQUIRE_PASS(mfmAllocate(stack, &v1, sizeof(mfmU8) * 3) == MF_ERROR_OKAY);

		mfmDestroyStackAllocator(stack);
	}

	mfTerminate();
	EXIT_PASS();
}