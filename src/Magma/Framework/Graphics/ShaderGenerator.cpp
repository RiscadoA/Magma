#include "ShaderGenerator.hpp"
#include "../String/Conversion.hpp"

#include <regex>
#include <sstream>
#include <map>

using namespace Magma::Framework;
using namespace Magma::Framework::Graphics;

std::string ShaderVariableTypeToBC(ShaderVariableType type)
{
	switch (type)
	{
		case ShaderVariableType::Int1: return "I1";
		case ShaderVariableType::Int2: return "I2";
		case ShaderVariableType::Int3: return "I3";
		case ShaderVariableType::Int4: return "I4";
		case ShaderVariableType::Int22: return "I22";
		case ShaderVariableType::Int33: return "I33";
		case ShaderVariableType::Int44: return "I44";

		case ShaderVariableType::Float1: return "F1";
		case ShaderVariableType::Float2: return "F2";
		case ShaderVariableType::Float3: return "F3";
		case ShaderVariableType::Float4: return "F4";
		case ShaderVariableType::Float22: return "F22";
		case ShaderVariableType::Float33: return "F33";
		case ShaderVariableType::Float44: return "F44";

		default:
		{
			std::stringstream ss;
			ss << "Failed to run ShaderGenerator:" << std::endl;
			ss << "Failed to convert shader variable type into a bytecode variable type";
			ss << "Unsupported/invalid shader variable type '" << ShaderVariableTypeToString(type) << "'";
			throw ShaderError(ss.str());
			break;
		}
	}
}

size_t GenerateExpression(const ShaderSTNode* exp, std::stringstream& out, ShaderCompilerData& data, size_t outVar)
{
	out << "OPSCOPE, # expression" << std::endl;

	if (exp->type == ShaderSTNodeType::Operator)
	{
		if (exp->operatorType == ShaderOperatorType::Assign)
		{
			size_t id0 = exp->child->lvalue;
			if (exp->child->lvalue)
				id0 = GenerateExpression(exp, out, data, -1);
			else
				
			auto id = 
		}
	}
	else if (exp->type == ShaderSTNodeType::Reference)
	{
		
	}

	out << "CLSCOPE, # expression" << std::endl;

	return -1;
}

void GenerateScope(const ShaderSTNode * scope, std::stringstream& out, ShaderCompilerData& data)
{
	out << "OPSCOPE, # scope" << std::endl;
	
	auto c = scope->child;
	while (c != nullptr)
	{
		if (c->type == ShaderSTNodeType::Scope)
			GenerateScope(c, out, data);
		else if (c->type == ShaderSTNodeType::Declaration)
		{
			out << "DECL" << ShaderVariableTypeToBC(c->reference->type) << ", db" << c->scope->nextVarID << "," << std::endl;
			c->reference->id = c->scope->nextVarID;
			c->scope->nextVarID++;
		}
		else if (c->type == ShaderSTNodeType::Operator)
			GenerateExpression(c, out, data, -1);
		c = c->next;
	}

	out << "CLSCOPE, # scope" << std::endl;
}

void GenerateMD(std::string& outMD, ShaderCompilerData& data)
{
	size_t nextVarID = 0;

	static std::regex inNameRegex(R"(in_(\d+))", std::regex_constants::icase);
	static std::regex positionNameRegex(R"(screen_position)", std::regex_constants::icase);
	static std::regex outNameRegex(R"(out_(\d+))", std::regex_constants::icase);
	static std::regex depthNameRegex(R"(depth_attachment)", std::regex_constants::icase);
	static std::regex attachmentNameRegex(R"(attachment_(\d+))", std::regex_constants::icase);
	std::smatch match;

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

	// Input vars
	for (auto& v : data.inputVariables)
	{
		if (std::regex_match(v.name, match, inNameRegex))
		{
			auto id = std::stoull(match.str(1));
			v.index = 8 + id;
			if (id >= 8)
			{
				std::stringstream ss;
				ss << "Failed to run ShaderGenerator:" << std::endl;
				ss << "Failed to generate input variable \"" << v.id << "\":";
				ss << "Name \"" << v.name << "\" input index can only contain the numbers between 0 and 7";
				throw ShaderError(ss.str());
			}
		}
		else
		{
			if (nextVarID >= 8)
			{
				std::stringstream ss;
				ss << "Failed to run ShaderGenerator:" << std::endl;
				ss << "Failed to generate input variable \"" << v.id << "\":";
				ss << "Too many input variables (8 max)";
				throw ShaderError(ss.str());
			}
			v.index = nextVarID;
			++nextVarID;
		}

		ss << "INPUT VAR" << std::endl;
		ss << "\tINDEX \"" << v.index << "\"" << std::endl;
		ss << "\tNAME \"" << v.name << "\"";
		ss << "\tTYPE \"" << ShaderVariableTypeToString(v.type) << "\"" << std::endl << std::endl;
	}

	// Output vars
	for (auto& v : data.outputVariables)
	{
		if (std::regex_match(v.name, match, outNameRegex))
		{
			auto id = std::stoull(match.str(1));
			v.index = 8 + id;
			if (id >= 8)
			{
				std::stringstream ss;
				ss << "Failed to run ShaderGenerator:" << std::endl;
				ss << "Failed to generate output variable \"" << v.id << "\":";
				ss << "Name \"" << v.name << "\" output index can only contain the numbers between 0 and 7";
				throw ShaderError(ss.str());
			}
		}
		else if (std::regex_match(v.name, match, attachmentNameRegex))
		{
			auto id = std::stoull(match.str(1));
			v.index = id;
			if (id >= 8)
			{
				std::stringstream ss;
				ss << "Failed to run ShaderGenerator:" << std::endl;
				ss << "Failed to generate output variable \"" << v.id << "\":";
				ss << "Name \"" << v.name << "\" attachment index can only contain the numbers between 0 and 7";
				throw ShaderError(ss.str());
			}
		}
		else if (std::regex_match(v.name, match, positionNameRegex))
		{
			v.index = 0;
			continue;
		}
		else if (std::regex_match(v.name, match, depthNameRegex))
		{
			v.index = 0;
			continue;
		}
		else
		{
			std::stringstream ss;
			ss << "Failed to run ShaderGenerator:" << std::endl;
			ss << "Failed to generate output variable \"" << v.id << "\":";
			ss << "Invalid output variable name \"" << v.name << "\"";
			throw ShaderError(ss.str());
		}

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

void Magma::Framework::Graphics::ShaderGenerator::Run(const ShaderSTNode * in, std::string& outBC, std::string& outMD, ShaderCompilerData& data)
{
	GenerateMD(outMD, data);

	std::stringstream ss;
	in->scope->nextVarID = 32;
	GenerateScope(in, ss, data);
	outBC = ss.str();
}


