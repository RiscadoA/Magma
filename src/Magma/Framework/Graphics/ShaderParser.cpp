#include "ShaderParser.hpp"

#include <sstream>

using namespace Magma::Framework;
using namespace Magma::Framework::Graphics;

struct ParserInfo
{
	ShaderSTNode* root;
	size_t lineNumber;
};

ShaderSTNode* CreateNode(ShaderSTNodeSymbol symbol, ShaderSTNodeType type, const std::string& attribute, ParserInfo& info)
{
	auto node = new ShaderSTNode();

	node->symbol = symbol;
	node->type = type;
	node->attribute = attribute;

	node->parent = nullptr;
	node->child = nullptr;
	node->next = nullptr;

	node->lineNumber = info.lineNumber;
	

	return node;
}

void AddToNode(ShaderSTNode* node, ShaderSTNode* child)
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

void DestroyNode(ShaderSTNode* node)
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

void Magma::Framework::Graphics::ShaderParser::Run(const std::vector<ShaderToken> & in, ShaderSTNode *& out, ShaderCompilerData& data)
{
	
}

void Magma::Framework::Graphics::ShaderParser::Clean(ShaderSTNode * node)
{
	DestroyNode(node);
}
