#include "Compiler.hpp"

#include <functional>
#include <iostream>
#include <sstream>
#include <regex>
#include <map>

void Magma::Framework::Graphics::MSL::Compiler::RunPreprocessor()
{
	std::map<std::string, std::string> defs = {};

	std::stringstream out;
	std::regex versionRegex(R"regex(^\s*#version (\d+)\.(\d+).(\d+)\s*$)regex");
	std::regex defineRegex(R"regex(^\s*#define (\w+)\s+(.*)$)regex");
	std::smatch regMatch;

	// Remove comments
	std::stringstream in(m_code);
	m_realCodeLines.clear();
	
	{
		std::string line;
		int lineCount = 0;
		bool multiLineComment = false;
		while (std::getline(in, line))
		{
			for (size_t i = 0; i < line.size(); ++i)
			{
				if (multiLineComment)
				{
					if (line[i] == '*' && i < line.size() - 1 && line[i + 1] == '/')
					{
						multiLineComment = false;
						++i;
					}
				}
				else
				{
					if (line[i] == '/' && i < line.size() - 1 && line[i + 1] == '/')
						break;
					else if (line[i] == '/' && i < line.size() - 1 && line[i + 1] == '*')
						multiLineComment = true;
					else
						out << line[i];
				}
			}

			out << std::endl;
			m_realCodeLines.push_back(lineCount);
			++lineCount;
		}
	}

	m_code = out.str();
	m_output = m_code;

	// Check preprocessor
	out.str("");
	out.clear();
	in.str(m_code);
	in.clear();

	{
		std::string line;
		int lineCount = 0;
		while (std::getline(in, line))
		{
			// Check version
			if (std::regex_match(line, regMatch, versionRegex))
			{
				if (std::stoull(regMatch.str(1)) != m_major)
				{
					std::stringstream ss;
					ss << "Failed to compile MSL code:" << std::endl;
					ss << "Preprocessor stage failed:" << std::endl;
					ss << "Unsupported major version '" << regMatch.str(1) << "." << regMatch.str(2) << "." << regMatch.str(3) << "'; current version is '" << m_major << "." << m_minor << "." << m_patch << "'" << std::endl;
					throw std::runtime_error(ss.str());
				}

				if (std::stoull(regMatch.str(2)) > m_minor)
				{
					std::stringstream ss;
					ss << "Failed to compile MSL code:" << std::endl;
					ss << "Preprocessor stage failed:" << std::endl;
					ss << "Unsupported minor version '" << regMatch.str(1) << "." << regMatch.str(2) << "." << regMatch.str(3) << "'; current version is '" << m_major << "." << m_minor << "." << m_patch << "'" << std::endl;
					throw std::runtime_error(ss.str());
				}

				m_realCodeLines.erase(m_realCodeLines.begin() + (lineCount));
				--lineCount;
			}
			else if (std::regex_match(line, regMatch, defineRegex))
			{
				defs[regMatch.str(1)] = regMatch.str(2);
				if (regMatch.str(2).find(regMatch.str(1)) != std::string::npos)
				{
					std::stringstream ss;
					ss << "Failed to compile MSL code:" << std::endl;
					ss << "Preprocessor stage failed:" << std::endl;
					ss << "A preprocessor definition cannot reference itself: #define " << regMatch.str(1) << " " << regMatch.str(2) << std::endl;
					ss << "Line: " << m_realCodeLines[lineCount - 1] << std::endl;
					throw std::runtime_error(ss.str());
				}

				m_realCodeLines.erase(m_realCodeLines.begin() + (lineCount - 1));
				--lineCount;
			}
			else
			{
				// Replace definition references
				for (auto& def : defs)
				{
					while (size_t pos = line.find(def.first, 0))
					{
						if (pos == std::string::npos)
							break;
						line.replace(pos, def.first.length(), def.second);
					}
				}

				out << line << std::endl;

			}

			++lineCount;
		}
	}

	m_code = out.str();
	m_output = m_code;
}