﻿#include <Magma/Framework/Memory/PoolAllocator.h>
#include <Magma/Framework/Input/Entry.h>
#include <stdio.h>
#include <stdlib.h>

void Main(int argc, char** argv)
{
	mfmError err = MFM_ERROR_OKAY;
	mfmPoolAllocator* pool32;

	// Create pool32
	{
		mfmPoolAllocatorDesc desc;
		desc.expandable = MFM_TRUE;
		desc.slotCount = 1;
		desc.slotSize = sizeof(mfmU32);
		err = mfmCreatePoolAllocator(&pool32, &desc);
		if (err != MFM_ERROR_OKAY)
			abort();
	}

	printf("Created pool, stats:\n");
	printf("Current free slot count: %d\n", mfmPoolGetFreeSlotCount(pool32));
	printf("Current occupied slot count: %d\n",  mfmPoolGetOccupiedSlotCount(pool32));
	printf("Current slot count: %d\n", mfmPoolGetSlotCount(pool32));
	printf("Current chunk count: %d\n", mfmPoolGetChunkCount(pool32));

	// Allocate and deallocate value
	{
		mfmU8* value;
		err = mfmPoolAllocate(pool32, (void**)&value, sizeof(mfmU32));
		if (err != MFM_ERROR_OKAY)
			abort();

		printf("Allocated on pool, stats:\n");
		printf("Current free slot count: %d\n", mfmPoolGetFreeSlotCount(pool32));
		printf("Current occupied slot count: %d\n", mfmPoolGetOccupiedSlotCount(pool32));
		printf("Current slot count: %d\n", mfmPoolGetSlotCount(pool32));
		printf("Current chunk count: %d\n", mfmPoolGetChunkCount(pool32));

		err = mfmPoolDeallocate(pool32, value);
		if (err != MFM_ERROR_OKAY)
			abort();

		printf("Deallocated on pool, stats:\n");
		printf("Current free slot count: %d\n", mfmPoolGetFreeSlotCount(pool32));
		printf("Current occupied slot count: %d\n", mfmPoolGetOccupiedSlotCount(pool32));
		printf("Current slot count: %d\n", mfmPoolGetSlotCount(pool32));
		printf("Current chunk count: %d\n", mfmPoolGetChunkCount(pool32));
	}

	{
		mfmU8* value;
		err = mfmPoolAllocate(pool32, (void**)&value, sizeof(mfmU32));
		if (err != MFM_ERROR_OKAY)
			abort();
	}

	printf("Allocated on pool, stats:\n");
	printf("Current free slot count: %d\n", mfmPoolGetFreeSlotCount(pool32));
	printf("Current occupied slot count: %d\n", mfmPoolGetOccupiedSlotCount(pool32));
	printf("Current slot count: %d\n", mfmPoolGetSlotCount(pool32));
	printf("Current chunk count: %d\n", mfmPoolGetChunkCount(pool32));

	{
		mfmU8* value;
		err = mfmPoolAllocate(pool32, (void**)&value, sizeof(mfmU32));
		if (err != MFM_ERROR_OKAY)
			abort();
	}

	printf("Allocated on pool, stats:\n");
	printf("Current free slot count: %d\n", mfmPoolGetFreeSlotCount(pool32));
	printf("Current occupied slot count: %d\n", mfmPoolGetOccupiedSlotCount(pool32));
	printf("Current slot count: %d\n", mfmPoolGetSlotCount(pool32));
	printf("Current chunk count: %d\n", mfmPoolGetChunkCount(pool32));


	// Destroy pool32
	mfmDestroyPoolAllocator(pool32);

	printf("Program finished. Enter any input to quit.\n");
	getchar();
}