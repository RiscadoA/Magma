#include "Compiler.hpp"

using namespace Magma::Framework::Graphics::MSL;

#include <unordered_map>
#include <regex>

// Token regular expressions
static const std::unordered_map<std::string, TokenSymbol> tokenRegexStrings
{
	std::make_pair<std::string, TokenSymbol>("\\{", TokenSymbol::OpenBrace),
	std::make_pair<std::string, TokenSymbol>("\\}", TokenSymbol::CloseBrace),
	std::make_pair<std::string, TokenSymbol>("\\(", TokenSymbol::OpenParenthesis),
	std::make_pair<std::string, TokenSymbol>("\\)", TokenSymbol::CloseParenthesis),
	std::make_pair<std::string, TokenSymbol>("\\;", TokenSymbol::Semicolon),
	std::make_pair<std::string, TokenSymbol>("\\,", TokenSymbol::Comma),

	std::make_pair<std::string, TokenSymbol>("\\bint\\b", TokenSymbol::Int),
	std::make_pair<std::string, TokenSymbol>("\\bfloat\\b", TokenSymbol::Float),
	std::make_pair<std::string, TokenSymbol>("\\bvec2\\b", TokenSymbol::Vec2),
	std::make_pair<std::string, TokenSymbol>("\\bvec3\\b", TokenSymbol::Vec3),
	std::make_pair<std::string, TokenSymbol>("\\bvec4\\b", TokenSymbol::Vec4),
	std::make_pair<std::string, TokenSymbol>("\\bivec2\\b", TokenSymbol::IVec2),
	std::make_pair<std::string, TokenSymbol>("\\bivec3\\b", TokenSymbol::IVec3),
	std::make_pair<std::string, TokenSymbol>("\\bivec4\\b", TokenSymbol::IVec4),
	std::make_pair<std::string, TokenSymbol>("\\bmat2\\b", TokenSymbol::Mat2),
	std::make_pair<std::string, TokenSymbol>("\\bmat3\\b", TokenSymbol::Mat3),
	std::make_pair<std::string, TokenSymbol>("\\bmat4\\b", TokenSymbol::Mat4),

	std::make_pair<std::string, TokenSymbol>("\\+", TokenSymbol::Add),
	std::make_pair<std::string, TokenSymbol>("\\-", TokenSymbol::Sub),
	std::make_pair<std::string, TokenSymbol>("\\*", TokenSymbol::Mul),
	std::make_pair<std::string, TokenSymbol>("\\/", TokenSymbol::Div),
	std::make_pair<std::string, TokenSymbol>("\\%", TokenSymbol::Mod),
	std::make_pair<std::string, TokenSymbol>("\\.", TokenSymbol::Member),
	std::make_pair<std::string, TokenSymbol>("\\=", TokenSymbol::Assignment),

	std::make_pair<std::string, TokenSymbol>("\\breturn\\b", TokenSymbol::Return),
	std::make_pair<std::string, TokenSymbol>("\\bVertexOutput\\b", TokenSymbol::VertexOutput),

	std::make_pair<std::string, TokenSymbol>("\\b(\\d+)\\b", TokenSymbol::IntLiteral),
	std::make_pair<std::string, TokenSymbol>("\\b(\\d+\\.\\d+)f?\\b", TokenSymbol::FloatLiteral),

	std::make_pair<std::string, TokenSymbol>("\\b([a-zA-Z]\\w*)\\b", TokenSymbol::Identifier),
};

void Magma::Framework::Graphics::MSL::Compiler::RunLexer()
{
	// Get regexes
	const static std::vector<std::pair<std::regex, TokenSymbol>> tokenRegexes = []() -> std::vector<std::pair<std::regex, TokenSymbol>>
	{
		std::vector<std::pair<std::regex, TokenSymbol>> tokenRegexes;
		for (auto& r : tokenRegexStrings)
			tokenRegexes.emplace_back(std::make_pair(std::regex(r.first), r.second));
		return tokenRegexes;
	} ();
	
	// Match token regexes to get the tokens
	m_tokens.clear();
	int line = 1;
	std::match_results<std::string::iterator> match;
	auto it = m_code.begin();
	while (it != m_code.end())
	{
		if (*it == '\n')
			line++;

		if (*it != '\n' &&
			*it != '\t' &&
			*it != ' ')
			for (auto& r : tokenRegexes)
			{
				if (std::regex_search(it, m_code.end(), match, r.first, std::regex_constants::match_default | std::regex_constants::match_continuous))
				{
					Token token;
					token.symbol = r.second;
					token.type = GetTokenType(token.symbol);
					token.attribute = match.size() > 1 ? match.str(1) : "";
					token.line = m_realCodeLines[line];
					m_tokens.push_back(token);
					it += match.length();
					break;
				}
			}
		else ++it;

	}
}
