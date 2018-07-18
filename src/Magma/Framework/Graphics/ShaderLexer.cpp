#include "ShaderLexer.hpp"

#include <regex>

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

extern const std::vector<std::tuple<std::string, ShaderTokenSymbol, ShaderTokenType>> ShaderTokens;
const std::vector<std::tuple<std::string, ShaderTokenSymbol, ShaderTokenType>> Magma::Framework::Graphics::ShaderTokens =
{
	std::make_tuple(R"(int)", ShaderTokenSymbol::Int1, ShaderTokenType::Type),
	std::make_tuple(R"(int1)", ShaderTokenSymbol::Int1, ShaderTokenType::Type),
	std::make_tuple(R"(int2)", ShaderTokenSymbol::Int2, ShaderTokenType::Type),
	std::make_tuple(R"(int3)", ShaderTokenSymbol::Int3, ShaderTokenType::Type),
	std::make_tuple(R"(int4)", ShaderTokenSymbol::Int4, ShaderTokenType::Type),
	std::make_tuple(R"(int22)", ShaderTokenSymbol::Int22, ShaderTokenType::Type),
	std::make_tuple(R"(int33)", ShaderTokenSymbol::Int33, ShaderTokenType::Type),
	std::make_tuple(R"(int44)", ShaderTokenSymbol::Int44, ShaderTokenType::Type),

	std::make_tuple(R"(float)", ShaderTokenSymbol::Float1, ShaderTokenType::Type),
	std::make_tuple(R"(float1)", ShaderTokenSymbol::Float1, ShaderTokenType::Type),
	std::make_tuple(R"(float2)", ShaderTokenSymbol::Float2, ShaderTokenType::Type),
	std::make_tuple(R"(float3)", ShaderTokenSymbol::Float3, ShaderTokenType::Type),
	std::make_tuple(R"(float4)", ShaderTokenSymbol::Float4, ShaderTokenType::Type),
	std::make_tuple(R"(float22)", ShaderTokenSymbol::Float22, ShaderTokenType::Type),
	std::make_tuple(R"(float33)", ShaderTokenSymbol::Float33, ShaderTokenType::Type),
	std::make_tuple(R"(float44)", ShaderTokenSymbol::Float44, ShaderTokenType::Type),

	std::make_tuple(R"(bool)", ShaderTokenSymbol::Bool, ShaderTokenType::Type),

	std::make_tuple(R"(shader)", ShaderTokenSymbol::Shader, ShaderTokenType::Keyword),
	std::make_tuple(R"(input)", ShaderTokenSymbol::Input, ShaderTokenType::Keyword),
	std::make_tuple(R"(output)", ShaderTokenSymbol::Output, ShaderTokenType::Keyword),
	std::make_tuple(R"(texture2d)", ShaderTokenSymbol::Texture2D, ShaderTokenType::Keyword),
	std::make_tuple(R"(constantbuffer)", ShaderTokenSymbol::ConstantBuffer, ShaderTokenType::Keyword),
	std::make_tuple(R"(if)", ShaderTokenSymbol::If, ShaderTokenType::Keyword),
	std::make_tuple(R"(else)", ShaderTokenSymbol::Else, ShaderTokenType::Keyword),
	std::make_tuple(R"(while)", ShaderTokenSymbol::While, ShaderTokenType::Keyword),
	std::make_tuple(R"(return)", ShaderTokenSymbol::Return, ShaderTokenType::Keyword),
	std::make_tuple(R"(discard)", ShaderTokenSymbol::Discard, ShaderTokenType::Keyword),

	std::make_tuple(R"(\{)", ShaderTokenSymbol::OpenBrace, ShaderTokenType::Punctuation),
	std::make_tuple(R"(\})", ShaderTokenSymbol::CloseBrace, ShaderTokenType::Punctuation),
	std::make_tuple(R"(\[)", ShaderTokenSymbol::OpenBrackets, ShaderTokenType::Punctuation),
	std::make_tuple(R"(\])", ShaderTokenSymbol::CloseBrackets, ShaderTokenType::Punctuation),
	std::make_tuple(R"(\()", ShaderTokenSymbol::OpenParenthesis, ShaderTokenType::Punctuation),
	std::make_tuple(R"(\))", ShaderTokenSymbol::CloseParenthesis, ShaderTokenType::Punctuation),
	std::make_tuple(R"(\;)", ShaderTokenSymbol::Semicolon, ShaderTokenType::Punctuation),
	std::make_tuple(R"(\:)", ShaderTokenSymbol::Colon, ShaderTokenType::Punctuation),
	std::make_tuple(R"(\,)", ShaderTokenSymbol::Comma, ShaderTokenType::Punctuation),

	std::make_tuple(R"(\+)", ShaderTokenSymbol::Add, ShaderTokenType::Operator),
	std::make_tuple(R"(\-)", ShaderTokenSymbol::Subtract, ShaderTokenType::Operator),
	std::make_tuple(R"(\*)", ShaderTokenSymbol::Multiply, ShaderTokenType::Operator),
	std::make_tuple(R"(\/)", ShaderTokenSymbol::Divide, ShaderTokenType::Operator),

	std::make_tuple(R"(\=\=)", ShaderTokenSymbol::Equal, ShaderTokenType::Operator),
	std::make_tuple(R"(\!\=)", ShaderTokenSymbol::Different, ShaderTokenType::Operator),
	std::make_tuple(R"(\>)", ShaderTokenSymbol::Greater, ShaderTokenType::Operator),
	std::make_tuple(R"(\<)", ShaderTokenSymbol::Less, ShaderTokenType::Operator),
	std::make_tuple(R"(\>\=)", ShaderTokenSymbol::GEqual, ShaderTokenType::Operator),
	std::make_tuple(R"(\<\=)", ShaderTokenSymbol::LEqual, ShaderTokenType::Operator),

	std::make_tuple(R"(\&\&)", ShaderTokenSymbol::And, ShaderTokenType::Operator),
	std::make_tuple(R"(\|\|)", ShaderTokenSymbol::Or, ShaderTokenType::Operator),
	std::make_tuple(R"(\!)", ShaderTokenSymbol::Not, ShaderTokenType::Operator),

	std::make_tuple(R"(\=)", ShaderTokenSymbol::Assign, ShaderTokenType::Operator),

	std::make_tuple(R"(\.)", ShaderTokenSymbol::Member, ShaderTokenType::Operator),
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

	
}
