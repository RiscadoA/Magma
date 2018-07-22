#include "ShaderCompiler.hpp"

#include "ShaderPreprocessor.hpp"
#include "ShaderLexer.hpp"
#include "ShaderParser.hpp"
#include "ShaderAnnotator.hpp"
#include "ShaderGenerator.hpp"

void Magma::Framework::Graphics::ShaderCompiler::Run(const std::string & in, std::string& outBC, std::string& outMD)
{
	ShaderCompilerData data;

	std::vector<ShaderLine> preprocessed;
	std::vector<ShaderToken> tokens;
	ShaderSTNode* tree;
	

	ShaderPreprocessor::Run(in, preprocessed, data);

	
	ShaderLexer::Run(preprocessed, tokens, data);

	

	ShaderParser::Run(tokens, tree, data);
	ShaderParser::Print(tree);

	ShaderAnnotator::Run(tree, data);
	ShaderAnnotator::Print(tree);

	ShaderGenerator::Run(tree, outBC, outMD, data);

	ShaderParser::Clean(tree);
}

Magma::Framework::Graphics::ShaderVariable* Magma::Framework::Graphics::ShaderScope::GetVar(const std::string& name)
{
	for (auto& v : this->variables)
		if (v.id == name)
			return &v;
	if (this->parent.expired())
		return nullptr;
	else
		return this->parent.lock()->GetVar(name);
}

