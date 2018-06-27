#include "Compiler.hpp"

#include <sstream>
#include <iostream>

/*

Tree generated:

2D Textures:

	Texture2D
		Identifier

Constant Buffers:

	ConstantBuffer
		Identifier
		Scope
			Type
				Identifier
			...

Vertex Output:

	VertexOutput
		Identifier
		Scope
			Type
				Identifier
			...

Functions:

	Function
		Return Type
		Identifier
		Params
			Type
				Identifier
			...
		Scope
			Statement
			Statement
			...

Statements:
	Statement
		Statement Type
		... (params)

	Statement
		Return
		Expression

	Statement
		Discard
*/

using namespace Magma::Framework::Graphics::MSL;

struct ParserInfo
{
	std::vector<Token>::const_iterator it;
	const std::vector<Token>& tokens;
	ASTNode* tree;
	Token lastToken;
};

std::string TokenSymbolToString(TokenSymbol token)
{
	switch (token)
	{
		case TokenSymbol::Add: return "+";
		case TokenSymbol::Sub: return "-";
		case TokenSymbol::Mul: return "*";
		case TokenSymbol::Div: return "/";
		case TokenSymbol::Mod: return "%";
		case TokenSymbol::And: return "&&";
		case TokenSymbol::Or: return "||";
		case TokenSymbol::Member: return ".";
		case TokenSymbol::Assignment: return "=";
		case TokenSymbol::Not: return "!";
		case TokenSymbol::Int: return "int";
		case TokenSymbol::Float: return "float";
		case TokenSymbol::Vec2: return "vec2";
		case TokenSymbol::Vec3: return "vec3";
		case TokenSymbol::Vec4: return "vec4";
		case TokenSymbol::IVec2: return "ivec2";
		case TokenSymbol::IVec3: return "ivec3";
		case TokenSymbol::IVec4: return "ivec4";
		case TokenSymbol::Mat2: return "mat2";
		case TokenSymbol::Mat3: return "mat3";
		case TokenSymbol::Mat4: return "mat4";
		case TokenSymbol::Bool: return "bool";
		case TokenSymbol::IntLiteral: return "int literal";
		case TokenSymbol::FloatLiteral: return "float literal";
		case TokenSymbol::Identifier: return "identifier";
		case TokenSymbol::OpenBrace: return "{";
		case TokenSymbol::CloseBrace: return "}";
		case TokenSymbol::OpenParenthesis: return "(";
		case TokenSymbol::CloseParenthesis: return ")";
		case TokenSymbol::OpenBrackets: return "[";
		case TokenSymbol::CloseBrackets: return "]";
		case TokenSymbol::Semicolon: return ";";
		case TokenSymbol::Comma: return ",";	
		case TokenSymbol::Return: return "return";
		case TokenSymbol::VertexOutput: return "VertexOutput";
		case TokenSymbol::Texture2D: return "Texture2D";
		case TokenSymbol::ConstantBuffer: return "ConstantBuffer";
		case TokenSymbol::If: return "if";
		case TokenSymbol::Else: return "else";
		case TokenSymbol::Discard: return "discard";
		case TokenSymbol::True: return "true";
		case TokenSymbol::False: return "false";
		case TokenSymbol::While: return "while";
		case TokenSymbol::Do: return "do";
		case TokenSymbol::For: return "for";

		case TokenSymbol::Invalid: return "???INVALID TOKEN SYMBOL???";
		default: return "???UNKNOWN TOKEN SYMBOL???";
	}
}

std::string TokenTypeToString(TokenType type)
{
	switch (type)
	{
		case TokenType::Punctuation: return "punctuation";
		case TokenType::Identifier: return "identifier";
		case TokenType::Literal: return "literal";
		case TokenType::Type: return "type";
		case TokenType::Operator: return "operator";
		case TokenType::Reserved: return "reserved";
		case TokenType::ConditionalOperator: return "conditional operator";
		case TokenType::Invalid: return "???INVALID TOKEN TYPE???";
		default: return "???UNKNOWN TOKEN TYPE???";
	}
}

ASTNodeSymbol TypeTokenToAST(TokenSymbol symbol)
{
	switch (symbol)
	{
		case TokenSymbol::Int: return ASTNodeSymbol::Int;
		case TokenSymbol::Float: return ASTNodeSymbol::Float;
		case TokenSymbol::Vec2: return ASTNodeSymbol::Vec2;
		case TokenSymbol::Vec3: return ASTNodeSymbol::Vec3;
		case TokenSymbol::Vec4: return ASTNodeSymbol::Vec4;
		case TokenSymbol::IVec2: return ASTNodeSymbol::IVec2;
		case TokenSymbol::IVec3: return ASTNodeSymbol::IVec3;
		case TokenSymbol::IVec4: return ASTNodeSymbol::IVec4;
		case TokenSymbol::Mat2: return ASTNodeSymbol::Mat2;
		case TokenSymbol::Mat3: return ASTNodeSymbol::Mat3;
		case TokenSymbol::Mat4: return ASTNodeSymbol::Mat4;
		case TokenSymbol::Bool: return ASTNodeSymbol::Bool;

		default:
		{
			std::stringstream ss;
			ss << "Failed to compile MSL code:" << std::endl;
			ss << "Parser stage failed:" << std::endl;
			ss << "Failed to convert type token symbol into AST node symbol" << std::endl;
			ss << "Token is not a type";
			throw std::runtime_error(ss.str());
		}
	}
}

ASTNodeSymbol OperatorTokenToAST(TokenSymbol symbol)
{
	switch (symbol)
	{
		case TokenSymbol::Add: return ASTNodeSymbol::Add;
		case TokenSymbol::Sub: return ASTNodeSymbol::Sub;
		case TokenSymbol::Mul: return ASTNodeSymbol::Mul;
		case TokenSymbol::Div: return ASTNodeSymbol::Div;
		case TokenSymbol::Mod: return ASTNodeSymbol::Mod;
		case TokenSymbol::Member: return ASTNodeSymbol::Member;
		case TokenSymbol::Assignment: return ASTNodeSymbol::Assignment;
		case TokenSymbol::Not: return ASTNodeSymbol::Not;
		case TokenSymbol::And: return ASTNodeSymbol::And;
		case TokenSymbol::Or: return ASTNodeSymbol::Or;

		case TokenSymbol::EqualTo: return ASTNodeSymbol::EqualTo;
		case TokenSymbol::NotEqualTo: return ASTNodeSymbol::NotEqualTo;
		case TokenSymbol::GreaterThan: return ASTNodeSymbol::GreaterThan;
		case TokenSymbol::LessThan: return ASTNodeSymbol::LessThan;
		case TokenSymbol::GreaterThanOrEqualTo: return ASTNodeSymbol::GreaterThanOrEqualTo;
		case TokenSymbol::LessThanOrEqualTo: return ASTNodeSymbol::LessThanOrEqualTo;

		default:
		{
			std::stringstream ss;
			ss << "Failed to compile MSL code:" << std::endl;
			ss << "Parser stage failed:" << std::endl;
			ss << "Failed to convert operator token symbol into AST node symbol" << std::endl;
			ss << "Token is not an operator";
			throw std::runtime_error(ss.str());
		}
	}
}

ASTNodeSymbol LiteralTokenToAST(TokenSymbol symbol)
{
	switch (symbol)
	{
		case TokenSymbol::IntLiteral: return ASTNodeSymbol::IntLiteral;
		case TokenSymbol::FloatLiteral: return ASTNodeSymbol::FloatLiteral;
		case TokenSymbol::True: return ASTNodeSymbol::True;
		case TokenSymbol::False: return ASTNodeSymbol::False;

		default:
		{
			std::stringstream ss;
			ss << "Failed to compile MSL code:" << std::endl;
			ss << "Parser stage failed:" << std::endl;
			ss << "Failed to convert literal token symbol into AST node symbol" << std::endl;
			ss << "Token is not a literal";
			throw std::runtime_error(ss.str());
		}
	}
}

TokenSymbol Peek(ParserInfo& info)
{
	info.lastToken = *info.it;
	return info.it->symbol;
}

TokenType PeekType(ParserInfo& info)
{
	info.lastToken = *info.it;
	return info.it->type;
}

bool Ended(ParserInfo& info)
{
	if (info.it == info.tokens.end())
		return true;
	return false;
}

bool Accept(TokenSymbol symbol, ParserInfo& info)
{
	if (info.it == info.tokens.end())
		return false;
	if (info.it->symbol == symbol)
	{
		info.lastToken = *info.it;
		++info.it;
		return true;
	}
	else return false;
}

bool AcceptType(TokenType type, ParserInfo& info)
{
	if (info.it == info.tokens.end())
		return false;
	if (info.it->type == type)
	{
		info.lastToken = *info.it;
		++info.it;
		return true;
	}
	else return false;
}

void Expect(TokenSymbol symbol, ParserInfo& info)
{
	if (Accept(symbol, info))
		return;
	std::stringstream ss;
	ss << "Failed to compile MSL code:" << std::endl;
	ss << "Parser stage failed:" << std::endl;
	ss << "Unexpected token '" << TokenSymbolToString(info.it->symbol) << "' \"" << info.it->attribute << "\" on line " << info.it->line << std::endl;
	ss << "Expected token symbol '" << TokenSymbolToString(symbol) << "'" << std::endl;
	ss << "Line: " << info.it->line;
	throw std::runtime_error(ss.str());
}

void ExpectType(TokenType type, ParserInfo& info)
{
	if (AcceptType(type, info))
		return;
	std::stringstream ss;
	ss << "Failed to compile MSL code:" << std::endl;
	ss << "Parser stage failed:" << std::endl;
	ss << "Unexpected token type '" << TokenTypeToString(info.it->type) << "' \"" << info.it->attribute << "\" on line " << info.it->line << std::endl;
	ss << "Expected token type '" << TokenTypeToString(type) << "'" << std::endl;
	ss << "Line: " << info.it->line;
	throw std::runtime_error(ss.str());
}

ASTNode* Expression(ParserInfo& info);
ASTNode* Scope(ParserInfo& info);

ASTNode* Declaration(ParserInfo& info)
{
	/*
		Type
			Identifier
		
		Array
			Identifier
			Type
			Int Literal (array size)
	*/

	// Add declaration type
	ExpectType(TokenType::Type, info);
	auto declarationNode = CreateTree(TypeTokenToAST(info.lastToken.symbol), "");

	Expect(TokenSymbol::Identifier, info);
	AddToTree(ASTNodeSymbol::Identifier, info.lastToken.attribute, declarationNode);

	// If array
	if (Accept(TokenSymbol::OpenBrackets, info))
	{
		AddToTree(declarationNode->symbol, "", declarationNode);
		declarationNode->symbol = ASTNodeSymbol::Array;
		
		Expect(TokenSymbol::IntLiteral, info);
		AddToTree(ASTNodeSymbol::IntLiteral, info.lastToken.attribute, declarationNode);

		Expect(TokenSymbol::CloseBrackets, info);
	}

	return declarationNode;
}

ASTNode* VertexOutput(ParserInfo& info)
{
	auto vertexOutputNode = CreateTree(ASTNodeSymbol::VertexOutput, "");

	// Get vertex output identifier
	Expect(TokenSymbol::Identifier, info);
	AddToTree(ASTNodeSymbol::Identifier, info.lastToken.attribute, vertexOutputNode);

	Expect(TokenSymbol::OpenBrace, info);

	// Get declarations
	auto declarationsNode = AddToTree(ASTNodeSymbol::Scope, "", vertexOutputNode);
	while (Peek(info) != TokenSymbol::CloseBrace)
	{
		AddToTree(Declaration(info), declarationsNode);
		Expect(TokenSymbol::Semicolon, info);
	}

	Expect(TokenSymbol::CloseBrace, info);
	
	return vertexOutputNode;
}

ASTNode* Texture2D(ParserInfo& info)
{
	auto textureNode = CreateTree(ASTNodeSymbol::Texture2D, "");

	// Get vertex output identifier
	Expect(TokenSymbol::Identifier, info);
	AddToTree(ASTNodeSymbol::Identifier, info.lastToken.attribute, textureNode);
	Expect(TokenSymbol::Semicolon, info);

	return textureNode;
}

ASTNode* ConstantBuffer(ParserInfo& info)
{
	auto constantBufferNode = CreateTree(ASTNodeSymbol::ConstantBuffer, "");

	// Get vertex output identifier
	Expect(TokenSymbol::Identifier, info);
	AddToTree(ASTNodeSymbol::Identifier, info.lastToken.attribute, constantBufferNode);

	Expect(TokenSymbol::OpenBrace, info);

	// Get declarations
	auto declarationsNode = AddToTree(ASTNodeSymbol::Scope, "", constantBufferNode);
	while (Peek(info) != TokenSymbol::CloseBrace)
	{
		AddToTree(Declaration(info), declarationsNode);
		Expect(TokenSymbol::Semicolon, info);
	}

	Expect(TokenSymbol::CloseBrace, info);

	return constantBufferNode;
}

ASTNode* Constructor(ASTNodeSymbol type, ParserInfo& info)
{
	// Start constructor
	auto constructorNode = CreateTree(ASTNodeSymbol::Constructor, "");

	// Add constructor type
	AddToTree(type, "", constructorNode);

	// Get params
	auto paramsNode = CreateTree(ASTNodeSymbol::Params, "");

	Expect(TokenSymbol::OpenParenthesis, info);
	if (Peek(info) != TokenSymbol::CloseParenthesis)
	{
		while (true)
		{
			// Add expressions as arguments
			AddToTree(Expression(info), paramsNode);

			// Stop getting arguments if next token isn't a comma
			if (!Accept(TokenSymbol::Comma, info))
				break;
		}
	}

	AddToTree(paramsNode, constructorNode);

	Expect(TokenSymbol::CloseParenthesis, info);

	return constructorNode;
}

ASTNode* IdentifierFunc(ParserInfo& info)
{
	// Get identifier
	Expect(TokenSymbol::Identifier, info);
	auto id1 = CreateTree(ASTNodeSymbol::Identifier, info.lastToken.attribute);

	// Function call
	if (Accept(TokenSymbol::OpenParenthesis, info))
	{
		id1->symbol = ASTNodeSymbol::Call;
		AddToTree(ASTNodeSymbol::Identifier, id1->attribute, id1);
		id1->attribute = "";

		auto paramsNode = CreateTree(ASTNodeSymbol::Params, "");

		if (Peek(info) != TokenSymbol::CloseParenthesis)
		{
			while (true)
			{
				// Add expressions as arguments
				AddToTree(Expression(info), paramsNode);

				// Stop getting arguments if next token isn't a comma
				if (!Accept(TokenSymbol::Comma, info))
					break;
			}
		}

		Expect(TokenSymbol::CloseParenthesis, info);

		AddToTree(paramsNode, id1);
	}
	// Array access
	else if (Accept(TokenSymbol::OpenBrackets, info))
	{
		// Add array identifier
		id1->symbol = ASTNodeSymbol::ArrayAccess;
		AddToTree(ASTNodeSymbol::Identifier, id1->attribute, id1);
		id1->attribute = "";

		// Add index expression
		AddToTree(Expression(info), id1);
		Expect(TokenSymbol::CloseBrackets, info);
	}
	// Simple identifier
	else
	{
		// While there are member access operations
		while (true)
		{
			if (Accept(TokenSymbol::Member, info))
			{
				// Create member operator
				auto op = CreateTree(ASTNodeSymbol::Member, "");

				// Get second term
				// Add identifiers to member operator
				AddToTree(id1, op);
				//Expect(TokenSymbol::Identifier, info);
				//AddToTree(CreateTree(ASTNodeSymbol::Identifier, info.lastToken.attribute), op);
				AddToTree(IdentifierFunc(info), op);
				id1 = op;
			}
			else break;
		}
	}

	return id1;
}

ASTNode* Factor(ParserInfo& info)
{
	// <identifier>
	if (Peek(info) == TokenSymbol::Identifier)
		return IdentifierFunc(info); // Get identifier

	// ( <exp> )
	else if (Accept(TokenSymbol::OpenParenthesis, info))
	{
		auto ret = Expression(info);
		Expect(TokenSymbol::CloseParenthesis, info);
		return ret;
	}

	// <unary_op> <factor>
	else if (Accept(TokenSymbol::Add, info) ||
			 Accept(TokenSymbol::Sub, info) ||
			 Accept(TokenSymbol::Not, info))
	{
		auto op = CreateTree(OperatorTokenToAST(info.lastToken.symbol), "");
		auto f = Factor(info);
		AddToTree(f, op);
		return op;
	}

	// <constructor>
	else if (AcceptType(TokenType::Type, info))
		return Constructor(TypeTokenToAST(info.lastToken.symbol), info);

	// <literal>
	else if (AcceptType(TokenType::Literal, info))
		return CreateTree(LiteralTokenToAST(info.lastToken.symbol), info.lastToken.attribute);

	return nullptr;
}

ASTNode* Term(ParserInfo& info)
{
	// Get factor
	auto factor1 = Factor(info);
	if (factor1 == nullptr)
		return nullptr;
	ASTNode* factor2 = nullptr;

	// While there are multiplication, division and module operations
	while (true)
	{
		if (Accept(TokenSymbol::Mul, info) ||
			Accept(TokenSymbol::Div, info) ||
			Accept(TokenSymbol::Mod, info))
		{
			// Create operator
			auto op = CreateTree(OperatorTokenToAST(info.lastToken.symbol), "");

			// Add first and second factor to operator
			AddToTree(factor1, op);
			AddToTree(Factor(info), op);
			factor1 = op;
		}
		else break;
	}

	return factor1;
}

ASTNode* Condition(ParserInfo& info)
{
	// Get term
	auto term1 = Term(info);
	if (term1 == nullptr)
		return nullptr;
	ASTNode* term2 = nullptr;

	// While there are addition and subtraction operations
	while (true)
	{
		if (Accept(TokenSymbol::Add, info) ||
			Accept(TokenSymbol::Sub, info))
		{
			// Create operator
			auto op = CreateTree(OperatorTokenToAST(info.lastToken.symbol), "");

			// Get second term
			term2 = Term(info);

			// Add terms to operator
			AddToTree(term1, op);
			AddToTree(term2, op);
			term1 = op;
		}
		else break;
	}

	// Return tree
	return term1;
}

ASTNode* Logic(ParserInfo& info)
{
	// Get term
	auto term1 = Condition(info);
	if (term1 == nullptr)
		return nullptr;
	ASTNode* term2 = nullptr;

	// While there are conditional operations
	while (true)
	{
		if (Accept(TokenSymbol::EqualTo, info) ||
			Accept(TokenSymbol::NotEqualTo, info) ||
			Accept(TokenSymbol::LessThan, info) ||
			Accept(TokenSymbol::GreaterThan, info) ||
			Accept(TokenSymbol::LessThanOrEqualTo, info) ||
			Accept(TokenSymbol::GreaterThanOrEqualTo, info))
		{
			// Create operator
			auto op = CreateTree(OperatorTokenToAST(info.lastToken.symbol), "");

			// Get second term
			term2 = Condition(info);

			// Add terms to operator
			AddToTree(term1, op);
			AddToTree(term2, op);
			term1 = op;
		}
		else break;
	}

	// Return tree
	return term1;
}

ASTNode* Assignment(ParserInfo& info)
{
	// Get term
	auto term1 = Logic(info);
	if (term1 == nullptr)
		return nullptr;
	ASTNode* term2 = nullptr;

	// While there are 'and' and 'or' operations
	while (true)
	{
		if (Accept(TokenSymbol::And, info) ||
			Accept(TokenSymbol::Or, info))
		{
			// Create operator
			auto op = CreateTree(OperatorTokenToAST(info.lastToken.symbol), "");

			// Get second term
			term2 = Logic(info);

			// Add terms to operator
			AddToTree(term1, op);
			AddToTree(term2, op);
			term1 = op;
		}
		else break;
	}

	// Return tree
	return term1;
}

ASTNode* Expression(ParserInfo& info)
{
	// Get term
	auto term1 = Assignment(info);
	if (term1 == nullptr)
	{
		std::stringstream ss;
		ss << "Failed to compile MSL code:" << std::endl;
		ss << "Parser stage failed:" << std::endl;
		ss << "Couldn't parse expression, unexpected token '" << TokenSymbolToString(Peek(info)) << "':" << std::endl;
		ss << "Line: " << info.it->line;
		throw std::runtime_error(ss.str());
	}
	ASTNode* term2 = nullptr;

	// While there are addition and subtraction operations
	while (true)
	{
		if (Accept(TokenSymbol::Assignment, info))
		{
			// Create operator
			auto op = CreateTree(ASTNodeSymbol::Assignment, "");

			// Get second term
			term2 = Assignment(info);

			// Add terms to operator
			AddToTree(term1, op);
			AddToTree(term2, op);
			term1 = op;
		}
		else break;
	}

	// Return tree
	return term1;
}

void AddDeclaration(ASTNode* statementNode, ParserInfo& info)
{
	ExpectType(TokenType::Type, info);
	AddToTree(ASTNodeSymbol::Declaration, "", statementNode);
	auto typeNode = AddToTree(TypeTokenToAST(info.lastToken.symbol), "", statementNode);
	// Get identifier
	Expect(TokenSymbol::Identifier, info);
	AddToTree(ASTNodeSymbol::Identifier, info.lastToken.attribute, statementNode);

	// If has initialization
	if (Accept(TokenSymbol::Assignment, info))
		AddToTree(Expression(info), statementNode);
	// Else if array
	else if (Accept(TokenSymbol::OpenBrackets, info))
	{
		AddToTree(typeNode->symbol, "", typeNode);
		typeNode->symbol = ASTNodeSymbol::Array;
		Expect(TokenSymbol::IntLiteral, info);
		AddToTree(ASTNodeSymbol::IntLiteral, info.lastToken.attribute, typeNode);
		Expect(TokenSymbol::CloseBrackets, info);
	}
	// End statement
	Expect(TokenSymbol::Semicolon, info);
}

ASTNode* Statement(ParserInfo& info)
{
	// Start statement
	auto statementNode = CreateTree(ASTNodeSymbol::Statement, "");

	// If return statement
	if (Accept(TokenSymbol::Return, info))
	{
		AddToTree(ASTNodeSymbol::Return, "", statementNode);
		// Expression
		AddToTree(Expression(info), statementNode);
		// End statement
		Expect(TokenSymbol::Semicolon, info);
	}
	// If Discard statement
	else if (Accept(TokenSymbol::Discard, info))
	{
		AddToTree(ASTNodeSymbol::Discard, "", statementNode);
		// End statement
		Expect(TokenSymbol::Semicolon, info);
	}
	// If declaration
	else if (PeekType(info) == TokenType::Type)
		AddDeclaration(statementNode, info);
	// If "if" statement
	else if (Accept(TokenSymbol::If, info))
	{
		AddToTree(ASTNodeSymbol::If, "", statementNode);
		
		// Add condition
		Expect(TokenSymbol::OpenParenthesis, info);
		AddToTree(Expression(info), statementNode);
		Expect(TokenSymbol::CloseParenthesis, info);

		// Add if body
		if (Peek(info) == TokenSymbol::OpenBrace)
			AddToTree(Scope(info), statementNode);
		else
			AddToTree(Statement(info), statementNode);

		// Add else body
		if (Accept(TokenSymbol::Else, info))
		{
			if (Peek(info) == TokenSymbol::OpenBrace)
				AddToTree(Scope(info), statementNode);
			else
				AddToTree(Statement(info), statementNode);
		}

		/*
			Statement
				If
				Expression
				If Body
				Else Body
		*/
	}
	else if (Accept(TokenSymbol::While, info))
	{
		AddToTree(ASTNodeSymbol::While, "", statementNode);

		// Add condition
		Expect(TokenSymbol::OpenParenthesis, info);
		AddToTree(Expression(info), statementNode);
		Expect(TokenSymbol::CloseParenthesis, info);

		// Add body
		if (Peek(info) == TokenSymbol::OpenBrace)
			AddToTree(Scope(info), statementNode);
		else
			AddToTree(Statement(info), statementNode);

		/*
			Statement
				While
				Expression
				Body
		*/
	}
	else if (Accept(TokenSymbol::Do, info))
	{
		AddToTree(ASTNodeSymbol::Do, "", statementNode);

		// Add body
		if (Peek(info) == TokenSymbol::OpenBrace)
			AddToTree(Scope(info), statementNode);
		else
			AddToTree(Statement(info), statementNode);

		// Add condition
		Expect(TokenSymbol::While, info);
		Expect(TokenSymbol::OpenParenthesis, info);
		AddToTree(Expression(info), statementNode);
		Expect(TokenSymbol::CloseParenthesis, info);
		Expect(TokenSymbol::Semicolon, info);

		/*
		Statement
			Do
			Body
			Expression
		*/
	}
	else if (Accept(TokenSymbol::For, info))
	{
		AddToTree(ASTNodeSymbol::For, "", statementNode);
		Expect(TokenSymbol::OpenParenthesis, info);

		// Add declaration
		auto declarationNode = AddToTree(ASTNodeSymbol::Statement, "", statementNode);
		AddDeclaration(declarationNode, info);

		// Add condition expression
		AddToTree(Expression(info), statementNode);
		
		// Add expression
		Expect(TokenSymbol::Semicolon, info);
		AddToTree(Expression(info), statementNode);

		Expect(TokenSymbol::CloseParenthesis, info);

		// Add body
		if (Peek(info) == TokenSymbol::OpenBrace)
			AddToTree(Scope(info), statementNode);
		else
			AddToTree(Statement(info), statementNode);

		/*
		Statement
			For
			Declaration Statement
			Expression
			Expression
			Body
		*/
	}
	// If empty statement
	else if(Accept(TokenSymbol::Semicolon, info))
		;
	// Else expression
	else
	{
		AddToTree(Expression(info), statementNode);
		// End statement
		Expect(TokenSymbol::Semicolon, info);
	}

	return statementNode;
}

ASTNode* Scope(ParserInfo& info)
{
	// Open scope
	Expect(TokenSymbol::OpenBrace, info);
	auto scopeNode = CreateTree(ASTNodeSymbol::Scope, "");

	// Get statements
	while (true)
	{
		// Found another scope
		if (Peek(info) ==  TokenSymbol::OpenBrace)
		{
			AddToTree(Scope(info), scopeNode);
			continue;
		}
		// Scope end
		if (Peek(info) == TokenSymbol::CloseBrace)
			break;
		// Statement
		AddToTree(Statement(info), scopeNode);
	}

	// Close scope
	Expect(TokenSymbol::CloseBrace, info);
	return scopeNode;
}

ASTNode* Function(ASTNodeSymbol type, ParserInfo& info)
{
	// Create function
	auto functionNode = CreateTree(ASTNodeSymbol::Function, "");
	
	// Add type
	AddToTree(type, "", functionNode);

	// Get and add function identifier
	Expect(TokenSymbol::Identifier, info);
	AddToTree(ASTNodeSymbol::Identifier, info.lastToken.attribute, functionNode);

	// Open params
	Expect(TokenSymbol::OpenParenthesis, info);
	auto paramsNode = AddToTree(ASTNodeSymbol::Params, "", functionNode);

	// Get params
	if (PeekType(info) == TokenType::Type)
	{
		while (true)
		{
			AddToTree(Declaration(info), paramsNode);

			if (!Accept(TokenSymbol::Comma, info))
				break;
		}
	}

	// Exit params
	Expect(TokenSymbol::CloseParenthesis, info);

	// Scope
	AddToTree(Scope(info), functionNode);

	return functionNode;
}

void Program(ParserInfo& info)
{
	while (true)
	{
		if (Ended(info))
			break;

		// Texture 2D
		if (Accept(TokenSymbol::Texture2D, info))
		{
			auto node = Texture2D(info);
			AddToTree(node, info.tree);
		}
		// Vertex output data struct
		else if (Accept(TokenSymbol::ConstantBuffer, info))
		{
			auto node = ConstantBuffer(info);
			AddToTree(node, info.tree);
		}
		// Vertex output data struct
		else if (Accept(TokenSymbol::VertexOutput, info))
		{
			auto node = VertexOutput(info);
			AddToTree(node, info.tree);
		}
		// Function
		else if (AcceptType(TokenType::Type, info))
		{
			auto node = Function(TypeTokenToAST(info.lastToken.symbol), info);
			AddToTree(node, info.tree);
		}
		// No more stuff to parse, program ended
		else break;
	}
}

void PrintTree(ASTNode* node, int depth = 0)
{
	for (int i = 0; i < depth; ++i)
		std::cout << "  ";
	std::cout << "0x" << std::hex << (int)node->symbol << " \"" << node->attribute << "\"" << std::endl;
	ASTNode* c = node->firstChild;
	while (c != nullptr)
	{
		PrintTree(c, depth + 1);
		c = c->next;
	}
}

void Magma::Framework::Graphics::MSL::Compiler::PrintTree()
{
	::PrintTree(m_astTree);
}

void FixTree(ASTNode* n)
{
	n->type = GetNodeType(n->symbol);
	ASTNode* c = n->firstChild;
	while (c != nullptr)
	{
		FixTree(c);
		c->parent = n;
		c = c->next;
	}
}

void Magma::Framework::Graphics::MSL::Compiler::RunParser()
{
	if (m_astTree != nullptr)
		DestroyTree(m_astTree);
	m_astTree = CreateTree(ASTNodeSymbol::Program, "");

	ParserInfo info = { m_tokens.begin(), m_tokens, m_astTree, {} };
	Program(info);
	FixTree(m_astTree);
}
