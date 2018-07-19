#pragma once

#include <string>
#include <vector>

#include "ShaderData.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace Graphics
		{
			/// <summary>
			///		Holds shader info during compilation
			/// </summary>
			struct ShaderCompilerData
			{
				unsigned int majorVersion;
				unsigned int minorVersion;

				ShaderType shaderType;

				std::string inputID;
				std::string outputID;

				std::vector<std::pair<std::string, ShaderInputVariable>> inputVars;
				std::vector<std::pair<std::string, ShaderOutputVariable>> outputVars;

				std::vector<ShaderConstantBuffer> constantBuffers;
				std::vector<ShaderConstantBufferVariable> constantBufferVars;
			};

			/// <summary>
			///		Static class for compiling MSL shaders into MSLBC shaders
			/// </summary>
			class ShaderCompiler
			{
			public:
				ShaderCompiler() = delete;
				~ShaderCompiler() = delete;

				/// <summary>
				///		Compiles a MSL shader into a MSLBC shader
				/// </summary>
				/// <param name="in">MSL shader source</param>
				/// <param name="outBC">MSLBC shader</param>
				/// <param name="outMD">MSLMD shader meta data</param>
				static void Run(const std::string& in, std::string& outBC, std::string& outMD);
			};
		}
	}
}
