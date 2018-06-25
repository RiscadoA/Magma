#include "Compiler.hpp"

using namespace Magma::Framework::Graphics::MSL;

Magma::Framework::Graphics::MSL::Compiler::Compiler(int major, int minor, int patch)
{
	m_major = major;
	m_minor = minor;
	m_patch = patch;

	m_astTree = nullptr;
}

Magma::Framework::Graphics::MSL::Compiler::~Compiler()
{
	if (m_astTree != nullptr)
		DestroyTree(m_astTree);
}

void Magma::Framework::Graphics::MSL::Compiler::Load(const std::string & code)
{
	m_code = code;
	m_output = "";
}

void Magma::Framework::Graphics::MSL::Compiler::Compile()
{
	this->RunPreprocessor();
	this->RunLexer();
	this->RunParser();
	this->ExtractInfo();
	this->GenerateCode();
}

TokenType Magma::Framework::Graphics::MSL::GetTokenType(TokenSymbol symbol)
{
	switch (symbol)
	{
		// Punctuation
		case TokenSymbol::OpenBrace:
		case TokenSymbol::CloseBrace:
		case TokenSymbol::OpenParenthesis:
		case TokenSymbol::CloseParenthesis:
		case TokenSymbol::Semicolon:
		case TokenSymbol::Comma:
			return TokenType::Punctuation;

		// Identifier
		case TokenSymbol::Identifier:
			return TokenType::Identifier;

		// Literals
		case TokenSymbol::IntLiteral:
		case TokenSymbol::FloatLiteral:
			return TokenType::Literal;

		// Types
		case TokenSymbol::Int:
		case TokenSymbol::Float:
		case TokenSymbol::Vec2:
		case TokenSymbol::Vec3:
		case TokenSymbol::Vec4:
		case TokenSymbol::IVec2:
		case TokenSymbol::IVec3:
		case TokenSymbol::IVec4:
		case TokenSymbol::Mat2:
		case TokenSymbol::Mat3:
		case TokenSymbol::Mat4:
			return TokenType::Type;

		// Operators
		case TokenSymbol::Add:
		case TokenSymbol::Sub:
		case TokenSymbol::Mul:
		case TokenSymbol::Div:
		case TokenSymbol::Mod:
		case TokenSymbol::Member:
		case TokenSymbol::Assignment:
			return TokenType::Operator;

		// Reserved keywords
		case TokenSymbol::Return:
		case TokenSymbol::VertexOutput:
			return TokenType::Reserved;

		// Unknown
		default:
			return TokenType::Invalid;
	}
}

ASTNodeType Magma::Framework::Graphics::MSL::GetNodeType(ASTNodeSymbol symbol)
{
	switch (symbol)
	{
		// Object
		case ASTNodeSymbol::Program:
		case ASTNodeSymbol::Params:
		case ASTNodeSymbol::Scope:
		case ASTNodeSymbol::Function:
		case ASTNodeSymbol::Statement:
		case ASTNodeSymbol::Constructor:
		case ASTNodeSymbol::VertexOutput:
			return ASTNodeType::Object;

		// Identifier
		case ASTNodeSymbol::Identifier:
			return ASTNodeType::Identifier;

		// Literals
		case ASTNodeSymbol::IntLiteral:
		case ASTNodeSymbol::FloatLiteral:
			return ASTNodeType::Literal;

		// Types
		case ASTNodeSymbol::Int:
		case ASTNodeSymbol::Float:
		case ASTNodeSymbol::Vec2:
		case ASTNodeSymbol::Vec3:
		case ASTNodeSymbol::Vec4:
		case ASTNodeSymbol::IVec2:
		case ASTNodeSymbol::IVec3:
		case ASTNodeSymbol::IVec4:
		case ASTNodeSymbol::Mat2:
		case ASTNodeSymbol::Mat3:
		case ASTNodeSymbol::Mat4:
			return ASTNodeType::Type;

		// Operators
		case ASTNodeSymbol::Add:
		case ASTNodeSymbol::Sub:
		case ASTNodeSymbol::Mul:
		case ASTNodeSymbol::Div:
		case ASTNodeSymbol::Mod:
		case ASTNodeSymbol::Member:
		case ASTNodeSymbol::Assignment:
			return ASTNodeType::Operator;

		// Reserved keywords
		case ASTNodeSymbol::Return:
			return ASTNodeType::Reserved;

		// Unknown
		default:
			return ASTNodeType::Invalid;
	}
}

ASTNode * Magma::Framework::Graphics::MSL::CreateTree(ASTNodeSymbol symbol, const std::string & attribute)
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

void Magma::Framework::Graphics::MSL::AddToTree(ASTNode * node, ASTNode * tree)
{
	if (tree->lastChild == nullptr)
		tree->firstChild = tree->lastChild = node;
	else
	{
		tree->lastChild->next = node;
		tree->lastChild = node;
	}
}

ASTNode * Magma::Framework::Graphics::MSL::AddToTree(ASTNodeSymbol symbol, const std::string & attribute, ASTNode * tree)
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

void Magma::Framework::Graphics::MSL::DestroyTree(ASTNode * node)
{
	ASTNode* c = node->firstChild;
	while (c != nullptr)
	{
		DestroyTree(c);
		c = c->next;
	}

	delete node;
}
