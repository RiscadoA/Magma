#pragma once

#include "Compiler.h"
#include "../Bytecode.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace Graphics
		{
			namespace V2X
			{
				namespace MSL
				{
					/// <summary>
					///		Represents a shader's type.
					/// </summary>
					enum class ShaderType
					{
						Vertex	= MFG_VERTEX_SHADER,
						Pixel	= MFG_PIXEL_SHADER,
					};

					/// <summary>
					///		Compiles a MSL shader into MSL bytecode.
					/// </summary>
					/// <param name="src">Source MSL code</param>
					/// <param name="outBytecode">Out bytecode array</param>
					/// <param name="bytecodeMaxSize">Bytecode array size</param>
					/// <param name="outBytecodeSize">Out bytecode size</param>
					/// <param name="outMetaData">Out meta data buffer</param>
					/// <param name="metaDataMaxSize">Meta data buffer size</param>
					/// <param name="outMetaDataSize">Out meta data size</param>
					/// <param name="type">Shader type</param>
					void Compile(const mfsUTF8CodeUnit* src, mfmU8* outBytecode, mfmU64 bytecodeMaxSize, mfmU64* outBytecodeSize, mfmU8* outMetaData, mfmU64 metaDataMaxSize, mfmU64* outMetaDataSize, ShaderType type);
				}
			}
		}
	}
}
