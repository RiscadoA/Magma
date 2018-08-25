#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

	/*
		MVM Bytecode version 1.0.
		
		Notes:
			- MVM Bytecode is stack based (it has no registers).
			- All literals are stored in big endianness.
			- Each virtual machine may have different memory and call stack sizes.

	*/
	
	// Stack operations
#define MFV_BYTECODE_POP				0x00	// Pops N bytes from stack (N = param 1 (1 byte))
#define MFV_BYTECODE_PUSH_COPY			0x01	// Makes a copy of the previous N bytes in the stack and pushes them again (N = param 1 (1 byte))
#define MFV_BYTECODE_PUSH8				0x02	// Pushs a 1 byte value into the stage (push value = param 1 (1 byte))
#define MFV_BYTECODE_PUSH16				0x03	// Pushs a 2 byte value into the stage (push value = param 1 (2 byte))
#define MFV_BYTECODE_PUSH32				0x04	// Pushs a 4 byte value into the stage (push value = param 1 (4 byte))

	// 1 byte signed integer operations
#define MFV_BYTECODE_ADDS8				0x10	// Pushes the top byte added to the next byte (1 byte signed integers)
#define MFV_BYTECODE_SUBS8				0x11	// Pushes the top byte subtracted to the next byte (1 byte signed integers)
#define MFV_BYTECODE_MULS8				0x12	// Pushes the top byte multiplied by the next byte (1 byte signed integers)
#define MFV_BYTECODE_DIVS8				0x13	// Pushes the top byte divided by the next byte (1 byte signed integers)
#define MFV_BYTECODE_MODS8				0x14	// Pushes the modulus of the top byte and the next byte (1 byte signed integers)

	// 2 byte signed integer operations
#define MFV_BYTECODE_ADDS16				0x20	// Pushes the top 2 bytes added to the next 2 bytes (2 byte signed integers)
#define MFV_BYTECODE_SUBS16				0x21	// Pushes the top 2 bytes subtracted to the next 2 bytes (2 byte signed integers)
#define MFV_BYTECODE_MULS16				0x22	// Pushes the top 2 bytes multiplied by the next 2 bytes (2 byte signed integers)
#define MFV_BYTECODE_DIVS16				0x23	// Pushes the top 2 bytes divided by the next 2 bytes (2 byte signed integers)
#define MFV_BYTECODE_MODS16				0x24	// Pushes the modulus of the top 2 bytes and the next 2 bytes (2 byte signed integers)

	// 4 byte signed integer operations
#define MFV_BYTECODE_ADDS32				0x30	// Pushes the top 4 bytes added to the next 4 bytes (4 byte signed integers)
#define MFV_BYTECODE_SUBS32				0x31	// Pushes the top 4 bytes subtracted to the next 4 bytes (4 byte signed integers)
#define MFV_BYTECODE_MULS32				0x32	// Pushes the top 4 bytes multiplied by the next 4 bytes (4 byte signed integers)
#define MFV_BYTECODE_DIVS32				0x33	// Pushes the top 4 bytes divided by the next 4 bytes (4 byte signed integers)
#define MFV_BYTECODE_MODS32				0x34	// Pushes the modulus of the top 4 bytes and the next 4 bytes (4 byte signed integers)

	// 1 byte unsigned integer operations
#define MFV_BYTECODE_ADDU8				0x40	// Pushes the top byte added to the next byte (1 byte unsigned integers)
#define MFV_BYTECODE_SUBU8				0x41	// Pushes the top byte subtracted to the next byte (1 byte unsigned integers)
#define MFV_BYTECODE_MULU8				0x42	// Pushes the top byte multiplied by the next byte (1 byte unsigned integers)
#define MFV_BYTECODE_DIVU8				0x43	// Pushes the top byte divided by the next byte (1 byte unsigned integers)
#define MFV_BYTECODE_MODU8				0x44	// Pushes the modulus of the top byte and the next byte (1 byte unsigned integers)

	// 2 byte unsigned integer operations
#define MFV_BYTECODE_ADDU16				0x50	// Pushes the top 2 bytes added to the next 2 bytes (2 byte unsigned integers)
#define MFV_BYTECODE_SUBU16				0x51	// Pushes the top 2 bytes subtracted to the next 2 bytes (2 byte unsigned integers)
#define MFV_BYTECODE_MULU16				0x52	// Pushes the top 2 bytes multiplied by the next 2 bytes (2 byte unsigned integers)
#define MFV_BYTECODE_DIVU16				0x53	// Pushes the top 2 bytes divided by the next 2 bytes (2 byte unsigned integers)
#define MFV_BYTECODE_MODU16				0x54	// Pushes the modulus of the top 2 bytes and the next 2 bytes (2 byte unsigned integers)

	// 4 byte unsigned integer operations
#define MFV_BYTECODE_ADDU32				0x60	// Pushes the top 4 bytes added to the next 4 bytes (4 byte unsigned integers)
#define MFV_BYTECODE_SUBU32				0x61	// Pushes the top 4 bytes subtracted to the next 4 bytes (4 byte unsigned integers)
#define MFV_BYTECODE_MULU32				0x62	// Pushes the top 4 bytes multiplied by the next 4 bytes (4 byte unsigned integers)
#define MFV_BYTECODE_DIVU32				0x63	// Pushes the top 4 bytes divided by the next 4 bytes (4 byte unsigned integers)
#define MFV_BYTECODE_MODU32				0x64	// Pushes the modulus of the top 4 bytes and the next 4 bytes (4 byte unsigned integers)

	// 4 byte floating point operations
#define MFV_BYTECODE_ADDF32				0x70	// Pushes the top 4 bytes added to the next 4 bytes (4 byte floating point)
#define MFV_BYTECODE_SUBF32				0x71	// Pushes the top 4 bytes subtracted to the next 4 bytes (4 byte floating point)
#define MFV_BYTECODE_MULF32				0x72	// Pushes the top 4 bytes multiplied by the next 4 bytes (4 byte floating point)
#define MFV_BYTECODE_DIVF32				0x73	// Pushes the top 4 bytes divided by the next 4 bytes (4 byte floating point)
#define MFV_BYTECODE_MODF32				0x74	// Pushes the modulus of the top 4 bytes and the next 4 bytes (4 byte floating point)
#define MFV_BYTECODE_FLOORF32			0x75	// Pushes the top 4 bytes floored (4 byte floating point) 
#define MFV_BYTECODE_CEILF32			0x76	// Pushes the top 4 bytes ceiled (4 byte floating point) 
#define MFV_BYTECODE_FRACTF32			0x77	// Pushes the fractional part of the top 4 bytes (4 byte floating point) 

	// Flow control instructions
#define MFV_BYTECODE_END				0x90	// Stops the program and resets the instruction pointer.
#define MFV_BYTECODE_YIELD				0x91	// Yields the program execution (tells the VM to pause the program).
#define MFV_BYTECODE_CALL				0x92	// Calls a function (address on the top 4 bytes on the stack).
#define MFV_BYTECODE_RETURN				0x93	// Returns from a function.
#define MFV_BYTECODE_JUMP				0x94	// Jumps to an address (address on the top 4 bytes on the stack).
#define MFV_BYTECODE_JUMP_I8_NOT_ZERO	0x95	// Jumps to an address (address on the top 4 bytes on the stack) if the next byte isn't zero.
#define MFV_BYTECODE_JUMP_I16_NOT_ZERO	0x96	// Jumps to an address (address on the top 4 bytes on the stack) if the next 2 bytes aren't zero.
#define MFV_BYTECODE_JUMP_I32_NOT_ZERO	0x97	// Jumps to an address (address on the top 4 bytes on the stack) if the next 4 bytes aren't zero.
#define MFV_BYTECODE_JUMP_F32_NOT_ZERO	0x98	// Jumps to an address (address on the top 4 bytes on the stack) if the next 4 byte floating point isn't zero.
#define MFV_BYTECODE_CALL_BUILTIN		0x99	// Calls a built-in function (function ID on the top 2 bytes on the stack). 

#ifdef __cplusplus
}
#endif
