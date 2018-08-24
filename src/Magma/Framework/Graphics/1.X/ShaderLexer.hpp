#pragma once

#include "ShaderPreprocessor.hpp"

#include <vector>
#include <string>
#include <tuple>

namespace Magma
{
	namespace Framework
	{
		namespace Graphics::V1X
		{
			namespace Version_1_X
			{
				/// <summary>
				///		Static class for the MSL compiler lexing stage
				/// </summary>
				class ShaderLexer
				{
				public:
					ShaderLexer() = delete;
					~ShaderLexer() = delete;

					/// <summary>
					///		Runs the lexer stage from the preprocessor output
					/// </summary>
					/// <param name="in">Preprocessor data</param>
					/// <param name="out">Shader tokens</param>
					/// <param name="data">Shader compiler data</param>
					static void Run(const std::vector<ShaderLine>& in, std::vector<ShaderToken>& out, ShaderCompilerData& data);
				};
			}
		}
	}
}
