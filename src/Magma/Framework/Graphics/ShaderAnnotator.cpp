#include "ShaderAnnotator.hpp"

#include <sstream>
#include <iostream>

using namespace Magma::Framework;
using namespace Magma::Framework::Graphics;

void DestroyShaderNode(ShaderSTNode* node)
{
	if (node->parent != nullptr)
	{
		auto c = node->parent->child;
		if (c == node)
			node->parent->child = node->next;
		else
		{
			while (c != nullptr && c->next != node)
				c = c->next;
			if (c != nullptr && c->next != nullptr)
				c->next = c->next->next;
		}
	}

	auto c = node->child;
	while (c != nullptr)
	{
		auto prev = c;
		c = c->next;
		DestroyShaderNode(prev);
	}

	delete node;
}

void Annotate(ShaderSTNode* node, ShaderCompilerData & data)
{
	node->lvalue = true;

	if (node->parent == nullptr)
	{
		node->scope = std::make_shared<ShaderScope>();
		node->scope->parent = std::weak_ptr<ShaderScope>();
		node->scope->child = nullptr;
		node->scope->next = nullptr;
		node->scope->variables = {};
		data.rootScope = node->scope;
	}
	else
	{
		node->scope = node->parent->scope;

		if (node->type == ShaderSTNodeType::Scope)
		{
			node->scope = std::make_shared<ShaderScope>();
			node->scope->parent = node->parent->scope;		
			node->scope->child = nullptr;
			node->scope->next = node->parent->scope->child;
			node->parent->scope->child = node->scope;
			node->scope->variables = {};

			node->lvalue = false;
		}
		else if (node->type == ShaderSTNodeType::Declaration)
		{
			ShaderVariable var;		
			
			var.bufferName = "";
			var.name = "";
			var.type = node->child->variableType;
			var.id = node->child->next->attribute;
			var.index = 0;

			node->scope->variables.push_back(var);
			node->reference = &node->scope->variables.back();

			DestroyShaderNode(node->child->next);
			DestroyShaderNode(node->child);

			node->lvalue = false;
		}
		else if (node->type == ShaderSTNodeType::Operator && node->operatorType == ShaderOperatorType::Member)
		{
			bool found = false;

			if (node->child->type == ShaderSTNodeType::Identifier)
			{
				// Is input
				if (node->child->attribute == data.inputName)
				{
					for (auto& v : data.inputVariables)
						if (v.id == node->child->next->attribute)
						{
							DestroyShaderNode(node->child->next);
							DestroyShaderNode(node->child);
							node->type = ShaderSTNodeType::Reference;
							node->reference = &v;
							found = true;
							break;
						}
				}
				// Is output
				else if (node->child->attribute == data.outputName)
				{
					for (auto& v : data.outputVariables)
						if (v.id == node->child->next->attribute)
						{
							DestroyShaderNode(node->child->next);
							DestroyShaderNode(node->child);
							node->type = ShaderSTNodeType::Reference;
							node->reference = &v;
							found = true;
							break;
						}
				}
				
				// Is constant buffer
				else
				{
					for (auto& c : data.constantBuffers)
					{
						if (node->child->attribute == c.id)
						{
							for (auto& v : data.constantBufferVariables)
								if (v.bufferName == c.id && v.id == node->child->next->attribute)
								{
									DestroyShaderNode(node->child->next);
									DestroyShaderNode(node->child);
									node->type = ShaderSTNodeType::Reference;
									node->reference = &v;
									found = true;
									break;
								}
						}
					}
				}
			}
			
			if (!found)
			{
				Annotate(node->child, data);

				if (!node->child->lvalue)
					node->lvalue = false;

				if (node->child->next->attribute == "x" || node->child->next->attribute == "r")
				{
					node->reference = node->child->reference;
					node->type = ShaderSTNodeType::ComponentX;
				}
				else if (node->child->next->attribute == "y" || node->child->next->attribute == "g")
				{
					node->reference = node->child->reference;
					node->type = ShaderSTNodeType::ComponentY;
				}
				else if (node->child->next->attribute == "z" || node->child->next->attribute == "b")
				{
					node->reference = node->child->reference;
					node->type = ShaderSTNodeType::ComponentZ;
				}
				else if (node->child->next->attribute == "w" || node->child->next->attribute == "a")
				{
					node->reference = node->child->reference;
					node->type = ShaderSTNodeType::ComponentW;
				}

				DestroyShaderNode(node->child->next);
				if (node->reference != nullptr)
					DestroyShaderNode(node->child);
			}
		}
		else if (node->type == ShaderSTNodeType::Operator && node->operatorType != ShaderOperatorType::Assign)
			node->lvalue = false;
		else if (node->type == ShaderSTNodeType::Identifier)
		{
			node->type = ShaderSTNodeType::Reference;
			
			bool found = false;

			// Is texture
			for (auto& t : data.texture2Ds)
				if (t.id == node->attribute)
				{
					node->reference = &t;
					found = true;
					break;
				}

			// Is local variable
			if (!found)
				node->reference = node->scope->GetVar(node->attribute);
		}
		else if (node->type == ShaderSTNodeType::Literal)
			node->lvalue = false;
	}

	auto c = node->child;
	while (c != nullptr)
	{
		Annotate(c, data);
		c = c->next;
	}
}

ShaderVariableType Check(ShaderSTNode* node, ShaderCompilerData& data)
{
	if (node->type == ShaderSTNodeType::Operator && node->operatorType == ShaderOperatorType::Assign)
	{
		auto type1 = Check(node->child, data);
		auto type2 = Check(node->child->next, data);

		if (!node->child->lvalue)
		{
			std::stringstream ss;
			ss << "Failed to run ShaderAnnotator:" << std::endl;
			ss << "Assign operand must be a lvalue" << std::endl;
			ss << "Line: " << node->lineNumber;
			throw ShaderError(ss.str());
		}

		if (type1 != type2)
		{
			std::stringstream ss;
			ss << "Failed to run ShaderAnnotator:" << std::endl;
			ss << "Assign operand types do not match:" << std::endl;
			ss << ShaderVariableTypeToString(type1) << " = " << ShaderVariableTypeToString(type2) << std::endl;
			ss << "Line: " << node->lineNumber;
			throw ShaderError(ss.str());
		}	
	}
	else if (node->type == ShaderSTNodeType::Constructor)
	{
		auto type = node->child->variableType;

		ShaderVariableType compType = ShaderVariableType::Invalid;
		size_t compCount;

		switch (type)
		{
			case ShaderVariableType::Int1: compCount = 1; compType = ShaderVariableType::Int1; break;
			case ShaderVariableType::Int2: compCount = 2; compType = ShaderVariableType::Int1; break;
			case ShaderVariableType::Int3: compCount = 3; compType = ShaderVariableType::Int1; break;
			case ShaderVariableType::Int4: compCount = 4; compType = ShaderVariableType::Int1; break;

			case ShaderVariableType::Float1: compCount = 1; compType = ShaderVariableType::Float1; break;
			case ShaderVariableType::Float2: compCount = 2; compType = ShaderVariableType::Float1; break;
			case ShaderVariableType::Float3: compCount = 3; compType = ShaderVariableType::Float1; break;
			case ShaderVariableType::Float4: compCount = 4; compType = ShaderVariableType::Float1; break;

			default:
			{
				std::stringstream ss;
				ss << "Failed to run ShaderAnnotator:" << std::endl;
				ss << "Unsupported constructor type:" << std::endl;
				ss << "'" << ShaderVariableTypeToString(type) << "'" << std::endl;
				ss << "Line: " << node->lineNumber;
				throw ShaderError(ss.str());
			}
		}

		auto c = node->child->next;
		for (size_t i = 0; i < compCount; ++i)
		{
			auto t = Check(c, data);
			if (t != compType)
			{
				std::stringstream ss;
				ss << "Failed to run ShaderAnnotator:" << std::endl;
				ss << "Constructor parameter type doesn't match constructor type:" << std::endl;
				ss << "'" << ShaderVariableTypeToString(t) << "' (param) " << "'" << ShaderVariableTypeToString(type) << "' (constructor)" << std::endl;
				ss << "Line: " << node->lineNumber;
				throw ShaderError(ss.str());
			}
			c = c->next;
		}

		return type;
	}
	else
	{
		if (node->type == ShaderSTNodeType::Literal)
			return node->variableType;
		else if (node->type == ShaderSTNodeType::Reference)
			return node->reference->type;
		else if (node->type == ShaderSTNodeType::ComponentX)
		{
			ShaderVariableType type;

			if (node->reference == nullptr)
			{
				auto exp = node->child;
				type = Check(exp, data);
			}
			else
				type = node->reference->type;

			switch (type)
			{
				case ShaderVariableType::Float2:
				case ShaderVariableType::Float3:
				case ShaderVariableType::Float4:
					return ShaderVariableType::Float1;
					break;

				case ShaderVariableType::Int2:
				case ShaderVariableType::Int3:
				case ShaderVariableType::Int4:
					return ShaderVariableType::Int1;
					break;

				default:
				{
					std::stringstream ss;
					ss << "Failed to run ShaderAnnotator:" << std::endl;
					ss << "Expression doesn't have a component 'x'" << std::endl;
					ss << "Line: " << node->lineNumber;
					throw ShaderError(ss.str());
					break;
				}
			}
		}
		else if (node->type == ShaderSTNodeType::ComponentY)
		{
			ShaderVariableType type;

			if (node->reference == nullptr)
			{
				auto exp = node->child;
				type = Check(exp, data);
			}
			else
				type = node->reference->type;

			switch (type)
			{
				case ShaderVariableType::Float2:
				case ShaderVariableType::Float3:
				case ShaderVariableType::Float4:
					node->resultType = ShaderVariableType::Float1;
					return ShaderVariableType::Float1;
					break;

				case ShaderVariableType::Int2:
				case ShaderVariableType::Int3:
				case ShaderVariableType::Int4:
					node->resultType = ShaderVariableType::Int1;
					return ShaderVariableType::Int1;
					break;

				default:
				{
					std::stringstream ss;
					ss << "Failed to run ShaderAnnotator:" << std::endl;
					ss << "Expression doesn't have a component 'y'" << std::endl;
					ss << "Line: " << node->lineNumber;
					throw ShaderError(ss.str());
					break;
				}
			}
		}
		else if (node->type == ShaderSTNodeType::ComponentZ)
		{
			ShaderVariableType type;

			if (node->reference == nullptr)
			{
				auto exp = node->child;
				type = Check(exp, data);
			}
			else
				type = node->reference->type;

			switch (type)
			{
				case ShaderVariableType::Float3:
				case ShaderVariableType::Float4:
					node->resultType = ShaderVariableType::Float1;
					return ShaderVariableType::Float1;
					break;

				case ShaderVariableType::Int3:
				case ShaderVariableType::Int4:
					node->resultType = ShaderVariableType::Int1;
					return ShaderVariableType::Int1;
					break;

				default:
				{
					std::stringstream ss;
					ss << "Failed to run ShaderAnnotator:" << std::endl;
					ss << "Expression doesn't have a component 'z'" << std::endl;
					ss << "Line: " << node->lineNumber;
					throw ShaderError(ss.str());
					break;
				}
			}
		}
		else if (node->type == ShaderSTNodeType::ComponentW)
		{
			ShaderVariableType type;

			if (node->reference == nullptr)
			{
				auto exp = node->child;
				type = Check(exp, data);
			}
			else
				type = node->reference->type;

			switch (type)
			{
				case ShaderVariableType::Float4:
					node->resultType = ShaderVariableType::Float1;
					return ShaderVariableType::Float1;
					break;

				case ShaderVariableType::Int4:
					node->resultType = ShaderVariableType::Int1;
					return ShaderVariableType::Int1;
					break;

				default:
				{
					std::stringstream ss;
					ss << "Failed to run ShaderAnnotator:" << std::endl;
					ss << "Expression doesn't have a component 'w'" << std::endl;
					ss << "Line: " << node->lineNumber;
					throw ShaderError(ss.str());
					break;
				}
			}
		}

		auto c = node->child;
		while (c != nullptr)
		{
			Check(c, data);
			c = c->next;
		}
	}

	return ShaderVariableType::Invalid;
}

void Magma::Framework::Graphics::ShaderAnnotator::Run(ShaderSTNode * tree, ShaderCompilerData & data)
{
	Annotate(tree, data);
	Check(tree, data);
}

void Magma::Framework::Graphics::ShaderAnnotator::Print(ShaderSTNode * node, size_t indentation)
{
	ShaderParser::Print(node, indentation);
}
