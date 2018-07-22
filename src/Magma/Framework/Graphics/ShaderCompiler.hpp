#pragma once

#include <string>
#include <vector>
#include <list>
#include <sstream>
#include <memory>

#include "ShaderData.hpp"

namespace Magma
{
	namespace Framework
	{
		namespace Graphics
		{
			/// <summary>
			///		Shader variable types
			/// </summary>
			enum class ShaderVariableType
			{
				Invalid = -1,

				Int1,
				Int2,
				Int3,
				Int4,
				Int22,
				Int33,
				Int44,

				Float1,
				Float2,
				Float3,
				Float4,
				Float22,
				Float33,
				Float44,

				Bool,

				ConstantBuffer,
				Texture2D,

				Count
			};

			/// <summary>
			///		Converts a shader variable type into a string
			/// </summary>
			inline std::string ShaderVariableTypeToString(ShaderVariableType type)
			{
				switch (type)
				{
					case ShaderVariableType::Int1: return "int1";
					case ShaderVariableType::Int2: return "int2";
					case ShaderVariableType::Int3: return "int3";
					case ShaderVariableType::Int4: return "int4";
					case ShaderVariableType::Int22: return "int22";
					case ShaderVariableType::Int33: return "int33";
					case ShaderVariableType::Int44: return "int44";

					case ShaderVariableType::Float1: return "float1";
					case ShaderVariableType::Float2: return "float2";
					case ShaderVariableType::Float3: return "float3";
					case ShaderVariableType::Float4: return "float4";
					case ShaderVariableType::Float22: return "float22";
					case ShaderVariableType::Float33: return "float33";
					case ShaderVariableType::Float44: return "float44";

					case ShaderVariableType::Bool: return "bool";

					case ShaderVariableType::ConstantBuffer: return "constant-buffer";
					case ShaderVariableType::Texture2D: return "texture2d";

					case ShaderVariableType::Invalid: return "$invalid-shader-var-type$";
					default: return "$unknown-shader-var-type$";
				}
			}

			/// <summary>
			///		Shader operator types
			/// </summary>
			enum class ShaderOperatorType
			{
				Invalid = -1,

				Add,
				Subtract,
				Multiply,
				Divide,
				Equal,
				Different,
				Greater,
				Less,
				GEqual,
				LEqual,
				And,
				Or,
				Not,
				Assign,
				Member,

				Count
			};

			/// <summary>
			///		Converts a shader operator type into a string
			/// </summary>
			inline std::string ShaderOperatorTypeToString(ShaderOperatorType type)
			{
				switch (type)
				{
					case ShaderOperatorType::Add: return "add";
					case ShaderOperatorType::Subtract: return "subtract";
					case ShaderOperatorType::Multiply: return "multiply";
					case ShaderOperatorType::Divide: return "divide";
					case ShaderOperatorType::Equal: return "equal";
					case ShaderOperatorType::Different: return "different";
					case ShaderOperatorType::Greater: return "greater";
					case ShaderOperatorType::Less: return "less";
					case ShaderOperatorType::GEqual: return "gequal";
					case ShaderOperatorType::LEqual: return "lequal";
					case ShaderOperatorType::And: return "and";
					case ShaderOperatorType::Or: return "or";
					case ShaderOperatorType::Not: return "not";
					case ShaderOperatorType::Assign: return "assign";
					case ShaderOperatorType::Member: return "member";

					case ShaderOperatorType::Invalid: return "$invalid-shader-operator-type$";
					default: return "$unknown-shader-operator-type$";
				}
			}

			/// <summary>
			///		Shader punctuation types
			/// </summary>
			enum class ShaderPunctuationType
			{
				Invalid = -1,

				OpenBraces,
				CloseBraces,
				OpenBrackets,
				CloseBrackets,
				OpenParenthesis,
				CloseParenthesis,
				Semicolon,
				Colon,
				Comma,

				Count
			};

			/// <summary>
			///		Converts a shader punctuation type into a string
			/// </summary>
			inline std::string ShaderPunctuationTypeToString(ShaderPunctuationType type)
			{
				switch (type)
				{
					case ShaderPunctuationType::OpenBraces: return "open-braces";
					case ShaderPunctuationType::CloseBraces: return "close-braces";
					case ShaderPunctuationType::OpenBrackets: return "open-brackets";
					case ShaderPunctuationType::CloseBrackets: return "close-brackets";
					case ShaderPunctuationType::OpenParenthesis: return "open-parenthesis";
					case ShaderPunctuationType::CloseParenthesis: return "close-parenthesis";
					case ShaderPunctuationType::Semicolon: return "semicolon";
					case ShaderPunctuationType::Colon: return "colon";
					case ShaderPunctuationType::Comma: return "comma";

					case ShaderPunctuationType::Invalid: return "$invalid-shader-punctuation-type$";
					default: return "$unknown-shader-punctuation-type$";
				}
			}

			/// <summary>
			///		Magma Shading Language token type
			/// </summary>
			enum class ShaderTokenType
			{
				/// <summary>
				///		Invalid token type
				/// </summary>
				Invalid = -1,

				Type,
				Operator,
				Punctuation,

				Literal,
				Identifier,

				Shader,
				Input,
				Output,
				Texture2D,
				ConstantBuffer,

				If,
				Else,
				While,
				Return,
				Discard,

				/// <summary>
				///		Token type count
				/// </summary>
				Count
			};

			/// <summary>
			///		Converts a shader punctuation type into a string
			/// </summary>
			inline std::string ShaderTokenTypeToString(ShaderTokenType type)
			{
				switch (type)
				{
					case ShaderTokenType::Type: return "type";
					case ShaderTokenType::Operator: return "operator";
					case ShaderTokenType::Punctuation: return "punctuation";
					case ShaderTokenType::Literal: return "literal";
					case ShaderTokenType::Identifier: return "identifier";
					case ShaderTokenType::Shader: return "shader";
					case ShaderTokenType::Input: return "input";
					case ShaderTokenType::Output: return "output";
					case ShaderTokenType::Texture2D: return "texture2d";
					case ShaderTokenType::ConstantBuffer: return "constant-buffer";
					case ShaderTokenType::If: return "if";
					case ShaderTokenType::Else: return "else";
					case ShaderTokenType::While: return "while";
					case ShaderTokenType::Return: return "return";
					case ShaderTokenType::Discard: return "discard";

					case ShaderTokenType::Invalid: return "$invalid-shader-token-type$";
					default: return "$unknown-shader-token-type$";
				}
			}

			/// <summary>
			///		Holds data about a shader token produced by the ShaderLexer::Run function
			/// </summary>
			struct ShaderToken
			{
				ShaderTokenType type;

				union
				{
					ShaderVariableType variableType;
					ShaderOperatorType operatorType;
					ShaderPunctuationType punctuationType;
				};

				std::string attribute;
				size_t lineNumber;
			};

			/// <summary>
			///		Converts a shader token into a string
			/// </summary>
			inline std::string ShaderTokenToString(const ShaderToken& tok)
			{
				std::stringstream ss;
				ss << "'" << ShaderTokenTypeToString(tok.type);

				if (tok.type == ShaderTokenType::Type ||
					tok.type == ShaderTokenType::Literal)
					ss << " [" << ShaderVariableTypeToString(tok.variableType) << "]";
				else if (tok.type == ShaderTokenType::Operator)
					ss << " [" << ShaderOperatorTypeToString(tok.operatorType) << "]";
				else if (tok.type == ShaderTokenType::Punctuation)
					ss << " [" << ShaderPunctuationTypeToString(tok.punctuationType) << "]";

				if (!tok.attribute.empty())
					ss << " {\"" << tok.attribute << "\"}";

				ss << "'";

				return ss.str();
			}

			/// <summary>
			///		Holds data about a shader variable
			/// </summary>
			struct ShaderVariable
			{
				std::string bufferName = "";
				std::string name = "";
				std::string id = "";
				ShaderVariableType type = ShaderVariableType::Invalid;
				size_t index = -1;
			};
			
			/// <summary>
			///		Holds data about a shader scope
			/// </summary>
			struct ShaderScope
			{
				std::weak_ptr<ShaderScope> parent;
				std::shared_ptr<ShaderScope> child = nullptr;
				std::shared_ptr<ShaderScope> next = nullptr;

				std::list<ShaderVariable> variables;

				ShaderVariable* GetVar(const std::string& name);
			};

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
					Reference
						Identifier
				*/
				Reference,

				/*
					ComponentX
						Reference
				*/
				ComponentX,
				
				/*
					ComponentY
						Reference
				*/
				ComponentY,

				/*
					ComponentZ
						Reference
				*/
				ComponentZ,

				/*
					ComponentW
						Reference
				*/
				ComponentW,

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
			inline std::string ShaderSTNodeTypeToString(ShaderSTNodeType type)
			{
				switch (type)
				{
					case ShaderSTNodeType::Type: return "type";
					case ShaderSTNodeType::Operator: return "operator";
					case ShaderSTNodeType::Literal: return "literal";
					case ShaderSTNodeType::Identifier: return "identifier";
					case ShaderSTNodeType::Reference: return "reference";
					case ShaderSTNodeType::ComponentX: return "componentx";
					case ShaderSTNodeType::ComponentY: return "componenty";
					case ShaderSTNodeType::ComponentZ: return "componentz";
					case ShaderSTNodeType::ComponentW: return "componentxw";
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

				union
				{
					ShaderVariableType variableType;
					ShaderOperatorType operatorType;
				};

				std::string attribute;
				ShaderVariable* reference = nullptr;

				ShaderSTNode* parent = nullptr;
				ShaderSTNode* child = nullptr;
				ShaderSTNode* next = nullptr;

				std::shared_ptr<ShaderScope> scope;

				size_t lineNumber;
			};

			/// <summary>
			///		Converts a shader variable into a string
			/// </summary>
			inline std::string ShaderVariableToString(const ShaderVariable& var)
			{
				std::stringstream ss;
				ss << "'" << ShaderVariableTypeToString(var.type);

				if (var.bufferName != "")
					ss << " ; [" << var.bufferName << "] ID: \"" << var.id << "\" NAME: \"" << var.name << "\"";
				else
					ss << " ; ID: \"" << var.id << "\" NAME: \"" << var.name << "\"";

				ss << " ; INDEX: " << var.index;
				ss << "'";

				return ss.str();
			}

			/// <summary>
			///		Converts a shader syntax tree node into a string
			/// </summary>
			inline std::string ShaderSTNodeToString(const ShaderSTNode* node)
			{
				std::stringstream ss;
				ss << "'" << ShaderSTNodeTypeToString(node->type);

				if (node->type == ShaderSTNodeType::Type ||
					node->type == ShaderSTNodeType::Literal)
					ss << " [" << ShaderVariableTypeToString(node->variableType) << "]";
				else if (node->type == ShaderSTNodeType::Operator)
					ss << " [" << ShaderOperatorTypeToString(node->operatorType) << "]";

				if (!node->attribute.empty())
					ss << " {\"" << node->attribute << "\"}";

				if (node->reference != nullptr)
					ss << " (" << ShaderVariableToString(*node->reference) << ")";

				ss << "'";

				return ss.str();
			}

			/// <summary>
			///		Holds shader info during compilation
			/// </summary>
			struct ShaderCompilerData
			{
				// Shader version
				unsigned int majorVersion;
				unsigned int minorVersion;

				// Shader type
				ShaderType shaderType;

				// Shader root scope
				std::shared_ptr<ShaderScope> rootScope = nullptr;

				// Input variables
				std::string inputName;
				std::vector<ShaderVariable> inputVariables;

				// Output variables
				std::string outputName;
				std::vector<ShaderVariable> outputVariables;

				// Constant buffers
				std::vector<ShaderVariable> constantBuffers;
				std::vector<ShaderVariable> constantBufferVariables;

				// 2D textures
				std::vector<ShaderVariable> texture2Ds;
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
