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

const int MinorVersion = 2;

using namespace Magma::Framework;
using namespace Magma::Framework::Graphics;
using namespace Magma::Framework::Graphics::V1X;
using namespace Magma::Framework::Graphics::V1X::Version_1_X;

const std::vector<std::tuple<std::string, ShaderTokenType, ShaderVariableType, ShaderOperatorType, ShaderPunctuationType>> ShaderTokens =
{
	// Literals
	std::make_tuple(R"(\b(\d+\.\d+)f?\b)", ShaderTokenType::Literal, ShaderVariableType::Float1, ShaderOperatorType::Invalid, ShaderPunctuationType::Invalid),
	std::make_tuple(R"(\b(\d+)\b)", ShaderTokenType::Literal, ShaderVariableType::Int1, ShaderOperatorType::Invalid, ShaderPunctuationType::Invalid),

	// Types
	std::make_tuple(R"(\bint\b)", ShaderTokenType::Type, ShaderVariableType::Int1, ShaderOperatorType::Invalid, ShaderPunctuationType::Invalid),
	std::make_tuple(R"(\bint1\b)", ShaderTokenType::Type, ShaderVariableType::Int1, ShaderOperatorType::Invalid, ShaderPunctuationType::Invalid),
	std::make_tuple(R"(\bint2\b)", ShaderTokenType::Type, ShaderVariableType::Int2, ShaderOperatorType::Invalid, ShaderPunctuationType::Invalid),
	std::make_tuple(R"(\bint3\b)", ShaderTokenType::Type, ShaderVariableType::Int3, ShaderOperatorType::Invalid, ShaderPunctuationType::Invalid),
	std::make_tuple(R"(\bint4\b)", ShaderTokenType::Type, ShaderVariableType::Int4, ShaderOperatorType::Invalid, ShaderPunctuationType::Invalid),
	std::make_tuple(R"(\bint22\b)", ShaderTokenType::Type, ShaderVariableType::Int22, ShaderOperatorType::Invalid, ShaderPunctuationType::Invalid),
	std::make_tuple(R"(\bint33\b)", ShaderTokenType::Type, ShaderVariableType::Int33, ShaderOperatorType::Invalid, ShaderPunctuationType::Invalid),
	std::make_tuple(R"(\bint44\b)", ShaderTokenType::Type, ShaderVariableType::Int44, ShaderOperatorType::Invalid, ShaderPunctuationType::Invalid),

	std::make_tuple(R"(\bfloat\b)", ShaderTokenType::Type, ShaderVariableType::Float1, ShaderOperatorType::Invalid, ShaderPunctuationType::Invalid),
	std::make_tuple(R"(\bfloat1\b)", ShaderTokenType::Type, ShaderVariableType::Float1, ShaderOperatorType::Invalid, ShaderPunctuationType::Invalid),
	std::make_tuple(R"(\bfloat2\b)", ShaderTokenType::Type, ShaderVariableType::Float2, ShaderOperatorType::Invalid, ShaderPunctuationType::Invalid),
	std::make_tuple(R"(\bfloat3\b)", ShaderTokenType::Type, ShaderVariableType::Float3, ShaderOperatorType::Invalid, ShaderPunctuationType::Invalid),
	std::make_tuple(R"(\bfloat4\b)", ShaderTokenType::Type, ShaderVariableType::Float4, ShaderOperatorType::Invalid, ShaderPunctuationType::Invalid),
	std::make_tuple(R"(\bfloat22\b)", ShaderTokenType::Type, ShaderVariableType::Float22, ShaderOperatorType::Invalid, ShaderPunctuationType::Invalid),
	std::make_tuple(R"(\bfloat33\b)", ShaderTokenType::Type, ShaderVariableType::Float33, ShaderOperatorType::Invalid, ShaderPunctuationType::Invalid),
	std::make_tuple(R"(\bfloat44\b)", ShaderTokenType::Type, ShaderVariableType::Float44, ShaderOperatorType::Invalid, ShaderPunctuationType::Invalid),

	std::make_tuple(R"(\bbool\b)", ShaderTokenType::Type, ShaderVariableType::Bool, ShaderOperatorType::Invalid, ShaderPunctuationType::Invalid),

	// Operators
	std::make_tuple(R"(\+)", ShaderTokenType::Operator, ShaderVariableType::Invalid, ShaderOperatorType::Add, ShaderPunctuationType::Invalid),
	std::make_tuple(R"(\-)", ShaderTokenType::Operator, ShaderVariableType::Invalid, ShaderOperatorType::Subtract, ShaderPunctuationType::Invalid),
	std::make_tuple(R"(\*)", ShaderTokenType::Operator, ShaderVariableType::Invalid, ShaderOperatorType::Multiply, ShaderPunctuationType::Invalid),
	std::make_tuple(R"(\/)", ShaderTokenType::Operator, ShaderVariableType::Invalid, ShaderOperatorType::Divide, ShaderPunctuationType::Invalid),

	std::make_tuple(R"(\=\=)", ShaderTokenType::Operator, ShaderVariableType::Invalid, ShaderOperatorType::Equal, ShaderPunctuationType::Invalid),
	std::make_tuple(R"(\!\=)", ShaderTokenType::Operator, ShaderVariableType::Invalid, ShaderOperatorType::Different, ShaderPunctuationType::Invalid),
	std::make_tuple(R"(\>\=)", ShaderTokenType::Operator, ShaderVariableType::Invalid, ShaderOperatorType::GEqual, ShaderPunctuationType::Invalid),
	std::make_tuple(R"(\<\=)", ShaderTokenType::Operator, ShaderVariableType::Invalid, ShaderOperatorType::LEqual, ShaderPunctuationType::Invalid),
	std::make_tuple(R"(\>)", ShaderTokenType::Operator, ShaderVariableType::Invalid, ShaderOperatorType::Greater, ShaderPunctuationType::Invalid),
	std::make_tuple(R"(\<)", ShaderTokenType::Operator, ShaderVariableType::Invalid, ShaderOperatorType::Less, ShaderPunctuationType::Invalid),

	std::make_tuple(R"(\&\&)", ShaderTokenType::Operator, ShaderVariableType::Invalid, ShaderOperatorType::And, ShaderPunctuationType::Invalid),
	std::make_tuple(R"(\|\|)", ShaderTokenType::Operator, ShaderVariableType::Invalid, ShaderOperatorType::Or, ShaderPunctuationType::Invalid),
	std::make_tuple(R"(\!)", ShaderTokenType::Operator, ShaderVariableType::Invalid, ShaderOperatorType::Not, ShaderPunctuationType::Invalid),

	std::make_tuple(R"(\=)", ShaderTokenType::Operator, ShaderVariableType::Invalid, ShaderOperatorType::Assign, ShaderPunctuationType::Invalid),

	std::make_tuple(R"(\.)", ShaderTokenType::Operator, ShaderVariableType::Invalid, ShaderOperatorType::Member, ShaderPunctuationType::Invalid),

	// Punctuation
	std::make_tuple(R"(\{)", ShaderTokenType::Punctuation, ShaderVariableType::Invalid, ShaderOperatorType::Invalid, ShaderPunctuationType::OpenBraces),
	std::make_tuple(R"(\})", ShaderTokenType::Punctuation, ShaderVariableType::Invalid, ShaderOperatorType::Invalid, ShaderPunctuationType::CloseBraces),
	std::make_tuple(R"(\[)", ShaderTokenType::Punctuation, ShaderVariableType::Invalid, ShaderOperatorType::Invalid, ShaderPunctuationType::OpenBrackets),
	std::make_tuple(R"(\])", ShaderTokenType::Punctuation, ShaderVariableType::Invalid, ShaderOperatorType::Invalid, ShaderPunctuationType::CloseBrackets),
	std::make_tuple(R"(\()", ShaderTokenType::Punctuation, ShaderVariableType::Invalid, ShaderOperatorType::Invalid, ShaderPunctuationType::OpenParenthesis),
	std::make_tuple(R"(\))", ShaderTokenType::Punctuation, ShaderVariableType::Invalid, ShaderOperatorType::Invalid, ShaderPunctuationType::CloseParenthesis),
	std::make_tuple(R"(\;)", ShaderTokenType::Punctuation, ShaderVariableType::Invalid, ShaderOperatorType::Invalid, ShaderPunctuationType::Semicolon),
	std::make_tuple(R"(\:)", ShaderTokenType::Punctuation, ShaderVariableType::Invalid, ShaderOperatorType::Invalid, ShaderPunctuationType::Colon),
	std::make_tuple(R"(\,)", ShaderTokenType::Punctuation, ShaderVariableType::Invalid, ShaderOperatorType::Invalid, ShaderPunctuationType::Comma),

	// Other
	std::make_tuple(R"(\bShader\b)", ShaderTokenType::Shader, ShaderVariableType::Invalid, ShaderOperatorType::Invalid, ShaderPunctuationType::Invalid),
	std::make_tuple(R"(\bInput\b)", ShaderTokenType::Input, ShaderVariableType::Invalid, ShaderOperatorType::Invalid, ShaderPunctuationType::Invalid),
	std::make_tuple(R"(\bOutput\b)", ShaderTokenType::Output, ShaderVariableType::Invalid, ShaderOperatorType::Invalid, ShaderPunctuationType::Invalid),
	std::make_tuple(R"(\bTexture2D\b)", ShaderTokenType::Texture2D, ShaderVariableType::Invalid, ShaderOperatorType::Invalid, ShaderPunctuationType::Invalid),
	std::make_tuple(R"(\bConstantBuffer\b)", ShaderTokenType::ConstantBuffer, ShaderVariableType::Invalid, ShaderOperatorType::Invalid, ShaderPunctuationType::Invalid),
	std::make_tuple(R"(\bif\b)", ShaderTokenType::If, ShaderVariableType::Invalid, ShaderOperatorType::Invalid, ShaderPunctuationType::Invalid),
	std::make_tuple(R"(\belse\b)", ShaderTokenType::Else, ShaderVariableType::Invalid, ShaderOperatorType::Invalid, ShaderPunctuationType::Invalid),
	std::make_tuple(R"(\bwhile\b)", ShaderTokenType::While, ShaderVariableType::Invalid, ShaderOperatorType::Invalid, ShaderPunctuationType::Invalid),
	std::make_tuple(R"(\breturn\b)", ShaderTokenType::Return, ShaderVariableType::Invalid, ShaderOperatorType::Invalid, ShaderPunctuationType::Invalid),
	std::make_tuple(R"(\bdiscard\b)", ShaderTokenType::Discard, ShaderVariableType::Invalid, ShaderOperatorType::Invalid, ShaderPunctuationType::Invalid),
	std::make_tuple(R"(\b([a-zA-Z]\w*)\b)",ShaderTokenType::Identifier, ShaderVariableType::Invalid, ShaderOperatorType::Invalid, ShaderPunctuationType::Invalid),
};

void Magma::Framework::Graphics::V1X::Version_1_X::ShaderLexer::Run(const std::vector<ShaderLine>& in, std::vector<ShaderToken>& out, ShaderCompilerData& data)
{
	if (data.minorVersion > MinorVersion)
	{
		std::stringstream ss;
		ss << "Failed to run ShaderLexer:" << std::endl;
		ss << "Unsupported minor version '" << data.majorVersion << "." << data.minorVersion << "' (the current version is '1." << MinorVersion << "')";
		throw ShaderError(ss.str());
	}

	// Get regexes
	const static auto tokenRegexes = []()
	{
		std::vector<std::tuple<std::regex, ShaderTokenType, ShaderVariableType, ShaderOperatorType, ShaderPunctuationType>> tokenRegexes;
		for (auto& t : ShaderTokens)
			tokenRegexes.emplace_back(std::make_tuple(std::regex(std::get<0>(t)), std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t)));
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
				bool matched = false;
				for (size_t i = 0; i < tokenRegexes.size(); ++i)
				{
					if (std::regex_search(it, l.str.end(), match, std::get<0>(tokenRegexes[i]), std::regex_constants::match_continuous))
					{
						ShaderToken token;

						token.type = std::get<1>(tokenRegexes[i]);

						if (std::get<2>(tokenRegexes[i]) != ShaderVariableType::Invalid)
							token.variableType = std::get<2>(tokenRegexes[i]);
						else if (std::get<3>(tokenRegexes[i]) != ShaderOperatorType::Invalid)
							token.operatorType = std::get<3>(tokenRegexes[i]);
						else if (std::get<4>(tokenRegexes[i]) != ShaderPunctuationType::Invalid)
							token.punctuationType = std::get<4>(tokenRegexes[i]);
						else
							token.variableType = ShaderVariableType::Invalid;

						token.attribute = match.size() > 1 ? match.str(1) : "";
						token.lineNumber = l.lineNumber;

						out.push_back(token);
						it += match.length() - 1;
						matched = true;

						break;
					}
				}

				if (matched == false)
				{
					std::stringstream ss;
					ss << "Failed to run ShaderLexer:" << std::endl;
					ss << "Unknown character '" << *it << "', no token matches this character:" << std::endl;
					ss << "Line: " << l.lineNumber;
					throw ShaderError(ss.str());
				}
			}
		}
}
