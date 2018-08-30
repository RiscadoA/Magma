#pragma once

#include "ShaderCompiler.hpp"

#include <vector>
#include <string>

namespace Magma
{
	namespace Framework
	{
		namespace Graphics::V1X
		{
			/// <summary>
			///		Holds data about a single shader line
			/// </summary>
			struct ShaderLine
			{
				std::string str;
				size_t lineNumber;
			};

			/// <summary>
			///		Static class for the MSL compiler preprocessor stage
			/// </summary>
			class ShaderPreprocessor
			{
			public:
				ShaderPreprocessor() = delete;
				~ShaderPreprocessor() = delete;

				/// <summary>
				///		Runs the preprocessor stage on a MSL shader
				/// </summary>
				/// <param name="in">In MSL shader source</param>
				/// <param name="out">Shader lines (with preprocessor applied)</param>
				/// <param name="data">Shader data obtained</param>
				static void Run(const std::string& in, std::vector<ShaderLine>& out, ShaderCompilerData& data);
			};
		}
	}
}