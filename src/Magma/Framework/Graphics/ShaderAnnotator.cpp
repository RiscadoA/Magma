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
			
			// If has another member operator
			else if (node->child->type == ShaderSTNodeType::Operator && node->child->operatorType == ShaderOperatorType::Member)
				Annotate(node->child, data);

			if (!found)
			{
				if (node->child->next->attribute == "x")
				{
					node->reference = node->child->reference;
					node->type = ShaderSTNodeType::ComponentX;
					
				}
				else if (node->child->next->attribute == "y")
				{
					node->reference = node->child->reference;
					node->type = ShaderSTNodeType::ComponentY;
				}
				else if (node->child->next->attribute == "z")
				{
					node->reference = node->child->reference;
					node->type = ShaderSTNodeType::ComponentZ;
				}
				else if (node->child->next->attribute == "w")
				{
					node->reference = node->child->reference;
					node->type = ShaderSTNodeType::ComponentW;
				}

				DestroyShaderNode(node->child->next);
				DestroyShaderNode(node->child);
			}
			
		}
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
	}

	auto c = node->child;
	while (c != nullptr)
	{
		Annotate(c, data);
		c = c->next;
	}
}

void Check(ShaderSTNode* node, ShaderCompilerData& data)
{
	if (node->type == ShaderSTNodeType::Operator && node->operatorType == ShaderOperatorType::Assign)
	{

	}

	auto c = node->child;
	while (c != nullptr)
	{
		Check(c, data);
		c = c->next;
	}
}

void Magma::Framework::Graphics::ShaderAnnotator::Run(ShaderSTNode * tree, ShaderCompilerData & data)
{
	Annotate(tree, data);
	Check(tree, data);
}

void Magma::Framework::Graphics::ShaderAnnotator::Print(ShaderSTNode * node, size_t indentation)
{

}
