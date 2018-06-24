#pragma once

#include <string>
#include <vector>
#include <list>

namespace Magma
{
	namespace Framework
	{
		namespace Graphics
		{
			namespace MSL
			{
				/*

					Formal grammar

					<program> ::= <vertex_output> <function> <function>

					<vertex_output> ::= "VertexOutput" <id> "{" {<type> <id> ";"} "}"

					<function> ::= <type> <id> "(" <input_param> ")" "{" <statement> "}"
					<input_param> ::= <type> <id> [ "," <input_param> ]

					<statement> ::= "return" <exp> ";"
								  |	<type> <id> [ "=" <exp> ] ";"

					<exp> ::= <variable> "=" <exp> | <term> { ("+" | "-") <term> }
					<term> ::= <factor> { ("*" | "/" | "%") <factor> }
					<factor> ::= "(" <exp> ")" | <unary_op> <factor> | <variable> | <literal> | <constructor>
					<variable> ::= <identifier> | <identifier> { <member_op> <identifier> }
				*/

				/// <summary>
				///		Token symbols
				/// </summary>
				enum class TokenSymbol
				{
					Invalid				= -1,

					// Punctuation
					OpenBrace			= 0x00,
					CloseBrace			= 0x01,
					OpenParenthesis		= 0x02,
					CloseParenthesis	= 0x03,
					Semicolon			= 0x04,
					Comma				= 0x05,

					// Identifier (variable names, function names, etc)
					Identifier			= 0x06,

					// Literals
					IntLiteral			= 0x07,
					FloatLiteral		= 0x08,

					// Type keywords
					Int					= 0x09,
					Float				= 0x0A,
					Vec2				= 0x0B,
					Vec3				= 0x0C,
					Vec4				= 0x0D,
					IVec2				= 0x0E,
					IVec3				= 0x0F,
					IVec4				= 0x10,
					Mat2				= 0x11,
					Mat3				= 0x12,
					Mat4				= 0x13,

					// Operators
					Add					= 0x14,
					Sub					= 0x15,
					Mul					= 0x16,
					Div					= 0x17,
					Mod					= 0x18,
					Member				= 0x19,
					Assignment			= 0x1A,

					// Reserved keywords
					Return				= 0x1B,
					VertexOutput		= 0x1C,

					Count
				};

				/// <summary>
				///		Token types
				/// </summary>
				enum class TokenType
				{
					Invalid				= -1,

					Punctuation			= 0x00,
					Identifier			= 0x01,	
					Literal				= 0x02,
					Type				= 0x03,
					Operator			= 0x04,
					Reserved			= 0x05,

					Count
				};

				/// <summary>
				///		Represents a token
				/// </summary>
				struct Token
				{
					int line;
					TokenSymbol symbol;
					TokenType type;
					std::string attribute;
				};

				/// <summary>
				///		Abstract syntax tree node symbols
				/// </summary>
				enum class ASTNodeSymbol
				{
					Invalid = -1,

					//	Program
					Program				= 0x00,
					Params				= 0x01,
					Scope				= 0x02,
					Function			= 0x03,
					Statement			= 0x04,
					Constructor			= 0x05,

					// Identifier (variable names, function names, etc)
					Identifier			= 0x06,

					// Literals
					IntLiteral			= 0x07,
					FloatLiteral		= 0x08,

					// Type keywords
					Int					= 0x09,
					Float				= 0x0A,
					Vec2				= 0x0B,
					Vec3				= 0x0C,
					Vec4				= 0x0D,
					IVec2				= 0x0E,
					IVec3				= 0x0F,
					IVec4				= 0x10,
					Mat2				= 0x11,
					Mat3				= 0x12,
					Mat4				= 0x13,

					// Operators
					Add					= 0x14,
					Sub					= 0x15,
					Mul					= 0x16,
					Div					= 0x17,
					Mod					= 0x18,
					Member				= 0x19,
					Assignment			= 0x1A,
					
					// Reserved keywords
					Return				= 0x1B,
					VertexOutput		= 0x1C,
				
					Count
				};

				/// <summary>
				///		Represents a node on the abstract syntax tree
				/// </summary>
				struct ASTNode
				{
					ASTNodeSymbol symbol;
					std::string attribute;
					ASTNode* firstChild;
					ASTNode* lastChild;
					ASTNode* next;
					ASTNode* parent;
				};

				/// <summary>
				///		Gets the type of a token symbol
				/// </summary>
				/// <param name="symbol">Token symbol</param>
				/// <returns>Token type</returns>
				TokenType GetTokenType(TokenSymbol symbol);

				class Compiler
				{
				public:
					Compiler(int major, int minor, int patch);

					void Load(const std::string& code);
					void Compile();
					
					void RunPreprocessor();
					void RunLexer();
					void RunParser();

					inline const std::string& GetOutput() const { return m_output; }

				private:
					int m_major, m_minor, m_patch;

					std::string m_code;
					std::string m_output;

					std::vector<Token> m_tokens;

					ASTNode m_astTree;
				};
			}
		}
	}
}