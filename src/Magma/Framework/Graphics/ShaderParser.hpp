#pragma once

#include <vector>
#include <string>

#include "ShaderLexer.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace Graphics
		{
			/// <summary>
			///		Static class for the MSL compiler parser stage
			/// </summary>
			class ShaderParser
			{
			public:
				ShaderParser() = delete;
				~ShaderParser() = delete;

				/// <summary>
				///		Parses the shader tokens into a syntax tree
				/// </summary>
				/// <param name="in">Shader tokens</param>
				/// <param name="out">Syntax tree root pointer</param>
				/// <param name="data">Shader compiler data</param>
				static void Run(const std::vector<ShaderToken>& in, ShaderSTNode*& out, ShaderCompilerData& data);

				/// <summary>
				///		Cleans an syntax tree
				/// </summary>
				/// <param name="node">Syntax tree root pointer</param>
				static void Clean(ShaderSTNode* node);
			};
		} 
	}
}