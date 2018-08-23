#include <Magma/Framework/Memory/PoolAllocator.h>
#include <Magma/Framework/Entry.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char** argv)
{
	if (mfInit(argc, argv) != MF_ERROR_OKAY)
		abort();

	mfError err = MFM_ERROR_OKAY;
	mfmPoolAllocator* pool48;

	// Create pool48
	{
		mfmPoolAllocatorDesc desc;
		desc.expandable = MFM_TRUE;
		desc.slotCount = 1;
		desc.slotSize = sizeof(mfmU32);
		err = mfmCreatePoolAllocator(&pool48, &desc);
		if (err != MFM_ERROR_OKAY)
			abort();
	}

	printf("Created pool, stats:\n");
	printf("Current free slot count: %d\n", mfmPoolGetFreeSlotCount(pool48));
	printf("Current occupied slot count: %d\n",  mfmPoolGetOccupiedSlotCount(pool48));
	printf("Current slot count: %d\n", mfmPoolGetSlotCount(pool48));
	printf("Current chunk count: %d\n", mfmPoolGetChunkCount(pool48));

	// Allocate and deallocate value
	{
		mfmU8* value;
		err = mfmPoolAllocate(pool48, (void**)&value, sizeof(mfmU32));
		if (err != MFM_ERROR_OKAY)
			abort();

		printf("Allocated on pool, stats:\n");
		printf("Current free slot count: %d\n", mfmPoolGetFreeSlotCount(pool48));
		printf("Current occupied slot count: %d\n", mfmPoolGetOccupiedSlotCount(pool48));
		printf("Current slot count: %d\n", mfmPoolGetSlotCount(pool48));
		printf("Current chunk count: %d\n", mfmPoolGetChunkCount(pool48));

		err = mfmPoolDeallocate(pool48, value);
		if (err != MFM_ERROR_OKAY)
			abort();

		printf("Deallocated on pool, stats:\n");
		printf("Current free slot count: %d\n", mfmPoolGetFreeSlotCount(pool48));
		printf("Current occupied slot count: %d\n", mfmPoolGetOccupiedSlotCount(pool48));
		printf("Current slot count: %d\n", mfmPoolGetSlotCount(pool48));
		printf("Current chunk count: %d\n", mfmPoolGetChunkCount(pool48));
	}

	{
		mfmU8* value;
		err = mfmPoolAllocate(pool48, (void**)&value, sizeof(mfmU32));
		if (err != MFM_ERROR_OKAY)
			abort();
	}

	printf("Allocated on pool, stats:\n");
	printf("Current free slot count: %d\n", mfmPoolGetFreeSlotCount(pool48));
	printf("Current occupied slot count: %d\n", mfmPoolGetOccupiedSlotCount(pool48));
	printf("Current slot count: %d\n", mfmPoolGetSlotCount(pool48));
	printf("Current chunk count: %d\n", mfmPoolGetChunkCount(pool48));

	{
		mfmU8* value;
		err = mfmPoolAllocate(pool48, (void**)&value, sizeof(mfmU32));
		if (err != MFM_ERROR_OKAY)
			abort();
	}

	printf("Allocated on pool, stats:\n");
	printf("Current free slot count: %d\n", mfmPoolGetFreeSlotCount(pool48));
	printf("Current occupied slot count: %d\n", mfmPoolGetOccupiedSlotCount(pool48));
	printf("Current slot count: %d\n", mfmPoolGetSlotCount(pool48));
	printf("Current chunk count: %d\n", mfmPoolGetChunkCount(pool48));


	// Destroy pool48
	mfmDestroyPoolAllocator(pool48);

	printf("Program finished. Enter any input to quit.\n");
	getchar();

	mfTerminate();
	return 0;
}