#pragma once

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
				///		Static class for the MSL compiler generator stage
				/// </summary>
				class ShaderGenerator
				{
				public:
					ShaderGenerator() = delete;
					~ShaderGenerator() = delete;

					/// <summary>
					///		Generates MSLBC and MSLMD shader source strings from an MSL abstract syntax tree
					/// </summary>
					/// <param name="in">Abstract syntax tree pointer</param>
					/// <param name="outBC">Output MSLBC source</param>
					/// <param name="outMD">Output MSLMD source</param>
					/// <param name="data">Shader compiler data</param>
					static void Run(const ShaderSTNode* in, std::string& outBC, std::string& outMD, ShaderCompilerData& data);
				};
			}
		}
	}
}
