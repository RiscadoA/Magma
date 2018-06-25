#include "Compiler.hpp"

#include <sstream>

using namespace Magma::Framework::Graphics::MSL;

void Magma::Framework::Graphics::MSL::Compiler::ExtractInfo()
{
	m_vertexOutID.clear();
	m_vertexOutID = std::string();
	m_vertexOut.clear();
	m_functions.clear();

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
		m_functions[id] = { returnType, {}, nullptr };

		// Get params
		n = n->next;
		auto c = n->firstChild;
		while (c != nullptr)
		{
			auto t = c->symbol;
			auto i = c->firstChild->attribute;
			m_functions[id].params.push_back({ t, i });
			c = c->next;
		}

		// Get scope
		n = n->next;
		m_functions[id].scope = n;
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
			case ASTNodeSymbol::Function:
				extractFunction(n);
				break;
		}
		n = n->next;
	}

	// Check vertex and pixel shader functions
	if (m_functions.find("VertexShader") == m_functions.end())
	{
		std::stringstream ss;
		ss << "Failed to compile MSL code:" << std::endl;
		ss << "Info extraction stage failed:" << std::endl;
		ss << "Couldn't find the VertexShader entry function" << std::endl;
		throw std::runtime_error(ss.str());
	}

	if (m_functions.find("PixelShader") == m_functions.end())
	{
		std::stringstream ss;
		ss << "Failed to compile MSL code:" << std::endl;
		ss << "Info extraction stage failed:" << std::endl;
		ss << "Couldn't find the PixelShader entry function" << std::endl;
		throw std::runtime_error(ss.str());
	}
}