#include <Magma/Framework/Entry.h>
#include <Magma/Framework/VM/VirtualMachine.h>
#include <Magma/Framework/String/Stream.h>

#include <stdlib.h>

mfError PrintU8(mfvVirtualMachine* vm)
{
	mfError err = MF_ERROR_OKAY;
	mfmU8 val;

	err = mfvVirtualMachinePop8(vm, &val);
	if (err != MF_ERROR_OKAY)
		return err;

	err = mfsPrintFormatUTF8(mfsOutStream, u8"(PrintU8): %d\n", val);
	return err;
}

int main(int argc, const char** argv)
{
	if (mfInit(argc, argv) != MF_ERROR_OKAY)
		abort();

	mfvVirtualMachine* vm;
	{
		mfvVirtualMachineDesc desc;
		desc.callStackSize = 16;
		desc.functionTableSize = 16;
		desc.registerCount = 16;
		desc.stackSize = 256;
		if (mfvCreateVirtualMachine(&vm, &desc, NULL) != MF_ERROR_OKAY)
			abort();
	}

	mfmU8 code[] =
	{
		/* 0d00 */ MFV_BYTECODE_PUSH8, 0x0A,
		/* 0d02 */ MFV_BYTECODE_STORE8, 0x00, 0x00, 0x00, 0x00,
		/* 0d07 */ MFV_BYTECODE_PUSH8, 0x01,
		/* 0d09 */ MFV_BYTECODE_LOAD8, 0x00, 0x00, 0x00, 0x00,
		/* 0d14 */ MFV_BYTECODE_SUBU8,
		/* 0d15 */ MFV_BYTECODE_PUSH_COPY, 0x01,
		/* 0d17 */ MFV_BYTECODE_STORE8, 0x00, 0x00, 0x00, 0x00,
		/* 0d22 */ MFV_BYTECODE_PUSH_COPY, 0x01,
		/* 0d24 */ MFV_BYTECODE_PUSH16, 0x00, 0x01,
		/* 0d27 */ MFV_BYTECODE_CALL_BUILTIN,
		/* 0d28 */ MFV_BYTECODE_PUSH32, 0, 0, 0, 7,
		/* 0d33 */ MFV_BYTECODE_JUMP_I8_NOT_ZERO,
		/* 0d34 */ MFV_BYTECODE_END,
	};
	
	if (mfvSetVirtualMachineFunction(vm, 0x0001, &PrintU8) != MF_ERROR_OKAY)
		abort();
	if (mfvSetVirtualMachineCode(vm, 0, code) != MF_ERROR_OKAY)
		abort();
	if (mfvRunVirtualMachine(vm, NULL, NULL, NULL) != MF_ERROR_OKAY)
		abort();

	mfvDestroyVirtualMachine(vm);

	mfTerminate();

	for (;;);
	return 0;
}