#include "ShaderCompiler.hpp"

#include "ShaderPreprocessor.hpp"
#include "1.X/ShaderLexer.hpp"
#include "1.X/ShaderParser.hpp"
#include "1.X/ShaderAnnotator.hpp"
#include "1.X/ShaderGenerator.hpp"

#include "BytecodeAssembler.hpp"
#include "MetaDataAssembler.hpp"

#include "../String/Conversion.hpp"

void Magma::Framework::Graphics::ShaderCompiler::Run(const std::string & in, std::string& outBC, std::string& outMD)
{
	ShaderCompilerData data;
	std::vector<ShaderLine> preprocessed;
	
	// Preprocessor
	ShaderPreprocessor::Run(in, preprocessed, data);

	// Version 1.X
	if (data.majorVersion == 1)
	{
		std::vector<ShaderToken> tokens;
		ShaderSTNode* tree;

		// Lexer
		Version_1_X::ShaderLexer::Run(preprocessed, tokens, data);

		// Parser
		Version_1_X::ShaderParser::Run(tokens, tree, data);

		// Annotator
		Version_1_X::ShaderAnnotator::Run(tree, data);

		// Generator
		Version_1_X::ShaderGenerator::Run(tree, outBC, outMD, data);
		Version_1_X::ShaderParser::Clean(tree);
	}
	// No other versions supported
	else
	{
		std::stringstream ss;
		ss << "Failed to run ShaderCompiler:" << std::endl;
		ss << "Unsupported MSL major version: '" << data.majorVersion << "." << data.minorVersion << "'";
		throw ShaderError(ss.str());
	}
}

size_t Magma::Framework::Graphics::ShaderCompiler::Run(const std::string & in, char * binaryObject, size_t maxSize)
{
	std::string bc, md;

	ShaderCompilerData data;
	std::vector<ShaderLine> preprocessed;

	// Preprocessor
	ShaderPreprocessor::Run(in, preprocessed, data);

	// Version 1.X
	if (data.majorVersion == 1)
	{
		std::vector<ShaderToken> tokens;
		ShaderSTNode* tree;

		// Lexer
		Version_1_X::ShaderLexer::Run(preprocessed, tokens, data);

		// Parser
		Version_1_X::ShaderParser::Run(tokens, tree, data);

		// Annotator
		Version_1_X::ShaderAnnotator::Run(tree, data);

		// Generator
		Version_1_X::ShaderGenerator::Run(tree, bc, md, data);
		Version_1_X::ShaderParser::Clean(tree);
	}
	// No other versions supported
	else
	{
		std::stringstream ss;
		ss << "Failed to run ShaderCompiler:" << std::endl;
		ss << "Unsupported MSL major version: '" << data.majorVersion << "." << data.minorVersion << "'";
		throw ShaderError(ss.str());
	}

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

