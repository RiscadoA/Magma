#include "ShaderGenerator.hpp"
#include "../String/Conversion.hpp"

#include <sstream>
#include <map>

using namespace Magma::Framework;
using namespace Magma::Framework::Graphics;

void GenerateMD(std::string& outMD, ShaderCompilerData& data)
{
	std::stringstream ss;

	// Header
	{
		ss << "MAJOR \"" << data.majorVersion << "\"" << std::endl;
		ss << "MINOR \"" << data.majorVersion << "\"" << std::endl;
		if (data.shaderType == ShaderType::Vertex)
			ss << "SHADER TYPE \"VERTEX\"" << std::endl;
		else if (data.shaderType == ShaderType::Pixel)
			ss << "SHADER TYPE \"PIXEL\"" << std::endl;
		else if (data.shaderType == ShaderType::Invalid)
		{
			std::stringstream ss;
			ss << "Failed to run ShaderGenerator:" << std::endl;
			ss << "Invalid shader type";
			throw ShaderError(ss.str());
		}
		else
		{
			std::stringstream ss;
			ss << "Failed to run ShaderGenerator:" << std::endl;
			ss << "Unsupported shader type";
			throw ShaderError(ss.str());
		}

		ss << std::endl;
	}

	// Inout vars
	for (auto& v : data.inputVariables)
	{
		ss << "INPUT VAR" << std::endl;
		ss << "\tINDEX \"" << v.index << "\"" << std::endl;
		ss << "\tNAME \"" << v.name << "\"";
		ss << "\tTYPE \"" << ShaderVariableTypeToString(v.type) << "\"" << std::endl << std::endl;
	}

	// Output vars
	for (auto& v : data.outputVariables)
	{
		ss << "OUTPUT VAR" << std::endl;
		ss << "\tINDEX \"" << v.index << "\"" << std::endl;
		ss << "\tNAME \"" << v.name << "\"";
		ss << "\tTYPE \"" << ShaderVariableTypeToString(v.type) << "\"" << std::endl << std::endl;
	}

	// Textures
	for (auto& t : data.texture2Ds)
	{
		std::stringstream ss;
		ss << "Failed to run ShaderGenerator:" << std::endl;
		ss << "2D textures not yet implemented";
		throw ShaderError(ss.str());
	}

	// Constant buffers
	for (auto& b : data.constantBuffers)
	{
		std::stringstream ss;
		ss << "Failed to run ShaderGenerator:" << std::endl;
		ss << "Constant buffers not yet implemented";
		throw ShaderError(ss.str());
	}

	outMD = ss.str();
}

void GenerateBC(std::string& outBC, ShaderCompilerData& data)
{

}

void Magma::Framework::Graphics::ShaderGenerator::Run(const ShaderSTNode * in, std::string& outBC, std::string& outMD, ShaderCompilerData& data)
{
	GenerateMD(outMD, data);
}


