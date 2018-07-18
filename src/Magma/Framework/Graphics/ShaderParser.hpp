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
			///		Shader abstract syntax tree node symbols
			/// </summary>
			enum class ShaderASTNodeSymbol
			{
				Invalid = -1,

				Count
			};

			/// <summary>
			///		Shader abstract syntax tree node types
			/// </summary>
			enum class ShaderASTNodeType
			{
				Invalid = -1,

				Count
			};

			/// <summary>
			///		Contains info about a shader AST node
			/// </summary>
			struct ShaderASTNode
			{
				ShaderASTNodeSymbol symbol;
				ShaderASTNodeType type;

				ShaderASTNode* parent;
				ShaderASTNode* child;
				ShaderASTNode* next;

				size_t lineNumber;
			};

			/// <summary>
			///		Static class for the MSL compiler parser stage
			/// </summary>
			class ShaderParser
			{
			public:
				ShaderParser() = delete;
				~ShaderParser() = delete;

				/// <summary>
				///		Parses the shader tokens into an abstract syntax tree
				/// </summary>
				/// <param name="in">Shader tokens</param>
				/// <param name="out">Abstract syntax tree</param>
				/// <param name="data">Shader compiler data</param>
				static void Run(const std::vector<ShaderToken>& in, ShaderASTNode*& out, ShaderCompilerData& data);

				/// <summary>
				///		Cleans an abstract syntax tree
				/// </summary>
				/// <param name="node">Abstract syntax tree pointer</param>
				static void Clean(ShaderASTNode* node);
			};
		} 
	}
}