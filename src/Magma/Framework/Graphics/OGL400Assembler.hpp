#pragma once

#include "ShaderData.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace Graphics
		{
			/// <summary>
			///		Assembles binary bytecode into GLSL (version 4.0.0)
			/// </summary>
			static class OGL400Assembler
			{
			public:
				OGL400Assembler() = delete;
				~OGL400Assembler() = delete;

				/// <summary>
				///		Assembles binary bytecode into GLSL (version 4.0.0)
				/// </summary>
				/// <param name="data">Shader data</param>
				/// <param name="out">Reference to output string</param>
				/// <exception cref="Magma::Framework::Graphics::ShaderError">Thrown if there is an error in the assembly</exception>
				static void Assemble(const ShaderData& data, std::string& out);
			};
		}
	}
}