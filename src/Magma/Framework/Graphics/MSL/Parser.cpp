#include "Compiler.hpp"

#include <sstream>
#include <iostream>

using namespace Magma::Framework::Graphics::MSL;

struct ParserInfo
{
	std::vector<Token>::const_iterator it;
	const std::vector<Token>& tokens;
	ASTNode& tree;
	ASTNode* node;
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
	ss << "Expected token symbol '" << (int)symbol << "'";
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
	ss << "Expected token type '" << (int)type << "'";
	throw std::runtime_error(ss.str());
}

ASTNode* CreateTree(ASTNodeSymbol symbol, const std::string& attribute)
{
	auto node = new ASTNode();
	node->symbol = symbol;
	node->attribute = attribute;
	node->parent = nullptr;
	node->firstChild = nullptr;
	node->lastChild = nullptr;
	node->next = nullptr;
	return node;
}

void AddToTree(ASTNode* node, ASTNode* tree)
{
	if (tree->lastChild == nullptr)
		tree->firstChild = tree->lastChild = node;
	else
	{
		tree->lastChild->next = node;
		tree->lastChild = node;
	}
}

ASTNode* AddToTree(ASTNodeSymbol symbol, const std::string& attribute, ASTNode* tree)
{
	auto node = new ASTNode();
	node->symbol = symbol;
	node->attribute = attribute;
	node->firstChild = nullptr;
	node->lastChild = nullptr;
	node->next = nullptr;
	AddToTree(node, tree);
	return node;
}

void DestroyTree(ASTNode* node)
{
	ASTNode* c = node->firstChild;
	while (c != nullptr)
	{
		DestroyTree(c);
		c = c->next;
	}

	delete node;
}

void VertexOutput(ParserInfo& info)
{
	/*

	AddNode(ASTNodeSymbol::VertexOutput, "", info);

	// Get vertex output identifier
	Expect(TokenSymbol::Identifier, info);
	AddTerminalNode(ASTNodeSymbol::Identifier, info.lastToken.attribute, info);

	Expect(TokenSymbol::OpenBrace, info);

	// Get declarations
	while (AcceptType(TokenType::Type, info))
	{
		AddNode(TypeTokenToAST(info.lastToken.symbol), "", info);

		Expect(TokenSymbol::Identifier, info);
		AddTerminalNode(ASTNodeSymbol::Identifier, info.lastToken.attribute, info);

		Expect(TokenSymbol::Semicolon, info);

		UpNode(info);
	}

	Expect(TokenSymbol::CloseBrace, info);
	UpNode(info);

	*/
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


// position.x.y
// (position).x).y

ASTNode* Identifier(ParserInfo& info)
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
			AddToTree(Identifier(info), op);
			id1 = op;
		}
		else break;
	}

	return id1;
}

ASTNode* Factor(ParserInfo& info)
{
	// ( <exp> )
	if (Accept(TokenSymbol::OpenParenthesis, info))
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

	// <identifier>
	else if (Peek(info) == TokenSymbol::Identifier)
		return Identifier(info); // Get identifier
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

ASTNode* Assignemnt(ParserInfo& info)
{
	// TO DO
	return nullptr;
}

ASTNode* Expression(ParserInfo& info)
{
	// Get term
	auto term1 = Term(info);
	ASTNode* term2 = nullptr;

	// 2 + 4 + 7 + 9
	// (((2 + 4) + 7) + 9)
	
	std::vector<Token> literals;


	int levels = 0;
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
			levels++;

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

void Statement(ParserInfo& info)
{
	// Start statement
	/*AddNode(ASTNodeSymbol::Statement, "", info);

	// If return statement
	if (Accept(TokenSymbol::Return, info))
	{
		AddTerminalNode(ASTNodeSymbol::Return, "", info);

		// Expression
		Expression(info);
	}

	// End statement
	Expect(TokenSymbol::Semicolon, info);
	UpNode(info);*/
}

void Scope(ParserInfo& info)
{
	// Open scope
	/*Expect(TokenSymbol::OpenBrace, info);
	AddNode(ASTNodeSymbol::Scope, "", info);

	// Get statements
	while (true)
	{
		// Found another scope
		if (Peek(info) ==  TokenSymbol::OpenBrace)
		{
			Scope(info);
			continue;
		}
		// Scope end
		if (Peek(info) == TokenSymbol::CloseBrace)
			break;
		// Statement
		Statement(info);
	}

	// Close scope
	Expect(TokenSymbol::CloseBrace, info);
	UpNode(info);

	*/
}

void Function(ASTNodeSymbol type, ParserInfo& info)
{
	/*

	// Add function type
	AddNode(ASTNodeSymbol::Function, "", info);
	AddTerminalNode(type, "", info);

	// Get and add function identifier
	Expect(TokenSymbol::Identifier, info);
	AddTerminalNode(ASTNodeSymbol::Identifier, info.lastToken.attribute, info);

	// Open params
	Expect(TokenSymbol::OpenParenthesis, info);
	AddNode(ASTNodeSymbol::Params, "", info);

	// Get params
	if (AcceptType(TokenType::Type, info))
	{
		while (true)
		{
			AddNode(TypeTokenToAST(info.lastToken.symbol), "", info);

			Expect(TokenSymbol::Identifier, info);
			AddTerminalNode(ASTNodeSymbol::Identifier, info.lastToken.attribute, info);

			UpNode(info);
			if (!Accept(TokenSymbol::Comma, info))
				break;
			ExpectType(TokenType::Type, info);
		}
	}

	// Exit params
	Expect(TokenSymbol::CloseParenthesis, info);
	UpNode(info);

	// Scope
	Scope(info);

	// Get out of function
	UpNode(info);
	*/
}

void Program(ParserInfo& info)
{
	bool newStuff = true;
	while (newStuff)
	{
		newStuff = false;

		if (Ended(info))
			break;

		// Vertex output data struct
		if (Accept(TokenSymbol::VertexOutput, info))
		{
			VertexOutput(info);
			newStuff = true;
		}
		// Function
		else if (AcceptType(TokenType::Type, info))
		{
			Function(TypeTokenToAST(info.lastToken.symbol), info);
			newStuff = true;
		}
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

void Magma::Framework::Graphics::MSL::Compiler::RunParser()
{
	m_astTree.symbol = ASTNodeSymbol::Program;
	m_astTree.attribute = "";
	m_astTree.parent = nullptr;
	m_astTree.next = nullptr;
	m_astTree.firstChild = nullptr;
	m_astTree.lastChild = nullptr;
	ParserInfo info = { m_tokens.begin(), m_tokens, m_astTree, &m_astTree, {} };

	//Program(info);
	AddToTree(Expression(info), &m_astTree);

	PrintTree(&m_astTree);
	getchar();
}
