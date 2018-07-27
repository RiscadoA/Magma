#include "ShaderCompiler.hpp"

#include "ShaderPreprocessor.hpp"
#include "ShaderLexer.hpp"
#include "ShaderParser.hpp"
#include "ShaderAnnotator.hpp"
#include "ShaderGenerator.hpp"

#include "BytecodeAssembler.hpp"
#include "MetaDataAssembler.hpp"

#include "../String/Conversion.hpp"

void Magma::Framework::Graphics::ShaderCompiler::Run(const std::string & in, std::string& outBC, std::string& outMD)
{
	ShaderCompilerData data;

	std::vector<ShaderLine> preprocessed;
	std::vector<ShaderToken> tokens;
	ShaderSTNode* tree;
	
	// Preprocessor
	ShaderPreprocessor::Run(in, preprocessed, data);

	// Lexer
	ShaderLexer::Run(preprocessed, tokens, data);

	// Parser
	ShaderParser::Run(tokens, tree, data);

	// Annotator
	ShaderAnnotator::Run(tree, data);

	// Generator
	ShaderGenerator::Run(tree, outBC, outMD, data);
	ShaderParser::Clean(tree);
}

size_t Magma::Framework::Graphics::ShaderCompiler::Run(const std::string & in, char * binaryObject, size_t maxSize)
{
	ShaderCompilerData data;

	std::vector<ShaderLine> preprocessed;
	std::vector<ShaderToken> tokens;
	ShaderSTNode* tree;

	// Preprocessor
	ShaderPreprocessor::Run(in, preprocessed, data);

	// Lexer
	ShaderLexer::Run(preprocessed, tokens, data);

	// Parser
	ShaderParser::Run(tokens, tree, data);

	// Annotator
	ShaderAnnotator::Run(tree, data);

	// Generator
	std::string bc, md;
	ShaderGenerator::Run(tree, bc, md, data);
	ShaderParser::Clean(tree);

	// Assemble bytecode and meta data
	char bytecode[4096];
	unsigned long bytecodeSize = BytecodeAssembler::Assemble(bc, bytecode, sizeof(bytecode));
	char metaData[4096];
	unsigned long metaDataSize = MetaDataAssembler::Assemble(md, metaData, sizeof(metaData));

	// Assemble binary object
	if ((bytecodeSize + metaDataSize + sizeof(unsigned long) * 2) > maxSize)
	{
		std::stringstream ss;
		ss << "Failed to run ShaderCompiler:" << std::endl;
		ss << "Failed to assemble binary object:" << std::endl;
		ss << "Maximum size exceeded (" << (bytecodeSize + metaDataSize + sizeof(unsigned long) * 2) << " > " << maxSize << ")";
		throw ShaderError(ss.str());
	}

	auto beBCSize = String::U32ToBE(bytecodeSize);
	auto beMDSize = String::U32ToBE(metaDataSize);
	memcpy(binaryObject, &beBCSize, sizeof(unsigned long));
	memcpy(binaryObject + sizeof(unsigned long), bytecode, bytecodeSize);
	memcpy(binaryObject + bytecodeSize + sizeof(unsigned long), &beMDSize, sizeof(unsigned long));
	memcpy(binaryObject + bytecodeSize + sizeof(unsigned long) * 2, metaData, metaDataSize);
	return bytecodeSize + metaDataSize + sizeof(unsigned long) * 2;
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

