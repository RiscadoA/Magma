#pragma once

#include "../Memory/Type.h"

/*
	MSL Bytecode version 2.0.
	Changes:
		- Now in C;
		- Changed the way vector and matrix components are handled;
		- Added array support;
		- Added some instructions;
*/

#ifdef __cplusplus
extern "C"
{
#endif

	// MSL Bytecode version 2.0

	// Boolean.
#define MFG_BYTECODE_DECLB1				0x00	// Declares a boolean variable { variable index stored on param 1x2 }.

	// Integer components.
#define MFG_BYTECODE_DECLI1				0x01	// Declares an integer scalar variable { variable index stored on param 1x2 }.
#define MFG_BYTECODE_DECLI2				0x02	// Declares an integer 2 component vector variable { variable index stored on param 1x2 }.
#define MFG_BYTECODE_DECLI3				0x03	// Declares an integer 3 component vector variable { variable index stored on param 1x2 }.
#define MFG_BYTECODE_DECLI4				0x04	// Declares an integer 4 component vector variable { variable index stored on param 1x2 }.
#define MFG_BYTECODE_DECLI22			0x05	// Declares an integer 2x2 matrix variable { variable index stored on param 1x2 }.
#define MFG_BYTECODE_DECLI33			0x06	// Declares an integer 3x3 matrix variable { variable index stored on param 1x2 }.
#define MFG_BYTECODE_DECLI44			0x07	// Declares an integer 4x4 matrix variable { variable index stored on param 1x2 }.

	// Integer component arrays.
#define MFG_BYTECODE_DECLI1A			0x10	// Declares an integer scalar variable array { array starting index stored on param 1x2, element count on param 2x2 }.
#define MFG_BYTECODE_DECLI2A			0x11	// Declares an integer 2 component vector variable array { array starting index stored on param 1x2, element count on param 2x2 }.
#define MFG_BYTECODE_DECLI3A			0x12	// Declares an integer 3 component vector variable array { array starting index stored on param 1x2, element count on param 2x2 }.
#define MFG_BYTECODE_DECLI4A			0x13	// Declares an integer 4 component vector variable array { array starting index stored on param 1x2, element count on param 2x2 }.
#define MFG_BYTECODE_DECLI22A			0x14	// Declares an integer 2x2 matrix variable array { array starting index stored on param 1x2, element count on param 2x2 }.
#define MFG_BYTECODE_DECLI33A			0x15	// Declares an integer 3x3 matrix variable array { array starting index stored on param 1x2, element count on param 2x2 }.
#define MFG_BYTECODE_DECLI44A			0x16	// Declares an integer 4x4 matrix variable array { array starting index stored on param 1x2, element count on param 2x2 }.

	// Floating point components.
#define MFG_BYTECODE_DECLF1				0x20	// Declares an floating point scalar variable { variable index stored on param 1x2 }.
#define MFG_BYTECODE_DECLF2				0x21	// Declares an floating point 2 component vector variable { variable index stored on param 1x2 }.
#define MFG_BYTECODE_DECLF3				0x22	// Declares an floating point 3 component vector variable { variable index stored on param 1x2 }.
#define MFG_BYTECODE_DECLF4				0x23	// Declares an floating point 4 component vector variable { variable index stored on param 1x2 }.
#define MFG_BYTECODE_DECLF22			0x24	// Declares an floating point 2x2 matrix variable { variable index stored on param 1x2 }.
#define MFG_BYTECODE_DECLF33			0x25	// Declares an floating point 3x3 matrix variable { variable index stored on param 1x2 }.
#define MFG_BYTECODE_DECLF44			0x26	// Declares an floating point 4x4 matrix variable { variable index stored on param 1x2 }.

	// Floating point component arrays.
#define MFG_BYTECODE_DECLF1A			0x30	// Declares an floating point scalar variable array { array starting index stored on param 1x2, element count on param 2x2 }.
#define MFG_BYTECODE_DECLF2A			0x31	// Declares an floating point 2 component vector variable array { array starting index stored on param 1x2, element count on param 2x2 }.
#define MFG_BYTECODE_DECLF3A			0x32	// Declares an floating point 3 component vector variable array { array starting index stored on param 1x2, element count on param 2x2 }.
#define MFG_BYTECODE_DECLF4A			0x33	// Declares an floating point 4 component vector variable array { array starting index stored on param 1x2, element count on param 2x2 }.
#define MFG_BYTECODE_DECLF22A			0x34	// Declares an floating point 2x2 matrix variable array { array starting index stored on param 1x2, element count on param 2x2 }.
#define MFG_BYTECODE_DECLF33A			0x35	// Declares an floating point 3x3 matrix variable array { array starting index stored on param 1x2, element count on param 2x2 }.
#define MFG_BYTECODE_DECLF44A			0x36	// Declares an floating point 4x4 matrix variable array { array starting index stored on param 1x2, element count on param 2x2 }.

	// Variable operations.
#define MFG_BYTECODE_ASSIGN				0x40	// Performs an assign operation between two variables { variable 1 index on param 1x2, variable index 2 on param 2x2 }.
#define MFG_BYTECODE_ADD				0x41	// Performs an addition operation between two variables { variable 1 index on param 1x2, variable index 2 on param 2x2 }.
#define MFG_BYTECODE_SUBTRACT			0x42	// Performs a subtraction operation between two variables { variable 1 index on param 1x2, variable index 2 on param 2x2 }.
#define MFG_BYTECODE_MULTIPLY			0x43	// Performs a multiplication operation between two variables { variable 1 index on param 1x2, variable index 2 on param 2x2 }.
#define MFG_BYTECODE_DIVIDE				0x44	// Performs a division operation between two variables { variable 1 index on param 1x2, variable index 2 on param 2x2 }.
#define MFG_BYTECODE_AND				0x45	// Performs a logical and operation between two variables { variable 1 index on param 1x2, variable index 2 on param 2x2 }.
#define MFG_BYTECODE_OR					0x46	// Performs a logical or operation between two variables { variable 1 index on param 1x2, variable index 2 on param 2x2 }.
#define MFG_BYTECODE_NOT				0x47	// Performs a logical not operation between on a variables { variable index on param 1x2 }.
#define MFG_BYTECODE_NEGATE				0x48	// Negates a variables { variable index on param 1x2 }.
#define MFG_BYTECODE_GREATER			0x49	// Performs a logical 'is greater than' operation between two variables { variable 1 index on param 1x2, variable index 2 on param 2x2 }.
#define MFG_BYTECODE_LESS				0x4A	// Performs a logical 'is less than' operation between two variables { variable 1 index on param 1x2, variable index 2 on param 2x2 }.
#define MFG_BYTECODE_GEQUAL				0x4B	// Performs a logical 'is greater than or equal to' operation between two variables { variable 1 index on param 1x2, variable index 2 on param 2x2 }.
#define MFG_BYTECODE_LEQUAL				0x4C	// Performs a logical 'is less than or equal to' operation between two variables { variable 1 index on param 1x2, variable index 2 on param 2x2 }.
#define MFG_BYTECODE_EQUAL				0x4D	// Performs a logical 'is equal to' operation between two variables { variable 1 index on param 1x2, variable index 2 on param 2x2 }.
#define MFG_BYTECODE_DIFFERENT			0x4E	// Performs a logical 'is not equal to' operation between two variables { variable 1 index on param 1x2, variable index 2 on param 2x2 }.

	// Boolean literal setters.
#define MFG_BYTECODE_LITB1TRUE			0x50	// Sets a variable to a boolean literal with the value 'true' { variable index on param 1x2 }.
#define MFG_BYTECODE_LITB1FALSE			0x51	// Sets a variable to a boolean literal with the value 'false' { variable index on param 1x2 }.

	// Integer literal setters.
#define MFG_BYTECODE_LITI1				0x52	// Sets a variable to a integer scalar literal { variable index on param 1x2, literal on param 2x4 }.
#define MFG_BYTECODE_LITI2				0x53	// Sets a variable to a 2 component integer vector literal { variable index on param 1x2, literal on param 2x8 }.
#define MFG_BYTECODE_LITI3				0x54	// Sets a variable to a 3 component integer vector literal { variable index on param 1x2, literal on param 2x12 }.
#define MFG_BYTECODE_LITI4				0x55	// Sets a variable to a 4 component integer vector literal { variable index on param 1x2, literal on param 2x16 }.
#define MFG_BYTECODE_LITI22				0x56	// Sets a variable to a integer 2x2 matrix literal { variable index on param 1x2, literal on param 2x16 }.
#define MFG_BYTECODE_LITI33				0x57	// Sets a variable to a integer 3x3 matrix literal { variable index on param 1x2, literal on param 2x36 }.
#define MFG_BYTECODE_LITI44				0x58	// Sets a variable to a integer 4x4 matrix literal { variable index on param 1x2, literal on param 2x64 }.

	// Floating point literal setters.
#define MFG_BYTECODE_LITF1				0x60	// Sets a variable to a floating point scalar literal { variable index on param 1x2, literal on param 2x4 }.
#define MFG_BYTECODE_LITF2				0x61	// Sets a variable to a 2 component floating point vector literal { variable index on param 1x2, literal on param 2x8 }.
#define MFG_BYTECODE_LITF3				0x62	// Sets a variable to a 3 component floating point vector literal { variable index on param 1x2, literal on param 2x12 }.
#define MFG_BYTECODE_LITF4				0x63	// Sets a variable to a 4 component floating point vector literal { variable index on param 1x2, literal on param 2x16 }.
#define MFG_BYTECODE_LITF22				0x64	// Sets a variable to a floating point 2x2 matrix literal { variable index on param 1x2, literal on param 2x16 }.
#define MFG_BYTECODE_LITF33				0x65	// Sets a variable to a floating point 3x3 matrix literal { variable index on param 1x2, literal on param 2x36 }.
#define MFG_BYTECODE_LITF44				0x66	// Sets a variable to a floating point 4x4 matrix literal { variable index on param 1x2, literal on param 2x64 }.

	// Vector/matrix component getters.
	// These getters turn a certain variable index into a reference to a vector or matrix component.
#define MFG_BYTECODE_GET2CMP			0x70	// Sets variable index { param 1x2 } to a reference to a component of a 2 component vector  { vector index on param 2x2, component index on param 3x1 }.
#define MFG_BYTECODE_GET3CMP			0x71	// Sets variable index { param 1x2 } to a reference to a component of a 3 component vector  { vector index on param 2x2, component index on param 3x1 }.
#define MFG_BYTECODE_GET4CMP			0x72	// Sets variable index { param 1x2 } to a reference to a component of a 4 component vector  { vector index on param 2x2, component index on param 3x1 }.
#define MFG_BYTECODE_GET22CMP			0x73	// Sets variable index { param 1x2 } to a reference to a component of a 2x2 matrix  { matrix index on param 2x2, component index on param 3x1 (the top left component has index 0, the bottom right component has the last index) }.
#define MFG_BYTECODE_GET33CMP			0x74	// Sets variable index { param 1x2 } to a reference to a component of a 3x3 matrix  { matrix index on param 2x2, component index on param 3x1 (the top left component has index 0, the bottom right component has the last index) }.
#define MFG_BYTECODE_GET44CMP			0x75	// Sets variable index { param 1x2 } to a reference to a component of a 4x4 matrix  { matrix index on param 2x2, component index on param 3x1 (the top left component has index 0, the bottom right component has the last index) }.

	// Scope and flow control
#define MFG_BYTECODE_OPSCOPE			0x80	// Open a new scope (new variable declarations with previously used indexes overrides previous variables until the scope is closed).
#define MFG_BYTECODE_CLSCOPE			0x81	// Closes a previously opened scope.
#define MFG_BYTECODE_DISCARD			0x82	// Exits immediately from the current shader (must be a pixel shader) and discards the pixel.
#define MFG_BYTECODE_RETURN				0x83	// Exits immediately from the current shader.
#define MFG_BYTECODE_WHILE				0x84	// Repeats the next instruction/scope as long as the variable with the index on { param 1x2 } evaluates to true.
#define MFG_BYTECODE_IF					0x85	// Executes the next instruction/scope as long as the variable with the index on { param 1x2 } evaluates to true.
#define MFG_BYTECODE_ELSE				0x86	// Executes the next instruction/scope if the previous 'if' failed.

	// Functions 1
#define MFG_BYTECODE_MULMAT				0x90	// Performs matrix on multiplication between the variable on the index on { param 1x2 } and the variable on the index on { param 2x2 }.
#define MFG_BYTECODE_SAMPLE1D			0x91	// Samples a pixel from the 1D texture on the index on { param 1x2 } from the coordinates on the index on { param 2x2 }.
#define MFG_BYTECODE_SAMPLE2D			0x92	// Samples a pixel from the 2D texture on the index on { param 1x2 } from the coordinates on the index on { param 2x2 }.
#define MFG_BYTECODE_SAMPLE3D			0x93	// Samples a pixel from the 3D texture on the index on { param 1x2 } from the coordinates on the index on { param 2x2 }.
#define MFG_BYTECODE_COS				0x94	// Sets the variable on the index on { param 1x2 } to the cosine of the variable on index { param 2x2 }.
#define MFG_BYTECODE_SIN				0x95	// Sets the variable on the index on { param 1x2 } to the sine of the variable on index { param 2x2 }.
#define MFG_BYTECODE_TAN				0x96	// Sets the variable on the index on { param 1x2 } to the tangent of the variable on index { param 2x2 }.
#define MFG_BYTECODE_ACOS				0x97	// Sets the variable on the index on { param 1x2 } to the arc cosine of the variable on index { param 2x2 }.
#define MFG_BYTECODE_ASIN				0x98	// Sets the variable on the index on { param 1x2 } to the arc sine of the variable on index { param 2x2 }.	
#define MFG_BYTECODE_ATAN				0x99	// Sets the variable on the index on { param 1x2 } to the arc tangent of the variable on index { param 2x2 }.
#define MFG_BYTECODE_DEGREES			0x9A	// Sets the variable on the index on { param 1x2 } to the value of the variable on index { param 2x2 } in degrees (from radians to degrees).
#define MFG_BYTECODE_RADIANS			0x9B	// Sets the variable on the index on { param 1x2 } to the value of the variable on index { param 2x2 } in degrees (from degrees to radians).
#define MFG_BYTECODE_EXP				0x9C	// Sets the variable on the index on { param 1x2 } to the constant 'e' raised to the variable on index { param 2x2 }.	
#define MFG_BYTECODE_LOG				0x9D	// Sets the variable on the index on { param 1x2 } to the power which the constant 'e' has to be raised to to produce the variable on index { param 2x2 }.
#define MFG_BYTECODE_EXP2				0x9E	// Sets the variable on the index on { param 1x2 } to 2 raised to the variable on index { param 2x2 }.	
#define MFG_BYTECODE_LOG2				0x9F	// Sets the variable on the index on { param 1x2 } to the power which 2 has to be raised to to produce the variable on index { param 2x2 }.

	// Functions 2
#define MFG_BYTECODE_POW				0xA0	// Sets the variable on the index on { param 1x2 } to the power of the variable on index { param 2x2 } to the variable on index { param 3x2 }.
#define MFG_BYTECODE_SQRT				0xA1	// Sets the variable on the index on { param 1x2 } to the square root of the variable on index { param 2x2 }.
#define MFG_BYTECODE_ISQRT				0xA2	// Sets the variable on the index on { param 1x2 } to the inverse square root of the variable on index { param 2x2 }.
#define MFG_BYTECODE_ABS				0xA3	// Sets the variable on the index on { param 1x2 } to the absolute value of the variable on index { param 2x2 }.
#define MFG_BYTECODE_SIGN				0xA4	// Sets the variable on the index on { param 1x2 } to the sign value of the variable on index { param 2x2 } (-1.0 when negative, 0.0 when zero and 1.0 when positive).
#define MFG_BYTECODE_FLOOR				0xA5	// Sets the variable on the index on { param 1x2 } to the floored value of the variable on index { param 2x2 }.
#define MFG_BYTECODE_CEIL				0xA6	// Sets the variable on the index on { param 1x2 } to the ceiled value of the variable on index { param 2x2 }.
#define MFG_BYTECODE_FRACT				0xA7	// Sets the variable on the index on { param 1x2 } to the fractional part of the variable on index { param 2x2 }.
#define MFG_BYTECODE_LERP				0xA8	// Sets the variable on the index on { param 1x2 } to the interpolation of the variable on index { param 2x2 } and the variable on index { param 3x2 } by the variable on index { param 4x2 }.
#define MFG_BYTECODE_CLAMP				0xA9	// Sets the variable on the index on { param 1x2 } to the clamped value of the variable on index { param 4x2 } between the minimum on the variable on index { param 2x2 } and the maximum on the variable on index { param 3x2 }.
#define MFG_BYTECODE_DOT				0xAA	// Sets the variable on the index on { param 1x2 } to the dot of the vector on index { param 2x2 } and the vector on index { param 3x2 }.
#define MFG_BYTECODE_CROSS				0xAB
#define MFG_BYTECODE_NORMALIZE			0xAC
#define MFG_BYTECODE_ROUND				0xAD
#define MFG_BYTECODE_TRANSPOSE			0xAE
#define MFG_BYTECODE_REFLECT			0xAF

	// Functions 3
#define MFG_BYTECODE_MIN				0xB0
#define MFG_BYTECODE_MAX				0xB1


#define MFG_BYTECODE_VERSION			0xFF	// Specifies the bytecode version 

#ifdef __cplusplus
}
#endif
