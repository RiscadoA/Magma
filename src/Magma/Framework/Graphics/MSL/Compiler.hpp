#pragma once

#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <set>

/*

TO DO:

- Update token conversion funcs
- Add new tokens to lexer

- Add if, else and discard to parser
- Add not operator to parser
- Add conditional operators to parser

- Update GLSL compiler

- Create HLSL compiler

*/

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
					Invalid					= -1,

					// Punctuation
					OpenBrace				= 0x00,
					CloseBrace				= 0x01,
					OpenParenthesis			= 0x02,
					CloseParenthesis		= 0x03,
					Semicolon				= 0x04,
					Comma					= 0x05,

					// Identifier (variable names, function names, etc)
					Identifier				= 0x06,

					// Literals
					IntLiteral				= 0x07,
					FloatLiteral			= 0x08,

					// Type keywords
					Int						= 0x09,
					Float					= 0x0A,
					Vec2					= 0x0B,
					Vec3					= 0x0C,
					Vec4					= 0x0D,
					IVec2					= 0x0E,
					IVec3					= 0x0F,
					IVec4					= 0x10,
					Mat2					= 0x11,
					Mat3					= 0x12,
					Mat4					= 0x13,
					Bool					= 0x14,

					// Operators
					Add						= 0x15,
					Sub						= 0x16,
					Mul						= 0x17,
					Div						= 0x18,
					Mod						= 0x19,
					Member					= 0x1A,
					Assignment				= 0x1B,
					Not						= 0x1C,
					And						= 0x1D,
					Or						= 0x1E,

					// Reserved keywords
					Return					= 0x1F,
					VertexOutput			= 0x20,
					Texture2D				= 0x21,
					ConstantBuffer			= 0x22,
					If						= 0x23,
					Else					= 0x24,
					Discard					= 0x25,
					True					= 0x26,
					False					= 0x27,

					// Conditional operators
					EqualTo					= 0x28,
					NotEqualTo				= 0x29,
					LessThan				= 0x2A,
					GreaterThan				= 0x2B,
					LessThanOrEqualTo		= 0x2C,
					GreaterThanOrEqualTo	= 0x2D,

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
					ConditionalOperator = 0x06,

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
					Invalid					= -1,

					//	Objects
					Program					= 0x00,
					Params					= 0x01,
					Scope					= 0x02,
					Function				= 0x03,
					Statement				= 0x04,
					Constructor				= 0x05,
					Declaration				= 0x06,

					// Identifier (variable names, function names, etc)
					Identifier				= 0x07,

					// Literals
					IntLiteral				= 0x08,
					FloatLiteral			= 0x09,

					// Type keywords
					Int						= 0x0A,
					Float					= 0x0B,
					Vec2					= 0x0C,
					Vec3					= 0x0D,
					Vec4					= 0x0E,
					IVec2					= 0x0F,
					IVec3					= 0x10,
					IVec4					= 0x11,
					Mat2					= 0x12,
					Mat3					= 0x13,
					Mat4					= 0x14,
					Bool					= 0x15,

					// Operators
					Add						= 0x16,
					Sub						= 0x17,
					Mul						= 0x18,
					Div						= 0x19,
					Mod						= 0x1A,
					Member					= 0x1B,
					Assignment				= 0x1C,
					Not						= 0x1D,
					And						= 0x1E,
					Or						= 0x1F,
					
					// Reserved keywords
					Return					= 0x20,
					VertexOutput			= 0x21,
					Texture2D				= 0x22,
					ConstantBuffer			= 0x23,
					Call					= 0x24,
					If						= 0x25,
					Discard					= 0x26,
					True					= 0x27,
					False					= 0x28,

					// Conditional operators
					EqualTo					= 0x29,
					NotEqualTo				= 0x2A,
					LessThan				= 0x2B,
					GreaterThan				= 0x2C,
					LessThanOrEqualTo		= 0x2D,
					GreaterThanOrEqualTo	= 0x2E,

					Count
				};

				/// <summary>
				///		AST node types
				/// </summary>
				enum class ASTNodeType
				{
					Invalid				= -1,

					Object				= 0x00,
					Identifier			= 0x01,
					Literal				= 0x02,
					Type				= 0x03,
					Operator			= 0x04,
					Reserved			= 0x05,
					ConditionalOperator	= 0x06,

					Count
				};

				/// <summary>
				///		Represents a node on the abstract syntax tree
				/// </summary>
				struct ASTNode
				{
					ASTNodeSymbol symbol;
					ASTNodeType type;
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

				/// <summary>
				///		Gets the type of a AST node symbol
				/// </summary>
				/// <param name="symbol">AST node symbol</param>
				/// <returns>AST node type</returns>
				ASTNodeType GetNodeType(ASTNodeSymbol symbol);

				/// <summary>
				///		Creates an abstract syntax tree node
				/// </summary>
				/// <param name="symbol">Node symbol</param>
				/// <param name="attribute">Node attribute</param>
				/// <returns>Node pointer</returns>
				ASTNode* CreateTree(ASTNodeSymbol symbol, const std::string& attribute);

				/// <summary>
				///		Adds an abstract syntax tree node to a tree
				/// </summary>
				/// <param name="node">Mpde pointer</param>
				/// <param name="tree">Tree pointer</param>
				void AddToTree(ASTNode* node, ASTNode* tree);

				/// <summary>
				///		Creates and adds an abstract syntax tree node to a tree
				/// </summary>
				/// <param name="symbol">Node symbol</param>
				/// <param name="attribute">Node attribute</param>
				/// <param name="tree">Tree pointer</param>
				ASTNode* AddToTree(ASTNodeSymbol symbol, const std::string& attribute, ASTNode* tree);

				/// <summary>
				///		Destroys an abstract syntax tree
				/// </summary>
				/// <param name="node">Abstract syntax tree root node</param>
				void DestroyTree(ASTNode* node);

				/// <summary>
				///		Stores info about an identifier declaration
				/// </summary>
				struct Identifier
				{
					enum class Type
					{
						Invalid = -1,

						VertexOutput,
						Function,
						Parameter,
						Variable,

						Count
					} type;
					ASTNode* node;
				};

				/// <summary>
				///		Holds data about a variable declaration
				/// </summary>
				struct VariableDeclaration
				{
					ASTNodeSymbol type;
					std::string name;
				};

				/// <summary>
				///		Function declaration data
				/// </summary>
				struct FunctionDeclaration
				{
					ASTNodeSymbol returnType;
					std::vector<VariableDeclaration> params;
					ASTNode* scope;
				};

				/// <summary>
				///		Abstract class for compiling MSL
				/// </summary>
				class Compiler
				{
				public:
					Compiler(int major, int minor, int patch);
					virtual ~Compiler();

					void Load(const std::string& code);
					void Compile();
					
					void RunPreprocessor();
					void RunLexer();
					void RunParser();
					void PrintTree();
					void ExtractInfo();
					virtual void GenerateCode() = 0;

					inline const std::string& GetOutput() const { return m_output; }

				private:
					int m_major, m_minor, m_patch;

					std::string m_code;				
					std::vector<int> m_realCodeLines;
					std::vector<Token> m_tokens;

				protected:
					std::string m_output;
					std::string m_vertexOutID;
					std::set<std::string> m_2dtextures;
					std::unordered_map<std::string, std::vector<VariableDeclaration>> m_constantBuffers;
					std::vector<VariableDeclaration> m_vertexOut;
					std::unordered_map<std::string, FunctionDeclaration> m_functions;

					ASTNode* m_astTree;
				};
			}
		}
	}
}