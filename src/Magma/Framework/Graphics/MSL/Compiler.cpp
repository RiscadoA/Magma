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
