#pragma once

#include "../Memory/Type.h"
#include "Bytecode.h"
#include "Error.h"

/*
	Variable data typedefs.
*/

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct mfvVirtualMachine mfvVirtualMachine;

	typedef struct
	{
		mfmU64 stackSize;
		mfmU64 callStackSize;
		mfmU64 functionTableSize;
	} mfvVirtualMachineDesc;

	mfError mfvCreateVirtualMachine(mfvVirtualMachine** vm, const mfvVirtualMachineDesc* desc, void* allocator);

	void mfmDestroyVirtualMachine(void* vm);

	mfError mfvSetVirtualMachineCode(mfvVirtualMachine* vm, mfmU64 ip, const mfmU8* instructions);
	
	mfError mfvStepVirtualMachine(mfvVirtualMachine* vm);

	mfError mfvRunVirtualMachine(mfvVirtualMachine* vm, const mfmU64* instructionCount, mfmU64* executedInstructions);

	mfError mfvSetVirtualMachineFunction(mfvVirtualMachine* vm, mfmU64 id, void(*func)(mfvVirtualMachine*));

	mfError mfvRemoveVirtualMachineFunction(mfvVirtualMachine* vm, mfmU64 id);

#ifdef __cplusplus
}
#endif
