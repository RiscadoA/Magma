#include "ShaderAnnotator.hpp"

#include <sstream>
#include <iostream>

using namespace Magma::Framework;
using namespace Magma::Framework::Graphics;

std::vector<ShaderFunction> Magma::Framework::Graphics::ShaderFunctions =
{
	{
		"cos",
		ShaderVariableType::Float1,
		{
			ShaderVariableType::Float1,
		}
	},
	{
		"sin",
		ShaderVariableType::Float1,
		{
			ShaderVariableType::Float1,
		}
	},
	{
		"tan",
		ShaderVariableType::Float1,
		{
			ShaderVariableType::Float1,
		}
	},
	{
		"acos",
		ShaderVariableType::Float1,
		{
			ShaderVariableType::Float1,
		}
	},
	{
		"asin",
		ShaderVariableType::Float1,
		{
			ShaderVariableType::Float1,
		}
	},
	{
		"atan",
		ShaderVariableType::Float1,
		{
			ShaderVariableType::Float1,
		}
	},
	{
		"degrees",
		ShaderVariableType::Float1,
		{
			ShaderVariableType::Float1,
		}
	},
	{
		"radians",
		ShaderVariableType::Float1,
		{
			ShaderVariableType::Float1,
		}
	},
	{
		"asin",
		ShaderVariableType::Float1,
		{
			ShaderVariableType::Float1,
		}
	},
	{
		"atan",
		ShaderVariableType::Float1,
		{
			ShaderVariableType::Float1,
		}
	},
	{
		"mulvec",
		ShaderVariableType::Float4,
		{
			ShaderVariableType::Float44,
			ShaderVariableType::Float4,
		}
	},
	{
		"mulmat",
		ShaderVariableType::Float44,
		{
			ShaderVariableType::Float44,
			ShaderVariableType::Float44,
		}
	},
	{
		"Sample2D",
		ShaderVariableType::Float4,
		{
			ShaderVariableType::Texture2D,
			ShaderVariableType::Float2,
		}
	},
	{
		"powf",
		ShaderVariableType::Float1,
		{
			ShaderVariableType::Float1,
			ShaderVariableType::Float1,
		}
	},
	{
		"powi",
		ShaderVariableType::Int1,
		{
			ShaderVariableType::Int1,
			ShaderVariableType::Int1,
		}
	},
	{
		"exp",
		ShaderVariableType::Float1,
		{
			ShaderVariableType::Float1,
		}
	},
	{
		"log",
		ShaderVariableType::Float1,
		{
			ShaderVariableType::Float1,
		}
	},
	{
		"exp2",
		ShaderVariableType::Float1,
		{
			ShaderVariableType::Float1,
		}
	},
	{
		"log2",
		ShaderVariableType::Float1,
		{
			ShaderVariableType::Float1,
		}
	},
	{
		"sqrt",
		ShaderVariableType::Float1,
		{
			ShaderVariableType::Float1,
		}
	},
	{
		"isqrt",
		ShaderVariableType::Float1,
		{
			ShaderVariableType::Float1,
		}
	},
	{
		"absf",
		ShaderVariableType::Float1,
		{
			ShaderVariableType::Float1,
		}
	},
	{
		"absi",
		ShaderVariableType::Int1,
		{
			ShaderVariableType::Int1,
		}
	},
	{
		"signf",
		ShaderVariableType::Float1,
		{
			ShaderVariableType::Float1,
		}
	},
	{
		"signi",
		ShaderVariableType::Int1,
		{
			ShaderVariableType::Float1,
		}
	},
	{
		"floorf",
		ShaderVariableType::Float1,
		{
			ShaderVariableType::Float1,
		}
	},
	{
		"floori",
		ShaderVariableType::Int1,
		{
			ShaderVariableType::Float1,
		}
	},
	{
		"ceilf",
		ShaderVariableType::Float1,
		{
			ShaderVariableType::Float1,
		}
	},
	{
		"ceili",
		ShaderVariableType::Int1,
		{
			ShaderVariableType::Float1,
		}
	},
	{
		"fract",
		ShaderVariableType::Float1,
		{
			ShaderVariableType::Float1,
		}
	},
	{
		"lerp1",
		ShaderVariableType::Float1,
		{
			ShaderVariableType::Float1,
			ShaderVariableType::Float1,
			ShaderVariableType::Float1,
		}
	},
	{
		"lerp2",
		ShaderVariableType::Float2,
		{
			ShaderVariableType::Float2,
			ShaderVariableType::Float2,
			ShaderVariableType::Float1,
		}
	},
	{
		"lerp3",
		ShaderVariableType::Float3,
		{
			ShaderVariableType::Float3,
			ShaderVariableType::Float3,
			ShaderVariableType::Float1,
		}
	},
	{
		"lerp4",
		ShaderVariableType::Float4,
		{
			ShaderVariableType::Float4,
			ShaderVariableType::Float4,
			ShaderVariableType::Float1,
		}
	},
};

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
					if (node->parent->type == ShaderSTNodeType::Operator && node->parent->operatorType == ShaderOperatorType::Assign && node == node->parent->child)
						node->lvalue = true;
					else
						node->lvalue = false;
					node->reference = node->child->reference;
					node->type = ShaderSTNodeType::ComponentX;
				}
				else if (node->child->next->attribute == "y" || node->child->next->attribute == "g")
				{
					if (node->parent->type == ShaderSTNodeType::Operator && node->parent->operatorType == ShaderOperatorType::Assign && node == node->parent->child)
						node->lvalue = true;
					else
						node->lvalue = false;
					node->reference = node->child->reference;
					node->type = ShaderSTNodeType::ComponentY;
				}
				else if (node->child->next->attribute == "z" || node->child->next->attribute == "b")
				{
					if (node->parent->type == ShaderSTNodeType::Operator && node->parent->operatorType == ShaderOperatorType::Assign && node == node->parent->child)
						node->lvalue = true;
					else
						node->lvalue = false;
					node->reference = node->child->reference;
					node->type = ShaderSTNodeType::ComponentZ;
				}
				else if (node->child->next->attribute == "w" || node->child->next->attribute == "a")
				{
					if (node->parent->type == ShaderSTNodeType::Operator && node->parent->operatorType == ShaderOperatorType::Assign && node == node->parent->child)
						node->lvalue = true;
					else
						node->lvalue = false;
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
		else if (node->type == ShaderSTNodeType::Constructor)
		{
			node->lvalue = false;
			node->resultType = node->child->variableType;
		}
		else if (node->type == ShaderSTNodeType::Call)
			node->lvalue = false;
		else if (node->type == ShaderSTNodeType::Identifier && !(node->parent->type == ShaderSTNodeType::Call && node == node->parent->child))
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
	if (node->type == ShaderSTNodeType::Branch)
	{
		auto expType = Check(node->child, data);
		if (expType != ShaderVariableType::Bool)
		{
			std::stringstream ss;
			ss << "Failed to run ShaderAnnotator:" << std::endl;
			ss << "Branch (if) expression must evaluate to bool (evaluates to '" << ShaderVariableTypeToString(expType) << "')" << std::endl;
			ss << "Line: " << node->lineNumber;
			throw ShaderError(ss.str());
		}
	}
	else if (node->type == ShaderSTNodeType::Operator && node->operatorType == ShaderOperatorType::Assign)
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
	else if (node->type == ShaderSTNodeType::Declaration)
	{
		auto type1 = node->reference->type;
		auto type2 = Check(node->child, data);

		if (type1 != type2)
		{
			std::stringstream ss;
			ss << "Failed to run ShaderAnnotator:" << std::endl;
			ss << "Declaration assign operand types do not match:" << std::endl;
			ss << ShaderVariableTypeToString(type1) << " = " << ShaderVariableTypeToString(type2) << std::endl;
			ss << "Line: " << node->lineNumber;
			throw ShaderError(ss.str());
		}
	}
	else if (node->type == ShaderSTNodeType::Constructor)
	{
		auto type = node->child->variableType;

		ShaderVariableType compType = ShaderVariableType::Invalid;
		size_t compCount = 0;

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
				ss << "Constructor parameter " << (i + 1) << " type doesn't match constructor parameter type:" << std::endl;
				ss << "'" << ShaderVariableTypeToString(t) << "' should be " << "'" << ShaderVariableTypeToString(type) << "'" << std::endl;
				ss << "Line: " << node->lineNumber;
				throw ShaderError(ss.str());
			}
			c = c->next;
		}

		return type;
	}
	else if (node->type == ShaderSTNodeType::Call)
	{
		for (auto& f : ShaderFunctions)
			if (f.name == node->child->attribute)
			{
				auto pc = node->child->next;
				size_t pi = 1;
				for (auto& p : f.params)
				{
					if (pc == nullptr)
					{
						std::stringstream ss;
						ss << "Failed to run ShaderAnnotator:" << std::endl;
						ss << "Failed to call function '" << node->child->attribute << "'" << std::endl;
						ss << "Failed to get param " << pi << ":" << std::endl;
						ss << "Function has " << f.params.size() << " params but call only assigns " << pi - 1 << " params" << std::endl;
						ss << "Line: " << node->lineNumber;
						throw ShaderError(ss.str());
					}

					auto tp = Check(pc, data);

					if (p != tp)
					{
						std::stringstream ss;
						ss << "Failed to run ShaderAnnotator:" << std::endl;
						ss << "Failed to call function '" << node->child->attribute << "'" << std::endl;
						ss << "Failed to get param " << pi << ":" << std::endl;
						ss << "Call parameter type doesn't match function parameter type:" << std::endl;
						ss << "'" << ShaderVariableTypeToString(tp) << "' should be " << "'" << ShaderVariableTypeToString(p) << "'" << std::endl;
						ss << "Line: " << node->lineNumber;
						throw ShaderError(ss.str());
					}

					pc = pc->next;
				}

				node->resultType = f.returnType;
				return node->resultType;
			}
		
		std::stringstream ss;
		ss << "Failed to run ShaderAnnotator:" << std::endl;
		ss << "Failed to call function '" << node->child->attribute << "'" << std::endl;
		ss << "Unsupported/unknown function" << std::endl;
		ss << "Line: " << node->lineNumber;
		throw ShaderError(ss.str());
	}
	else if (node->type == ShaderSTNodeType::Operator)
	{
		auto c = node->child;
		while (c != nullptr)
		{
			Check(c, data);
			c = c->next;
		}

		switch (node->operatorType)
		{
			case ShaderOperatorType::Equal:
			case ShaderOperatorType::Different:
			case ShaderOperatorType::Greater:
			case ShaderOperatorType::Less:
			case ShaderOperatorType::GEqual:
			case ShaderOperatorType::LEqual:
				node->resultType = ShaderVariableType::Bool;
				break;

			default:
				node->resultType = node->child->resultType;
				break;
		}

		
		return node->resultType;
	}
	else
	{
		if (node->type == ShaderSTNodeType::Literal)
		{
			node->resultType = node->variableType;
			return node->variableType;
		}
		else if (node->type == ShaderSTNodeType::Reference)
		{
			node->resultType = node->reference->type;
			return node->reference->type;
		}
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
					node->resultType = ShaderVariableType::Float1;
					break;

				case ShaderVariableType::Int2:
				case ShaderVariableType::Int3:
				case ShaderVariableType::Int4:
					node->resultType = ShaderVariableType::Int1;
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

			return node->resultType;
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
					break;

				case ShaderVariableType::Int2:
				case ShaderVariableType::Int3:
				case ShaderVariableType::Int4:
					node->resultType = ShaderVariableType::Int1;
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

			return node->resultType;
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
					break;

				case ShaderVariableType::Int3:
				case ShaderVariableType::Int4:
					node->resultType = ShaderVariableType::Int1;
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

			return node->resultType;
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
					break;

				case ShaderVariableType::Int4:
					node->resultType = ShaderVariableType::Int1;
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

			return node->resultType;
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
