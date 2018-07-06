#pragma once

#include "Exception.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace Graphics
		{
			/// <summary>
			///		Shader meta data assembler
			/// </summary>
			class MetaDataAssembler
			{
			public:
				MetaDataAssembler() = delete;
				~MetaDataAssembler() = delete;

				/// <summary>
				///		Assembles binary meta data from meta data string
				/// </summary>
				/// <param name="code">Meta data string</param>
				/// <param name="out">Binary meta data</param>
				/// <param name="maxSize">Max meta data size</param>
				/// <returns>Binary meta data size in bytes</returns>
				/// <exception cref="Magma::Framework::Graphics::ShaderError">Thrown if there is an error in the assembly</exception>
				static size_t Assemble(const std::string& code, char* out, size_t maxSize);
			};
		}
	}
}