#pragma once

#include "ShaderPreprocessor.hpp"

#include <vector>
#include <string>
#include <tuple>

namespace Magma
{
	namespace Framework
	{
		namespace Graphics
		{
			/// <summary>
			///		Magma Shading Language token
			/// </summary>
			enum class ShaderTokenSymbol
			{
				/// <summary>
				///		Invalid token
				/// </summary>
				Invalid = -1,

				// Types
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

				// Keywords
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

				// Punctuation

				OpenBrace, // {
				CloseBrace, // }
				OpenBrackets, // [
				CloseBrackets, // ]
				OpenParenthesis, // (
				CloseParenthesis, // )
				Semicolon, // ;
				Colon, // :
				Comma, // ,

				// Operators

				Add, // +
				Subtract, // -
				Multiply, // *
				Divide, // /				
				Equal, // ==
				Different, // !=
				Greater, // >
				Less, // <
				GEqual, // >=
				LEqual,	// <=
				And, // &&
				Or, // ||
				Not, // !
				Assign,	// =
				Member, // . (member access)

				// Identifier
				Identifier,

				/// <summary>
				///		Token count
				/// </summary>
				Count
			};

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

				Keyword,

				Punctuation,

				Operator,

				Identifier,

				/// <summary>
				///		Token type count
				/// </summary>
				Count
			};

			/// <summary>
			///		Converts a Magma Shading Language token symbol into a string
			/// </summary>
			/// <param name="symbol">Magma Shading Language token symbol</param>
			/// <returns>String</returns>
			std::string ShaderTokenSymbolToString(ShaderTokenSymbol symbol);

			/// <summary>
			///		Magma Shading Language token info
			/// </summary>
			extern const std::vector<std::tuple<std::string, ShaderTokenSymbol, ShaderTokenType, std::string>> ShaderTokens;

			/// <summary>
			///		Holds data about a lexed shader token
			/// </summary>
			struct ShaderToken
			{
				ShaderTokenSymbol symbol;
				ShaderTokenType type;
				std::string attribute;
				size_t lineNumber;
			};

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
