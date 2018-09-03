#include "ShaderPreprocessor.hpp"

#include "../../String/Conversion.hpp"

#include <sstream>
#include <regex>
#include <iostream>

void Magma::Framework::Graphics::V1X::ShaderPreprocessor::Run(const std::string & in, std::vector<ShaderLine>& out, ShaderCompilerData& data)
{
	std::stringstream ss(in);
	std::string line;
	size_t lineCount = 0;

	data.majorVersion = 0;
	data.minorVersion = 0;
	data.shaderType = ShaderType::Invalid;

	std::regex versionRegex(R"(^\s*#version (\d+)\.(\d+)\s*$)");
	std::regex typeRegex(R"(^#type (\w+)\s+$)");
	std::smatch match;

	// Run preprocessor
	out.clear();
	bool isComment = false;
	while (std::getline(ss, line))
	{
		lineCount++;

		if (!isComment && std::regex_match(line, match, versionRegex))
		{
			data.majorVersion = std::stoul(match.str(1));
			data.minorVersion = std::stoul(match.str(2));
		}
		else if (!isComment && std::regex_match(line, match, typeRegex))
		{
			std::string type = match.str(1);
			String::ToLower(type);
			if (type == "vertex")
				data.shaderType = ShaderType::Vertex;
			else if (type == "pixel")
				data.shaderType = ShaderType::Pixel;
			else
			{
				std::stringstream ss;
				ss << "Failed to run ShaderPreprocessor:" << std::endl;
				ss << "Unknown shader type \"" << type << "\"" << std::endl;
				ss << "Line: " << lineCount;
				throw ShaderError(ss.str());
			}
		}
		else
		{
			out.emplace_back();
			auto& l = out.back();
			l.lineNumber = lineCount;

			l.str.reserve(line.size());
			for (auto it = line.begin(); it != line.end(); ++it)
			{
				if (*it == '\r')
					continue;

				if (!isComment && *(it) == '/' && it + 1 != line.end() && *(it + 1) == '/')
					break;
				else if (*(it) == '/' && it + 1 != line.end() && *(it + 1) == '*')
				{
					isComment = true;
					++it;
				}
				else if (*(it) == '*' && it + 1 != line.end() && *(it + 1) == '/')
				{
					isComment = false;
					++it;
				}
				else if (!isComment)
				{
					l.str += *it;
				}
			}
		}
	}

	if (data.shaderType == ShaderType::Invalid)
	{
		std::stringstream ss;
		ss << "Failed to run ShaderPreprocessor:" << std::endl;
		ss << "The shader must define its type (didn't find \"#type [TYPE]\")";
		throw ShaderError(ss.str());
	}
}
