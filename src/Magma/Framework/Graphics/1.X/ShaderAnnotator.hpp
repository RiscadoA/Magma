#pragma once

#include <vector>
#include <string>

#include "ShaderParser.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace Graphics_V1X
		{
			namespace Version_1_X
			{
				/// <summary>
				///		Holds data about a shader function
				/// </summary>
				struct ShaderFunction
				{
					std::string name;
					ShaderVariableType returnType;
					std::vector<ShaderVariableType> params;
					size_t minorVersion = 0;
				};

				extern std::vector<ShaderFunction> ShaderFunctions;

				/// <summary>
				///		Static class for the MSL compiler annotator stage
				/// </summary>
				class ShaderAnnotator
				{
				public:
					ShaderAnnotator() = delete;
					~ShaderAnnotator() = delete;

					/// <summary>
					///		Turns the shader syntax tree into an annotated syntax tree
					/// </summary>
					/// <param name="tree">Shader syntax tree root node pointer to annotate</param>
					/// <param name="data">Shader compiler data</param>
					static void Run(ShaderSTNode* tree, ShaderCompilerData& data);

					/// <summary>
					///		Prints an annotated syntax tree
					/// </summary>
					/// <param name="node">Annotated syntax tree root node pointer</param>
					/// <param name="indentation">Starting indentation</param>
					static void Print(ShaderSTNode* node, size_t indentation = 0);
				};
			}
		}
	}
}