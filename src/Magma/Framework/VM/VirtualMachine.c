#include "VirtualMachine.h"
#include "Config.h"
#include "../Memory/Allocator.h"
#include "../Memory/Endianness.h"

#include <string.h>
#include <stdlib.h>
#include <math.h>

struct mfvVirtualMachine
{
	mfmObject object;
	mfvVirtualMachineDesc desc;
	mfvInstructionPointer ip;
	void* allocator;
	const mfmU8* code;
	mfmU64 stackHead;
	mfmU64 callStackHead;
	mfmU8* stack;
	mfvInstructionPointer* callStack;
	mfvVirtualMachineFunction* functionTable;
	mfmU8* registers8;
	mfmU16* registers16;
	mfmU32* registers32;
};

mfError mfvCreateVirtualMachine(mfvVirtualMachine ** vm, const mfvVirtualMachineDesc * desc, void * allocator)
{
	mfmU8* memory = NULL;
	mfError err = mfmAllocate(allocator, &memory,
							  sizeof(mfvVirtualMachine) +
							  desc->stackSize +
							  desc->callStackSize * sizeof(mfvInstructionPointer) +
							  desc->functionTableSize * sizeof(mfvVirtualMachineFunction) +
							  desc->registerCount * sizeof(mfmU32));
	if (err != MF_ERROR_OKAY)
		return err;

	*vm = memory;
	memcpy(&(*vm)->desc, desc, sizeof(mfvVirtualMachineDesc));

	{
		mfError err = mfmInitObject(&(*vm)->object);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	(*vm)->object.destructorFunc = &mfvDestroyVirtualMachine;
	(*vm)->allocator = allocator;
	if ((*vm)->allocator != NULL)
	{
		mfError err = mfmIncObjectRef(((mfmObject*)(*vm)->allocator));
		if (err != MF_ERROR_OKAY)
			return err;
	}

	(*vm)->stack = memory + sizeof(mfvVirtualMachine);
	(*vm)->callStack = memory + sizeof(mfvVirtualMachine) + desc->stackSize;
	(*vm)->functionTable = memory + sizeof(mfvVirtualMachine) + desc->stackSize + desc->callStackSize * sizeof(mfvInstructionPointer);
	(*vm)->registers32 =
		memory +
		sizeof(mfvVirtualMachine) +
		desc->stackSize +
		desc->callStackSize * sizeof(mfvInstructionPointer) +
		desc->functionTableSize * sizeof(mfvVirtualMachineFunction);
	(*vm)->registers16 = (*vm)->registers32;
	(*vm)->registers8 = (*vm)->registers32;

	for (mfmU64 i = 0; i < desc->functionTableSize; ++i)
		(*vm)->functionTable[i] = NULL;

	(*vm)->code = NULL;
	(*vm)->ip = 0;
	(*vm)->stackHead = 0;
	(*vm)->callStackHead = 0;

	return MF_ERROR_OKAY;
}

void mfvDestroyVirtualMachine(void * vm)
{
	if (vm == NULL)
		abort();
	mfvVirtualMachine* v = (mfvVirtualMachine*)vm;
	if (v->allocator != NULL)
	{
		mfError err = mfmDecObjectRef((mfmObject*)v->allocator);
		if (err != MF_ERROR_OKAY)
			return err;
	}
	if (mfmDestroyObject(&v->object) != MF_ERROR_OKAY)
		abort();
	if (mfmDeallocate(v->allocator, v) != MF_ERROR_OKAY)
		abort();
}

mfError mfvSetVirtualMachineCode(mfvVirtualMachine * vm, mfvInstructionPointer ip, const mfmU8 * instructions)
{
	if (vm == NULL)
		return MFV_ERROR_INVALID_ARGUMENTS;
	vm->code = instructions;
	vm->ip = ip;
	return MF_ERROR_OKAY;
}

mfError mfvStepVirtualMachine(mfvVirtualMachine * vm, mfvVirtualMachineState* state)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (vm == NULL)
		return MFV_ERROR_INVALID_ARGUMENTS;
	if (vm->code == MFV_ERROR_NULL_CODE)
		return MFV_ERROR_NULL_CODE;
#endif

	mfError err = MF_ERROR_OKAY;

	if (state != NULL)
		*state = MFV_STATE_UNFINISHED;
	
	switch (vm->code[vm->ip])
	{
		// Stack operations
		{
			case MFV_BYTECODE_POP:
			{
				mfmU8 size = vm->code[vm->ip + 1];
				vm->ip += 2;
				if (vm->stackHead < size)
					return MFV_ERROR_STACK_UNDERFLOW;
				vm->stackHead -= size;
				break;
			}

			case MFV_BYTECODE_PUSH_COPY:
			{
				mfmU8 count = vm->code[vm->ip + 1];
				vm->ip += 2;
				if (vm->stackHead < count)
					return MFV_ERROR_STACK_UNDERFLOW;
				if (vm->stackHead + count > vm->desc.stackSize)
					return MFV_ERROR_STACK_OVERFLOW;
				memcpy(vm->stack + vm->stackHead, vm->stack + vm->stackHead - count, count);
				vm->stackHead += count;
				break;
			}

			case MFV_BYTECODE_PUSH8:
			{
				err = mfvVirtualMachinePush8(vm, vm->code + vm->ip + 1);
				vm->ip += 2;
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_PUSH16:
			{
				mfmU16 value;
				mfmFromBigEndian2(vm->code + vm->ip + 1, &value);
				err = mfvVirtualMachinePush16(vm, &value);
				vm->ip += 3;
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_PUSH32:
			{
				mfmU32 value;
				mfmFromBigEndian4(vm->code + vm->ip + 1, &value);
				err = mfvVirtualMachinePush32(vm, &value);
				vm->ip += 5;
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}
		}

		// 8 bit operations
		{
			case MFV_BYTECODE_ADDS8:
			{
				vm->ip += 1;

				mfmI8 val1, val2;
				err = mfvVirtualMachinePop8(vm, &val1);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfvVirtualMachinePop8(vm, &val2);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmI8 val = val1 + val2;
				err = mfvVirtualMachinePush8(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_SUBS8:
			{
				vm->ip += 1;

				mfmI8 val1, val2;
				err = mfvVirtualMachinePop8(vm, &val1);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfvVirtualMachinePop8(vm, &val2);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmI8 val = val1 - val2;
				err = mfvVirtualMachinePush8(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_MULS8:
			{
				vm->ip += 1;

				mfmI8 val1, val2;
				err = mfvVirtualMachinePop8(vm, &val1);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfvVirtualMachinePop8(vm, &val2);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmI8 val = val1 * val2;
				err = mfvVirtualMachinePush8(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_DIVS8:
			{
				vm->ip += 1;

				mfmI8 val1, val2;
				err = mfvVirtualMachinePop8(vm, &val1);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfvVirtualMachinePop8(vm, &val2);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmI8 val = val1 / val2;
				err = mfvVirtualMachinePush8(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_MODS8:
			{
				vm->ip += 1;

				mfmI8 val1, val2;
				err = mfvVirtualMachinePop8(vm, &val1);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfvVirtualMachinePop8(vm, &val2);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmI8 val = val1 % val2;
				err = mfvVirtualMachinePush8(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_ADDU8:
			{
				vm->ip += 1;

				mfmU8 val1, val2;
				err = mfvVirtualMachinePop8(vm, &val1);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfvVirtualMachinePop8(vm, &val2);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmU8 val = val1 + val2;
				err = mfvVirtualMachinePush8(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_SUBU8:
			{
				vm->ip += 1;

				mfmU8 val1, val2;
				err = mfvVirtualMachinePop8(vm, &val1);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfvVirtualMachinePop8(vm, &val2);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmU8 val = val1 - val2;
				err = mfvVirtualMachinePush8(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_MULU8:
			{
				vm->ip += 1;

				mfmU8 val1, val2;
				err = mfvVirtualMachinePop8(vm, &val1);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfvVirtualMachinePop8(vm, &val2);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmU8 val = val1 * val2;
				err = mfvVirtualMachinePush8(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_DIVU8:
			{
				vm->ip += 1;

				mfmU8 val1, val2;
				err = mfvVirtualMachinePop8(vm, &val1);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfvVirtualMachinePop8(vm, &val2);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmU8 val = val1 / val2;
				err = mfvVirtualMachinePush8(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_MODU8:
			{
				vm->ip += 1;

				mfmU8 val1, val2;
				err = mfvVirtualMachinePop8(vm, &val1);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfvVirtualMachinePop8(vm, &val2);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmU8 val = val1 % val2;
				err = mfvVirtualMachinePush8(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}
		}

		// 16 bit operations
		{
			case MFV_BYTECODE_ADDS16:
			{
				vm->ip += 1;

				mfmI16 val1, val2;
				err = mfvVirtualMachinePop16(vm, &val1);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfvVirtualMachinePop16(vm, &val2);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmI16 val = val1 + val2;
				err = mfvVirtualMachinePush16(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_SUBS16:
			{
				vm->ip += 1;

				mfmI16 val1, val2;
				err = mfvVirtualMachinePop16(vm, &val1);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfvVirtualMachinePop16(vm, &val2);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmI16 val = val1 - val2;
				err = mfvVirtualMachinePush16(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_MULS16:
			{
				vm->ip += 1;

				mfmI16 val1, val2;
				err = mfvVirtualMachinePop16(vm, &val1);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfvVirtualMachinePop16(vm, &val2);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmI16 val = val1 * val2;
				err = mfvVirtualMachinePush16(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_DIVS16:
			{
				vm->ip += 1;

				mfmI16 val1, val2;
				err = mfvVirtualMachinePop16(vm, &val1);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfvVirtualMachinePop16(vm, &val2);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmI16 val = val1 / val2;
				err = mfvVirtualMachinePush16(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_MODS16:
			{
				vm->ip += 1;

				mfmI16 val1, val2;
				err = mfvVirtualMachinePop16(vm, &val1);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfvVirtualMachinePop16(vm, &val2);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmI16 val = val1 % val2;
				err = mfvVirtualMachinePush16(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_ADDU16:
			{
				vm->ip += 1;

				mfmU16 val1, val2;
				err = mfvVirtualMachinePop16(vm, &val1);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfvVirtualMachinePop16(vm, &val2);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmU16 val = val1 + val2;
				err = mfvVirtualMachinePush16(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_SUBU16:
			{
				vm->ip += 1;

				mfmU16 val1, val2;
				err = mfvVirtualMachinePop16(vm, &val1);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfvVirtualMachinePop16(vm, &val2);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmU16 val = val1 - val2;
				err = mfvVirtualMachinePush16(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_MULU16:
			{
				vm->ip += 1;

				mfmU16 val1, val2;
				err = mfvVirtualMachinePop16(vm, &val1);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfvVirtualMachinePop16(vm, &val2);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmU16 val = val1 * val2;
				err = mfvVirtualMachinePush16(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_DIVU16:
			{
				vm->ip += 1;

				mfmU16 val1, val2;
				err = mfvVirtualMachinePop16(vm, &val1);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfvVirtualMachinePop16(vm, &val2);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmU16 val = val1 / val2;
				err = mfvVirtualMachinePush16(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_MODU16:
			{
				vm->ip += 1;

				mfmU16 val1, val2;
				err = mfvVirtualMachinePop16(vm, &val1);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfvVirtualMachinePop16(vm, &val2);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmU16 val = val1 % val2;
				err = mfvVirtualMachinePush16(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}
		}

		// 32 bit operations
		{
			case MFV_BYTECODE_ADDS32:
			{
				vm->ip += 1;

				mfmI32 val1, val2;
				err = mfvVirtualMachinePop32(vm, &val1);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfvVirtualMachinePop32(vm, &val2);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmI32 val = val1 + val2;
				err = mfvVirtualMachinePush32(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_SUBS32:
			{
				vm->ip += 1;

				mfmI32 val1, val2;
				err = mfvVirtualMachinePop32(vm, &val1);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfvVirtualMachinePop32(vm, &val2);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmI32 val = val1 - val2;
				err = mfvVirtualMachinePush32(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_MULS32:
			{
				vm->ip += 1;

				mfmI32 val1, val2;
				err = mfvVirtualMachinePop32(vm, &val1);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfvVirtualMachinePop32(vm, &val2);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmI32 val = val1 * val2;
				err = mfvVirtualMachinePush32(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_DIVS32:
			{
				vm->ip += 1;

				mfmI32 val1, val2;
				err = mfvVirtualMachinePop32(vm, &val1);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfvVirtualMachinePop32(vm, &val2);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmI32 val = val1 / val2;
				err = mfvVirtualMachinePush32(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_MODS32:
			{
				vm->ip += 1;

				mfmI32 val1, val2;
				err = mfvVirtualMachinePop32(vm, &val1);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfvVirtualMachinePop32(vm, &val2);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmI32 val = val1 % val2;
				err = mfvVirtualMachinePush32(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_ADDU32:
			{
				vm->ip += 1;

				mfmU32 val1, val2;
				err = mfvVirtualMachinePop32(vm, &val1);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfvVirtualMachinePop32(vm, &val2);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmU32 val = val1 + val2;
				err = mfvVirtualMachinePush32(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_SUBU32:
			{
				vm->ip += 1;

				mfmU32 val1, val2;
				err = mfvVirtualMachinePop32(vm, &val1);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfvVirtualMachinePop32(vm, &val2);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmU32 val = val1 - val2;
				err = mfvVirtualMachinePush32(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_MULU32:
			{
				vm->ip += 1;

				mfmU32 val1, val2;
				err = mfvVirtualMachinePop32(vm, &val1);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfvVirtualMachinePop32(vm, &val2);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmU32 val = val1 * val2;
				err = mfvVirtualMachinePush32(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_DIVU32:
			{
				vm->ip += 1;

				mfmU32 val1, val2;
				err = mfvVirtualMachinePop32(vm, &val1);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfvVirtualMachinePop32(vm, &val2);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmU32 val = val1 / val2;
				err = mfvVirtualMachinePush32(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_MODU32:
			{
				vm->ip += 1;

				mfmU32 val1, val2;
				err = mfvVirtualMachinePop32(vm, &val1);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfvVirtualMachinePop32(vm, &val2);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmU32 val = val1 % val2;
				err = mfvVirtualMachinePush32(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}
		}


		// 32 bit floating point operations
		{
			case MFV_BYTECODE_ADDF32:
			{
				vm->ip += 1;

				mfmF32 val1, val2;
				err = mfvVirtualMachinePop32(vm, &val1);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfvVirtualMachinePop32(vm, &val2);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmF32 val = val1 + val2;
				err = mfvVirtualMachinePush32(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_SUBF32:
			{
				vm->ip += 1;

				mfmF32 val1, val2;
				err = mfvVirtualMachinePop32(vm, &val1);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfvVirtualMachinePop32(vm, &val2);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmF32 val = val1 - val2;
				err = mfvVirtualMachinePush32(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_MULF32:
			{
				vm->ip += 1;

				mfmF32 val1, val2;
				err = mfvVirtualMachinePop32(vm, &val1);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfvVirtualMachinePop32(vm, &val2);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmF32 val = val1 * val2;
				err = mfvVirtualMachinePush32(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_DIVF32:
			{
				vm->ip += 1;

				mfmF32 val1, val2;
				err = mfvVirtualMachinePop32(vm, &val1);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfvVirtualMachinePop32(vm, &val2);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmF32 val = val1 / val2;
				err = mfvVirtualMachinePush32(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_MODF32:
			{
				vm->ip += 1;

				mfmF32 val1, val2;
				err = mfvVirtualMachinePop32(vm, &val1);
				if (err != MF_ERROR_OKAY)
					return err;
				err = mfvVirtualMachinePop32(vm, &val2);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmF32 val = fmodf(val1, val2);
				err = mfvVirtualMachinePush32(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_FLOORF32:
			{
				vm->ip += 1;

				mfmF32 val;
				err = mfvVirtualMachinePop32(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmF32 ret = floorf(val);
				err = mfvVirtualMachinePush32(vm, &ret);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_CEILF32:
			{
				vm->ip += 1;

				mfmF32 val;
				err = mfvVirtualMachinePop32(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmF32 ret = ceilf(val);
				err = mfvVirtualMachinePush32(vm, &ret);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_FRACTF32:
			{
				vm->ip += 1;

				mfmF32 val;
				err = mfvVirtualMachinePop32(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmF32 temp;
				mfmF32 ret = modff(val, &temp);
				err = mfvVirtualMachinePush32(vm, &ret);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}
		}

		// Flow control
		{
			case MFV_BYTECODE_END:
			{
				vm->ip = 0;
				if (state != NULL)
					*state = MFV_STATE_FINISHED;
				break;
			}

			case MFV_BYTECODE_YIELD:
			{
				vm->ip += 1;
				if (state != NULL)
					*state = MFV_STATE_UNFINISHED;
				break;
			}

			case MFV_BYTECODE_CALL:
			{
				mfvInstructionPointer ip = 0;
				err = mfvVirtualMachinePop32(vm, &ip);
				if (err != MF_ERROR_OKAY)
					return err;
				vm->ip += 1;
				if (vm->callStackHead >= vm->desc.callStackSize)
					return MFV_ERROR_CALL_STACK_OVERFLOW;
				vm->callStack[vm->callStackHead] = vm->ip;
				++vm->callStackHead;
				vm->ip = ip;
				break;
			}

			case MFV_BYTECODE_RETURN:
			{
				vm->ip += 1;
				if (vm->callStackHead == 0)
					return MFV_ERROR_CALL_STACK_UNDERFLOW;
				--vm->callStackHead;
				vm->ip = vm->callStack[vm->callStackHead];
				break;
			}

			case MFV_BYTECODE_JUMP:
			{
				mfvInstructionPointer ip = 0;
				err = mfvVirtualMachinePop32(vm, &ip);
				if (err != MF_ERROR_OKAY)
					return err;
				vm->ip = ip;
				break;
			}

			case MFV_BYTECODE_JUMP_I8_NOT_ZERO:
			{
				mfvInstructionPointer ip = 0;
				err = mfvVirtualMachinePop32(vm, &ip);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmU8 val = 0;
				err = mfvVirtualMachinePop8(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				if (val == 0)
					vm->ip += 1;
				else
					vm->ip = ip;
				break;
			}

			case MFV_BYTECODE_JUMP_I16_NOT_ZERO:
			{
				mfvInstructionPointer ip = 0;
				err = mfvVirtualMachinePop32(vm, &ip);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmU16 val = 0;
				err = mfvVirtualMachinePop16(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				if (val == 0)
					vm->ip += 1;
				else
					vm->ip = ip;
				break;
			}

			case MFV_BYTECODE_JUMP_I32_NOT_ZERO:
			{
				mfvInstructionPointer ip = 0;
				err = mfvVirtualMachinePop32(vm, &ip);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmU32 val = 0;
				err = mfvVirtualMachinePop32(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				if (val == 0)
					vm->ip += 1;
				else
					vm->ip = ip;
				break;
			}

			case MFV_BYTECODE_JUMP_F32_NOT_ZERO:
			{
				mfvInstructionPointer ip = 0;
				err = mfvVirtualMachinePop32(vm, &ip);
				if (err != MF_ERROR_OKAY)
					return err;
				mfmF32 val = 0;
				err = mfvVirtualMachinePop32(vm, &val);
				if (err != MF_ERROR_OKAY)
					return err;
				if (val == 0.0f)
					vm->ip += 1;
				else
					vm->ip = ip;
				break;
			}

			case MFV_BYTECODE_CALL_BUILTIN:
			{
				vm->ip += 1;

				mfmU16 id;
				err = mfvVirtualMachinePop16(vm, &id);
				if (err != MF_ERROR_OKAY)
					return err;
				if (id >= vm->desc.functionTableSize || vm->functionTable[id] == NULL)
					return MFV_ERROR_FUNCTION_NOT_DEFINED;
				err = vm->functionTable[id](vm);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}
		}

		// Register operations
		{
			case MFV_BYTECODE_STORE8:
			{
				mfmU32 id;
				mfmFromBigEndian4(vm->code + vm->ip + 1, &id);
				vm->ip += 5;
				mfmU8 value;
				err = mfvVirtualMachinePop8(vm, &value);
				if (err != MF_ERROR_OKAY)
					return err;
				if (id >= vm->desc.registerCount * 4)
					return MFV_ERROR_REGISTER_OUT_OF_BOUNDS;
				vm->registers8[id] = value;
				break;
			}

			case MFV_BYTECODE_STORE16:
			{
				mfmU32 id;
				mfmFromBigEndian4(vm->code + vm->ip + 1, &id);
				vm->ip += 5;
				mfmU16 value;
				err = mfvVirtualMachinePop16(vm, &value);
				if (err != MF_ERROR_OKAY)
					return err;
				if (id >= vm->desc.registerCount * 2)
					return MFV_ERROR_REGISTER_OUT_OF_BOUNDS;
				vm->registers16[id] = value;
				break;
			}

			case MFV_BYTECODE_STORE32:
			{
				mfmU32 id;
				mfmFromBigEndian4(vm->code + vm->ip + 1, &id);
				vm->ip += 5;
				mfmU8 value;
				err = mfvVirtualMachinePop32(vm, &value);
				if (err != MF_ERROR_OKAY)
					return err;
				if (id >= vm->desc.registerCount)
					return MFV_ERROR_REGISTER_OUT_OF_BOUNDS;
				vm->registers32[id] = value;
				break;
			}

			case MFV_BYTECODE_LOAD8:
			{
				mfmU32 id;
				mfmFromBigEndian4(vm->code + vm->ip + 1, &id);
				vm->ip += 5;
				if (id >= vm->desc.registerCount * 4)
					return MFV_ERROR_REGISTER_OUT_OF_BOUNDS;
				err = mfvVirtualMachinePush8(vm, &vm->registers8[id]);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_LOAD16:
			{
				mfmU32 id;
				mfmFromBigEndian4(vm->code + vm->ip + 1, &id);
				vm->ip += 5;
				if (id >= vm->desc.registerCount * 2)
					return MFV_ERROR_REGISTER_OUT_OF_BOUNDS;
				err = mfvVirtualMachinePush16(vm, &vm->registers16[id]);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}

			case MFV_BYTECODE_LOAD32:
			{
				mfmU32 id;
				mfmFromBigEndian4(vm->code + vm->ip + 1, &id);
				vm->ip += 5;
				if (id >= vm->desc.registerCount)
					return MFV_ERROR_REGISTER_OUT_OF_BOUNDS;
				err = mfvVirtualMachinePush32(vm, &vm->registers32[id]);
				if (err != MF_ERROR_OKAY)
					return err;
				break;
			}
		}

		default:
			return MFV_ERROR_INVALID_INSTRUCTION;
	}

	return err;
}

mfError mfvRunVirtualMachine(mfvVirtualMachine * vm, const mfmU64 * instructionCount, mfmU64 * executedInstructions, mfvVirtualMachineState* state)
{
#ifdef MAGMA_FRAMEWORK_DEBUG
	if (vm == NULL)
		return MFV_ERROR_INVALID_ARGUMENTS;
	if (vm->code == MFV_ERROR_NULL_CODE)
		return MFV_ERROR_NULL_CODE;
#endif

	mfError err = MF_ERROR_OKAY;

	if (instructionCount == NULL)
	{
		for (mfmU64 i = 0;; ++i)
		{
			mfvVirtualMachineState s;
			err = mfvStepVirtualMachine(vm, &s);
			if (state != NULL)
				*state = s;
			if (err != MF_ERROR_OKAY)
			{
				if (executedInstructions != NULL)
					*executedInstructions = i;
				return err;
			}

			if (s == MFV_STATE_FINISHED || s == MFV_STATE_YIELD)
			{
				if (executedInstructions != NULL)
					*executedInstructions = i + 1;
				return MF_ERROR_OKAY;
			}
		}
	}
	else
	{
		for (mfmU64 i = 0; i < *instructionCount; ++i)
		{
			mfvVirtualMachineState s;
			err = mfvStepVirtualMachine(vm, &s);
			if (state != NULL)
				*state = s;
			if (err != MF_ERROR_OKAY)
			{
				if (executedInstructions != NULL)
					*executedInstructions = i;
				return err;
			}

			if (s == MFV_STATE_FINISHED || s == MFV_STATE_YIELD)
			{
				if (executedInstructions != NULL)
					*executedInstructions = i + 1;
				return MF_ERROR_OKAY;
			}
		}

		if (executedInstructions != NULL)
			*executedInstructions = *instructionCount;
		if (state != NULL)
			*state = MFV_STATE_UNFINISHED;
	}

	return err;
}

mfError mfvSetVirtualMachineFunction(mfvVirtualMachine * vm, mfmU16 id, mfvVirtualMachineFunction func)
{
	if (vm == NULL)
		return MFV_ERROR_INVALID_ARGUMENTS;
	if (vm->desc.functionTableSize <= id)
		return MFV_ERROR_FUNCTION_ALREADY_DEFINED;
	if (vm->functionTable[id] != NULL)
		return MFV_ERROR_FUNCTION_ALREADY_DEFINED;
	vm->functionTable[id] = func;
	return MF_ERROR_OKAY;
}

mfError mfvRemoveVirtualMachineFunction(mfvVirtualMachine * vm, mfmU16 id)
{
	if (vm == NULL)
		return MFV_ERROR_INVALID_ARGUMENTS;
	if (vm->desc.functionTableSize <= id)
		return MFV_ERROR_FUNCTION_NOT_DEFINED;
	if (vm->functionTable[id] == NULL)
		return MFV_ERROR_FUNCTION_NOT_DEFINED;
	vm->functionTable[id] = NULL;
	return MF_ERROR_OKAY;
}

mfError mfvVirtualMachinePush8(mfvVirtualMachine * vm, const void * value)
{
	if (vm->stackHead + 1 >= vm->desc.stackSize)
		return MFV_ERROR_STACK_OVERFLOW;
	memcpy(vm->stack + vm->stackHead, value, 1);
	vm->stackHead += 1;
	return MF_ERROR_OKAY;
}

mfError mfvVirtualMachinePush16(mfvVirtualMachine * vm, const void * value)
{
	if (vm->stackHead + 2 >= vm->desc.stackSize)
		return MFV_ERROR_STACK_OVERFLOW;
	memcpy(vm->stack + vm->stackHead, value, 2);
	vm->stackHead += 2;
	return MF_ERROR_OKAY;
}

mfError mfvVirtualMachinePush32(mfvVirtualMachine * vm, const void * value)
{
	if (vm->stackHead + 4 >= vm->desc.stackSize)
		return MFV_ERROR_STACK_OVERFLOW;
	memcpy(vm->stack + vm->stackHead, value, 4);
	vm->stackHead += 4;
	return MF_ERROR_OKAY;
}

mfError mfvVirtualMachinePop8(mfvVirtualMachine * vm, void * value)
{
	if (vm->stackHead < 1)
		return MFV_ERROR_STACK_UNDERFLOW;
	memcpy(value, vm->stack + vm->stackHead - 1, 1);
	vm->stackHead -= 1;
	return MF_ERROR_OKAY;
}

mfError mfvVirtualMachinePop16(mfvVirtualMachine * vm, void * value)
{
	if (vm->stackHead < 2)
		return MFV_ERROR_STACK_UNDERFLOW;
	memcpy(value, vm->stack + vm->stackHead - 2, 2);
	vm->stackHead -= 2;
	return MF_ERROR_OKAY;
}

mfError mfvVirtualMachinePop32(mfvVirtualMachine * vm, void * value)
{
	if (vm->stackHead < 4)
		return MFV_ERROR_STACK_UNDERFLOW;
	memcpy(value, vm->stack + vm->stackHead - 4, 4);
	vm->stackHead -= 4;
	return MF_ERROR_OKAY;
}
