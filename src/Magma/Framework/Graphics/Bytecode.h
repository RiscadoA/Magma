#pragma once

#include "../Memory/Type.h"

#ifdef __cplusplus
extern "C"
{
#endif

	// Integer components
#define MFG_BYTECODE_DECLI1				0x01	// Declares an integer scalar variable { variable index stored on param 1x2 }
#define MFG_BYTECODE_DECLI2				0x02	// Declares an integer 2 component vector variable { variable index stored on param 1x2 }
#define MFG_BYTECODE_DECLI3				0x03	// Declares an integer 3 component vector variable { variable index stored on param 1x2 }
#define MFG_BYTECODE_DECLI4				0x04	// Declares an integer 4 component vector variable { variable index stored on param 1x2 }
#define MFG_BYTECODE_DECLI22			0x05	// Declares an integer 2x2 matrix variable { variable index stored on param 1x2 }
#define MFG_BYTECODE_DECLI33			0x06	// Declares an integer 3x3 matrix variable { variable index stored on param 1x2 }
#define MFG_BYTECODE_DECLI44			0x07	// Declares an integer 4x4 matrix variable { variable index stored on param 1x2 }

	// Integer component arrays
#define MFG_BYTECODE_DECLI1A			0x08	// Declares an integer scalar variable array { variable index stored on param 1x2, element count on param 2x2 }
#define MFG_BYTECODE_DECLI2A			0x09	// Declares an integer 2 component vector variable array { variable index stored on param 1x2, element count on param 2x2 }
#define MFG_BYTECODE_DECLI3A			0x0A	// Declares an integer 3 component vector variable array { variable index stored on param 1x2, element count on param 2x2 }
#define MFG_BYTECODE_DECLI4A			0x0B	// Declares an integer 4 component vector variable array { variable index stored on param 1x2, element count on param 2x2 }
#define MFG_BYTECODE_DECLI22A			0x0C	// Declares an integer 2x2 matrix variable array { variable index stored on param 1x2, element count on param 2x2 }
#define MFG_BYTECODE_DECLI33A			0x0D	// Declares an integer 3x3 matrix variable array { variable index stored on param 1x2, element count on param 2x2 }
#define MFG_BYTECODE_DECLI44A			0x0E	// Declares an integer 4x4 matrix variable array { variable index stored on param 1x2, element count on param 2x2 }

	// Floating point components
#define MFG_BYTECODE_DECLF1				0x0F	// Declares an floating point scalar variable { variable index stored on param 1x2 }
#define MFG_BYTECODE_DECLF2				0x10	// Declares an floating point 2 component vector variable { variable index stored on param 1x2 }
#define MFG_BYTECODE_DECLF3				0x11	// Declares an floating point 3 component vector variable { variable index stored on param 1x2 }
#define MFG_BYTECODE_DECLF4				0x12	// Declares an floating point 4 component vector variable { variable index stored on param 1x2 }
#define MFG_BYTECODE_DECLF22			0x13	// Declares an floating point 2x2 matrix variable { variable index stored on param 1x2 }
#define MFG_BYTECODE_DECLF33			0x14	// Declares an floating point 3x3 matrix variable { variable index stored on param 1x2 }
#define MFG_BYTECODE_DECLF44			0x15	// Declares an floating point 4x4 matrix variable { variable index stored on param 1x2 }

	// Floating point component arrays
#define MFG_BYTECODE_DECLF1A			0x16	// Declares an floating point scalar variable array { variable index stored on param 1x2, element count on param 2x2 }
#define MFG_BYTECODE_DECLF2A			0x17	// Declares an floating point 2 component vector variable array { variable index stored on param 1x2, element count on param 2x2 }
#define MFG_BYTECODE_DECLF3A			0x18	// Declares an floating point 3 component vector variable array { variable index stored on param 1x2, element count on param 2x2 }
#define MFG_BYTECODE_DECLF4A			0x19	// Declares an floating point 4 component vector variable array { variable index stored on param 1x2, element count on param 2x2 }
#define MFG_BYTECODE_DECLF22A			0x1A	// Declares an floating point 2x2 matrix variable array { variable index stored on param 1x2, element count on param 2x2 }
#define MFG_BYTECODE_DECLF33A			0x1B	// Declares an floating point 3x3 matrix variable array { variable index stored on param 1x2, element count on param 2x2 }
#define MFG_BYTECODE_DECLF44A			0x1C	// Declares an floating point 4x4 matrix variable array { variable index stored on param 1x2, element count on param 2x2 }

	// Boolean
#define MFG_BYTECODE_DECLB1				0x1D	// Declares a boolean variable { variable index stored on param 1x2 }

	// Operations
#define MFG_BYTECODE_ASSIGN				0x1E
#define MFG_BYTECODE_ADD				0x1F
#define MFG_BYTECODE_SUBTRACT			0x20
#define MFG_BYTECODE_MULTIPLY			0x21
#define MFG_BYTECODE_DIVIDE				0x22
#define MFG_BYTECODE_AND				0x23
#define MFG_BYTECODE_OR					0x24
#define MFG_BYTECODE_NOT				0x25
#define MFG_BYTECODE_NEGATE				0x26
#define MFG_BYTECODE_GREATER			0x27
#define MFG_BYTECODE_LESS				0x28

#ifdef __cplusplus
}
#endif
