#include "ShaderCompiler.hpp"

#include "ShaderPreprocessor.hpp"
#include "ShaderLexer.hpp"
#include "ShaderParser.hpp"
#include "ShaderAnnotator.hpp"
#include "ShaderGenerator.hpp"

void Magma::Framework::Graphics::ShaderCompiler::Run(const std::string & in, std::string& outBC, std::string& outMD)
{
	ShaderCompilerData data;

	std::vector<ShaderLine> preprocessorOut;
	ShaderPreprocessor::Run(in, preprocessorOut, data);

	std::vector<ShaderToken> lexerOut;
	ShaderLexer::Run(preprocessorOut, lexerOut, data);

	ShaderSTNode* parserOut;
	ShaderParser::Run(lexerOut, parserOut, data);
	ShaderParser::Print(parserOut);

	ShaderAnnotator::Run(parserOut, data);

	ShaderGenerator::Run(parserOut, outBC, outMD, data);

	ShaderParser::Clean(parserOut);
}
