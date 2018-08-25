#include "VirtualMachine.h"
#include "Config.h"
#include "../Memory/Allocator.h"
#include "../Memory/Endianness.h"

#include <string.h>
#include <stdlib.h>

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
};

mfError mfvCreateVirtualMachine(mfvVirtualMachine ** vm, const mfvVirtualMachineDesc * desc, void * allocator)
{
	mfmU8* memory = NULL;
	mfError err = mfmAllocate(allocator, &memory,
							  sizeof(mfvVirtualMachine) +
							  desc->stackSize +
							  desc->callStackSize * sizeof(mfvInstructionPointer) +
							  desc->functionTableSize * sizeof(mfvVirtualMachineFunction));
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
		case MFV_BYTECODE_POP:
		{
			mfmU8 size = vm->code[vm->ip + 1];
			vm->ip += 2;
			if (vm->stackHead < size)
				return MFV_ERROR_STACK_UNDERFLOW;
			vm->stackHead -= size;
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
			err = mfvVirtualMachinePush16(vm, vm->code + vm->ip + 1);
			vm->ip += 3;
			if (err != MF_ERROR_OKAY)
				return err;
			break;
		}

		case MFV_BYTECODE_PUSH32:
		{
			err = mfvVirtualMachinePush32(vm, vm->code + vm->ip + 1);
			vm->ip += 5;
			if (err != MF_ERROR_OKAY)
				return err;
			break;
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

		case MFV_BYTECODE_END:
		{
			vm->ip = 0;
			if (state != NULL)
				*state = MFV_STATE_FINISHED;
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
	mfmFromBigEndian2(value, vm->stack + vm->stackHead);
	vm->stackHead += 2;
	return MF_ERROR_OKAY;
}

mfError mfvVirtualMachinePush32(mfvVirtualMachine * vm, const void * value)
{
	if (vm->stackHead + 4 >= vm->desc.stackSize)
		return MFV_ERROR_STACK_OVERFLOW;
	mfmFromBigEndian4(value, vm->stack + vm->stackHead);
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
