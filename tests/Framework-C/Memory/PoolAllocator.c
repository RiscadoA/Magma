#include "../../Test.h"

#include <Magma/Framework/Memory/PoolAllocator.h>
#include <Magma/Framework/Entry.h>

int main(int argc, char** argv)
{
	TEST_REQUIRE_PASS(mfInit(argc, argv) == MF_ERROR_OKAY);

	{
		mfmPoolAllocator* pool = NULL;
		mfmPoolAllocatorDesc desc;

		desc.expandable = MFM_FALSE;
		desc.slotCount = 1;
		desc.slotSize = sizeof(mfmU8);

		TEST_REQUIRE_PASS(mfmCreatePoolAllocator(&pool, &desc) == MF_ERROR_OKAY);

		mfmU8* v0 = NULL;
		mfmU8* v1 = NULL;
		TEST_REQUIRE_PASS(mfmAllocate(pool, &v0, sizeof(mfmU8)) == MF_ERROR_OKAY);
		TEST_REQUIRE_FAIL(mfmAllocate(pool, &v1, sizeof(mfmU8)) == MF_ERROR_OKAY);
		TEST_REQUIRE_FAIL(mfmDeallocate(pool, v1) == MF_ERROR_OKAY);
		TEST_REQUIRE_PASS(mfmDeallocate(pool, v0) == MF_ERROR_OKAY);
		TEST_REQUIRE_PASS(mfmAllocate(pool, &v1, sizeof(mfmU8)) == MF_ERROR_OKAY);

		mfmDestroyPoolAllocator(pool);
	}

	mfTerminate();
	EXIT_PASS();
}