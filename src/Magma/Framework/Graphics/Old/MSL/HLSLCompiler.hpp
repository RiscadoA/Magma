#pragma once

#include "Compiler.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace Graphics
		{
			namespace MSL
			{
				class HLSLCompiler : public Compiler
				{
				public:
					HLSLCompiler();

					virtual void GenerateCode() final;
				};
			}
		}
	}
}
