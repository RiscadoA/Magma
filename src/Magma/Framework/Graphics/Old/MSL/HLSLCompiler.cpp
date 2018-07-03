#include "HLSLCompiler.hpp"
#include "../Context.hpp"

using namespace Magma::Framework::Graphics;
using namespace Magma::Framework::Graphics::MSL;

#include <sstream>
#include <iostream>
#include <functional>

struct Info
{
	ASTNode* tree;
	std::stringstream out;
	std::pair<std::string, FunctionDeclaration> currentFunc;
	std::string vertexOutID;
	std::set<std::string> textures2d;
	std::unordered_map<std::string, std::vector<VariableDeclaration>> constantBuffers;
	std::vector<VariableDeclaration> vertexOut;
};

std::string TypeToHLSLType(ASTNodeSymbol type)
{
	switch (type)
	{
		case ASTNodeSymbol::Int: return "int";
		case ASTNodeSymbol::Float: return "float";
		case ASTNodeSymbol::Vec2: return "float2";
		case ASTNodeSymbol::Vec3: return "float3";
		case ASTNodeSymbol::Vec4: return "float4";
		case ASTNodeSymbol::IVec2: return "int2";
		case ASTNodeSymbol::IVec3: return "int3";
		case ASTNodeSymbol::IVec4: return "int4";
		case ASTNodeSymbol::Mat2: return "float2x2";
		case ASTNodeSymbol::Mat3: return "float3x3";
		case ASTNodeSymbol::Mat4: return "float4x4";
		case ASTNodeSymbol::Bool: return "bool";
		default:
		{
			std::stringstream ss;
			ss << "Failed to compile MSL code:" << std::endl;
			ss << "Code generation stage failed:" << std::endl;
			ss << "Failed to convert MSL type into HLSL type";
			throw std::runtime_error(ss.str());
		} break;
	}
}

void GenerateScope(Info& info, ASTNode* node, int indentation);

void GenerateExpression(Info& info, ASTNode* expressionNode, int indentation = 0, bool isMemberOp = false)
{
	switch (expressionNode->symbol)
	{
		case ASTNodeSymbol::IntLiteral: info.out << expressionNode->attribute; break;
		case ASTNodeSymbol::FloatLiteral: info.out << expressionNode->attribute; break;
		case ASTNodeSymbol::True: info.out << "true"; break;
		case ASTNodeSymbol::False: info.out << "false"; break;
		case ASTNodeSymbol::Identifier:
		{
			if (isMemberOp)
			{
				info.out << expressionNode->attribute;
				break;
			}

			bool is = false;
			for (auto& p : info.currentFunc.second.params)
				if (p.name == expressionNode->attribute)
				{
					is = true;
					break;
				}

			if (is) // Is param
			{
				if (info.currentFunc.first == "VertexShader")
					info.out << expressionNode->attribute;
				else
					info.out << "param_" << expressionNode->attribute;
				break;
			}

			// Is local variable
			info.out << "local_" << expressionNode->attribute;
		} break;
		case ASTNodeSymbol::Member:
		{
			// Access vertex output
			if (expressionNode->firstChild->symbol == ASTNodeSymbol::Identifier && expressionNode->firstChild->attribute == info.vertexOutID)
			{
				info.out << "vertexOut.vertexOut_" << expressionNode->firstChild->next->attribute;
			}
			else
			{
				// Search for constant buffer
				bool foundConstantBuffer = false;
				for (auto& c : info.constantBuffers)
					if (expressionNode->firstChild->symbol == ASTNodeSymbol::Identifier && expressionNode->firstChild->attribute == c.first)
					{
						foundConstantBuffer = true;
						break;
					}

				// Access constant buffer
				if (foundConstantBuffer)
				{
					info.out << expressionNode->firstChild->attribute << "_";
					GenerateExpression(info, expressionNode->firstChild->next, indentation, true);
				}
				// Normal access
				else
				{
					info.out << "(";
					GenerateExpression(info, expressionNode->firstChild, indentation, false);
					info.out << ").";
					GenerateExpression(info, expressionNode->firstChild->next, indentation, true);
				}
			}
		} break;
		case ASTNodeSymbol::ArrayAccess:
		{
			GenerateExpression(info, expressionNode->firstChild, indentation, isMemberOp);
			info.out << "[";
			GenerateExpression(info, expressionNode->firstChild->next, indentation, true);
			info.out << "]";
		} break;

		case ASTNodeSymbol::Constructor:
		{
			info.out << TypeToHLSLType(expressionNode->firstChild->symbol) << "(";
			// Get params
			auto c = expressionNode->firstChild->next->firstChild;
			while (c != nullptr)
			{
				GenerateExpression(info, c, indentation);
				if (c != expressionNode->firstChild->next->lastChild)
					info.out << ", ";
				c = c->next;
			}

			info.out << ")";
		} break;
		case ASTNodeSymbol::Add:
		{
			// Is unary
			if (expressionNode->firstChild->next == nullptr)
			{
				info.out << "+(";
				GenerateExpression(info, expressionNode->firstChild, indentation);
				info.out << ")";
			}
			// If is binary
			else
			{
				info.out << "(";
				GenerateExpression(info, expressionNode->firstChild, indentation);
				info.out << ") + (";
				GenerateExpression(info, expressionNode->firstChild->next, indentation);
				info.out << ")";
			}
		} break;
		case ASTNodeSymbol::Sub:
		{
			// Is unary
			if (expressionNode->firstChild->next == nullptr)
			{
				info.out << "-(";
				GenerateExpression(info, expressionNode->firstChild, indentation);
				info.out << ")";
			}
			// If is binary
			else
			{
				info.out << "(";
				GenerateExpression(info, expressionNode->firstChild, indentation);
				info.out << ") - (";
				GenerateExpression(info, expressionNode->firstChild->next, indentation);
				info.out << ")";
			}
		} break;
		case ASTNodeSymbol::Mul:
		{
			info.out << "(";
			GenerateExpression(info, expressionNode->firstChild, indentation);
			info.out << ") * (";
			GenerateExpression(info, expressionNode->firstChild->next, indentation);
			info.out << ")";
		} break;
		case ASTNodeSymbol::Div:
		{
			info.out << "(";
			GenerateExpression(info, expressionNode->firstChild, indentation);
			info.out << ") / (";
			GenerateExpression(info, expressionNode->firstChild->next, indentation);
			info.out << ")";
		} break;
		case ASTNodeSymbol::Mod:
		{
			info.out << "(";
			GenerateExpression(info, expressionNode->firstChild, indentation);
			info.out << ") % (";
			GenerateExpression(info, expressionNode->firstChild->next, indentation);
			info.out << ")";
		} break;
		case ASTNodeSymbol::Assignment:
		{
			info.out << "(";
			GenerateExpression(info, expressionNode->firstChild, indentation);
			info.out << ") = (";
			GenerateExpression(info, expressionNode->firstChild->next, indentation);
			info.out << ")";
		} break;
		case ASTNodeSymbol::Call:
		{
			auto id = expressionNode->firstChild->attribute;
			auto paramsNode = expressionNode->firstChild->next;

			if (id == "Sample2D")
			{
				auto texNode = paramsNode->firstChild;
				if (texNode == nullptr || texNode->symbol != ASTNodeSymbol::Identifier)
				{
					std::stringstream ss;
					ss << "Failed to compile MSL code:" << std::endl;
					ss << "Code generation stage failed:" << std::endl;
					ss << "Couldn't call built in function \"Sample2D\":" << std::endl;
					ss << "First parameter must be a 2D texture identfier";
					throw std::runtime_error(ss.str());
				}

				if (info.textures2d.find(texNode->attribute) == info.textures2d.end())
				{
					std::stringstream ss;
					ss << "Failed to compile MSL code:" << std::endl;
					ss << "Code generation stage failed:" << std::endl;
					ss << "Couldn't call built in function \"Sample2D\":" << std::endl;
					ss << "No 2D texture \"" << texNode->attribute << "\" defined";
					throw std::runtime_error(ss.str());
				}

				auto uvsNode = texNode->next;
				if (uvsNode == nullptr)
				{
					std::stringstream ss;
					ss << "Failed to compile MSL code:" << std::endl;
					ss << "Code generation stage failed:" << std::endl;
					ss << "Couldn't call built in function \"Sample2D\":" << std::endl;
					ss << "Second parameter must be a vector containing the coordinates to sample the texture from";
					throw std::runtime_error(ss.str());
				}

				info.out << texNode->attribute << ".Sample(" << texNode->attribute << "_sampler, (";
				GenerateExpression(info, uvsNode, indentation);
				info.out << "))";
			}
			else if (id == "cos")
			{
				auto expNode = paramsNode->firstChild;
				info.out << "cos(";
				GenerateExpression(info, expNode, indentation);
				info.out << ")";
			}
			else if (id == "sin")
			{
				auto expNode = paramsNode->firstChild;
				info.out << "sin(";
				GenerateExpression(info, expNode, indentation);
				info.out << ")";
			}
			else if (id == "tan")
			{
				auto expNode = paramsNode->firstChild;
				info.out << "tan(";
				GenerateExpression(info, expNode, indentation);
				info.out << ")";
			}
			else if (id == "acos")
			{
				auto expNode = paramsNode->firstChild;
				info.out << "acos(";
				GenerateExpression(info, expNode, indentation);
				info.out << ")";
			}
			else if (id == "asin")
			{
				auto expNode = paramsNode->firstChild;
				info.out << "asin(";
				GenerateExpression(info, expNode, indentation);
				info.out << ")";
			}
			else if (id == "atan")
			{
				auto expNode = paramsNode->firstChild;
				info.out << "atan(";
				GenerateExpression(info, expNode, indentation);
				info.out << ")";
			}
			else if (id == "radians")
			{
				auto expNode = paramsNode->firstChild;
				info.out << "radians(";
				GenerateExpression(info, expNode, indentation);
				info.out << ")";
			}
			else if (id == "degrees")
			{
				auto expNode = paramsNode->firstChild;
				info.out << "degrees(";
				GenerateExpression(info, expNode, indentation);
				info.out << ")";
			}
			else if (id == "pow")
			{
				auto expNode = paramsNode->firstChild;
				auto exp2Node = expNode->next;
				info.out << "pow(";
				GenerateExpression(info, expNode, indentation);
				info.out << ", ";
				GenerateExpression(info, exp2Node, indentation);
				info.out << ")";
			}
			else if (id == "mul")
			{
				auto expNode = paramsNode->firstChild;
				auto exp2Node = expNode->next;
				info.out << "mul(";
				GenerateExpression(info, expNode, indentation);
				info.out << ", ";
				GenerateExpression(info, exp2Node, indentation);
				info.out << ")";
			}
			else if (id == "exp")
			{
				auto expNode = paramsNode->firstChild;
				info.out << "exp(";
				GenerateExpression(info, expNode, indentation);
				info.out << ")";
			}
			else if (id == "log")
			{
				auto expNode = paramsNode->firstChild;
				info.out << "log(";
				GenerateExpression(info, expNode, indentation);
				info.out << ")";
			}
			else if (id == "exp2")
			{
				auto expNode = paramsNode->firstChild;
				info.out << "exp2(";
				GenerateExpression(info, expNode, indentation);
				info.out << ")";
			}
			else if (id == "log2")
			{
				auto expNode = paramsNode->firstChild;
				info.out << "log2(";
				GenerateExpression(info, expNode, indentation);
				info.out << ")";
			}
			else if (id == "sqrt")
			{
				auto expNode = paramsNode->firstChild;
				info.out << "sqrt(";
				GenerateExpression(info, expNode, indentation);
				info.out << ")";
			}
			else if (id == "inversesqrt")
			{
				auto expNode = paramsNode->firstChild;
				info.out << "rsqrt(";
				GenerateExpression(info, expNode, indentation);
				info.out << ")";
			}
			else if (id == "abs")
			{
				auto expNode = paramsNode->firstChild;
				info.out << "abs(";
				GenerateExpression(info, expNode, indentation);
				info.out << ")";
			}
			else if (id == "sign")
			{
				auto expNode = paramsNode->firstChild;
				info.out << "sign(";
				GenerateExpression(info, expNode, indentation);
				info.out << ")";
			}
			else if (id == "floor")
			{
				auto expNode = paramsNode->firstChild;
				info.out << "floor(";
				GenerateExpression(info, expNode, indentation);
				info.out << ")";
			}
			else if (id == "ceil")
			{
				auto expNode = paramsNode->firstChild;
				info.out << "ceil(";
				GenerateExpression(info, expNode, indentation);
				info.out << ")";
			}
			else if (id == "fract")
			{
				auto expNode = paramsNode->firstChild;
				info.out << "frac(";
				GenerateExpression(info, expNode, indentation);
				info.out << ")";
			}
			else
			{
				info.out << "function_" << id << "(";
				auto param = paramsNode->firstChild;
				while (param != nullptr)
				{
					GenerateExpression(info, param, indentation);
					if (param != paramsNode->lastChild)
						info.out << ", ";
					param = paramsNode->next;
				}
				info.out << ")";
			}
		} break;
		case ASTNodeSymbol::Not:
		{
			info.out << "!(";
			GenerateExpression(info, expressionNode->firstChild, indentation);
			info.out << ")";
		} break;
		case ASTNodeSymbol::And:
		{
			info.out << "(";
			GenerateExpression(info, expressionNode->firstChild, indentation);
			info.out << ") && (";
			GenerateExpression(info, expressionNode->firstChild->next, indentation);
			info.out << ")";
		} break;
		case ASTNodeSymbol::Or:
		{
			info.out << "(";
			GenerateExpression(info, expressionNode->firstChild, indentation);
			info.out << ") || (";
			GenerateExpression(info, expressionNode->firstChild->next, indentation);
			info.out << ")";
		} break;
		case ASTNodeSymbol::EqualTo:
		{
			info.out << "(";
			GenerateExpression(info, expressionNode->firstChild, indentation);
			info.out << ") == (";
			GenerateExpression(info, expressionNode->firstChild->next, indentation);
			info.out << ")";
		} break;
		case ASTNodeSymbol::NotEqualTo:
		{
			info.out << "(";
			GenerateExpression(info, expressionNode->firstChild, indentation);
			info.out << ") != (";
			GenerateExpression(info, expressionNode->firstChild->next, indentation);
			info.out << ")";
		} break;
		case ASTNodeSymbol::LessThan:
		{
			info.out << "(";
			GenerateExpression(info, expressionNode->firstChild, indentation);
			info.out << ") < (";
			GenerateExpression(info, expressionNode->firstChild->next, indentation);
			info.out << ")";
		} break;
		case ASTNodeSymbol::GreaterThan:
		{
			info.out << "(";
			GenerateExpression(info, expressionNode->firstChild, indentation);
			info.out << ") > (";
			GenerateExpression(info, expressionNode->firstChild->next, indentation);
			info.out << ")";
		} break;
		case ASTNodeSymbol::LessThanOrEqualTo:
		{
			info.out << "(";
			GenerateExpression(info, expressionNode->firstChild, indentation);
			info.out << ") <= (";
			GenerateExpression(info, expressionNode->firstChild->next, indentation);
			info.out << ")";
		} break;
		case ASTNodeSymbol::GreaterThanOrEqualTo:
		{
			info.out << "(";
			GenerateExpression(info, expressionNode->firstChild, indentation);
			info.out << ") >= (";
			GenerateExpression(info, expressionNode->firstChild->next, indentation);
			info.out << ")";
		} break;
	}
}

void GenerateStatement(Info& info, ASTNode* node, int indentation = 0)
{
	auto statementType = node->firstChild;
	if (statementType == nullptr)
	{
		for (int i = 0; i < indentation; ++i)
			info.out << "\t";
		info.out << ";" << std::endl;
		return;
	}

	switch (statementType->symbol)
	{
		case ASTNodeSymbol::Return:
		{
			if (info.currentFunc.first == "VertexShader")
			{
				for (int i = 0; i < indentation; ++i)
					info.out << "\t";
				info.out << "vertexOut.vertexResult = ";
				GenerateExpression(info, statementType->next, indentation);
				info.out << ";" << std::endl;

				for (int i = 0; i < indentation; ++i)
					info.out << "\t";
				info.out << "return vertexOut;" << std::endl;
			}
			else
			{
				for (int i = 0; i < indentation; ++i)
					info.out << "\t";
				info.out << "return ";
				GenerateExpression(info, statementType->next, indentation);
				info.out << ";" << std::endl;
			}
		} break;
		case ASTNodeSymbol::Discard:
		{
			if (info.currentFunc.first != "PixelShader")
			{
				std::stringstream ss;
				ss << "Failed to compile MSL code:" << std::endl;
				ss << "Code generation stage failed:" << std::endl;
				ss << "Discard statements can only be inside pixel shader functions";
				throw std::runtime_error(ss.str());
			}

			for (int i = 0; i < indentation; ++i)
				info.out << "\t";
			info.out << "discard;" << std::endl;
		} break;
		case ASTNodeSymbol::Declaration:
		{
			if (statementType->next->next->next == nullptr)
			{
				for (int i = 0; i < indentation; ++i)
					info.out << "\t";
				info.out << TypeToHLSLType(statementType->next->symbol) << " local_" << statementType->next->next->attribute << ";" << std::endl;
			}
			else
			{
				for (int i = 0; i < indentation; ++i)
					info.out << "\t";
				info.out << TypeToHLSLType(statementType->next->symbol) << " local_" << statementType->next->next->attribute << " = " << "(";
				GenerateExpression(info, statementType->next->next->next, indentation);
				info.out << ");" << std::endl;
			}
		} break;
		case ASTNodeSymbol::If:
		{
			auto exp = statementType->next;

			for (int i = 0; i < indentation; ++i)
				info.out << "\t";
			info.out << "if (";
			GenerateExpression(info, exp, indentation);
			info.out << ")" << std::endl;

			auto ifBody = exp->next;
			if (ifBody->symbol == ASTNodeSymbol::Statement)
				GenerateStatement(info, ifBody, indentation + 1);
			else
				GenerateScope(info, ifBody, indentation);

			auto elseBody = ifBody->next;
			if (elseBody != nullptr)
			{
				for (int i = 0; i < indentation; ++i)
					info.out << "\t";
				info.out << "else" << std::endl;
				if (elseBody->symbol == ASTNodeSymbol::Statement)
					GenerateStatement(info, elseBody, indentation + 1);
				else
					GenerateScope(info, elseBody, indentation);
			}
		} break;
		case ASTNodeSymbol::While:
		{
			auto exp = statementType->next;
			auto body = exp->next;

			for (int i = 0; i < indentation; ++i)
				info.out << "\t";
			info.out << "while (";
			GenerateExpression(info, exp, indentation);
			info.out << ")" << std::endl;

			if (body->symbol == ASTNodeSymbol::Statement)
				GenerateStatement(info, body, indentation + 1);
			else
				GenerateScope(info, body, indentation);
		} break;
		case ASTNodeSymbol::Do:
		{
			auto body = statementType->next;
			auto exp = body->next;

			for (int i = 0; i < indentation; ++i)
				info.out << "\t";
			info.out << "do" << std::endl;
			if (body->symbol == ASTNodeSymbol::Statement)
				GenerateStatement(info, body, indentation + 1);
			else
				GenerateScope(info, body, indentation);

			for (int i = 0; i < indentation; ++i)
				info.out << "\t";
			info.out << "while (";
			GenerateExpression(info, exp, indentation);
			info.out << ");" << std::endl;

		} break;
		case ASTNodeSymbol::For:
		{
			auto decl = statementType->next;
			auto cExp = decl->next;
			auto iExp = cExp->next;
			auto body = iExp->next;

			for (int i = 0; i < indentation; ++i)
				info.out << "\t";
			info.out << "for (";
			GenerateStatement(info, decl, indentation);
			GenerateExpression(info, cExp, indentation);
			info.out << "; ";
			GenerateExpression(info, iExp, indentation);
			info.out << ")" << std::endl;

			if (body->symbol == ASTNodeSymbol::Statement)
				GenerateStatement(info, body, indentation);
			else
				GenerateScope(info, body, indentation);
		} break;
		default:
			for (int i = 0; i < indentation; ++i)
				info.out << "\t";
			GenerateExpression(info, statementType, indentation);
			info.out << ";" << std::endl;
			break;
	}
}

void GenerateScope(Info& info, ASTNode* node, int indentation = 0)
{
	for (int i = 0; i < indentation; ++i)
		info.out << "\t";
	info.out << "{" << std::endl;

	if (node->parent->symbol == ASTNodeSymbol::Function && info.currentFunc.first == "VertexShader")
	{
		for (int i = 0; i < indentation + 1; ++i)
			info.out << "\t";
		info.out << "VS_OUTPUT vertexOut;" << std::endl;
	}

	auto statement = node->firstChild;
	while (statement != nullptr)
	{
		if (statement->symbol == ASTNodeSymbol::Statement)
			GenerateStatement(info, statement, indentation + 1);
		else if (statement->symbol == ASTNodeSymbol::Scope)
			GenerateScope(info, statement, indentation + 1);
		else
		{
			std::stringstream ss;
			ss << "Failed to compile MSL code:" << std::endl;
			ss << "Code generation stage failed:" << std::endl;
			ss << "Failed to generate scope, invalid statement symbol";
			throw std::runtime_error(ss.str());
		}
		statement = statement->next;
	}

	for (int i = 0; i < indentation; ++i)
		info.out << "\t";
	info.out << "}" << std::endl;
}

void GenerateFunction(Info& info, const std::pair<std::string, FunctionDeclaration>& f, ShaderType type)
{
	info.currentFunc = f;
	if (type == ShaderType::Vertex)
	{
		info.out << "VS_OUTPUT VS(";
		for (size_t i = 0; i < f.second.params.size(); ++i)
		{
			info.out << TypeToHLSLType(f.second.params[i].type) << " " << f.second.params[i].name;
			info.out << " : " << f.second.params[i].name;
			if (i != f.second.params.size() - 1)
				info.out << ", ";
		}
		info.out << ")" << std::endl;
		GenerateScope(info, f.second.scope);
	}
	else if (type == ShaderType::Pixel)
	{
		info.out << TypeToHLSLType(f.second.returnType) << " PS(";
		info.out << "VS_OUTPUT vertexOut";
		info.out << ") : SV_TARGET" << std::endl;
		GenerateScope(info, f.second.scope);
	}
	else
	{
		info.out << TypeToHLSLType(f.second.returnType) << " function_" << f.first << "(";
		for (size_t i = 0; i < f.second.params.size(); ++i)
		{
			info.out << TypeToHLSLType(f.second.params[i].type) << " param_" << f.second.params[i].name;
			if (i != f.second.params.size() - 1)
				info.out << ", ";
		}
		info.out << ")" << std::endl;
		GenerateScope(info, f.second.scope);
	}
}

void Magma::Framework::Graphics::MSL::HLSLCompiler::GenerateCode()
{
	Info info;
	info.tree = m_astTree;
	info.out.str(std::string());
	info.out.clear();
	info.vertexOutID = m_vertexOutID;
	info.textures2d = m_2dtextures;
	info.vertexOut = m_vertexOut;
	info.constantBuffers = m_constantBuffers;

	// Write textures
	for (auto& t : m_2dtextures)
	{
		info.out << "Texture2D " << t << ";" << std::endl;
		info.out << "SamplerState " << t << "_sampler;" << std::endl;
	}
	if (!m_2dtextures.empty())
		info.out << std::endl;

	// Write constant buffers
	for (auto& c : m_constantBuffers)
	{
		info.out << "cbuffer " << c.first << std::endl;
		info.out << "{" << std::endl;
		for (auto& d : c.second)
		{
			if (d.isArray)
				info.out << "\t" << TypeToHLSLType(d.type) << " " << c.first << "_" << d.name << "[" << d.arraySize << "];" << std::endl;
			else
				info.out << "\t" << TypeToHLSLType(d.type) << " " << c.first << "_" << d.name << ";" << std::endl; 
		}
		info.out << "};" << std::endl;
	}
	if (!m_constantBuffers.empty())
		info.out << std::endl;

	// Write vertex output
	info.out << "struct VS_OUTPUT" << std::endl;
	info.out << "{" << std::endl;
	info.out << "\tfloat4 vertexResult : SV_POSITION;" << std::endl;
	for (auto& d : m_vertexOut)
	{
		if (d.isArray)
			info.out << "\t" << TypeToHLSLType(d.type) << " vertexOut_" << d.name << "[" << d.arraySize << "];" << std::endl;
		else
			info.out << "\t" << TypeToHLSLType(d.type) << " vertexOut_" << d.name << " : " << d.name << ";" << std::endl;
	}
	info.out << "};" << std::endl;
	info.out << std::endl;

	// Write functions
	for (auto& f : m_functions)
	{
		if (f.first == "VertexShader")
		{
			if (f.second.returnType != ASTNodeSymbol::Vec4)
			{
				std::stringstream ss;
				ss << "Failed to compile MSL code:" << std::endl;
				ss << "Code generation stage failed:" << std::endl;
				ss << "VertexShader entry function must return a vec4 containing the vertex position";
				throw std::runtime_error(ss.str());
			}
			
			GenerateFunction(info, f, ShaderType::Vertex);
		}
		else if (f.first == "PixelShader")
			GenerateFunction(info, f, ShaderType::Pixel);
		else
			GenerateFunction(info, f, ShaderType::Invalid);

		info.out << std::endl;
	}

	// Get output into string
	m_output = info.out.str();
}

Magma::Framework::Graphics::MSL::HLSLCompiler::HLSLCompiler()
{

}
