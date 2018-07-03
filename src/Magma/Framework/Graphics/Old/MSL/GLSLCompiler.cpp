#include "GLSLCompiler.hpp"

using namespace Magma::Framework::Graphics::MSL;

#include <sstream>
#include <iostream>
#include <functional>

Magma::Framework::Graphics::MSL::GLSLCompiler::GLSLCompiler()
{

}

std::string TypeToGLSLType(ASTNodeSymbol type)
{
	switch (type)
	{
		case ASTNodeSymbol::Int: return "int";
		case ASTNodeSymbol::Float: return "float";
		case ASTNodeSymbol::Vec2: return "vec2";
		case ASTNodeSymbol::Vec3: return "vec3";
		case ASTNodeSymbol::Vec4: return "vec4";
		case ASTNodeSymbol::IVec2: return "ivec2";
		case ASTNodeSymbol::IVec3: return "ivec3";
		case ASTNodeSymbol::IVec4: return "ivec4";
		case ASTNodeSymbol::Mat2: return "mat2";
		case ASTNodeSymbol::Mat3: return "mat3";
		case ASTNodeSymbol::Mat4: return "mat4";
		case ASTNodeSymbol::Bool: return "bool";
		default:
		{
			std::stringstream ss;
			ss << "Failed to compile MSL code:" << std::endl;
			ss << "Code generation stage failed:" << std::endl;
			ss << "Failed to convert MSL type into GLSL type";
			throw std::runtime_error(ss.str());
		} break;
	}
}

void Magma::Framework::Graphics::MSL::GLSLCompiler::GenerateCode()
{
	std::stringstream out;

	std::function<void(const std::pair<std::string, FunctionDeclaration>&, ASTNode*, ShaderType, int)> generateScope;

	std::function<void(const std::pair<std::string, FunctionDeclaration>&, ASTNode*, ShaderType, int, bool)> generateExpression = [&, this](const std::pair<std::string, FunctionDeclaration>& f, ASTNode* expressionNode, ShaderType type, int indentation, bool isMemberOp) -> void
	{
		switch (expressionNode->symbol)
		{
			case ASTNodeSymbol::IntLiteral: out << expressionNode->attribute; break;
			case ASTNodeSymbol::FloatLiteral: out << expressionNode->attribute; break;
			case ASTNodeSymbol::True: out << "true"; break;
			case ASTNodeSymbol::False: out << "false"; break;
			case ASTNodeSymbol::Identifier:
			{
				if (isMemberOp)
				{
					out << expressionNode->attribute;
					break;
				}

				bool is = false;
				for (auto& p : f.second.params)
					if (p.name == expressionNode->attribute)
					{
						is = true;
						break;
					}

				if (is) // Is param
				{
					if (f.first == "VertexShader")
						out << expressionNode->attribute;
					else
						out << "param_" << expressionNode->attribute;
					break;
				}
				
				// Is local variable
				out << "local_" << expressionNode->attribute;
			} break;
			case ASTNodeSymbol::Member:
			{
				// Access vertex output
				if (expressionNode->firstChild->symbol == ASTNodeSymbol::Identifier && expressionNode->firstChild->attribute == m_vertexOutID)
				{
					out << "vertexOut_" << expressionNode->firstChild->next->attribute;
				}
				
				else
				{
					// Search for constant buffer
					bool foundConstantBuffer = false;
					for (auto& c : m_constantBuffers)
						if (expressionNode->firstChild->symbol == ASTNodeSymbol::Identifier && expressionNode->firstChild->attribute == c.first)
						{
							foundConstantBuffer = true;
							break;
						}

					// Access constant buffer
					if (foundConstantBuffer)
					{
						out << expressionNode->firstChild->attribute << "_";
						generateExpression(f, expressionNode->firstChild->next, type, indentation, true);
					}
					// Normal access
					else
					{
						out << "(";
						generateExpression(f, expressionNode->firstChild, type, indentation, false);
						out << ").";
						generateExpression(f, expressionNode->firstChild->next, type, indentation, true);
					}
				}

				break;
			}
			case ASTNodeSymbol::ArrayAccess:
			{
				generateExpression(f, expressionNode->firstChild, type, indentation, isMemberOp);
				out << "[";
				generateExpression(f, expressionNode->firstChild->next, type, indentation, true);
				out << "]";
			} break;
			case ASTNodeSymbol::Constructor:
			{
				out << TypeToGLSLType(expressionNode->firstChild->symbol) << "(";
				// Get params
				auto c = expressionNode->firstChild->next->firstChild;
				while (c != nullptr)
				{
					generateExpression(f, c, type, indentation, false);
					if (c != expressionNode->firstChild->next->lastChild)
						out << ", ";
					c = c->next;
				}

				out << ")";
			} break;
			case ASTNodeSymbol::Add:
			{
				// Is unary
				if (expressionNode->firstChild->next == nullptr)
				{
					out << "+(";
					generateExpression(f, expressionNode->firstChild, type, indentation, false);
					out << ")";
				}
				// If is binary
				else
				{
					out << "(";
					generateExpression(f, expressionNode->firstChild, type, indentation, false);
					out << ") + (";
					generateExpression(f, expressionNode->firstChild->next, type, indentation, false);
					out << ")";
				}
			} break;
			case ASTNodeSymbol::Sub:
			{
				// Is unary
				if (expressionNode->firstChild->next == nullptr)
				{
					out << "-(";
					generateExpression(f, expressionNode->firstChild, type, indentation, false);
					out << ")";
				}
				// If is binary
				else
				{
					out << "(";
					generateExpression(f, expressionNode->firstChild, type, indentation, false);
					out << ") - (";
					generateExpression(f, expressionNode->firstChild->next, type, indentation, false);
					out << ")";
				}
			} break;
			case ASTNodeSymbol::Mul:
			{
				out << "(";
				generateExpression(f, expressionNode->firstChild, type, indentation, false);
				out << ") * (";
				generateExpression(f, expressionNode->firstChild->next, type, indentation, false);
				out << ")";
			} break;
			case ASTNodeSymbol::Div:
			{
				out << "(";
				generateExpression(f, expressionNode->firstChild, type, indentation, false);
				out << ") / (";
				generateExpression(f, expressionNode->firstChild->next, type, indentation, false);
				out << ")";
			} break;
			case ASTNodeSymbol::Mod:
			{
				out << "mod(";
				generateExpression(f, expressionNode->firstChild, type, indentation, false);
				out << ", ";
				generateExpression(f, expressionNode->firstChild->next, type, indentation, false);
				out << ")";
			} break;
			case ASTNodeSymbol::Assignment:
			{
				out << "(";
				generateExpression(f, expressionNode->firstChild, type, indentation, false);
				out << ") = (";
				generateExpression(f, expressionNode->firstChild->next, type, indentation, false);
				out << ")";
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

					if (m_2dtextures.find(texNode->attribute) == m_2dtextures.end())
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

					out << "texture(" << texNode->attribute << ", (";
					generateExpression(f, uvsNode, type, indentation, false);
					out << "))";
				}
				else if (id == "cos")
				{
					auto expNode = paramsNode->firstChild;
					out << "cos(";
					generateExpression(f, expNode, type, indentation, false);
					out << ")";
				}
				else if (id == "sin")
				{
					auto expNode = paramsNode->firstChild;
					out << "sin(";
					generateExpression(f, expNode, type, indentation, false);
					out << ")";
				}
				else if (id == "tan")
				{
					auto expNode = paramsNode->firstChild;
					out << "tan(";
					generateExpression(f, expNode, type, indentation, false);
					out << ")";
				}
				else if (id == "acos")
				{
					auto expNode = paramsNode->firstChild;
					out << "acos(";
					generateExpression(f, expNode, type, indentation, false);
					out << ")";
				}
				else if (id == "asin")
				{
					auto expNode = paramsNode->firstChild;
					out << "asin(";
					generateExpression(f, expNode, type, indentation, false);
					out << ")";
				}
				else if (id == "atan")
				{
					auto expNode = paramsNode->firstChild;
					out << "atan(";
					generateExpression(f, expNode, type, indentation, false);
					out << ")";
				}
				else if (id == "radians")
				{
					auto expNode = paramsNode->firstChild;
					out << "radians(";
					generateExpression(f, expNode, type, indentation, false);
					out << ")";
				}
				else if (id == "degrees")
				{
					auto expNode = paramsNode->firstChild;
					out << "degrees(";
					generateExpression(f, expNode, type, indentation, false);
					out << ")";
				}
				else if (id == "pow")
				{
					auto expNode = paramsNode->firstChild;
					auto exp2Node = expNode->next;
					out << "pow(";
					generateExpression(f, expNode, type, indentation, false);
					out << ", ";
					generateExpression(f, exp2Node, type, indentation, false);
					out << ")";
				}
				else if (id == "mul")
				{
					auto expNode = paramsNode->firstChild;
					auto exp2Node = expNode->next;
					out << "(";
					generateExpression(f, expNode, type, indentation, false);
					out << ") * (";
					generateExpression(f, exp2Node, type, indentation, false);
					out << ")";
				}
				else if (id == "exp")
				{
					auto expNode = paramsNode->firstChild;
					out << "exp(";
					generateExpression(f, expNode, type, indentation, false);
					out << ")";
				}
				else if (id == "log")
				{
					auto expNode = paramsNode->firstChild;
					out << "log(";
					generateExpression(f, expNode, type, indentation, false);
					out << ")";
				}
				else if (id == "exp2")
				{
					auto expNode = paramsNode->firstChild;
					out << "exp2(";
					generateExpression(f, expNode, type, indentation, false);
					out << ")";
				}
				else if (id == "log2")
				{
					auto expNode = paramsNode->firstChild;
					out << "log2(";
					generateExpression(f, expNode, type, indentation, false);
					out << ")";
				}
				else if (id == "sqrt")
				{
					auto expNode = paramsNode->firstChild;
					out << "sqrt(";
					generateExpression(f, expNode, type, indentation, false);
					out << ")";
				}
				else if (id == "inversesqrt")
				{
					auto expNode = paramsNode->firstChild;
					out << "inversesqrt(";
					generateExpression(f, expNode, type, indentation, false);
					out << ")";
				}
				else if (id == "abs")
				{
					auto expNode = paramsNode->firstChild;
					out << "abs(";
					generateExpression(f, expNode, type, indentation, false);
					out << ")";
				}
				else if (id == "sign")
				{
					auto expNode = paramsNode->firstChild;
					out << "sign(";
					generateExpression(f, expNode, type, indentation, false);
					out << ")";
				}
				else if (id == "floor")
				{
					auto expNode = paramsNode->firstChild;
					out << "floor(";
					generateExpression(f, expNode, type, indentation, false);
					out << ")";
				}
				else if (id == "ceil")
				{
					auto expNode = paramsNode->firstChild;
					out << "ceil(";
					generateExpression(f, expNode, type, indentation, false);
					out << ")";
				}
				else if (id == "fract")
				{
					auto expNode = paramsNode->firstChild;
					out << "fract(";
					generateExpression(f, expNode, type, indentation, false);
					out << ")";
				}
				else
				{
					out << "function_" << id << "(";
					auto param = paramsNode->firstChild;
					while (param != nullptr)
					{
						generateExpression(f, param, type, indentation, false);
						if (param != paramsNode->lastChild)
							out << ", ";
						param = paramsNode->next;
					}
					out << ")";
				}
			} break;
			case ASTNodeSymbol::Not:
			{
				out << "!(";
				generateExpression(f, expressionNode->firstChild, type, indentation, false);
				out << ")";
			} break;
			case ASTNodeSymbol::And:
			{
				out << "(";
				generateExpression(f, expressionNode->firstChild, type, indentation, false);
				out << ") && (";
				generateExpression(f, expressionNode->firstChild->next, type, indentation, false);
				out << ")";
			} break;
			case ASTNodeSymbol::Or:
			{
				out << "(";
				generateExpression(f, expressionNode->firstChild, type, indentation, false);
				out << ") || (";
				generateExpression(f, expressionNode->firstChild->next, type, indentation, false);
				out << ")";
			} break;
			case ASTNodeSymbol::EqualTo:
			{
				out << "(";
				generateExpression(f, expressionNode->firstChild, type, indentation, false);
				out << ") == (";
				generateExpression(f, expressionNode->firstChild->next, type, indentation, false);
				out << ")";
			} break;
			case ASTNodeSymbol::NotEqualTo:
			{
				out << "(";
				generateExpression(f, expressionNode->firstChild, type, indentation, false);
				out << ") != (";
				generateExpression(f, expressionNode->firstChild->next, type, indentation, false);
				out << ")";
			} break;
			case ASTNodeSymbol::LessThan:
			{
				out << "(";
				generateExpression(f, expressionNode->firstChild, type, indentation, false);
				out << ") < (";
				generateExpression(f, expressionNode->firstChild->next, type, indentation, false);
				out << ")";
			} break;
			case ASTNodeSymbol::GreaterThan:
			{
				out << "(";
				generateExpression(f, expressionNode->firstChild, type, indentation, false);
				out << ") > (";
				generateExpression(f, expressionNode->firstChild->next, type, indentation, false);
				out << ")";
			} break;
			case ASTNodeSymbol::LessThanOrEqualTo:
			{
				out << "(";
				generateExpression(f, expressionNode->firstChild, type, indentation, false);
				out << ") <= (";
				generateExpression(f, expressionNode->firstChild->next, type, indentation, false);
				out << ")";
			} break;
			case ASTNodeSymbol::GreaterThanOrEqualTo:
			{
				out << "(";
				generateExpression(f, expressionNode->firstChild, type, indentation, false);
				out << ") >= (";
				generateExpression(f, expressionNode->firstChild->next, type, indentation, false);
				out << ")";
			} break;
		}
	};

	std::function<void(const std::pair<std::string, FunctionDeclaration>&, ASTNode*, ShaderType, int)> generateStatement = [&, this](const std::pair<std::string, FunctionDeclaration>& f, ASTNode* statementNode, ShaderType type, int indentation) -> void
	{
		auto typeNode = statementNode->firstChild;
		if (typeNode == nullptr)
		{
			for (int i = 0; i < indentation; ++i)
				out << "\t";
			out << ";" << std::endl;
			return;
		}

		switch (typeNode->symbol)
		{
			case ASTNodeSymbol::Return:
			{
				if (type == ShaderType::Vertex)
				{
					for (int i = 0; i < indentation; ++i)
						out << "\t";
					out << "gl_Position = ";
					generateExpression(f, typeNode->next, type, indentation, false);
					out << ";" << std::endl;
					for (int i = 0; i < indentation; ++i)
						out << "\t";
					out << "return;" << std::endl;
				}
				else if (type == ShaderType::Pixel)
				{
					for (int i = 0; i < indentation; ++i)
						out << "\t";
					out << "fragOut = ";
					generateExpression(f, typeNode->next, type, indentation, false);
					out << ";" << std::endl;
					for (int i = 0; i < indentation; ++i)
						out << "\t";
					out << "return;" << std::endl;
				}
				else
				{
					for (int i = 0; i < indentation; ++i)
						out << "\t";
					out << "return ";
					generateExpression(f, typeNode->next, type, indentation, false);
					out << ";" << std::endl;
				}
			} break;
			case ASTNodeSymbol::Discard:
			{
				if (type != ShaderType::Pixel)
				{
					std::stringstream ss;
					ss << "Failed to compile MSL code:" << std::endl;
					ss << "Code generation stage failed:" << std::endl;
					ss << "Discard statements can only be inside pixel shader functions";
					throw std::runtime_error(ss.str());
				}

				for (int i = 0; i < indentation; ++i)
					out << "\t";
				out << "discard;" << std::endl;
			} break;
			case ASTNodeSymbol::Declaration:
			{
				if (typeNode->next->next->next == nullptr)
				{
					for (int i = 0; i < indentation; ++i)
						out << "\t";
					out << TypeToGLSLType(typeNode->next->symbol) << " local_" << typeNode->next->next->attribute << ";" << std::endl;
				}
				else
				{
					for (int i = 0; i < indentation; ++i)
						out << "\t";
					out << TypeToGLSLType(typeNode->next->symbol) << " local_" << typeNode->next->next->attribute << " = " << "(";
					generateExpression(f, typeNode->next->next->next, type, indentation, false);
					out << ");" << std::endl;
				}
			} break;
			case ASTNodeSymbol::If:
			{
				auto exp = typeNode->next;

				for (int i = 0; i < indentation; ++i)
					out << "\t";
				out << "if (";
				generateExpression(f, exp, type, indentation, false);
				out << ")" << std::endl;

				auto ifBody = exp->next;
				if (ifBody->symbol == ASTNodeSymbol::Statement)
					generateStatement(f, ifBody, type, indentation + 1);
				else
					generateScope(f, ifBody, type, indentation);

				auto elseBody = ifBody->next;
				if (elseBody != nullptr)
				{
					for (int i = 0; i < indentation; ++i)
						out << "\t";
					out << "else" << std::endl;
					if (elseBody->symbol == ASTNodeSymbol::Statement)
						generateStatement(f, elseBody, type, indentation + 1);
					else
						generateScope(f, elseBody, type, indentation);
				}
			} break;
			case ASTNodeSymbol::While:
			{
				auto exp = typeNode->next;
				auto body = exp->next;

				for (int i = 0; i < indentation; ++i)
					out << "\t";
				out << "while (";
				generateExpression(f, exp, type, indentation, false);
				out << ")" << std::endl;

				if (body->symbol == ASTNodeSymbol::Statement)
					generateStatement(f, body, type, indentation + 1);
				else
					generateScope(f, body, type, indentation);
			} break;
			case ASTNodeSymbol::Do:
			{
				auto body = typeNode->next;
				auto exp = body->next;

				for (int i = 0; i < indentation; ++i)
					out << "\t";
				out << "do" << std::endl;
				if (body->symbol == ASTNodeSymbol::Statement)
					generateStatement(f, body, type, indentation + 1);
				else
					generateScope(f, body, type, indentation);

				for (int i = 0; i < indentation; ++i)
					out << "\t";
				out << "while (";
				generateExpression(f, exp, type, indentation, false);
				out << ");" << std::endl;
				
			} break;
			case ASTNodeSymbol::For:
			{
				auto decl = typeNode->next;
				auto cExp = decl->next;
				auto iExp = cExp->next;
				auto body = iExp->next;

				for (int i = 0; i < indentation; ++i)
					out << "\t";
				out << "for (";
				generateStatement(f, decl, type, indentation);
				generateExpression(f, cExp, type, indentation, false);
				out << "; ";
				generateExpression(f, iExp, type, indentation, false);
				out << ")" << std::endl;

				if (body->symbol == ASTNodeSymbol::Statement)
					generateStatement(f, body, type, indentation + 1);
				else
					generateScope(f, body, type, indentation);
			} break;
			default:
				for (int i = 0; i < indentation; ++i)
					out << "\t";
				generateExpression(f, typeNode, type, indentation, false);
				out << ";" << std::endl;
				break;
		}
	};
	
	generateScope = [&, this](const std::pair<std::string, FunctionDeclaration>& f, ASTNode* scopeNode, ShaderType type, int indentation) -> void
	{
		for (int i = 0; i < indentation; ++i)
			out << "\t";
		out << "{" << std::endl;
		ASTNode* c = scopeNode->firstChild;
		while (c != nullptr)
		{
			switch (c->symbol)
			{
				case ASTNodeSymbol::Statement: generateStatement(f, c, type, indentation + 1); break;
				case ASTNodeSymbol::Scope: generateScope(f, c, type, indentation + 1); break;
			}
			
			c = c->next;
		}
		for (int i = 0; i < indentation; ++i)
			out << "\t";
		out << "}" << std::endl;
	};

	auto generateFunction = [&, this](const std::pair<std::string, FunctionDeclaration>& f, ShaderType type) -> void
	{
		if (type == ShaderType::Invalid)
		{
			out << TypeToGLSLType(f.second.returnType) << " function_" << f.first << "(";
			for (size_t i = 0; i < f.second.params.size(); ++i)
				out << TypeToGLSLType(f.second.params[i].type) << " param_" << f.second.params[i].name << (i == f.second.params.size() - 1 ? ")" : ", ");
		}
		else out << "void main()";
		out << std::endl;
		generateScope(f, f.second.scope, type, 0);
		out << std::endl;
	};

	out << "#version 330 core" << std::endl << std::endl;
	out << "// This file was automatically generated by the MSL compiler" << std::endl;
	out << "// " << (m_shaderType == ShaderType::Vertex ? "Vertex" : "Pixel") << " Shader" << std::endl << std::endl;

	// Add textures
	for (auto& t : m_2dtextures)
		out << "uniform sampler2D " << t << ";" << std::endl;
	if (!m_2dtextures.empty())
		out << std::endl;

	// Add constant buffers
	for (auto& c : m_constantBuffers)
	{
		out << "layout (std140) uniform " << c.first << std::endl << "{" << std::endl;
		for (auto& d : c.second)
		{
			if (d.isArray)
				out << "\t" << TypeToGLSLType(d.type) << " " << c.first << "_" << d.name << "[" << d.arraySize << "];" << std::endl;
			else
				out << "\t" << TypeToGLSLType(d.type) << " " << c.first << "_" << d.name << ";" << std::endl;
		}
		out << "};" << std::endl;
	}

	if (!m_constantBuffers.empty())
		out << std::endl;

	switch (m_shaderType)
	{
		case ShaderType::Vertex:
		{
			auto it = m_functions.begin();
			for (; it != m_functions.end(); ++it)
				if (it->first == "VertexShader")
					break;

			const auto& params = it->second.params;
			for (const auto& p : params)
				out << "in " << TypeToGLSLType(p.type) << " " << p.name << ";" << std::endl;
			out << std::endl;
			for (auto& v : m_vertexOut)
			{
				if (v.type == ASTNodeSymbol::Int ||
					v.type == ASTNodeSymbol::IVec2 ||
					v.type == ASTNodeSymbol::IVec3 ||
					v.type == ASTNodeSymbol::IVec4)
					out << "flat ";

				out << "out ";

				if (v.isArray)
					out << TypeToGLSLType(v.type) << " vertexOut_" << v.name << "[" << v.arraySize << "];" << std::endl;
				else
					out << TypeToGLSLType(v.type) << " vertexOut_" << v.name << ";" << std::endl;
			}
			out << std::endl;
		} break;
		case ShaderType::Pixel:
		{
			auto it = m_functions.begin();
			for (; it != m_functions.end(); ++it)
				if (it->first == "PixelShader")
					break;

			for (auto& v : m_vertexOut)
			{
				if (v.type == ASTNodeSymbol::Int ||
					v.type == ASTNodeSymbol::IVec2 ||
					v.type == ASTNodeSymbol::IVec3 ||
					v.type == ASTNodeSymbol::IVec4 )
					out << "flat ";

				out << "in ";

				if (v.isArray)
					out << TypeToGLSLType(v.type) << " vertexOut_" << v.name << "[" << v.arraySize << "];" << std::endl;
				else
					out << TypeToGLSLType(v.type) << " vertexOut_" << v.name << ";" << std::endl;
			}
			out << std::endl;
			out << "out " << TypeToGLSLType(it->second.returnType) << " fragOut;" << std::endl;
			out << std::endl;
			break;
		}
	}

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
			if (m_shaderType == ShaderType::Vertex)
				generateFunction(f, ShaderType::Vertex);
		}
		else if (f.first == "PixelShader")
		{
			if (m_shaderType == ShaderType::Pixel)
				generateFunction(f, ShaderType::Pixel);
		}
		else
			generateFunction(f, ShaderType::Invalid);
	}

	m_output = out.str();
}
