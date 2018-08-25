#pragma once

#include "../Memory/Type.h"
#include "../Memory/Object.h"
#include "Bytecode.h"
#include "Error.h"

/*
	Variable data typedefs.
*/

#ifdef __cplusplus
extern "C"
{
#endif

#define MFV_STATE_UNFINISHED	0x01
#define MFV_STATE_FINISHED		0x02
#define MFV_STATE_YIELD			0x03

	// Is a mfmObject
	typedef struct mfvVirtualMachine mfvVirtualMachine;

	typedef mfError(*mfvVirtualMachineFunction)(mfvVirtualMachine *);
	typedef mfmU32 mfvInstructionPointer;
	typedef mfmU32 mfvVirtualMachineState;

	typedef struct
	{
		mfmU64 stackSize;
		mfmU64 callStackSize;
		mfmU32 registerCount;
		mfmU16 functionTableSize;
	} mfvVirtualMachineDesc;

	mfError mfvCreateVirtualMachine(mfvVirtualMachine** vm, const mfvVirtualMachineDesc* desc, void* allocator);

	void mfvDestroyVirtualMachine(void* vm);

	mfError mfvSetVirtualMachineCode(mfvVirtualMachine* vm, mfvInstructionPointer ip, const mfmU8* instructions);
	
	mfError mfvStepVirtualMachine(mfvVirtualMachine* vm, mfvVirtualMachineState* state);

	mfError mfvRunVirtualMachine(mfvVirtualMachine* vm, const mfmU64* instructionCount, mfmU64* executedInstructions, mfvVirtualMachineState* state);

	mfError mfvSetVirtualMachineFunction(mfvVirtualMachine* vm, mfmU16 id, mfvVirtualMachineFunction func);

	mfError mfvRemoveVirtualMachineFunction(mfvVirtualMachine* vm, mfmU16 id);

	mfError mfvVirtualMachinePush8(mfvVirtualMachine* vm, const void* value);

	mfError mfvVirtualMachinePush16(mfvVirtualMachine* vm, const void* value);

	mfError mfvVirtualMachinePush32(mfvVirtualMachine* vm, const void* value);

	mfError mfvVirtualMachinePop8(mfvVirtualMachine* vm, void* value);

	mfError mfvVirtualMachinePop16(mfvVirtualMachine* vm, void* value);

	mfError mfvVirtualMachinePop32(mfvVirtualMachine* vm, void* value);

#ifdef __cplusplus
}
#endif
