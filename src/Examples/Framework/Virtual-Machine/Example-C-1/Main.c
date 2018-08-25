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
		desc.stackSize = 256;
		if (mfvCreateVirtualMachine(&vm, &desc, NULL) != MF_ERROR_OKAY)
			abort();
	}

	mfmU8 code[] =
	{
		MFV_BYTECODE_PUSH8, 55U,
		MFV_BYTECODE_PUSH8, 200U,
		MFV_BYTECODE_SUBU8,
		MFV_BYTECODE_PUSH16, 0x00, 0x01,
		MFV_BYTECODE_CALL_BUILTIN,
		MFV_BYTECODE_END,
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