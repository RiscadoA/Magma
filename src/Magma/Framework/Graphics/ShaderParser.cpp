#include "ShaderParser.hpp"

#include <sstream>

using namespace Magma::Framework;
using namespace Magma::Framework::Graphics;

struct ParserInfo
{
	ShaderASTNode* root;
	size_t lineNumber;

	const std::vector<ShaderToken>& tokens;
	std::vector<ShaderToken>::const_iterator acceptedToken;
	std::vector<ShaderToken>::const_iterator token;

	std::string inputID;
	std::string outputID;

	std::vector<std::pair<std::string, ShaderInputVariable>> inputVars;
	std::vector<std::pair<std::string, ShaderOutputVariable>> outputVars;

	std::vector<ShaderConstantBuffer> constantBuffers;
};

ShaderVariableType ShaderTokenSymbolToVarType(ShaderTokenSymbol symbol)
{
	switch (symbol)
	{
		case ShaderTokenSymbol::Int1: return ShaderVariableType::Int1;
		case ShaderTokenSymbol::Int2: return ShaderVariableType::Int2;
		case ShaderTokenSymbol::Int3: return ShaderVariableType::Int3;
		case ShaderTokenSymbol::Int4: return ShaderVariableType::Int4;
		case ShaderTokenSymbol::Int22: return ShaderVariableType::Int22;
		case ShaderTokenSymbol::Int33: return ShaderVariableType::Int33;
		case ShaderTokenSymbol::Int44: return ShaderVariableType::Int44;

		case ShaderTokenSymbol::Float1: return ShaderVariableType::Float1;
		case ShaderTokenSymbol::Float2: return ShaderVariableType::Float2;
		case ShaderTokenSymbol::Float3: return ShaderVariableType::Float3;
		case ShaderTokenSymbol::Float4: return ShaderVariableType::Float4;
		case ShaderTokenSymbol::Float22: return ShaderVariableType::Float22;
		case ShaderTokenSymbol::Float33: return ShaderVariableType::Float33;
		case ShaderTokenSymbol::Float44: return ShaderVariableType::Float44;

		case ShaderTokenSymbol::Bool: return ShaderVariableType::Int1;

		default: return ShaderVariableType::Invalid;
	}
}

ShaderASTNode* CreateNode(ShaderASTNodeSymbol symbol, ShaderASTNodeType type, const std::string& attribute, ParserInfo& info)
{
	auto node = new ShaderASTNode();

	node->symbol = symbol;
	node->type = type;
	node->attribute = attribute;

	node->parent = nullptr;
	node->child = nullptr;
	node->next = nullptr;

	node->lineNumber = info.lineNumber;
	

	return node;
}

void AddToNode(ShaderASTNode* node, ShaderASTNode* child)
{
	child->parent = node;
	if (node->child == nullptr)
		node->child = child;
	else
	{
		auto c = node->child;
		while (c->next != nullptr)
			c = c->next;
		c->next = child;
	}
}

void DestroyNode(ShaderASTNode* node)
{
	if (node->parent != nullptr)
	{
		auto c = node->parent->child;
		if (c == node)
			node->parent->child = node->next;
		else
		{
			while (c != nullptr && c->next != node)
				c = c->next;
			if (c != nullptr && c->next != nullptr)
				c->next = c->next->next;
		}
	}

	auto c = node->child;
	while (c != nullptr)
	{
		auto prev = c;
		c = c->next;
		DestroyNode(prev);
	}

	delete node;
}

bool AcceptTokenSymbol(ShaderTokenSymbol symbol, ParserInfo& info)
{
	if (info.token == info.tokens.end())
		throw ShaderError("Failed to run ShaderParser:\nAcceptTokenSymbol got EOF unexpectedly");

	if (info.token->symbol != symbol)
		return false;

	info.acceptedToken = info.token;

	++info.token;
	if (info.token != info.tokens.end())
		info.lineNumber = info.token->lineNumber;

	return true;
}

bool AcceptTokenType(ShaderTokenType type, ParserInfo& info)
{
	if (info.token == info.tokens.end())
		throw ShaderError("Failed to run ShaderParser:\nAcceptTokenType got EOF unexpectedly");

	if (info.token->type != type)
		return false;

	info.acceptedToken = info.token;

	++info.token;
	if (info.token != info.tokens.end())
		info.lineNumber = info.token->lineNumber;

	return true;
}

const ShaderToken& ExpectTokenSymbol(ShaderTokenSymbol symbol, ParserInfo& info)
{
	if (info.token == info.tokens.end())
		throw ShaderError("Failed to run ShaderParser:\nExpectTokenSymbol got EOF unexpectedly");

	if (info.token->symbol != symbol)
	{
		std::stringstream ss;
		ss << "Failed to run ShaderParser:" << std::endl;
		ss << "Unexpected token '" << ShaderTokenSymbolToString(info.token->symbol) << "' \"" << info.token->attribute << "\"" << std::endl;
		ss << "Expected token symbol '" << ShaderTokenSymbolToString(symbol) << "'" << std::endl;
		ss << "Line: " << info.lineNumber;
		throw ShaderError(ss.str());
	}

	const auto& tok = *info.token;

	++info.token;
	if (info.token != info.tokens.end())
		info.lineNumber = info.token->lineNumber;

	return tok;
}

const ShaderToken& ExpectTokenType(ShaderTokenType type, ParserInfo& info)
{
	if (info.token == info.tokens.end())
		throw ShaderError("Failed to run ShaderParser:\nExpectTokenType got EOF unexpectedly");

	if (info.token->type != type)
	{
		std::stringstream ss;
		ss << "Failed to run ShaderParser:" << std::endl;
		ss << "Unexpected token '" << ShaderTokenSymbolToString(info.token->symbol) << "' \"" << info.token->attribute << "\"" << std::endl;
		ss << "Expected token symbol '";
		switch (type)
		{
			case ShaderTokenType::Identifier: ss << "identifier"; break;
			case ShaderTokenType::Keyword: ss << "keyword"; break;
			case ShaderTokenType::Literal: ss << "literal"; break;
			case ShaderTokenType::Operator: ss << "operator"; break;
			case ShaderTokenType::Punctuation: ss << "punctuation"; break;
			case ShaderTokenType::Type: ss << "type"; break;
			case ShaderTokenType::Invalid: ss << "$invalid$"; break;
			default: ss << "$unsupported-" << (int)type << "$"; break;
		}
		ss << "'" << std::endl;
		ss << "Line: " << info.lineNumber;
		throw ShaderError(ss.str());
	}

	const auto& tok = *info.token;

	++info.token;
	if (info.token != info.tokens.end())
		info.lineNumber = info.token->lineNumber;

	return tok;
}

void ParseInput(ParserInfo& info)
{
	if (info.inputID != "")
	{
		std::stringstream ss;
		ss << "Failed to run ShaderParser:" << std::endl;
		ss << "Input is defined multiple times" << std::endl;
		ss << "Line: " << info.lineNumber;
		throw ShaderError(ss.str());
	}

	ExpectTokenSymbol(ShaderTokenSymbol::Input, info);
	info.inputID = ExpectTokenSymbol(ShaderTokenSymbol::Identifier, info).attribute;
	ExpectTokenSymbol(ShaderTokenSymbol::OpenBrace, info);

	// Get input vars
	while (AcceptTokenType(ShaderTokenType::Type, info))
	{
		ShaderInputVariable var;

		var.type = ShaderTokenSymbolToVarType(info.acceptedToken->symbol);
		auto id = ExpectTokenSymbol(ShaderTokenSymbol::Identifier, info).attribute;
		var.index = -1;

		ExpectTokenSymbol(ShaderTokenSymbol::Colon, info);
		var.name = ExpectTokenSymbol(ShaderTokenSymbol::Identifier, info).attribute;

		info.inputVars.push_back(std::make_pair(id, var));

		ExpectTokenSymbol(ShaderTokenSymbol::Semicolon, info);
	}


	ExpectTokenSymbol(ShaderTokenSymbol::CloseBrace, info);
	ExpectTokenSymbol(ShaderTokenSymbol::Semicolon, info);
}

void ParseOutput(ParserInfo& info)
{
	
}

ShaderASTNode* ParseShader(ParserInfo& info)
{
	return nullptr;
}

// Main parsing function (whole shader)
void ParseWholeShader(ParserInfo& info)
{
	info.inputID = "";
	info.outputID = "";

	while (info.token != info.tokens.end())
	{
		if (info.token->symbol == ShaderTokenSymbol::Input)
			ParseInput(info);
		else if (info.token->symbol == ShaderTokenSymbol::Output)
			ParseOutput(info);
		else if (info.token->symbol == ShaderTokenSymbol::Shader)
		{
			auto node = ParseShader(info);
			AddToNode(info.root, node);
		}
		else
		{
			std::stringstream ss;
			ss << "Failed to run ShaderParser:" << std::endl;
			ss << "Unexpected token '" << ShaderTokenSymbolToString(info.token->symbol) << "' \"" << info.token->attribute << "\"" << std::endl;
			ss << "Line: " << info.lineNumber;
			throw ShaderError(ss.str());
		}
	}
}

void Magma::Framework::Graphics::ShaderParser::Run(const std::vector<ShaderToken> & in, ShaderASTNode *& out, ShaderCompilerData& data)
{
	ParserInfo info
	{
		CreateNode(ShaderASTNodeSymbol::Root, ShaderASTNodeType::Other, "", info),
		0,
		in,
	};

	info.token = info.tokens.begin();
	ParseWholeShader(info);

	out = info.root;
}

void Magma::Framework::Graphics::ShaderParser::Clean(ShaderASTNode * node)
{
	DestroyNode(node);
}
