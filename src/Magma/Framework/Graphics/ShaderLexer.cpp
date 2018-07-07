#include "ShaderLexer.hpp"

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

extern const std::vector<std::tuple<std::string, ShaderToken, ShaderTokenType>> ShaderTokens;
const std::vector<std::tuple<std::string, ShaderToken, ShaderTokenType>> Magma::Framework::Graphics::ShaderTokens =
{
	std::make_tuple(R"(\{)", ShaderToken::OpenBrace, ShaderTokenType::Punctuation),
	std::make_tuple(R"(\})", ShaderToken::CloseBrace, ShaderTokenType::Punctuation),
	std::make_tuple(R"(\[)", ShaderToken::OpenBrackets, ShaderTokenType::Punctuation),
	std::make_tuple(R"(\])", ShaderToken::CloseBrackets, ShaderTokenType::Punctuation),
	std::make_tuple(R"(\()", ShaderToken::OpenParenthesis, ShaderTokenType::Punctuation),
	std::make_tuple(R"(\))", ShaderToken::CloseParenthesis, ShaderTokenType::Punctuation),
	std::make_tuple(R"(\;)", ShaderToken::Semicolon, ShaderTokenType::Punctuation),
	std::make_tuple(R"(\:)", ShaderToken::Colon, ShaderTokenType::Punctuation),
	std::make_tuple(R"(\,)", ShaderToken::Comma, ShaderTokenType::Punctuation),

	std::make_tuple(R"(\+)", ShaderToken::Add, ShaderTokenType::Operator),
	std::make_tuple(R"(\-)", ShaderToken::Subtract, ShaderTokenType::Operator),
	std::make_tuple(R"(\*)", ShaderToken::Multiply, ShaderTokenType::Operator),
	std::make_tuple(R"(\/)", ShaderToken::Divide, ShaderTokenType::Operator),

	std::make_tuple(R"(\=\=)", ShaderToken::Equal, ShaderTokenType::Operator),
	std::make_tuple(R"(\!\=)", ShaderToken::Different, ShaderTokenType::Operator),
	std::make_tuple(R"(\>)", ShaderToken::Greater, ShaderTokenType::Operator),
	std::make_tuple(R"(\<)", ShaderToken::Less, ShaderTokenType::Operator),
	std::make_tuple(R"(\>\=)", ShaderToken::GEqual, ShaderTokenType::Operator),
	std::make_tuple(R"(\<\=)", ShaderToken::LEqual, ShaderTokenType::Operator),

	std::make_tuple(R"(\&\&)", ShaderToken::And, ShaderTokenType::Operator),
	std::make_tuple(R"(\|\|)", ShaderToken::Or, ShaderTokenType::Operator),
	std::make_tuple(R"(\!)", ShaderToken::Not, ShaderTokenType::Operator),

	std::make_tuple(R"(\=)", ShaderToken::Assign, ShaderTokenType::Operator),

	std::make_tuple(R"(\.)", ShaderToken::Member, ShaderTokenType::Operator),
};