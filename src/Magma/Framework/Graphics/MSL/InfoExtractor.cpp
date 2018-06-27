#include "Compiler.hpp"

#include <sstream>

using namespace Magma::Framework::Graphics::MSL;

void Magma::Framework::Graphics::MSL::Compiler::ExtractInfo()
{
	m_vertexOutID.clear();
	m_vertexOutID = std::string();
	m_vertexOut.clear();
	m_functions.clear();
	m_constantBuffers.clear();
	m_2dtextures.clear();

	//	VertexOutput
	//		Identifier
	//		Scope
	//			Type
	//				Identifier
	//			Type
	//				Identifier
	//			...
	auto extractVertexOutput = [&, this](ASTNode* node) -> void
	{
		auto c = node->firstChild;
		m_vertexOutID = c->attribute;
		c = c->next;
		c = c->firstChild;
		while (c != nullptr)
		{
			auto t = c->symbol;
			auto i = c->firstChild->attribute;
			m_vertexOut.push_back({t, i});
			c = c->next;
		}
	};

	//	Constant buffer
	//		Identifier
	//		Scope
	//			Type
	//				Identifier
	//			Type
	//				Identifier
	//			...
	auto extractConstantBuffer = [&, this](ASTNode* node) -> void
	{
		auto c = node->firstChild;
		auto id = c->attribute;

		if (m_constantBuffers.find(id) != m_constantBuffers.end())
		{
			std::stringstream ss;
			ss << "Failed to compile MSL code:" << std::endl;
			ss << "Info extraction stage failed:" << std::endl;
			ss << "A constant buffer cannot be defined twice: \"" << id << "\"" << std::endl;
			throw std::runtime_error(ss.str());
		}

		m_constantBuffers[id] = {};

		c = c->next;
		c = c->firstChild;
		while (c != nullptr)
		{
			auto t = c->symbol;
			auto i = c->firstChild->attribute;
			m_constantBuffers[id].push_back({ t, i });
			c = c->next;
		}
	};

	//	Texture2D
	//		Identifier
	auto extractTexture2D = [&, this](ASTNode* node) -> void
	{
		auto c = node->firstChild;
		auto id = c->attribute;

		if (m_2dtextures.find(id) != m_2dtextures.end())
		{
			std::stringstream ss;
			ss << "Failed to compile MSL code:" << std::endl;
			ss << "Info extraction stage failed:" << std::endl;
			ss << "A 2D texture cannot be defined twice: \"" << id << "\"" << std::endl;
			throw std::runtime_error(ss.str());
		}

		if (id == "texture")
		{
			std::stringstream ss;
			ss << "Failed to compile MSL code:" << std::endl;
			ss << "Info extraction stage failed:" << std::endl;
			ss << "A 2D texture cannot be named as \"texture\", since it is a reserved keyword" << std::endl;
			throw std::runtime_error(ss.str());
		}

		m_2dtextures.insert(id);
	};

	//	Function
	//		Return Type
	//		Identifier
	//		Params
	//			Type
	//				Identifier
	//			...
	//		Scope
	//			Statement
	//			Statement
	//			...
	auto extractFunction = [&, this](ASTNode* node) -> void
	{
		// Get return type
		auto n = node->firstChild;
		auto returnType = n->symbol;

		// Get ID
		n = n->next;
		auto id = n->attribute;
		FunctionDeclaration funcDecl;
		funcDecl.returnType = returnType;
		funcDecl.params = {};
		funcDecl.scope = nullptr;

		// Get params
		n = n->next;
		auto c = n->firstChild;
		while (c != nullptr)
		{
			auto t = c->symbol;
			auto i = c->firstChild->attribute;
			funcDecl.params.push_back({ t, i });
			c = c->next;
		}

		// Get scope
		n = n->next;
		funcDecl.scope = n;

		m_functions.push_back(std::make_pair(id, funcDecl));
	};

	auto n = m_astTree->firstChild;
	while (n != nullptr)
	{
		switch (n->symbol)
		{
			case ASTNodeSymbol::VertexOutput:
				if (!m_vertexOutID.empty())
				{
					std::stringstream ss;
					ss << "Failed to compile MSL code:" << std::endl;
					ss << "Info extraction stage failed:" << std::endl;
					ss << "VertexOutput can only be defined one time" << std::endl;
					throw std::runtime_error(ss.str());
				}
				extractVertexOutput(n);
				break;
			case ASTNodeSymbol::Texture2D:
				extractTexture2D(n);
				break;
			case ASTNodeSymbol::ConstantBuffer:
				extractConstantBuffer(n);
				break;
			case ASTNodeSymbol::Function:
				extractFunction(n);
				break;
		}
		n = n->next;
	}

	// Check vertex and pixel shader functions
	auto it = m_functions.begin();
	for (; it != m_functions.end(); ++it)
		if (it->first == "VertexShader")
			break;
	if (it == m_functions.end())
	{
		std::stringstream ss;
		ss << "Failed to compile MSL code:" << std::endl;
		ss << "Info extraction stage failed:" << std::endl;
		ss << "Couldn't find the VertexShader entry function" << std::endl;
		throw std::runtime_error(ss.str());
	}

	it = m_functions.begin();
	for (; it != m_functions.end(); ++it)
		if (it->first == "PixelShader")
			break;
	if (it == m_functions.end())
	{
		std::stringstream ss;
		ss << "Failed to compile MSL code:" << std::endl;
		ss << "Info extraction stage failed:" << std::endl;
		ss << "Couldn't find the PixelShader entry function" << std::endl;
		throw std::runtime_error(ss.str());
	}
}