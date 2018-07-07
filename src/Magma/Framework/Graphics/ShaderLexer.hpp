#pragma once

#include <vector>
#include <tuple>

namespace Magma
{
	namespace Framework
	{
		namespace Graphics
		{
			/// <summary>
			///		Mamga Shading Language token
			/// </summary>
			enum class ShaderToken
			{
				/// <summary>
				///		Invalid token
				/// </summary>
				Invalid = -1,

				// Keywords

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

			enum class ShaderTokenType
			{
				/// <summary>
				///		Invalid token type
				/// </summary>
				Invalid = -1,

				Keyword,

				Punctuation,

				Operator,

				/// <summary>
				///		Token type count
				/// </summary>
				Count
			};

			extern const std::vector<std::tuple<std::string, ShaderToken, ShaderTokenType>> ShaderTokens;
		}
	}
}