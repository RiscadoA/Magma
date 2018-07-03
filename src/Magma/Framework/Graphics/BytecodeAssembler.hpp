#pragma once

#include <string>

#include "Exception.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace Graphics
		{
			/// <summary>
			///		Used to describe a bytecode instruction operation code
			/// </summary>
			enum class BytecodeOpCode : unsigned char
			{
				// Declarations
				DeclI1	= 0x00, // Declares a local integer { variable index stored on param 1x1 }
				DeclI2	= 0x01, // Declares a local two component integer vector { variable index stored on param 1x1 }
				DeclI3	= 0x02,
				DeclI4	= 0x03,
				DeclI22	= 0x04,
				DeclI33	= 0x05,
				DeclI44	= 0x06,

				DeclF1	= 0x07,
				DeclF2	= 0x08,
				DeclF3	= 0x09,
				DeclF4	= 0x0A,
				DeclF22 = 0x0B,
				DeclF33	= 0x0C,
				DeclF44	= 0x0D,

				// Registers
				VarIn0	= 0x0E,
				VarIn1	= 0x0F,
				VarOut	= 0x10,

				// Operations
				Assign	= 0x11,
				Add		= 0x12,
				Sub		= 0x13,
				Mul		= 0x14,
				Div		= 0x15,
				Mod		= 0x16,
				And		= 0x17,
				Or		= 0x18,
				Not		= 0x19,
				Greater	= 0x1A,
				Less	= 0x1B,
				GEqual	= 0x1C,
				LEqual	= 0x1D,
				Equal	= 0x1E,
				NEqual	= 0x1F,

				// Literals
				SetI1	= 0x20, // Sets a int variable { index on param 1x1 } to an int literal { param 2x4 }
				SetI2	= 0x21, // Sets a two component integer vector variable { index on param 1x1 } to a two component integer literal { param 2x8 }
				SetI3	= 0x22, // Sets a three component integer vector variable { index on param 1x1 } to a three component integer literal { param 2x8 }
				SetI4	= 0x23, // Sets a four component integer vector variable { index on param 1x1 } to a four component integer literal { param 2x8 }
				SetF1	= 0x24, // Sets a float variable { index on param 1x1 } to a float literal { param 2x4 }
				SetF2	= 0x25, // Sets a two component floating point vector variable { index on param 1x1 } to a two component floating point literal { param 2x8 }
				SetF3	= 0x26, // Sets a three component floating point vector variable { index on param 1x1 } to a three component floating point literal { param 2x8 }
				SetF4	= 0x27, // Sets a four component floating point vector variable { index on param 1x1 } to a four component floating point literal { param 2x8 }

				// Vector component operations
				AsF2Cmp	= 0x28, // Assigns { VarOut } F2 component { index on param 1x1 } to { VarIn0 } F1
				AsF3Cmp = 0x29, // Assigns { VarOut } F3 component { index on param 1x1 } to { VarIn0 } F1
				AsF4Cmp = 0x2A, // Assigns { VarOut } F4 component { index on param 1x1 } to { VarIn0 } F1
				AsI2Cmp = 0x2B, // Assigns { VarOut } I2 component { index on param 1x1 } to { VarIn0 } I1
				AsI3Cmp = 0x2C, // Assigns { VarOut } I3 component { index on param 1x1 } to { VarIn0 } I1
				AsI4Cmp = 0x2D, // Assigns { VarOut } I4 component { index on param 1x1 } to { VarIn0 } I1

				GeF2Cmp	= 0x2E, // Assigns { VarOut } F1 to { VarIn0 } F2 component { index on param 1x1 }
				GeF3Cmp = 0x2F, // Assigns { VarOut } F1 to { VarIn0 } F3 component { index on param 1x1 }
				GeF4Cmp = 0x30, // Assigns { VarOut } F1 to { VarIn0 } F4 component { index on param 1x1 }
				GeI2Cmp = 0x31, // Assigns { VarOut } I1 to { VarIn0 } I2 component { index on param 1x1 }
				GeI3Cmp = 0x32, // Assigns { VarOut } I1 to { VarIn0 } I3 component { index on param 1x1 }
				GeI4Cmp = 0x33, // Assigns { VarOut } I1 to { VarIn0 } I4 component { index on param 1x1 }
			
				// Scope and flow control
				OpScope	= 0x34,	// Opens a scope (declarations inside override previous indexes)
				ClScope	= 0x35,	// Closes the previously opened scope
				Discard	= 0x36,	// Discards the current pixel and exits the shader (only works on pixel shaders)
				Return	= 0x37,	// Exits the shader
				While	= 0x38, // Repeats the next instruction/scope as long as the variable with the index on { param 1x1 } evaluates to true

				// Math functions
				MulMat	= 0x39,	// Performs matrix multiplication between VarIn0 and VarIn1 and stores the result on VarOut
				Smple2D	= 0x3A,	// Sets { VarOut } to the sampled value of a texture on { VarIn0 } on the coordinates on { VarIn1 }
			};

			/// <summary>
			///		Static class used to assemble binary bytecode from a bytecode string
			/// </summary>
			class BytecodeAssembler
			{
			public:
				BytecodeAssembler() = delete;
				~BytecodeAssembler() = delete;

				/// <summary>
				///		Assembles binary bytecode from bytecode string
				/// </summary>
				/// <param name="code">Bytecode string</param>
				/// <param name="out">Binary bytecode</param>
				/// <param name="maxSize">Max bytecode size</param>
				/// <returns>Binary bytecode size in bytes</returns>
				/// <exception cref="Magma::Framework::Graphics::ShaderError">Thrown if there is an error in the assembly</exception>
				static size_t Assemble(const std::string& code, char* out, size_t maxSize);
			};
		}
	}
}