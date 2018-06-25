#include "GLSLCompiler.hpp"

using namespace Magma::Framework::Graphics::MSL;

#include <sstream>
#include <iostream>
#include <functional>

const int MajorVersion = 1;
const int MinorVersion = 3;
const int PatchVersion = 6;

Magma::Framework::Graphics::MSL::GLSLCompiler::GLSLCompiler()
	: Compiler(MajorVersion, MinorVersion, PatchVersion)
{

}

std::string TypeToGLType(ASTNodeSymbol type)
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

	std::function<void(const std::pair<std::string, FunctionDeclaration>&, ASTNode*, ShaderType, int, bool)> generateExpression = [&, this](const std::pair<std::string, FunctionDeclaration>& f, ASTNode* expressionNode, ShaderType type, int indentation, bool isMemberOp) -> void
	{
		switch (expressionNode->symbol)
		{
			case ASTNodeSymbol::IntLiteral: out << expressionNode->attribute; break;
			case ASTNodeSymbol::FloatLiteral: out << expressionNode->attribute; break;
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

				is = false;
				for (auto& v : m_vertexOut)
					if (v.name == expressionNode->attribute)
					{
						is = true;
						break;
					}

				if (is) // Is vertex output param
				{
					out << "vertexOut_" << expressionNode->attribute;
					break;
				}
				else // Is local variable
				{
					out << "local_" << expressionNode->attribute;
				}
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
						generateExpression(f, expressionNode->firstChild->next, type, indentation, true);
						//out << expressionNode->firstChild->next->attribute;
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
			case ASTNodeSymbol::Constructor:
			{
				out << TypeToGLType(expressionNode->firstChild->symbol) << "(";
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
				out << "(";
				generateExpression(f, expressionNode->firstChild, type, indentation, false);
				out << ") + (";
				generateExpression(f, expressionNode->firstChild->next, type, indentation, false);
				out << ")";
			} break;
			case ASTNodeSymbol::Sub:
			{
				out << "(";
				generateExpression(f, expressionNode->firstChild, type, indentation, false);
				out << ") - (";
				generateExpression(f, expressionNode->firstChild->next, type, indentation, false);
				out << ")";
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
				out << "(";
				generateExpression(f, expressionNode->firstChild, type, indentation, false);
				out << ") % (";
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
		}
	};

	auto generateStatement = [&, this](const std::pair<std::string, FunctionDeclaration>& f, ASTNode* statementNode, ShaderType type, int indentation) -> void
	{
		auto typeNode = statementNode->firstChild;
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
			case ASTNodeSymbol::Declaration:
			{
				if (typeNode->next->next->next == nullptr)
				{
					for (int i = 0; i < indentation; ++i)
						out << "\t";
					out << TypeToGLType(typeNode->next->symbol) << " local_" << typeNode->next->next->attribute << ";" << std::endl;
				}
				else
				{
					for (int i = 0; i < indentation; ++i)
						out << "\t";
					out << TypeToGLType(typeNode->next->symbol) << " local_" << typeNode->next->next->attribute << " = " << "(";
					generateExpression(f, typeNode->next->next->next, type, indentation, false);
					out << ");" << std::endl;
				}
			} break;
			default:
				for (int i = 0; i < indentation; ++i)
					out << "\t";
				generateExpression(f, typeNode, type, indentation, false);
				out << ";" << std::endl;
				break;
		}
	};
	
	std::function<void(const std::pair<std::string, FunctionDeclaration>&, ASTNode*, ShaderType, int)> generateScope = [&, this](const std::pair<std::string, FunctionDeclaration>& f, ASTNode* scopeNode, ShaderType type, int indentation) -> void
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
			out << TypeToGLType(f.second.returnType) << " function_" << f.first << "(";
			for (size_t i = 0; i < f.second.params.size(); ++i)
				out << TypeToGLType(f.second.params[i].type) << " param_" << f.second.params[i].name << (i == f.second.params.size() - 1 ? ")" : ", ");
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
			out << "\t" << TypeToGLType(d.type) << " " << d.name << ";" << std::endl;
		out << "};" << std::endl;
	}

	if (!m_constantBuffers.empty())
		out << std::endl;

	switch (m_shaderType)
	{
		case ShaderType::Vertex:
		{
			const auto& params = m_functions["VertexShader"].params;
			for (const auto& p : params)
				out << "in " << TypeToGLType(p.type) << " " << p.name << ";" << std::endl;
			out << std::endl;
			for (auto& v : m_vertexOut)
				out << "out " << TypeToGLType(v.type) << " vertexOut_" << v.name << ";" << std::endl;
			out << std::endl;
		} break;
		case ShaderType::Pixel:
			for (auto& v : m_vertexOut)
				out << "in " << TypeToGLType(v.type) << " vertexOut_" << v.name << ";" << std::endl;
			out << std::endl;
			out << "out " << TypeToGLType(m_functions["PixelShader"].returnType) << " fragOut;" << std::endl;
			out << std::endl;
			break;
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

	//std::cout << m_output << std::endl;
}
