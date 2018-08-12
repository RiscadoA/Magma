#pragma once

#include "../Memory/Type.h"

#ifdef __cplusplus
extern "C"
{
#endif

	// Boolean
#define MFG_BYTECODE_DECLB1				0x00	// Declares a boolean variable { variable index stored on param 1x2 }

	// Integer components
#define MFG_BYTECODE_DECLI1				0x01	// Declares an integer scalar variable { variable index stored on param 1x2 }
#define MFG_BYTECODE_DECLI2				0x02	// Declares an integer 2 component vector variable { variable index stored on param 1x2 }
#define MFG_BYTECODE_DECLI3				0x03	// Declares an integer 3 component vector variable { variable index stored on param 1x2 }
#define MFG_BYTECODE_DECLI4				0x04	// Declares an integer 4 component vector variable { variable index stored on param 1x2 }
#define MFG_BYTECODE_DECLI22			0x05	// Declares an integer 2x2 matrix variable { variable index stored on param 1x2 }
#define MFG_BYTECODE_DECLI33			0x06	// Declares an integer 3x3 matrix variable { variable index stored on param 1x2 }
#define MFG_BYTECODE_DECLI44			0x07	// Declares an integer 4x4 matrix variable { variable index stored on param 1x2 }

	// Integer component arrays
#define MFG_BYTECODE_DECLI1A			0x10	// Declares an integer scalar variable array { array starting index stored on param 1x2, element count on param 2x2 }
#define MFG_BYTECODE_DECLI2A			0x11	// Declares an integer 2 component vector variable array { array starting index stored on param 1x2, element count on param 2x2 }
#define MFG_BYTECODE_DECLI3A			0x12	// Declares an integer 3 component vector variable array { array starting index stored on param 1x2, element count on param 2x2 }
#define MFG_BYTECODE_DECLI4A			0x13	// Declares an integer 4 component vector variable array { array starting index stored on param 1x2, element count on param 2x2 }
#define MFG_BYTECODE_DECLI22A			0x14	// Declares an integer 2x2 matrix variable array { array starting index stored on param 1x2, element count on param 2x2 }
#define MFG_BYTECODE_DECLI33A			0x15	// Declares an integer 3x3 matrix variable array { array starting index stored on param 1x2, element count on param 2x2 }
#define MFG_BYTECODE_DECLI44A			0x16	// Declares an integer 4x4 matrix variable array { array starting index stored on param 1x2, element count on param 2x2 }

	// Floating point components
#define MFG_BYTECODE_DECLF1				0x20	// Declares an floating point scalar variable { variable index stored on param 1x2 }
#define MFG_BYTECODE_DECLF2				0x21	// Declares an floating point 2 component vector variable { variable index stored on param 1x2 }
#define MFG_BYTECODE_DECLF3				0x22	// Declares an floating point 3 component vector variable { variable index stored on param 1x2 }
#define MFG_BYTECODE_DECLF4				0x23	// Declares an floating point 4 component vector variable { variable index stored on param 1x2 }
#define MFG_BYTECODE_DECLF22			0x24	// Declares an floating point 2x2 matrix variable { variable index stored on param 1x2 }
#define MFG_BYTECODE_DECLF33			0x25	// Declares an floating point 3x3 matrix variable { variable index stored on param 1x2 }
#define MFG_BYTECODE_DECLF44			0x26	// Declares an floating point 4x4 matrix variable { variable index stored on param 1x2 }

	// Floating point component arrays
#define MFG_BYTECODE_DECLF1A			0x30	// Declares an floating point scalar variable array { array starting index stored on param 1x2, element count on param 2x2 }
#define MFG_BYTECODE_DECLF2A			0x31	// Declares an floating point 2 component vector variable array { array starting index stored on param 1x2, element count on param 2x2 }
#define MFG_BYTECODE_DECLF3A			0x32	// Declares an floating point 3 component vector variable array { array starting index stored on param 1x2, element count on param 2x2 }
#define MFG_BYTECODE_DECLF4A			0x33	// Declares an floating point 4 component vector variable array { array starting index stored on param 1x2, element count on param 2x2 }
#define MFG_BYTECODE_DECLF22A			0x34	// Declares an floating point 2x2 matrix variable array { array starting index stored on param 1x2, element count on param 2x2 }
#define MFG_BYTECODE_DECLF33A			0x35	// Declares an floating point 3x3 matrix variable array { array starting index stored on param 1x2, element count on param 2x2 }
#define MFG_BYTECODE_DECLF44A			0x36	// Declares an floating point 4x4 matrix variable array { array starting index stored on param 1x2, element count on param 2x2 }

	// Variable operations
#define MFG_BYTECODE_ASSIGN				0x40	// Performs an assign operation between two variables { variable 1 index on param 1x2, variable index 2 on param 2x2 }
#define MFG_BYTECODE_ADD				0x41	// Performs an addition operation between two variables { variable 1 index on param 1x2, variable index 2 on param 2x2 }
#define MFG_BYTECODE_SUBTRACT			0x42	// Performs a subtraction operation between two variables { variable 1 index on param 1x2, variable index 2 on param 2x2 }
#define MFG_BYTECODE_MULTIPLY			0x43	// Performs a multiplication operation between two variables { variable 1 index on param 1x2, variable index 2 on param 2x2 }
#define MFG_BYTECODE_DIVIDE				0x44	// Performs a division operation between two variables { variable 1 index on param 1x2, variable index 2 on param 2x2 }
#define MFG_BYTECODE_AND				0x45	// Performs a logical and operation between two variables { variable 1 index on param 1x2, variable index 2 on param 2x2 }
#define MFG_BYTECODE_OR					0x46	// Performs a logical or operation between two variables { variable 1 index on param 1x2, variable index 2 on param 2x2 }
#define MFG_BYTECODE_NOT				0x47	// Performs a logical not operation between on a variables { variable index on param 1x2 }
#define MFG_BYTECODE_NEGATE				0x48	// Negates a variables { variable index on param 1x2 }
#define MFG_BYTECODE_GREATER			0x49	// Performs a logical 'is greater than' operation between two variables { variable 1 index on param 1x2, variable index 2 on param 2x2 }
#define MFG_BYTECODE_LESS				0x4A	// Performs a logical 'is less than' operation between two variables { variable 1 index on param 1x2, variable index 2 on param 2x2 }
#define MFG_BYTECODE_GEQUAL				0x4B	// Performs a logical 'is greater than or equal to' operation between two variables { variable 1 index on param 1x2, variable index 2 on param 2x2 }
#define MFG_BYTECODE_LEQUAL				0x4C	// Performs a logical 'is less than or equal to' operation between two variables { variable 1 index on param 1x2, variable index 2 on param 2x2 }
#define MFG_BYTECODE_EQUAL				0x4D	// Performs a logical 'is equal to' operation between two variables { variable 1 index on param 1x2, variable index 2 on param 2x2 }
#define MFG_BYTECODE_DIFFERENT			0x4E	// Performs a logical 'is not equal to' operation between two variables { variable 1 index on param 1x2, variable index 2 on param 2x2 }

	// Boolean literal setters
#define MFG_BYTECODE_LITB1TRUE			0x50	// Sets a variable to a boolean literal with the value 'true' { variable index on param 1x2 }
#define MFG_BYTECODE_LITB1FALSE			0x51	// Sets a variable to a boolean literal with the value 'false' { variable index on param 1x2 }

	// Integer literal setters
#define MFG_BYTECODE_LITI1				0x52	// Sets a variable to a integer scalar literal { variable index on param 1x2, literal on param 2x4 }
#define MFG_BYTECODE_LITI2				0x53	// Sets a variable to a 2 component integer vector literal { variable index on param 1x2, literal on param 2x8 }
#define MFG_BYTECODE_LITI3				0x54	// Sets a variable to a 3 component integer vector literal { variable index on param 1x2, literal on param 2x12 }
#define MFG_BYTECODE_LITI4				0x55	// Sets a variable to a 4 component integer vector literal { variable index on param 1x2, literal on param 2x16 }
#define MFG_BYTECODE_LITI22				0x56	// Sets a variable to a integer 2x2 matrix literal { variable index on param 1x2, literal on param 2x16 }
#define MFG_BYTECODE_LITI33				0x57	// Sets a variable to a integer 3x3 matrix literal { variable index on param 1x2, literal on param 2x36 }
#define MFG_BYTECODE_LITI44				0x58	// Sets a variable to a integer 4x4 matrix literal { variable index on param 1x2, literal on param 2x64 }

	// Floating point literal setters
#define MFG_BYTECODE_LITF1				0x60	// Sets a variable to a floating point scalar literal { variable index on param 1x2, literal on param 2x4 }
#define MFG_BYTECODE_LITF2				0x61	// Sets a variable to a 2 component floating point vector literal { variable index on param 1x2, literal on param 2x8 }
#define MFG_BYTECODE_LITF3				0x62	// Sets a variable to a 3 component floating point vector literal { variable index on param 1x2, literal on param 2x12 }
#define MFG_BYTECODE_LITF4				0x63	// Sets a variable to a 4 component floating point vector literal { variable index on param 1x2, literal on param 2x16 }
#define MFG_BYTECODE_LITF22				0x64	// Sets a variable to a floating point 2x2 matrix literal { variable index on param 1x2, literal on param 2x16 }
#define MFG_BYTECODE_LITF33				0x65	// Sets a variable to a floating point 3x3 matrix literal { variable index on param 1x2, literal on param 2x36 }
#define MFG_BYTECODE_LITF44				0x66	// Sets a variable to a floating point 4x4 matrix literal { variable index on param 1x2, literal on param 2x64 }

	// Vector/matrix component getters
	// These getters turn a certain variable index into a reference to a vector or matrix component
#define MFG_BYTECODE_GET2CMP			0x70	// Sets variable index { param 1x2 } to a reference to a component of a 2 component vector  { vector index on param 2x2, component index on param 3x1 }
#define MFG_BYTECODE_GET3CMP			0x71	// Sets variable index { param 1x2 } to a reference to a component of a 3 component vector  { vector index on param 2x2, component index on param 3x1 }
#define MFG_BYTECODE_GET4CMP			0x72	// Sets variable index { param 1x2 } to a reference to a component of a 4 component vector  { vector index on param 2x2, component index on param 3x1 }
#define MFG_BYTECODE_GET22CMP			0x73	// Sets variable index { param 1x2 } to a reference to a component of a 2x2 matrix  { matrix index on param 2x2, component index on param 3x1 (the top left component has index 0, the bottom right component has the last index) }
#define MFG_BYTECODE_GET33CMP			0x74	// Sets variable index { param 1x2 } to a reference to a component of a 3x3 matrix  { matrix index on param 2x2, component index on param 3x1 (the top left component has index 0, the bottom right component has the last index) }
#define MFG_BYTECODE_GET44CMP			0x75	// Sets variable index { param 1x2 } to a reference to a component of a 4x4 matrix  { matrix index on param 2x2, component index on param 3x1 (the top left component has index 0, the bottom right component has the last index) }

	// Scope and flow control
#define MFG_BYTECODE_OPSCOPE			0x80

#ifdef __cplusplus
}
#endif
