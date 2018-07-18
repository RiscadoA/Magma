#include "ShaderLexer.hpp"

#include <regex>
#include <sstream>

/*
	#version 1.0
	#type VERTEX

	Input vertexIn
	{
		float3 position : POSITION;
		float2 uvs : UVS;
	};

	Output vertexOut
	{
		float4 position : R_VERTEX_POSITION;
		float2 uvs : VERTEX_UVS;	
	};

	ConstantBuffer transform : TRANSFORM
	{
		float44 mvp;
	};

	Shader
	{
		vertexOut.position = transform.mvp * float4(vertexIn.position.x, vertexIn.position.y, vertexIn.position.z, 1.0);
		vertexOut.uvs = vertexIn.uvs;
	};
*/

using namespace Magma::Framework;
using namespace Magma::Framework::Graphics;
const std::vector<std::tuple<std::string, ShaderTokenSymbol, ShaderTokenType, std::string>> Magma::Framework::Graphics::ShaderTokens =
{
	std::make_tuple(R"(\bint\b)", ShaderTokenSymbol::Int1, ShaderTokenType::Type, "int"),
	std::make_tuple(R"(\bint1\b)", ShaderTokenSymbol::Int1, ShaderTokenType::Type, "int"),
	std::make_tuple(R"(\bint2\b)", ShaderTokenSymbol::Int2, ShaderTokenType::Type, "int2"),
	std::make_tuple(R"(\bint3\b)", ShaderTokenSymbol::Int3, ShaderTokenType::Type, "int3"),
	std::make_tuple(R"(\bint4\b)", ShaderTokenSymbol::Int4, ShaderTokenType::Type, "int4"),
	std::make_tuple(R"(\bint22\b)", ShaderTokenSymbol::Int22, ShaderTokenType::Type, "int22"),
	std::make_tuple(R"(\bint33\b)", ShaderTokenSymbol::Int33, ShaderTokenType::Type, "int33"),
	std::make_tuple(R"(\bint44\b)", ShaderTokenSymbol::Int44, ShaderTokenType::Type, "int44"),

	std::make_tuple(R"(\bfloat\b)", ShaderTokenSymbol::Float1, ShaderTokenType::Type, "float"),
	std::make_tuple(R"(\bfloat1\b)", ShaderTokenSymbol::Float1, ShaderTokenType::Type, "float"),
	std::make_tuple(R"(\bfloat2\b)", ShaderTokenSymbol::Float2, ShaderTokenType::Type, "float2"),
	std::make_tuple(R"(\bfloat3\b)", ShaderTokenSymbol::Float3, ShaderTokenType::Type, "float3"),
	std::make_tuple(R"(\bfloat4\b)", ShaderTokenSymbol::Float4, ShaderTokenType::Type, "float4"),
	std::make_tuple(R"(\bfloat22\b)", ShaderTokenSymbol::Float22, ShaderTokenType::Type, "float22"),
	std::make_tuple(R"(\bfloat33\b)", ShaderTokenSymbol::Float33, ShaderTokenType::Type, "float33"),
	std::make_tuple(R"(\bfloat44\b)", ShaderTokenSymbol::Float44, ShaderTokenType::Type, "float44"),

	std::make_tuple(R"(\bbool\b)", ShaderTokenSymbol::Bool, ShaderTokenType::Type, "bool"),

	std::make_tuple(R"(\bShader\b)", ShaderTokenSymbol::Shader, ShaderTokenType::Keyword, "Shader"),
	std::make_tuple(R"(\bInput\b)", ShaderTokenSymbol::Input, ShaderTokenType::Keyword, "Input"),
	std::make_tuple(R"(\bOutput\b)", ShaderTokenSymbol::Output, ShaderTokenType::Keyword, "Output"),
	std::make_tuple(R"(\bTexture2D\b)", ShaderTokenSymbol::Texture2D, ShaderTokenType::Keyword, "Texture2D"),
	std::make_tuple(R"(\bConstantBuffer\b)", ShaderTokenSymbol::ConstantBuffer, ShaderTokenType::Keyword, "ConstantBuffer"),
	std::make_tuple(R"(\bif\b)", ShaderTokenSymbol::If, ShaderTokenType::Keyword, "if"),
	std::make_tuple(R"(\belse\b)", ShaderTokenSymbol::Else, ShaderTokenType::Keyword, "else"),
	std::make_tuple(R"(\bwhile\b)", ShaderTokenSymbol::While, ShaderTokenType::Keyword, "while"),
	std::make_tuple(R"(\breturn\b)", ShaderTokenSymbol::Return, ShaderTokenType::Keyword, "return"),
	std::make_tuple(R"(\bdiscard\b)", ShaderTokenSymbol::Discard, ShaderTokenType::Keyword, "discard"),

	std::make_tuple(R"(\{)", ShaderTokenSymbol::OpenBrace, ShaderTokenType::Punctuation, "{"),
	std::make_tuple(R"(\})", ShaderTokenSymbol::CloseBrace, ShaderTokenType::Punctuation, "}"),
	std::make_tuple(R"(\[)", ShaderTokenSymbol::OpenBrackets, ShaderTokenType::Punctuation, "["),
	std::make_tuple(R"(\])", ShaderTokenSymbol::CloseBrackets, ShaderTokenType::Punctuation, "]"),
	std::make_tuple(R"(\()", ShaderTokenSymbol::OpenParenthesis, ShaderTokenType::Punctuation, "("),
	std::make_tuple(R"(\))", ShaderTokenSymbol::CloseParenthesis, ShaderTokenType::Punctuation, ")"),
	std::make_tuple(R"(\;)", ShaderTokenSymbol::Semicolon, ShaderTokenType::Punctuation, ";"),
	std::make_tuple(R"(\:)", ShaderTokenSymbol::Colon, ShaderTokenType::Punctuation, ":"),
	std::make_tuple(R"(\,)", ShaderTokenSymbol::Comma, ShaderTokenType::Punctuation, ","),

	std::make_tuple(R"(\+)", ShaderTokenSymbol::Add, ShaderTokenType::Operator, "+"),
	std::make_tuple(R"(\-)", ShaderTokenSymbol::Subtract, ShaderTokenType::Operator, "-"),
	std::make_tuple(R"(\*)", ShaderTokenSymbol::Multiply, ShaderTokenType::Operator, "*"),
	std::make_tuple(R"(\/)", ShaderTokenSymbol::Divide, ShaderTokenType::Operator, "/"),

	std::make_tuple(R"(\=\=)", ShaderTokenSymbol::Equal, ShaderTokenType::Operator, "=="),
	std::make_tuple(R"(\!\=)", ShaderTokenSymbol::Different, ShaderTokenType::Operator, "!="),
	std::make_tuple(R"(\>)", ShaderTokenSymbol::Greater, ShaderTokenType::Operator, ">"),
	std::make_tuple(R"(\<)", ShaderTokenSymbol::Less, ShaderTokenType::Operator, "<"),
	std::make_tuple(R"(\>\=)", ShaderTokenSymbol::GEqual, ShaderTokenType::Operator, ">="),
	std::make_tuple(R"(\<\=)", ShaderTokenSymbol::LEqual, ShaderTokenType::Operator, "<="),

	std::make_tuple(R"(\&\&)", ShaderTokenSymbol::And, ShaderTokenType::Operator, "&&"),
	std::make_tuple(R"(\|\|)", ShaderTokenSymbol::Or, ShaderTokenType::Operator, "||"),
	std::make_tuple(R"(\!)", ShaderTokenSymbol::Not, ShaderTokenType::Operator, "!"),

	std::make_tuple(R"(\=)", ShaderTokenSymbol::Assign, ShaderTokenType::Operator, "="),

	std::make_tuple(R"(\.)", ShaderTokenSymbol::Member, ShaderTokenType::Operator, "."),

	std::make_tuple(R"(\b([a-zA-Z]\w*)\b)", ShaderTokenSymbol::Identifier, ShaderTokenType::Identifier, "identifier"),
};

void Magma::Framework::Graphics::ShaderLexer::Run(const std::vector<ShaderLine>& in, std::vector<ShaderToken>& out, ShaderCompilerData& data)
{
	// Get regexes
	const static auto tokenRegexes = []()
	{
		std::vector<std::tuple<std::regex, ShaderTokenSymbol, ShaderTokenType>> tokenRegexes;
		for (auto& t : ShaderTokens)
			tokenRegexes.emplace_back(std::make_tuple(std::regex(std::get<0>(t)), std::get<1>(t), std::get<2>(t)));
		return tokenRegexes;
	} ();

	// Tokenize input
	std::smatch match;
	out.clear();
	for (auto& l : in)
		for (auto it = l.str.begin(); it != l.str.end(); ++it)
		{
			if (*it != '\n' &&
				*it != '\t' &&
				*it != ' ')
			{
				bool did = false;
				for (size_t i = 0; i < tokenRegexes.size(); ++i)
				{
					if (std::regex_search(it, l.str.end(), match, std::get<0>(tokenRegexes[i]), std::regex_constants::match_continuous))
					{
						ShaderToken token;
						token.symbol = std::get<1>(tokenRegexes[i]);
						token.type = std::get<2>(tokenRegexes[i]);
						token.attribute = match.size() > 1 ? match.str(1) : "";
						token.lineNumber = l.lineNumber;
						out.push_back(token);
						it += match.length() - 1;
						did = true;
						break;
					}
				}

				if (did == false)
				{
					std::stringstream ss;
					ss << "Failed to run ShaderLexer:" << std::endl;
					ss << "Unknown character '" << *it << "', no token matches this character:" << std::endl;
					ss << "Line: " << l.lineNumber;
					throw ShaderError(ss.str());
				}
			}
		}

	//for (auto& t : out)
	//	printf("'%s' \"%s\"\n", ShaderTokenSymbolToString(t.symbol).c_str(), t.attribute.c_str());
}

std::string Magma::Framework::Graphics::ShaderTokenSymbolToString(ShaderTokenSymbol symbol)
{
	for (auto& t : ShaderTokens)
		if (std::get<1>(t) == symbol)
			return std::get<3>(t);
	if (symbol == ShaderTokenSymbol::Invalid)
		return "$invalid-token$";
	else
		return "$unsupported-token$";
}
