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
			///		Shader syntax tree node symbols
			/// </summary>
			enum class ShaderSTNodeType
			{
				Invalid = -1,

				/*
					Type
				*/
				Type,

				/*
					Operator
				*/
				Operator,

				// Other

				/*
					Identifier
				*/
				Identifier,

				/*
					Literal
				*/
				Literal,

				/*
					Declaration
						Type
						Identifier
						[OPTIONAL DEFINITION EXPRESSION]
				*/
				Declaration,

				/*
					Constructor
						Type
						Param 1 (expression)
						...
						Param x (expression)
				*/
				Constructor,

				/*
					Scope
						Statement 1
						Statement 2
				*/
				Scope,

				/*
					Branch
						Condition
						If scope
						Else scope (may be null)
				*/
				Branch,

				/*
					While
						Condition
						Body scope
				*/
				While,

				/*
					Return
				*/
				Return,

				/*
					Discard
				*/
				Discard,

				/*
					Call
						Identifier
						Param 1 (expression)
						...
						Param X (expression)
				*/
				Call,

				Count
			};

			/// <summary>
			///		Converts a shader syntax tree node type into a string
			/// </summary>
			std::string ShaderSTNodeTypeToString(ShaderSTNodeType type)
			{
				switch (type)
				{
					case ShaderSTNodeType::Type: return "type";
					case ShaderSTNodeType::Operator: return "operator";
					case ShaderSTNodeType::Literal: return "literal";			
					case ShaderSTNodeType::Identifier: return "identifier";
					case ShaderSTNodeType::Declaration: return "declaration";
					case ShaderSTNodeType::Constructor: return "constructor";
					case ShaderSTNodeType::Scope: return "scope";
					case ShaderSTNodeType::Branch: return "branch";
					case ShaderSTNodeType::While: return "while";
					case ShaderSTNodeType::Return: return "return";
					case ShaderSTNodeType::Discard: return "discard";
					case ShaderSTNodeType::Call: return "call";

					case ShaderSTNodeType::Invalid: return "$invalid-shader-syntax-tree-node-type$";
					default: return "$unknown-shader-syntax-tree-node-type$";
				}
			}

			/// <summary>
			///		Contains info about a shader ST node
			/// </summary>
			struct ShaderSTNode
			{
				ShaderSTNodeType type;

				ShaderVariableType variableType;
				ShaderOperatorType operatorType;

				std::string attribute;

				ShaderSTNode* parent;
				ShaderSTNode* child;
				ShaderSTNode* next;

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
				///		Parses the shader tokens into a syntax tree
				/// </summary>
				/// <param name="in">Shader tokens</param>
				/// <param name="out">Syntax tree root pointer</param>
				/// <param name="data">Shader compiler data</param>
				static void Run(const std::vector<ShaderToken>& in, ShaderSTNode*& out, ShaderCompilerData& data);

				/// <summary>
				///		Cleans an syntax tree
				/// </summary>
				/// <param name="node">Syntax tree root pointer</param>
				static void Clean(ShaderSTNode* node);
			};
		} 
	}
}