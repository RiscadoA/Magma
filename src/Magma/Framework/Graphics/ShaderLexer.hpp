#pragma once

#include "ShaderPreprocessor.hpp"

#include <vector>
#include <string>
#include <tuple>
#include <sstream>

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
					ss << " {\"" << tok.attribute << "\"}'";
				return ss.str();
			}

			/// <summary>
			///		Static class for the MSL compiler lexing stage
			/// </summary>
			class ShaderLexer
			{
			public:
				ShaderLexer() = delete;
				~ShaderLexer() = delete;

				/// <summary>
				///		Runs the lexer stage from the preprocessor output
				/// </summary>
				/// <param name="in">Preprocessor data</param>
				/// <param name="out">Shader tokens</param>
				/// <param name="data">Shader compiler data</param>
				static void Run(const std::vector<ShaderLine>& in, std::vector<ShaderToken>& out, ShaderCompilerData& data);
			};
		}
	}
}
