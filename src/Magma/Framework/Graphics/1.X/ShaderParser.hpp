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
			namespace Version_1_X
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
					/// <param name="out">Syntax tree root node pointer</param>
					/// <param name="data">Shader compiler data</param>
					static void Run(const std::vector<ShaderToken>& in, ShaderSTNode*& out, ShaderCompilerData& data);

					/// <summary>
					///		Cleans a syntax tree
					/// </summary>
					/// <param name="node">Syntax tree root node pointer</param>
					static void Clean(ShaderSTNode* node);

					/// <summary>
					///		Prints a syntax tree
					/// </summary>
					/// <param name="node">Syntax tree root node pointer</param>
					/// <param name="indentation">Starting indentation</param>
					static void Print(ShaderSTNode* node, size_t indentation = 0);
				};
			}
		} 
	}
}