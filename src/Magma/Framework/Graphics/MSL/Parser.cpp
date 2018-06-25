#include "Compiler.hpp"

#include <sstream>
#include <iostream>

using namespace Magma::Framework::Graphics::MSL;

struct ParserInfo
{
	std::vector<Token>::const_iterator it;
	const std::vector<Token>& tokens;
	ASTNode* tree;
	Token lastToken;
};

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
	ss << "Unexpected token '" << (int)info.it->symbol << "' \"" << info.it->attribute << "\" on line " << info.it->line << std::endl;
	ss << "Expected token symbol '" << (int)symbol << "'" << std::endl;
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
	ss << "Unexpected token type '" << (int)info.it->type << "' \"" << info.it->attribute << "\" on line " << info.it->line << std::endl;
	ss << "Expected token type '" << (int)type << "'" << std::endl;
	ss << "Line: " << info.it->line;
	throw std::runtime_error(ss.str());
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
	while (AcceptType(TokenType::Type, info))
	{
		// Add declaration type
		auto declarationNode = AddToTree(TypeTokenToAST(info.lastToken.symbol), "", declarationsNode);

		Expect(TokenSymbol::Identifier, info);
		AddToTree(ASTNodeSymbol::Identifier, info.lastToken.attribute, declarationNode);

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
	while (AcceptType(TokenType::Type, info))
	{
		// Add declaration type
		auto declarationNode = AddToTree(TypeTokenToAST(info.lastToken.symbol), "", declarationsNode);

		Expect(TokenSymbol::Identifier, info);
		AddToTree(ASTNodeSymbol::Identifier, info.lastToken.attribute, declarationNode);

		Expect(TokenSymbol::Semicolon, info);
	}

	Expect(TokenSymbol::CloseBrace, info);

	return constantBufferNode;
}

ASTNode* Expression(ParserInfo& info);

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
			Expect(TokenSymbol::Identifier, info);
			AddToTree(CreateTree(ASTNodeSymbol::Identifier, info.lastToken.attribute), op);
			id1 = op;
		}
		else break;
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
		Accept(TokenSymbol::Sub, info))
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

	throw std::runtime_error("INVALID FACTOR");
}

ASTNode* Term(ParserInfo& info)
{
	// Get factor
	auto factor1 = Factor(info);
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

ASTNode* Assignment(ParserInfo& info)
{
	// Get term
	auto term1 = Term(info);
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

ASTNode* Expression(ParserInfo& info)
{
	// Get term
	auto term1 = Assignment(info);
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
	}
	// If declaration
	else if (AcceptType(TokenType::Type, info))
	{
		AddToTree(ASTNodeSymbol::Declaration, "", statementNode);
		AddToTree(TypeTokenToAST(info.lastToken.symbol), "", statementNode);
		// Get identifier
		Expect(TokenSymbol::Identifier, info);
		AddToTree(ASTNodeSymbol::Identifier, info.lastToken.attribute, statementNode);

		if (Accept(TokenSymbol::Assignment, info))
			AddToTree(Expression(info), statementNode);
	}
	// Else expression
	else
	{
		AddToTree(Expression(info), statementNode);
	}

	// End statement
	Expect(TokenSymbol::Semicolon, info);

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
	if (AcceptType(TokenType::Type, info))
	{
		while (true)
		{
			auto paramNode = AddToTree(TypeTokenToAST(info.lastToken.symbol), "", paramsNode);

			Expect(TokenSymbol::Identifier, info);
			AddToTree(ASTNodeSymbol::Identifier, info.lastToken.attribute, paramNode);

			if (!Accept(TokenSymbol::Comma, info))
				break;
			ExpectType(TokenType::Type, info);
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
	//PrintTree(m_astTree);
}
