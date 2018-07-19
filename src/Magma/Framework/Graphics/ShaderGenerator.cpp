#include "ShaderGenerator.hpp"
#include "../String/Conversion.hpp"

#include <sstream>
#include <map>

using namespace Magma::Framework;
using namespace Magma::Framework::Graphics;

struct GeneratorInfo
{
	std::stringstream bc;
	std::stringstream md;

	std::map<std::string, size_t> varIndexes;
	size_t nextVarIndex = 0;
};

std::string ShaderVarTypeToString(ShaderVariableType type)
{
	switch (type)
	{
		case ShaderVariableType::Int1: return "int1";
		case ShaderVariableType::Int2: return "int2";
		case ShaderVariableType::Int3: return "int3";
		case ShaderVariableType::Int4: return "int4";
		case ShaderVariableType::Int22: return "int22";
		case ShaderVariableType::Int33: return "int33";
		case ShaderVariableType::Int44: return "int44";
		case ShaderVariableType::Float1: return "float1";
		case ShaderVariableType::Float2: return "float2";
		case ShaderVariableType::Float3: return "float3";
		case ShaderVariableType::Float4: return "float4";
		case ShaderVariableType::Float22: return "float22";
		case ShaderVariableType::Float33: return "float33";
		case ShaderVariableType::Float44: return "float44";
		case ShaderVariableType::Invalid: return "$invalid-type$";
		default: return "$unsupported-type$";
	}
}

void GenerateStatement(const ShaderASTNode* statement, GeneratorInfo& info)
{

}

void GenerateScope(const ShaderASTNode* scope, GeneratorInfo& info)
{
	auto c = scope->child;
	while (c != nullptr)
	{
		GenerateStatement(c);
		c = c->next;
	}
}

void GenerateShader(const ShaderASTNode* shader, GeneratorInfo& info)
{
	auto scope = shader->child;
	GenerateScope(scope, info);
}

void Magma::Framework::Graphics::ShaderGenerator::Run(const ShaderASTNode * in, std::string& outBC, std::string& outMD, ShaderCompilerData& data)
{
	GeneratorInfo info;

	info.nextVarIndex = 32;

	// Generate MD
	{
		// Put version
		info.md << "MAJOR \"" << data.majorVersion << "\"" << std::endl;
		info.md << "MINOR \"" << data.minorVersion << "\"" << std::endl;

		// Put type
		if (data.shaderType == ShaderType::Vertex)
			info.md << "SHADER TYPE \"VERTEX\"" << std::endl;
		else if (data.shaderType == ShaderType::Pixel)
			info.md << "SHADER TYPE \"PIXEL\"" << std::endl;
		else
		{
			std::stringstream ss;
			ss << "Failed to run ShaderGenerator:" << std::endl;
			ss << "Unsupported/invalid shader type";
			throw ShaderError(ss.str());
		}
		info.md << std::endl;

		// Get output vars
		for (auto& i : data.outputVars)
		{
			auto id = i.second.name;
			String::ToLower(id);

			if (id == "screen_position")
			{
				i.second.index = 0;
				info.varIndexes.insert(std::make_pair(i.first, i.second.index));
				continue;
			}
			else if (id == "depth_attachment")
				i.second.index = 0;
			else if (id == "attachment_0")
				i.second.index = 1;
			else if (id == "attachment_1")
				i.second.index = 2;
			else if (id == "attachment_2")
				i.second.index = 3;
			else if (id == "attachment_3")
				i.second.index = 4;
			else if (id == "attachment_4")
				i.second.index = 5;
			else if (id == "attachment_5")
				i.second.index = 6;
			else if (id == "attachment_6")
				i.second.index = 7;
			else if (id == "attachment_7")
				i.second.index = 8;
			else if (id == "out_0")
				i.second.index = 9;
			else if (id == "out_1")
				i.second.index = 10;
			else if (id == "out_2")
				i.second.index = 11;
			else if (id == "out_3")
				i.second.index = 12;
			else if (id == "out_4")
				i.second.index = 13;
			else if (id == "out_5")
				i.second.index = 14;
			else if (id == "out_6")
				i.second.index = 15;
			else if (id == "out_7")
				i.second.index = 16;
			else
			{
				std::stringstream ss;
				ss << "Failed to run ShaderGenerator:" << std::endl;
				ss << "Failed to generate shader meta data:" << std::endl;
				ss << "Invalid output variable name '" << i.second.name << "'";
				throw ShaderError(ss.str());
			}

			info.md << "OUTPUT VAR" << std::endl;
			info.md << "\tINDEX \"" << i.second.index << "\"" << std::endl;
			info.md << "\tNAME \"" << i.second.name << "\"" << std::endl;
			info.md << "\tTYPE \"" << ShaderVarTypeToString(i.second.type) << "\"" << std::endl << std::endl;

			info.varIndexes.insert(std::make_pair(i.first, i.second.index));
		}

		// Get input vars
		for (auto& i : data.inputVars)
		{
			auto id = i.second.name;
			String::ToLower(id);

			i.second.index = 0;

			if (id == "in_0")
				i.second.index = 9;
			else if (id == "in_1")
				i.second.index = 10;
			else if (id == "in_2")
				i.second.index = 11;
			else if (id == "in_3")
				i.second.index = 12;
			else if (id == "in_4")
				i.second.index = 13;
			else if (id == "in_5")
				i.second.index = 14;
			else if (id == "in_6")
				i.second.index = 15;
			else if (id == "in_7")
				i.second.index = 16;

			if (i.second.index == 0)
			{
				i.second.index = info.nextVarIndex;
				info.nextVarIndex++;
				info.md << "INPUT VAR" << std::endl;
				info.md << "\tINDEX \"" << i.second.index << "\"" << std::endl;
				info.md << "\tNAME \"" << i.second.name << "\"" << std::endl;
				info.md << "\tTYPE \"" << ShaderVarTypeToString(i.second.type) << "\"" << std::endl << std::endl;
			}
			else
			{
				if (data.shaderType == ShaderType::Vertex)
					i.second.index += 8;

				info.md << "INPUT VAR" << std::endl;
				info.md << "\tINDEX \"" << i.second.index << "\"" << std::endl;
				info.md << "\tNAME \"" << i.first << "\"" << std::endl;
				info.md << "\tTYPE \"" << ShaderVarTypeToString(i.second.type) << "\"" << std::endl << std::endl;
			}

			info.varIndexes.insert(std::make_pair(i.first, i.second.index));
		}
	}

	// Generate BC
	GenerateShader(in, info);

	outBC = info.bc.str();
	outMD = info.md.str();
}
