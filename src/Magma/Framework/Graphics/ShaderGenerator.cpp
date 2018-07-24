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
			ss << "Failed to convert shader variable type into a bytecode variable type:" << std::endl;
			ss << "Unsupported/invalid shader variable type '" << ShaderVariableTypeToString(type) << "'" << std::endl;
			throw ShaderError(ss.str());
			break;
		}
	}
}

size_t GenerateExpression(const ShaderSTNode* exp, std::stringstream& out, ShaderCompilerData& data, size_t outVar)
{
	if (exp->type == ShaderSTNodeType::Operator)
	{
		out << "OPSCOPE, # expression" << std::endl;

		size_t retId = -1;

		if (exp->operatorType == ShaderOperatorType::Assign)
		{
			size_t id0 = exp->child->reference->index;

			size_t id1 = 0;
			if (exp->child->next->lvalue)
				id1 = GenerateExpression(exp->child->next, out, data, -1);
			else
			{
				out << "DECL" << ShaderVariableTypeToBC(exp->child->next->resultType) << ", db" << exp->scope->nextVarID << "," << std::endl;
				id1 = exp->scope->nextVarID;
				exp->scope->nextVarID++;
				GenerateExpression(exp->child->next, out, data, id1);
			}

			out << "VARIN0, db" << id1 << "," << std::endl;
			out << "VAROUT, db" << id0 << "," << std::endl;
			if (exp->child->type == ShaderSTNodeType::ComponentX)
				out << "AS" << ShaderVariableTypeToBC(exp->child->reference->type) << "CMP, db0," << std::endl;
			else if (exp->child->type == ShaderSTNodeType::ComponentY)
				out << "AS" << ShaderVariableTypeToBC(exp->child->reference->type) << "CMP, db1," << std::endl;
			else if (exp->child->type == ShaderSTNodeType::ComponentZ)
				out << "AS" << ShaderVariableTypeToBC(exp->child->reference->type) << "CMP, db2," << std::endl;
			else if (exp->child->type == ShaderSTNodeType::ComponentW)
				out << "AS" << ShaderVariableTypeToBC(exp->child->reference->type) << "CMP, db3," << std::endl;
			else
				out << "ASSIGN," << std::endl;

			retId = id0;
		}
		else
		{
			if (outVar == -1)
				return -1;

			if (exp->child->next == nullptr)
			{
				size_t id0 = 0;
				if (exp->child->lvalue)
					id0 = GenerateExpression(exp->child, out, data, -1);
				else
				{
					out << "DECL" << ShaderVariableTypeToBC(exp->child->resultType) << ", db" << exp->scope->nextVarID << "," << std::endl;
					id0 = exp->scope->nextVarID;
					exp->scope->nextVarID++;
					GenerateExpression(exp->child, out, data, id0);
				}

				if (exp->operatorType == ShaderOperatorType::Add ||
					exp->operatorType == ShaderOperatorType::Subtract)
				{
					out << "DECL" << ShaderVariableTypeToBC(exp->child->resultType) << ", db" << exp->scope->nextVarID << "," << std::endl;
					out << "SET" << ShaderVariableTypeToBC(exp->child->resultType) << ", db" << exp->scope->nextVarID << ", ";
					if (exp->child->resultType == ShaderVariableType::Int1)
						out << "di0,";
					else if (exp->child->resultType == ShaderVariableType::Int2)
						out << "di0, di0,";
					else if (exp->child->resultType == ShaderVariableType::Int3)
						out << "di0, di0, di0,";
					else if (exp->child->resultType == ShaderVariableType::Int4)
						out << "di0, di0, di0, di0,";
					else if (exp->child->resultType == ShaderVariableType::Float1)
						out << "f0,";
					else if (exp->child->resultType == ShaderVariableType::Float2)
						out << "f0, f0,";
					else if (exp->child->resultType == ShaderVariableType::Float3)
						out << "f0, f0, f0,";
					else if (exp->child->resultType == ShaderVariableType::Float4)
						out << "f0, f0, f0, f0,";
					out << std::endl;
					out << "VARIN0, db" << exp->scope->nextVarID << "," << std::endl;
					out << "VARIN1, db" << id0 << "," << std::endl;

					exp->scope->nextVarID++;
				}
				else
					out << "VARIN0, db" << id0 << "," << std::endl;
				out << "VAROUT, db" << outVar << "," << std::endl;

				switch (exp->operatorType)
				{
					case ShaderOperatorType::Add: out << "ADD," << std::endl; break;
					case ShaderOperatorType::Subtract: out << "SUB," << std::endl; break;
					case ShaderOperatorType::Not: out << "NOT," << std::endl; break;
					default:
					{
						std::stringstream ss;
						ss << "Failed to run ShaderGenerator:" << std::endl;
						ss << "Failed to generate expression:" << std::endl;
						ss << "Unsupported/invalid shader unary operator type '" << ShaderOperatorTypeToString(exp->operatorType) << "'" << std::endl;
						ss << "Line: " << exp->lineNumber;
						throw ShaderError(ss.str());
						break;
					}
				}
			}
			else
			{
				size_t id0 = 0;
				if (exp->child->lvalue)
					id0 = GenerateExpression(exp->child, out, data, -1);
				else
				{
					out << "DECL" << ShaderVariableTypeToBC(exp->child->resultType) << ", db" << exp->scope->nextVarID << "," << std::endl;
					id0 = exp->scope->nextVarID;
					exp->scope->nextVarID++;
					GenerateExpression(exp->child, out, data, id0);
				}

				size_t id1 = 0;
				if (exp->child->next->lvalue)
					id1 = GenerateExpression(exp->child->next, out, data, -1);
				else
				{
					out << "DECL" << ShaderVariableTypeToBC(exp->child->next->resultType) << ", db" << exp->scope->nextVarID << "," << std::endl;
					id1 = exp->scope->nextVarID;
					exp->scope->nextVarID++;
					GenerateExpression(exp->child->next, out, data, id1);
				}
			}		
		}

		out << "CLSCOPE, # expression" << std::endl;

		return retId;
	}
	else if (exp->type == ShaderSTNodeType::Constructor)
	{
		out << "OPSCOPE, # constructor" << std::endl;

		auto cmpType = ShaderVariableType::Invalid;
		if (exp->child->variableType == ShaderVariableType::Float1 ||
			exp->child->variableType == ShaderVariableType::Float2 || 
			exp->child->variableType == ShaderVariableType::Float3 || 
			exp->child->variableType == ShaderVariableType::Float4)
			cmpType = ShaderVariableType::Float1;
		else if (exp->child->variableType == ShaderVariableType::Int1 ||
				 exp->child->variableType == ShaderVariableType::Int2 ||
				 exp->child->variableType == ShaderVariableType::Int3 ||
				 exp->child->variableType == ShaderVariableType::Int4)
			cmpType = ShaderVariableType::Int1;
		else
		{
			std::stringstream ss;
			ss << "Failed to run ShaderGenerator:" << std::endl;
			ss << "Failed to generate constructor:" << std::endl;
			ss << "Unsupported/invalid shader constructor type '" << ShaderVariableTypeToString(exp->child->variableType) << "'" << std::endl;
			ss << "Line: " << exp->lineNumber;
			throw ShaderError(ss.str());
		}

		auto c = exp->child->next;
		size_t cmpId = 0;

		out << "DECL" << ShaderVariableTypeToBC(cmpType) << ", db" << exp->scope->nextVarID << "," << std::endl;
		auto id = exp->scope->nextVarID;
		exp->scope->nextVarID++;

		while (c != nullptr)
		{
			GenerateExpression(c, out, data, id);
			out << "VARIN0, db" << id << "," << std::endl;
			out << "VAROUT, db" << outVar << "," << std::endl;
			out << "AS" << ShaderVariableTypeToBC(exp->child->variableType) << "CMP, db" << cmpId << "," << std::endl;
			c = c->next;
			++cmpId;
		}

		out << "CLSCOPE, # constructor" << std::endl;
	}
	else if (exp->type == ShaderSTNodeType::ComponentX)
	{
		if (exp->parent->type == ShaderSTNodeType::Operator && exp->parent->operatorType == ShaderOperatorType::Assign && exp == exp->parent->child)
			// As lvalue
			return -1;
		else
		{
			// Not as lvalue
			if (outVar != -1)
			{
				if (exp->reference == nullptr)
				{
					out << "DECL" << ShaderVariableTypeToBC(exp->child->resultType) << ", db" << exp->scope->nextVarID << "," << std::endl;
					auto id = exp->scope->nextVarID;
					exp->scope->nextVarID++;
					GenerateExpression(exp->child, out, data, id);
					out << "VARIN0, db" << id << "," << std::endl;
					out << "VAROUT, db" << outVar << "," << std::endl;
					out << "GE" << ShaderVariableTypeToBC(exp->child->resultType) << "CMP, db00," << std::endl;
				}
				else
				{
					out << "VARIN0, db" << exp->reference->index << "," << std::endl;
					out << "VAROUT, db" << outVar << "," << std::endl;
					out << "GE" << ShaderVariableTypeToBC(exp->reference->type) << "CMP, db00," << std::endl;
				}
			}
		}
	}
	else if (exp->type == ShaderSTNodeType::ComponentY)
	{
		if (exp->parent->type == ShaderSTNodeType::Operator && exp->parent->operatorType == ShaderOperatorType::Assign && exp == exp->parent->child)
			// As lvalue
			return -1;
		else
		{
			// Not as lvalue
			if (outVar != -1)
			{
				if (exp->reference == nullptr)
				{
					out << "DECL" << ShaderVariableTypeToBC(exp->child->resultType) << ", db" << exp->scope->nextVarID << "," << std::endl;
					auto id = exp->scope->nextVarID;
					exp->scope->nextVarID++;
					GenerateExpression(exp->child, out, data, id);
					out << "VARIN0, db" << id << "," << std::endl;
					out << "VAROUT, db" << outVar << "," << std::endl;
					out << "GE" << ShaderVariableTypeToBC(exp->child->resultType) << "CMP, db01," << std::endl;
				}
				else
				{
					out << "VARIN0, db" << exp->reference->index << "," << std::endl;
					out << "VAROUT, db" << outVar << "," << std::endl;
					out << "GE" << ShaderVariableTypeToBC(exp->reference->type) << "CMP, db01," << std::endl;
				}
			}
		}
	}
	else if (exp->type == ShaderSTNodeType::ComponentZ)
	{
		if (exp->parent->type == ShaderSTNodeType::Operator && exp->parent->operatorType == ShaderOperatorType::Assign && exp == exp->parent->child)
			// As lvalue
			return -1;
		else
		{
			// Not as lvalue
			if (outVar != -1)
			{
				if (exp->reference == nullptr)
				{
					out << "DECL" << ShaderVariableTypeToBC(exp->child->resultType) << ", db" << exp->scope->nextVarID << "," << std::endl;
					auto id = exp->scope->nextVarID;
					exp->scope->nextVarID++;
					GenerateExpression(exp->child, out, data, id);
					out << "VARIN0, db" << id << "," << std::endl;
					out << "VAROUT, db" << outVar << "," << std::endl;
					out << "GE" << ShaderVariableTypeToBC(exp->child->resultType) << "CMP, db02," << std::endl;
				}
				else
				{
					out << "VARIN0, db" << exp->reference->index << "," << std::endl;
					out << "VAROUT, db" << outVar << "," << std::endl;
					out << "GE" << ShaderVariableTypeToBC(exp->reference->type) << "CMP, db02," << std::endl;
				}
			}
		}
	}
	else if (exp->type == ShaderSTNodeType::ComponentW)
	{
		if (exp->parent->type == ShaderSTNodeType::Operator && exp->parent->operatorType == ShaderOperatorType::Assign && exp == exp->parent->child)
			// As lvalue
			return -1;
		else
		{
			// Not as lvalue
			if (outVar != -1)
			{
				if (exp->reference == nullptr)
				{
					out << "DECL" << ShaderVariableTypeToBC(exp->child->resultType) << ", db" << exp->scope->nextVarID << "," << std::endl;
					auto id = exp->scope->nextVarID;
					exp->scope->nextVarID++;
					GenerateExpression(exp->child, out, data, id);
					out << "VARIN0, db" << id << "," << std::endl;
					out << "VAROUT, db" << outVar << "," << std::endl;
					out << "GE" << ShaderVariableTypeToBC(exp->child->resultType) << "CMP, db03," << std::endl;
				}
				else
				{
					out << "VARIN0, db" << exp->reference->index << "," << std::endl;
					out << "VAROUT, db" << outVar << "," << std::endl;
					out << "GE" << ShaderVariableTypeToBC(exp->reference->type) << "CMP, db03," << std::endl;
				}
			}
		}
	}
	else if (exp->type == ShaderSTNodeType::Reference)
		return exp->reference->index;
	else if (exp->type == ShaderSTNodeType::Literal)
	{
		if (outVar == -1)
			return -1;

		switch (exp->variableType)
		{
			case ShaderVariableType::Int1:
				out << "SETI1, db" << outVar << ", di" << exp->attribute << "," << std::endl;
				break;
			case ShaderVariableType::Float1:
				out << "SETF1, db" << outVar << ", f" << exp->attribute << "," << std::endl;
				break;
			default:
			{
				std::stringstream ss;
				ss << "Failed to run ShaderGenerator:" << std::endl;
				ss << "Failed to generate literal" << std::endl;
				ss << "Unsupported/invalid shader literal type '" << ShaderVariableTypeToString(exp->variableType) << "'" << std::endl;
				ss << "Line: " << exp->lineNumber;
				throw ShaderError(ss.str());
				break;
			}				
		}
	}

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
			c->reference->index = c->scope->nextVarID;
			c->scope->nextVarID++;
			if (c->child != nullptr)
			{
				if (c->child->lvalue)
				{
					out << "VARIN0, db" << c->child->reference->index << "," << std::endl;
					out << "VAROUT, db" << c->reference->index << "," << std::endl;
					out << "ASSIGN," << std::endl;
				}
				else
					GenerateExpression(c->child, out, data, c->reference->index);
			}
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
			v.index = 16 + nextVarID;
			++nextVarID;
		}

		ss << "INPUT VAR" << std::endl;
		ss << "\tINDEX \"" << v.index << "\"" << std::endl;
		ss << "\tNAME \"" << v.name << "\"" << std::endl;
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
		ss << "\tNAME \"" << v.name << "\"" << std::endl;
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


