#pragma once

#include "ShaderData.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace Graphics_V1X
		{
			/// <summary>
			///		Assembles binary bytecode into GLSL (version 4.0.0)
			/// </summary>
			class OGL410Assembler
			{
			public:
				OGL410Assembler() = delete;
				~OGL410Assembler() = delete;

				/// <summary>
				///		Assembles binary bytecode into GLSL (version 4.0.0)
				/// </summary>
				/// <param name="data">Shader data</param>
				/// <param name="out">Reference to output string</param>
				/// <exception cref="Magma::Framework::Graphics_V1X::ShaderError">Thrown if there is an error in the assembly</exception>
				static void Assemble(const ShaderData& data, std::string& out);
			};
		}
	}
}