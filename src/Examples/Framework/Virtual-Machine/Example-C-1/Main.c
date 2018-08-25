#include <Magma/Framework/Entry.h>
#include <Magma/Framework/VM/VirtualMachine.h>
#include <Magma/Framework/String/Stream.h>

#include <stdlib.h>

mfError PrintU32(mfvVirtualMachine* vm)
{
	mfError err = MF_ERROR_OKAY;
	mfmU32 val;

	err = mfvVirtualMachinePop32(vm, &val);
	if (err != MF_ERROR_OKAY)
		return err;

	err = mfsPrintFormatUTF8(mfsOutStream, u8"(PrintU32): %d\n", val);
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
		/* 0x00 */	MFV_BYTECODE_PUSH32, 0x00, 0x00, 0x00, 0x16,
		/* 0x05 */	MFV_BYTECODE_JUMP,
		/* 0x06 */	MFV_BYTECODE_PUSH32, 0x00, 0x00, 0xFF, 0xFF,
		/* 0x0B */	MFV_BYTECODE_PUSH32, 0x00, 0x00, 0xFF, 0xFF,
		/* 0x10 */	MFV_BYTECODE_ADDU32,
		/* 0x11 */	MFV_BYTECODE_PUSH16, 0x00, 0x01,
		/* 0x14 */	MFV_BYTECODE_CALL_BUILTIN,
		/* 0x15	*/	MFV_BYTECODE_RETURN,
		/* 0x16 */
		MFV_BYTECODE_PUSH32, 0x00, 0x00, 0x00, 0x06,
		MFV_BYTECODE_CALL,
		MFV_BYTECODE_PUSH32, 0x00, 0x00, 0x00, 0x16,
		MFV_BYTECODE_JUMP,
		MFV_BYTECODE_END,
	};
	
	if (mfvSetVirtualMachineFunction(vm, 0x0001, &PrintU32) != MF_ERROR_OKAY)
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