#include "ShaderParser.hpp"

#include <sstream>
#include <iostream>

using namespace Magma::Framework;
using namespace Magma::Framework::Graphics;

struct ParserInfo
{
	ShaderCompilerData& data;

	const std::vector<ShaderToken>& tokens;
	std::vector<ShaderToken>::const_iterator nextToken;

	ShaderSTNode* root;
	size_t lineNumber;
};

inline ShaderSTNode* CreateNode(ShaderSTNodeType type, const std::string& attribute, ParserInfo& info)
{
	auto node = new ShaderSTNode();

	node->type = type;
	node->attribute = attribute;

	node->parent = nullptr;
	node->child = nullptr;
	node->next = nullptr;

	node->lineNumber = info.lineNumber;

	return node;
}

inline void AddToNode(ShaderSTNode* node, ShaderSTNode* child)
{
	child->parent = node;
	if (node->child == nullptr)
		node->child = child;
	else
	{
		auto c = node->child;
		while (c->next != nullptr)
			c = c->next;
		c->next = child;
	}
}

void DestroyNode(ShaderSTNode* node)
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
		DestroyNode(prev);
	}

	delete node;
}

void ThrowNotImplemented(const std::string& feature, ParserInfo& info)
{
	std::stringstream ss;
	ss << "Failed to run ShaderParser:" << std::endl;
	ss << "Feature not yet implemented:" << std::endl;
	ss << feature << std::endl;
	ss << "Line: " << info.lineNumber;
	throw ShaderError(ss.str());
}

void SkipToken(ParserInfo& info)
{
	info.nextToken++;
	if (info.nextToken != info.tokens.end())
		info.lineNumber = info.nextToken->lineNumber;
}

const ShaderToken* PeekToken(ParserInfo& info)
{
	if (info.nextToken == info.tokens.end())
		return nullptr;
	return &(*info.nextToken);
}

bool PeekPunctuationType(ShaderPunctuationType type, ParserInfo& info)
{
	if (info.nextToken == info.tokens.end())
	{
		std::stringstream ss;
		ss << "Failed to run ShaderParser:" << std::endl;
		ss << "Failed to peek punctuation type:" << std::endl;
		ss << "Unexpected EOF" << std::endl;
		ss << "Line: " << info.lineNumber;
		throw ShaderError(ss.str());
	}
	if (info.nextToken->type != ShaderTokenType::Punctuation)
		return false;
	if (info.nextToken->punctuationType != type)
		return false;
	return true;
}

bool PeekOperatorType(ShaderOperatorType type, ParserInfo& info)
{
	if (info.nextToken == info.tokens.end())
	{
		std::stringstream ss;
		ss << "Failed to run ShaderParser:" << std::endl;
		ss << "Failed to peek operator type:" << std::endl;
		ss << "Unexpected EOF" << std::endl;
		ss << "Line: " << info.lineNumber;
		throw ShaderError(ss.str());
	}
	if (info.nextToken->type != ShaderTokenType::Operator)
		return false;
	if (info.nextToken->operatorType != type)
		return false;
	return true;
}

const ShaderToken* AcceptTokenType(ShaderTokenType type, ParserInfo& info)
{
	if (info.nextToken == info.tokens.end())
		return nullptr;
	if (info.nextToken->type != type)
		return nullptr;

	auto tok = info.nextToken;
	info.nextToken++;
	if (info.nextToken != info.tokens.end())
		info.lineNumber = info.nextToken->lineNumber;
	return &(*tok);
}

const ShaderToken* AcceptOperatorType(ShaderOperatorType type, ParserInfo& info)
{
	if (info.nextToken == info.tokens.end())
		return nullptr;
	if (info.nextToken->type != ShaderTokenType::Operator)
		return nullptr;
	if (info.nextToken->operatorType != type)
		return nullptr;

	auto tok = info.nextToken;
	info.nextToken++;
	if (info.nextToken != info.tokens.end())
		info.lineNumber = info.nextToken->lineNumber;
	return &(*tok);
}

const ShaderToken* AcceptPunctuationType(ShaderPunctuationType type, ParserInfo& info)
{
	if (info.nextToken == info.tokens.end())
		return nullptr;
	if (info.nextToken->type != ShaderTokenType::Punctuation)
		return nullptr;
	if (info.nextToken->punctuationType != type)
		return nullptr;

	auto tok = info.nextToken;
	info.nextToken++;
	if (info.nextToken != info.tokens.end())
		info.lineNumber = info.nextToken->lineNumber;
	return &(*tok);
}

const ShaderToken* ExpectTokenType(ShaderTokenType type, ParserInfo& info)
{
	if (info.nextToken == info.tokens.end())
	{
		std::stringstream ss;
		ss << "Failed to run ShaderParser:" << std::endl;
		ss << "Unexpected EOF:" << std::endl;
		ss << "Expected token type '" << ShaderTokenTypeToString(type) << "':" << std::endl;
		ss << "Line: " << info.lineNumber;
		throw ShaderError(ss.str());
	}
	else
	{
		if (info.nextToken->type == type)
		{
			auto tok = info.nextToken;
			info.nextToken++;
			if (info.nextToken != info.tokens.end())
				info.lineNumber = info.nextToken->lineNumber;
			return &(*tok);
		}
		else
		{
			std::stringstream ss;
			ss << "Failed to run ShaderParser:" << std::endl;
			ss << "Unexpected token: " << ShaderTokenToString(*info.nextToken) << std::endl;
			ss << "Expected token type '" << ShaderTokenTypeToString(type) << "':" << std::endl;
			ss << "Line: " << info.lineNumber;
			throw ShaderError(ss.str());
		}
	}
}

const ShaderToken* ExpectOperatorType(ShaderOperatorType type, ParserInfo& info)
{
	if (info.nextToken == info.tokens.end())
	{
		std::stringstream ss;
		ss << "Failed to run ShaderParser:" << std::endl;
		ss << "Unexpected EOF:" << std::endl;
		ss << "Expected operator token '" << ShaderOperatorTypeToString(type) << "':" << std::endl;
		ss << "Line: " << info.lineNumber;
		throw ShaderError(ss.str());
	}
	else
	{
		if (info.nextToken->type == ShaderTokenType::Operator &&
			info.nextToken->operatorType == type)
		{
			auto tok = info.nextToken;
			info.nextToken++;
			if (info.nextToken != info.tokens.end())
				info.lineNumber = info.nextToken->lineNumber;
			return &(*tok);
		}
		else
		{
			std::stringstream ss;
			ss << "Failed to run ShaderParser:" << std::endl;
			ss << "Unexpected token: " << ShaderTokenToString(*info.nextToken) << std::endl;
			ss << "Expected operator token '" << ShaderOperatorTypeToString(type) << "':" << std::endl;
			ss << "Line: " << info.lineNumber;
			throw ShaderError(ss.str());
		}
	}
}

const ShaderToken* ExpectPunctuationType(ShaderPunctuationType type, ParserInfo& info)
{
	if (info.nextToken == info.tokens.end())
	{
		std::stringstream ss;
		ss << "Failed to run ShaderParser:" << std::endl;
		ss << "Unexpected EOF:" << std::endl;
		ss << "Expected punctuation token '" << ShaderPunctuationTypeToString(type) << "':" << std::endl;
		ss << "Line: " << info.lineNumber;
		throw ShaderError(ss.str());
	}
	else
	{
		if (info.nextToken->type == ShaderTokenType::Punctuation &&
			info.nextToken->punctuationType == type)
		{
			auto tok = info.nextToken;
			info.nextToken++;
			if (info.nextToken != info.tokens.end())
				info.lineNumber = info.nextToken->lineNumber;
			return &(*tok);
		}
		else
		{
			std::stringstream ss;
			ss << "Failed to run ShaderParser:" << std::endl;
			ss << "Unexpected token: " << ShaderTokenToString(*info.nextToken) << std::endl;
			ss << "Expected punctuation token '" << ShaderPunctuationTypeToString(type) << "':" << std::endl;
			ss << "Line: " << info.lineNumber;
			throw ShaderError(ss.str());
		}
	}
}

ShaderSTNode* ParseExpression(ParserInfo& info);
ShaderSTNode* ParseStatement(ParserInfo& info);
ShaderSTNode* ParseScope(ParserInfo& info);

ShaderSTNode* ParseOperatorLast(ParserInfo& info)
{
	// (<exp>)
	if (AcceptPunctuationType(ShaderPunctuationType::OpenParenthesis, info) != nullptr)
	{
		auto node = ParseExpression(info);
		ExpectPunctuationType(ShaderPunctuationType::CloseParenthesis, info);
		return node;
	}

	// <id> or <call>
	else if (auto id = AcceptTokenType(ShaderTokenType::Identifier, info))
	{
		// <call>
		if (AcceptPunctuationType(ShaderPunctuationType::OpenParenthesis, info))
		{
			auto node = CreateNode(ShaderSTNodeType::Call, "", info);
			AddToNode(node, CreateNode(ShaderSTNodeType::Identifier, id->attribute, info));

			size_t paramCount = 0;

			while (!PeekPunctuationType(ShaderPunctuationType::CloseParenthesis, info))
			{
				auto exp = ParseExpression(info);
				if (exp == nullptr)
				{
					std::stringstream ss;
					ss << "Failed to run ShaderParser:" << std::endl;
					ss << "Failed to parse function call:" << std::endl;
					ss << "Failed to parse exception on parameter " << paramCount + 1 << std::endl;
					ss << "Line: " << info.lineNumber;
					throw ShaderError(ss.str());
				}
				AddToNode(node, exp);
				++paramCount;
				if (PeekPunctuationType(ShaderPunctuationType::CloseParenthesis, info))
					break;
				ExpectPunctuationType(ShaderPunctuationType::Comma, info);
			}
			
			ExpectPunctuationType(ShaderPunctuationType::CloseParenthesis, info);
			return node;
		}
		// <id>
		else return CreateNode(ShaderSTNodeType::Identifier, id->attribute, info);
	}

	// <constructor>
	else if (auto type = AcceptTokenType(ShaderTokenType::Type, info))
	{
		auto node = CreateNode(ShaderSTNodeType::Constructor, "", info);

		size_t componentCount = 0;

		switch (type->variableType)
		{
			case ShaderVariableType::Int1:
			case ShaderVariableType::Float1:
				componentCount = 1;
				break;

			case ShaderVariableType::Int2:
			case ShaderVariableType::Float2:
				componentCount = 2;
				break;

			case ShaderVariableType::Int3:
			case ShaderVariableType::Float3:
				componentCount = 3;
				break;

			case ShaderVariableType::Int4:
			case ShaderVariableType::Float4:
				componentCount = 4;
				break;

			default:
			{
				std::stringstream ss;
				ss << "Failed to run ShaderParser:" << std::endl;
				ss << "Failed to parse constructor:" << std::endl;
				ss << "Unsupported constructor type: '" << ShaderVariableTypeToString(type->variableType) << "'" << std::endl;
				ss << "Line: " << info.lineNumber;
				throw ShaderError(ss.str());
			}
		}

		auto typeNode = CreateNode(ShaderSTNodeType::Type, "", info);
		typeNode->variableType = type->variableType;
		AddToNode(node, typeNode);

		ExpectPunctuationType(ShaderPunctuationType::OpenParenthesis, info);

		for (size_t i = 0; i < componentCount; ++i)
		{
			auto exp = ParseExpression(info);
			if (exp == nullptr)
			{
				std::stringstream ss;
				ss << "Failed to run ShaderParser:" << std::endl;
				ss << "Failed to parse constructor:" << std::endl;
				ss << "Failed to parse expression on param " << i + 1 << std::endl;
				ss << "Line: " << info.lineNumber;
				throw ShaderError(ss.str());
			}
			
			AddToNode(node, exp);

			if (i + 1 != componentCount)
				ExpectPunctuationType(ShaderPunctuationType::Comma, info);
		}

		ExpectPunctuationType(ShaderPunctuationType::CloseParenthesis, info);

		return node;
	}

	// <literal>
	else if (auto lit = AcceptTokenType(ShaderTokenType::Literal, info))
	{
		auto node = CreateNode(ShaderSTNodeType::Literal, lit->attribute, info);
		node->variableType = lit->variableType;
		return node;
	}

	// Not an operator
	else return nullptr;
}

ShaderSTNode* ParseOperator6(ParserInfo& info)
{
	// <unary-op>
	if (PeekOperatorType(ShaderOperatorType::Add, info) ||
		PeekOperatorType(ShaderOperatorType::Subtract, info) ||
		PeekOperatorType(ShaderOperatorType::Not, info))
	{
		auto op = AcceptTokenType(ShaderTokenType::Operator, info);

		auto node = CreateNode(ShaderSTNodeType::Operator, "", info);
		node->operatorType = op->operatorType;
		AddToNode(node, ParseOperator6(info));
		return node;
	}
	
	// <member-op>
	ShaderSTNode* term1 = ParseOperatorLast(info);
	if (term1 == nullptr)
		return nullptr;

	while (PeekToken(info)->type == ShaderTokenType::Operator &&
		PeekToken(info)->operatorType == ShaderOperatorType::Member)
	{
		auto op = CreateNode(ShaderSTNodeType::Operator, "", info);
		op->operatorType = AcceptTokenType(ShaderTokenType::Operator, info)->operatorType;

		auto term2 = ParseOperatorLast(info);

		AddToNode(op, term1);
		AddToNode(op, term2);

		term1 = op;
	}

	return term1;
}

ShaderSTNode* ParseOperator5(ParserInfo& info)
{
	ShaderSTNode* term1 = ParseOperator6(info);
	if (term1 == nullptr)
		return nullptr;

	ShaderSTNode* term2 = nullptr;

	while (PeekToken(info)->type == ShaderTokenType::Operator && (
		PeekToken(info)->operatorType == ShaderOperatorType::Add ||
		PeekToken(info)->operatorType == ShaderOperatorType::Subtract))
	{
		auto op = CreateNode(ShaderSTNodeType::Operator, "", info);
		op->operatorType = AcceptTokenType(ShaderTokenType::Operator, info)->operatorType;

		auto term2 = ParseOperator6(info);

		AddToNode(op, term1);
		AddToNode(op, term2);

		term1 = op;
	}

	return term1;
}

ShaderSTNode* ParseOperator4(ParserInfo& info)
{
	ShaderSTNode* term1 = ParseOperator5(info);
	if (term1 == nullptr)
		return nullptr;

	ShaderSTNode* term2 = nullptr;

	while (PeekToken(info)->type == ShaderTokenType::Operator && (
		PeekToken(info)->operatorType == ShaderOperatorType::Multiply ||
		PeekToken(info)->operatorType == ShaderOperatorType::Divide))
	{
		auto op = CreateNode(ShaderSTNodeType::Operator, "", info);
		op->operatorType = AcceptTokenType(ShaderTokenType::Operator, info)->operatorType;

		auto term2 = ParseOperator5(info);

		AddToNode(op, term1);
		AddToNode(op, term2);

		term1 = op;
	}

	return term1;
}

ShaderSTNode* ParseOperator3(ParserInfo& info)
{
	ShaderSTNode* term1 = ParseOperator4(info);
	if (term1 == nullptr)
		return nullptr;

	ShaderSTNode* term2 = nullptr;

	while (PeekToken(info)->type == ShaderTokenType::Operator && (
		PeekToken(info)->operatorType == ShaderOperatorType::Multiply ||
		PeekToken(info)->operatorType == ShaderOperatorType::Divide))
	{
		auto op = CreateNode(ShaderSTNodeType::Operator, "", info);
		op->operatorType = AcceptTokenType(ShaderTokenType::Operator, info)->operatorType;

		auto term2 = ParseOperator4(info);

		AddToNode(op, term1);
		AddToNode(op, term2);

		term1 = op;
	}

	return term1;
}

ShaderSTNode* ParseOperator2(ParserInfo& info)
{
	ShaderSTNode* term1 = ParseOperator3(info);
	if (term1 == nullptr)
		return nullptr;

	ShaderSTNode* term2 = nullptr;

	while (PeekToken(info)->type == ShaderTokenType::Operator && (
		PeekToken(info)->operatorType == ShaderOperatorType::Equal ||
		PeekToken(info)->operatorType == ShaderOperatorType::Different ||
		PeekToken(info)->operatorType == ShaderOperatorType::Greater ||
		PeekToken(info)->operatorType == ShaderOperatorType::Less || 
		PeekToken(info)->operatorType == ShaderOperatorType::GEqual ||
		PeekToken(info)->operatorType == ShaderOperatorType::LEqual))
	{
		auto op = CreateNode(ShaderSTNodeType::Operator, "", info);
		op->operatorType = AcceptTokenType(ShaderTokenType::Operator, info)->operatorType;

		auto term2 = ParseOperator3(info);

		AddToNode(op, term1);
		AddToNode(op, term2);

		term1 = op;
	}

	return term1;
}

ShaderSTNode* ParseOperator1(ParserInfo& info)
{
	ShaderSTNode* term1 = ParseOperator2(info);
	if (term1 == nullptr)
		return nullptr;

	ShaderSTNode* term2 = nullptr;

	while (PeekToken(info)->type == ShaderTokenType::Operator && (
		PeekToken(info)->operatorType == ShaderOperatorType::And ||
		PeekToken(info)->operatorType == ShaderOperatorType::Or))
	{
		auto op = CreateNode(ShaderSTNodeType::Operator, "", info);
		op->operatorType = AcceptTokenType(ShaderTokenType::Operator, info)->operatorType;

		auto term2 = ParseOperator2(info);

		AddToNode(op, term1);
		AddToNode(op, term2);

		term1 = op;
	}

	return term1;
}

ShaderSTNode* ParseExpression(ParserInfo& info)
{
	ShaderSTNode* term1 = ParseOperator1(info);
	if (term1 == nullptr)
		return nullptr;

	ShaderSTNode* term2 = nullptr;

	while (PeekToken(info)->type == ShaderTokenType::Operator && (
		   PeekToken(info)->operatorType == ShaderOperatorType::Assign))
	{
		auto op = CreateNode(ShaderSTNodeType::Operator, "", info);
		op->operatorType = AcceptTokenType(ShaderTokenType::Operator, info)->operatorType;

		auto term2 = ParseOperator1(info);
		
		AddToNode(op, term1);
		AddToNode(op, term2);

		term1 = op;
	}
	
	return term1;
}

ShaderSTNode* ParseDeclaration(ParserInfo& info)
{
	auto type = AcceptTokenType(ShaderTokenType::Type, info);
	if (type == nullptr)
		return nullptr;
	auto id = ExpectTokenType(ShaderTokenType::Identifier, info);

	auto node = CreateNode(ShaderSTNodeType::Declaration, "", info);
	auto typeNode = CreateNode(ShaderSTNodeType::Type, "", info);
	typeNode->variableType = type->variableType;

	AddToNode(node, typeNode);
	AddToNode(node, CreateNode(ShaderSTNodeType::Identifier, id->attribute, info));

	// If has definition
	if (AcceptOperatorType(ShaderOperatorType::Assign, info))
	{
		auto exp = ParseExpression(info);

		if (exp == nullptr)
		{
			std::stringstream ss;
			ss << "Failed to run ShaderParser:" << std::endl;
			ss << "Failed to parse declaration definition expression" << std::endl;
			ss << "Line: " << info.lineNumber;
			throw ShaderError(ss.str());
		}

		AddToNode(node, exp);
	}

	ExpectPunctuationType(ShaderPunctuationType::Semicolon, info);

	return node;
}

ShaderSTNode* ParseStatement(ParserInfo& info)
{
	ShaderSTNode* node = nullptr;

	// <declaration> ;
	node = ParseDeclaration(info);
	if (node != nullptr)
		return node;

	// <exp> ;
	node = ParseExpression(info);
	if (node != nullptr)
	{
		ExpectPunctuationType(ShaderPunctuationType::Semicolon, info);
		return node;
	}

	return node;
}

ShaderSTNode* ParseScope(ParserInfo& info)
{
	auto node = CreateNode(ShaderSTNodeType::Scope, "", info);

	ExpectPunctuationType(ShaderPunctuationType::OpenBraces, info);

	while (!PeekPunctuationType(ShaderPunctuationType::CloseBraces, info))
	{
		auto statement = ParseStatement(info);

		if (statement == nullptr)
		{
			std::stringstream ss;
			ss << "Failed to run ShaderParser:" << std::endl;
			ss << "Failed to parse statement" << std::endl;
			ss << "Line: " << info.lineNumber;
			throw ShaderError(ss.str());
		}

		AddToNode(node, statement);
	}

	ExpectPunctuationType(ShaderPunctuationType::CloseBraces, info);

	return node;
}

void Magma::Framework::Graphics::ShaderParser::Run(const std::vector<ShaderToken> & in, ShaderSTNode *& out, ShaderCompilerData& data)
{
	ParserInfo info
	{
		data,
		in,
	};

	info.nextToken = info.tokens.begin();
	info.lineNumber = info.nextToken->lineNumber;

	while (PeekToken(info) != nullptr)
	{
		// Input vars
		if (AcceptTokenType(ShaderTokenType::Input, info))
		{
			data.inputName = ExpectTokenType(ShaderTokenType::Identifier, info)->attribute;

			ExpectPunctuationType(ShaderPunctuationType::OpenBraces, info);

			while (!PeekPunctuationType(ShaderPunctuationType::CloseBraces, info))
			{
				ShaderVariable var;

				var.type = ExpectTokenType(ShaderTokenType::Type, info)->variableType;
				var.id = ExpectTokenType(ShaderTokenType::Identifier, info)->attribute;
				ExpectPunctuationType(ShaderPunctuationType::Colon, info);
				var.name = ExpectTokenType(ShaderTokenType::Identifier, info)->attribute;
				ExpectPunctuationType(ShaderPunctuationType::Semicolon, info);

				data.inputVariables.push_back(var);
			}

			ExpectPunctuationType(ShaderPunctuationType::CloseBraces, info);
			ExpectPunctuationType(ShaderPunctuationType::Semicolon, info);
		}

		// Output vars
		else if (AcceptTokenType(ShaderTokenType::Output, info))
		{
			data.outputName = ExpectTokenType(ShaderTokenType::Identifier, info)->attribute;

			ExpectPunctuationType(ShaderPunctuationType::OpenBraces, info);

			while (!PeekPunctuationType(ShaderPunctuationType::CloseBraces, info))
			{
				ShaderVariable var;

				var.type = ExpectTokenType(ShaderTokenType::Type, info)->variableType;
				var.id = ExpectTokenType(ShaderTokenType::Identifier, info)->attribute;
				ExpectPunctuationType(ShaderPunctuationType::Colon, info);
				var.name = ExpectTokenType(ShaderTokenType::Identifier, info)->attribute;
				ExpectPunctuationType(ShaderPunctuationType::Semicolon, info);

				data.outputVariables.push_back(var);
			}

			ExpectPunctuationType(ShaderPunctuationType::CloseBraces, info);
			ExpectPunctuationType(ShaderPunctuationType::Semicolon, info);
		}

		// Texture 2D vars
		else if (AcceptTokenType(ShaderTokenType::Texture2D, info))
		{
			ShaderVariable var;

			var.id = ExpectTokenType(ShaderTokenType::Identifier, info)->attribute;
			ExpectPunctuationType(ShaderPunctuationType::Colon, info);
			var.name = ExpectTokenType(ShaderTokenType::Identifier, info)->attribute;
			var.type = ShaderVariableType::Texture2D;

			ExpectPunctuationType(ShaderPunctuationType::Semicolon, info);
			
			data.texture2Ds.push_back(var);
		}

		// Constant Buffers
		else if (AcceptTokenType(ShaderTokenType::ConstantBuffer, info))
		{
			ShaderVariable buf;

			buf.id = ExpectTokenType(ShaderTokenType::Identifier, info)->attribute;
			ExpectPunctuationType(ShaderPunctuationType::Colon, info);
			buf.name = ExpectTokenType(ShaderTokenType::Identifier, info)->attribute;
			buf.type = ShaderVariableType::ConstantBuffer;
			data.constantBuffers.push_back(buf);

			ExpectPunctuationType(ShaderPunctuationType::OpenBraces, info);

			while (!PeekPunctuationType(ShaderPunctuationType::CloseBraces, info))
			{
				ShaderVariable var;

				var.bufferName = buf.id;
				var.type = ExpectTokenType(ShaderTokenType::Type, info)->variableType;
				var.id = ExpectTokenType(ShaderTokenType::Identifier, info)->attribute;
				ExpectPunctuationType(ShaderPunctuationType::Semicolon, info);

				data.constantBufferVariables.push_back(var);
			}

			ExpectPunctuationType(ShaderPunctuationType::CloseBraces, info);
			ExpectPunctuationType(ShaderPunctuationType::Semicolon, info);
		}

		// Shader
		else if (AcceptTokenType(ShaderTokenType::Shader, info))
		{
			if (info.root != nullptr)
			{
				std::stringstream ss;
				ss << "Failed to run ShaderParser:" << std::endl;
				ss << "Shader is defined multiple times" << std::endl;
				ss << "Line: " << info.lineNumber;
				throw ShaderError(ss.str());
			}

			info.root = ParseScope(info);

			ExpectPunctuationType(ShaderPunctuationType::Semicolon, info);
		}

		// Unexpected token
		else
		{
			std::stringstream ss;
			ss << "Failed to run ShaderParser:" << std::endl;
			ss << "Unexpected token " << ShaderTokenToString(*PeekToken(info)) << "on the global scope:" << std::endl;
			ss << "Line: " << info.lineNumber;
			throw ShaderError(ss.str());
		}
	}

	out = info.root;
}

void Magma::Framework::Graphics::ShaderParser::Clean(ShaderSTNode * node)
{
	DestroyNode(node);
}

void Magma::Framework::Graphics::ShaderParser::Print(ShaderSTNode * node, size_t indentation)
{
	for (size_t i = 0; i < indentation; ++i)
		std::cout << "  ";
	std::cout << ShaderSTNodeToString(node) << std::endl;

	ShaderSTNode* c = node->child;
	while (c != nullptr)
	{
		ShaderParser::Print(c, indentation + 1);
		c = c->next;
	}
}
