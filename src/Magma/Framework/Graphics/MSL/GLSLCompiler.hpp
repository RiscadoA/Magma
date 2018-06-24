#pragma once

#include "Compiler.hpp"
#include "../Context.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace Graphics
		{
			namespace MSL
			{
				/// <summary>
				///		Implements the MSL compiler for GLSL
				/// </summary>
				class GLSLCompiler : public Compiler
				{
				public:
					GLSLCompiler();

					/// <summary>
					///		Sets the shader type to be compiled
					/// </summary>
					/// <param name="shaderType">Shader type to compile</param>
					inline void SetShaderType(ShaderType shaderType) { m_shaderType = shaderType; }

				private:
					ShaderType m_shaderType;
				};
			}
		}
	}
}
