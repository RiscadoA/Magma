#include "ShaderParser.hpp"

void Magma::Framework::Graphics::ShaderParser::Run(const std::vector<ShaderToken> & in, ShaderASTNode *& out, ShaderCompilerData& data)
{
	out = nullptr; // TO DO
}

void Magma::Framework::Graphics::ShaderParser::Clean(ShaderASTNode * node)
{
	delete node;
}
