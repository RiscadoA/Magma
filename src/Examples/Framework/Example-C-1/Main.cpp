#include <Magma/Framework/Memory/PoolAllocator.h>
#include <Magma/Framework/Input/Entry.h>
#include <cstdio>
#include <cstdlib>

void Main(int argc, char** argv)
{
	mfmError err = MFM_ERROR_OKAY;
	mfmPoolAllocator* pool;

	// Create pool
	{
		mfmPoolAllocatorDesc desc;
		desc.expandable = true;
		desc.slotCount = 1;
		desc.slotSize = sizeof(mfmU32);
		err = mfmCreatePoolAllocator(&pool, &desc);
		if (err != MFM_ERROR_OKAY)
			abort();
	}

	printf("Created pool, stats:\n");
	printf("Current free slot count: %d\n", mfmPoolGetFreeSlotCount(pool));
	printf("Current occupied slot count: %d\n",  mfmPoolGetOccupiedSlotCount(pool));
	printf("Current slot count: %d\n", mfmPoolGetSlotCount(pool));
	printf("Current chunk count: %d\n", mfmPoolGetChunkCount(pool));

	// Allocate and deallocate value
	{
		mfmU8* value;
		err = mfmPoolAllocate(pool, (void**)&value, sizeof(mfmU32));
		if (err != MFM_ERROR_OKAY)
			abort();

		printf("Allocated on pool, stats:\n");
		printf("Current free slot count: %d\n", mfmPoolGetFreeSlotCount(pool));
		printf("Current occupied slot count: %d\n", mfmPoolGetOccupiedSlotCount(pool));
		printf("Current slot count: %d\n", mfmPoolGetSlotCount(pool));
		printf("Current chunk count: %d\n", mfmPoolGetChunkCount(pool));

		err = mfmPoolDeallocate(value, pool);
		if (err != MFM_ERROR_OKAY)
			abort();

		printf("Deallocated on pool, stats:\n");
		printf("Current free slot count: %d\n", mfmPoolGetFreeSlotCount(pool));
		printf("Current occupied slot count: %d\n", mfmPoolGetOccupiedSlotCount(pool));
		printf("Current slot count: %d\n", mfmPoolGetSlotCount(pool));
		printf("Current chunk count: %d\n", mfmPoolGetChunkCount(pool));
	}

	{
		mfmU8* value;
		err = mfmPoolAllocate(pool, (void**)&value, sizeof(mfmU32));
		if (err != MFM_ERROR_OKAY)
			abort();
	}

	printf("Allocated on pool, stats:\n");
	printf("Current free slot count: %d\n", mfmPoolGetFreeSlotCount(pool));
	printf("Current occupied slot count: %d\n", mfmPoolGetOccupiedSlotCount(pool));
	printf("Current slot count: %d\n", mfmPoolGetSlotCount(pool));
	printf("Current chunk count: %d\n", mfmPoolGetChunkCount(pool));

	{
		mfmU8* value;
		err = mfmPoolAllocate(pool, (void**)&value, sizeof(mfmU32));
		if (err != MFM_ERROR_OKAY)
			abort();
	}

	printf("Allocated on pool, stats:\n");
	printf("Current free slot count: %d\n", mfmPoolGetFreeSlotCount(pool));
	printf("Current occupied slot count: %d\n", mfmPoolGetOccupiedSlotCount(pool));
	printf("Current slot count: %d\n", mfmPoolGetSlotCount(pool));
	printf("Current chunk count: %d\n", mfmPoolGetChunkCount(pool));


	// Destroy pool
	mfmDestroyPoolAllocator(pool);

	printf("Program finished. Enter any input to quit.\n");
	getchar();
}