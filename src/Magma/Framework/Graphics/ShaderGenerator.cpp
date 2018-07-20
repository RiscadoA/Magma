#include "ShaderGenerator.hpp"
#include "../String/Conversion.hpp"

#include <sstream>
#include <map>

using namespace Magma::Framework;
using namespace Magma::Framework::Graphics;

struct Scope;

struct GeneratorInfo
{
	ShaderCompilerData& data;

	std::stringstream bc;
	std::stringstream md;

	Scope* rootScope;
	Scope* currentScope;

	std::pair<ShaderASTNodeSymbol, size_t> GetVar(const std::string& name);
	void OpenScope();
	void CloseScope();
	size_t Declare(ShaderASTNodeSymbol type, std::string name);
};

struct Scope
{
	Scope* parent;
	std::map<std::string, std::pair<ShaderASTNodeSymbol, size_t>> varIndexes;
	size_t nextVarIndex = 0;

	std::pair<ShaderASTNodeSymbol, size_t> GetVar(const std::string& name)
	{
		auto it = this->varIndexes.find(name);
		if (it == this->varIndexes.end())
		{
			if (parent == nullptr)
			{
				std::stringstream ss;
				ss << "Failed to run ShaderGenerator:" << std::endl;
				ss << "Couldn't find variable '" << name << "':";
				ss << "Variable isn't defined";
				throw ShaderError(ss.str());
			}
			else return parent->GetVar(name);
		}
		else return it->second;
	}
};

std::pair<ShaderASTNodeSymbol, size_t> GeneratorInfo::GetVar(const std::string& name)
{
	return currentScope->GetVar(name);
}

size_t GeneratorInfo::Declare(ShaderASTNodeSymbol type, std::string name)
{
	currentScope->varIndexes.insert(std::make_pair(name, std::make_pair(type, currentScope->nextVarIndex)));
	return currentScope->nextVarIndex++;
}

void GeneratorInfo::OpenScope()
{
	auto scope = new Scope();
	scope->parent = currentScope;
	scope->nextVarIndex = currentScope->nextVarIndex;
	scope->varIndexes = {};
	currentScope = scope;
}

void GeneratorInfo::CloseScope()
{
	auto old = currentScope;
	currentScope = currentScope->parent;
	delete old;
}

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

ShaderASTNodeSymbol ShaderVarTypeToASTNodeSymbol(ShaderVariableType symbol)
{
	switch (symbol)
	{
		case ShaderVariableType::Int1: return ShaderASTNodeSymbol::Int1;
		case ShaderVariableType::Int2: return ShaderASTNodeSymbol::Int2;
		case ShaderVariableType::Int3: return ShaderASTNodeSymbol::Int3;
		case ShaderVariableType::Int4: return ShaderASTNodeSymbol::Int4;
		case ShaderVariableType::Int22: return ShaderASTNodeSymbol::Int22;
		case ShaderVariableType::Int33: return ShaderASTNodeSymbol::Int33;
		case ShaderVariableType::Int44: return ShaderASTNodeSymbol::Int44;

		case ShaderVariableType::Float1: return ShaderASTNodeSymbol::Float1;
		case ShaderVariableType::Float2: return ShaderASTNodeSymbol::Float2;
		case ShaderVariableType::Float3: return ShaderASTNodeSymbol::Float3;
		case ShaderVariableType::Float4: return ShaderASTNodeSymbol::Float4;
		case ShaderVariableType::Float22: return ShaderASTNodeSymbol::Float22;
		case ShaderVariableType::Float33: return ShaderASTNodeSymbol::Float33;
		case ShaderVariableType::Float44: return ShaderASTNodeSymbol::Float44;

		default: return ShaderASTNodeSymbol::Invalid;
	}
}

void GenerateOperation(const ShaderASTNode* operation, size_t out, GeneratorInfo& info);

auto GetIdentifier(const ShaderASTNode* id, GeneratorInfo& info)
{
	if (id->symbol == ShaderASTNodeSymbol::Identifier)
		return info.GetVar(id->attribute);
	else if (id->symbol == ShaderASTNodeSymbol::Member)
	{
		if (id->child->attribute == info.data.inputID)
			return info.GetVar("in." + id->child->next->attribute);
		else if (id->child->attribute == info.data.outputID)
			return info.GetVar("out." + id->child->next->attribute);
		else
		{
			std::stringstream ss;
			ss << "Failed to run ShaderGenerator:" << std::endl;
			ss << "Failed to access member of identifier '" << id->child->attribute << "':" << std::endl;
			ss << "Unknown identifier" << std::endl;
			ss << "Line: " << id->lineNumber;
			throw ShaderError(ss.str());
		}
	}
}

void GenerateOperand(const ShaderASTNode* operand, size_t out, GeneratorInfo& info)
{
	if (operand->symbol == ShaderASTNodeSymbol::Identifier)
	{
		info.bc << "VARIN0, db" << info.GetVar(operand->attribute).second << "," << std::endl;
		info.bc << "VAROUT, db" << out << "," << std::endl;
		info.bc << "ASSIGN," << std::endl;
	}
	else if (operand->symbol == ShaderASTNodeSymbol::Member)
	{
		if (operand->child->symbol == ShaderASTNodeSymbol::Member)
			GenerateOperand(operand->child, out, info);

		//	Member
		//		Member
		//			in
		//			position
		//		x

		if (operand->child->attribute == info.data.inputID)
			
			operand->child->next->attribute;
	}
	else GenerateOperation(operand, out, info);

	/*
	
	switch (var.first)
	{
	case ShaderASTNodeSymbol::Int1: info.bc << "DECLI1, db" << varin1 << "," << std::endl;
	case ShaderASTNodeSymbol::Int2: info.bc << "DECLI2, db" << varin1 << "," << std::endl;
	case ShaderASTNodeSymbol::Int3: info.bc << "DECLI3, db" << varin1 << "," << std::endl;
	case ShaderASTNodeSymbol::Int4: info.bc << "DECLI4, db" << varin1 << "," << std::endl;
	case ShaderASTNodeSymbol::Int22: info.bc << "DECLI22, db" << varin1 << "," << std::endl;
	case ShaderASTNodeSymbol::Int33: info.bc << "DECLI33, db" << varin1 << "," << std::endl;
	case ShaderASTNodeSymbol::Int44: info.bc << "DECLI44, db" << varin1 << "," << std::endl;

	case ShaderASTNodeSymbol::Float1: info.bc << "DECLF1, db" << varin1 << "," << std::endl;
	case ShaderASTNodeSymbol::Float2: info.bc << "DECLF2, db" << varin1 << "," << std::endl;
	case ShaderASTNodeSymbol::Float3: info.bc << "DECLF3, db" << varin1 << "," << std::endl;
	case ShaderASTNodeSymbol::Float4: info.bc << "DECLF4, db" << varin1 << "," << std::endl;
	case ShaderASTNodeSymbol::Float22: info.bc << "DECLF22, db" << varin1 << "," << std::endl;
	case ShaderASTNodeSymbol::Float33: info.bc << "DECLF33, db" << varin1 << "," << std::endl;
	case ShaderASTNodeSymbol::Float44: info.bc << "DECLF44, db" << varin1 << "," << std::endl;

	default: throw ShaderError("Failed to run ShaderGenerator:\nUnsupported variable type on operation (second operand)");
	}

	*/
}

void GenerateOperation(const ShaderASTNode* operation, size_t out, GeneratorInfo& info)
{
	if (operation->symbol == ShaderASTNodeSymbol::Identifier ||
		operation->symbol == ShaderASTNodeSymbol::Member)
	{
		GenerateOperand(operation, out, info);
		return;
	}

	// Temp vars to store the operands
	size_t varout = out;
	
	size_t varin0 = -1;
	{
		if (operation->child->symbol == ShaderASTNodeSymbol::Identifier ||
			operation->child->symbol == ShaderASTNodeSymbol::Member)
			varin0 = GetIdentifier(operation->child, info).second;
		else
		{
			varin0 = info.currentScope->nextVarIndex++;
			GenerateOperation(operation->child, varin0, info);
		}
	}

	size_t varin1 = -1;
	if (operation->child->next != nullptr)
	{
		if (operation->child->next->symbol == ShaderASTNodeSymbol::Identifier ||
			operation->child->next->symbol == ShaderASTNodeSymbol::Member)
			varin1 = GetIdentifier(operation->child->next, info).second;
		else
		{
			varin1 = info.currentScope->nextVarIndex++;
			GenerateOperation(operation->child->next, varin1, info);
		}
	}

	// Assignment operation
	if (operation->symbol == ShaderASTNodeSymbol::Assign)
	{
		info.bc << "VARIN0, db" << varin1 << "," << std::endl;
		info.bc << "VAROUT, db" << varin0 << "," << std::endl;
		info.bc << "ASSIGN," << std::endl;

		if (out != -1)
		{
			info.bc << "VAROUT, db" << out << "," << std::endl;
			info.bc << "ASSIGN," << std::endl;
		}
	}
	// Other operations
	else
	{
		// Set operands
		info.bc << "VARIN0, db" << varin0 << "," << std::endl;
		if (varin1 != -1)
			info.bc << "VARIN1, db" << varin1 << "," << std::endl;
		info.bc << "VAROUT, db" << varout << "," << std::endl;

		switch (operation->symbol)
		{
			// TO DO: add other operations
			default:
				throw ShaderError("Failed to run ShaderGenerator:\nUnsupported operation symbol");
				break;
		}
	}
}

void GenerateExpression(const ShaderASTNode* expression, size_t out, GeneratorInfo& info)
{
	info.OpenScope();
	info.bc << "OPSCOPE," << std::endl;

	GenerateOperation(expression, out, info);

	if (out != -1)
	{
		// TO DO
	}

	info.bc << "CLSCOPE," << std::endl;
	info.CloseScope();
}

void GenerateStatement(const ShaderASTNode* statement, GeneratorInfo& info)
{
	if (statement->symbol == ShaderASTNodeSymbol::Declaration)
	{
		auto type = statement->child->symbol;
		auto name = statement->child->next->attribute;

		auto id = info.Declare(type, name);

		switch (type)
		{
			case ShaderASTNodeSymbol::Int1: info.bc << "DECLI1, db" << id << "," << std::endl;
			case ShaderASTNodeSymbol::Int2: info.bc << "DECLI2, db" << id << "," << std::endl;
			case ShaderASTNodeSymbol::Int3: info.bc << "DECLI3, db" << id << "," << std::endl;
			case ShaderASTNodeSymbol::Int4: info.bc << "DECLI4, db" << id << "," << std::endl;
			case ShaderASTNodeSymbol::Int22: info.bc << "DECLI22, db" << id << "," << std::endl;
			case ShaderASTNodeSymbol::Int33: info.bc << "DECLI33, db" << id << "," << std::endl;
			case ShaderASTNodeSymbol::Int44: info.bc << "DECLI44, db" << id << "," << std::endl;

			case ShaderASTNodeSymbol::Float1: info.bc << "DECLF1, db" << id << "," << std::endl;
			case ShaderASTNodeSymbol::Float2: info.bc << "DECLF2, db" << id << "," << std::endl;
			case ShaderASTNodeSymbol::Float3: info.bc << "DECLF3, db" << id << "," << std::endl;
			case ShaderASTNodeSymbol::Float4: info.bc << "DECLF4, db" << id << "," << std::endl;
			case ShaderASTNodeSymbol::Float22: info.bc << "DECLF22, db" << id << "," << std::endl;
			case ShaderASTNodeSymbol::Float33: info.bc << "DECLF33, db" << id << "," << std::endl;
			case ShaderASTNodeSymbol::Float44: info.bc << "DECLF44, db" << id << "," << std::endl;

			default: throw ShaderError("Failed to run ShaderGenerator:\nUnsupported variable type on declaration");
		}

		// If has definition
		if (statement->child->next->next != nullptr)
			GenerateExpression(statement->child->next->next, id, info);
	}
	else GenerateExpression(statement, -1, info);
}

void GenerateScope(const ShaderASTNode* scope, GeneratorInfo& info)
{
	info.OpenScope();
	info.bc << "OPSCOPE," << std::endl;

	auto c = scope->child;
	while (c != nullptr)
	{
		GenerateStatement(c, info);
		c = c->next;
	}

	info.bc << "CLSCOPE," << std::endl;
	info.CloseScope();
}

void GenerateShader(const ShaderASTNode* shader, GeneratorInfo& info)
{
	auto scope = shader->child;
	GenerateScope(scope, info);
}

void Magma::Framework::Graphics::ShaderGenerator::Run(const ShaderASTNode * in, std::string& outBC, std::string& outMD, ShaderCompilerData& data)
{
	GeneratorInfo info
	{
		data,
		std::stringstream(""),
		std::stringstream(""),
	};

	info.rootScope = new Scope();
	info.rootScope->parent = nullptr;
	info.rootScope->nextVarIndex = 32;
	info.rootScope->varIndexes = {};
	info.currentScope = info.rootScope;

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
				info.rootScope->varIndexes.insert(std::make_pair("out." + i.first, std::make_pair(ShaderVarTypeToASTNodeSymbol(i.second.type), i.second.index)));
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

			info.rootScope->varIndexes.insert(std::make_pair("out." + i.first, std::make_pair(ShaderVarTypeToASTNodeSymbol(i.second.type), i.second.index)));
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
				i.second.index = info.rootScope->nextVarIndex;
				info.rootScope->nextVarIndex++;
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

			info.rootScope->varIndexes.insert(std::make_pair("in." + i.first, std::make_pair(ShaderVarTypeToASTNodeSymbol(i.second.type), i.second.index)));
		}
	}

	// Generate BC
	GenerateShader(in, info);

	outBC = info.bc.str();
	outMD = info.md.str();

	delete info.rootScope;
}


