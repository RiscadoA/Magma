#include "Path.hpp"

#include <regex>
#include <sstream>

using namespace Magma::Framework::File;

Magma::Framework::File::Path::Path(const std::string & path)
{
	if (path.empty() || path.front() != '/')
	{
		std::stringstream ss;
		ss << "Failed to create path \"" << path << "\":" << std::endl;
		ss << "Path cannot be empty and must start by '/'";
		throw FileError(ss.str());
	}

	this->Parse(path);
}

Magma::Framework::File::Path::Path(const Path & path)
{
	m_path = path.m_path;
}

Magma::Framework::File::Path::Path(Path && path)
{
	m_path = std::move(path.m_path);
}

Magma::Framework::File::Path::Path(const Path & relative, const std::string & path)
{
	if (!relative.IsDirectory())
	{
		std::stringstream ss;
		ss << "Failed to set path to \"" << path << "\" relative to \"" << relative.ToString() << "\":" << std::endl;
		ss << "A path must be relative to a directory (can't be relative to a file)";
		throw FileError(ss.str());
	}

	this->Parse(relative.ToString() + path);
}

Path & Magma::Framework::File::Path::operator=(const Path & path)
{
	m_path = path.m_path;
	return *this;
}

Path & Magma::Framework::File::Path::operator=(const std::string & path)
{
	if (path.empty() || path.front() != '/')
	{
		std::stringstream ss;
		ss << "Failed to set path to \"" << path << "\":" << std::endl;
		ss << "Path cannot be empty and must start by '/'";
		throw FileError(ss.str());
	}
	this->Parse(path);
	return *this;
}

std::string Magma::Framework::File::Path::GetExtension() const
{
	if (this->IsFile())
	{
		static std::regex regex(R"regex(^.+\.(.+)$)regex");
		std::smatch match;
		if (std::regex_match(m_path.back(), match, regex))
			return match.str(1);
		else
			return "";
	}
	else
	{
		std::stringstream ss;
		ss << "Failed to get extension of path \"" << this->ToString() << "\":" << std::endl;
		ss << "Path doesn't point to a file";
		throw FileError(ss.str());
	}
}

std::string Magma::Framework::File::Path::GetName() const
{
	if (this->IsFile())
	{
		static std::regex regex1(R"regex(^(.+)\..+$)regex");
		static std::regex regex2(R"regex(^(.+)$)regex");
		std::smatch match;
		if (std::regex_match(m_path.back(), match, regex1))
			return match.str(1);
		else if (std::regex_match(m_path.back(), match, regex2))
			return match.str(1);
		else
		{
			std::stringstream ss;
			ss << "Failed to get path \"" << this->ToString() << "\" name:" << std::endl;
			ss << "Regex doesn't match, invalid path";
			throw FileError(ss.str());
		}
	}
	else
	{
		static std::regex regex(R"regex(^(.+)\/$)regex");
		std::smatch match;
		if (std::regex_match(m_path.back(), match, regex))
			return match.str(1);
		else
		{
			std::stringstream ss;
			ss << "Failed to get path \"" << this->ToString() << "\" name:" << std::endl;
			ss << "Regex doesn't match, invalid path";
			throw FileError(ss.str());
		}
	}
}

bool Magma::Framework::File::Path::IsFile() const
{
	return !m_path.empty() && m_path.back().back() != '/';
}

bool Magma::Framework::File::Path::IsDirectory() const
{
	return m_path.empty() || m_path.back().back() == '/';
}

Path Magma::Framework::File::Path::GetDirectory() const
{
	std::string str = "/";
	for (auto it = m_path.begin(); it != m_path.end() - 1; ++it)
		str += *it;
	return str;
}

std::string Magma::Framework::File::Path::ToString() const
{
	std::stringstream ss;
	ss << "/";
	for (auto& p : m_path)
		ss << p;
	return ss.str();
}

void Magma::Framework::File::Path::Parse(const std::string& str)
{
	if (str.empty())
	{
		std::stringstream ss;
		ss << "Failed to parse path \"" << str << "\":" << std::endl;
		ss << "A path cannot be empty";
		throw FileError(ss.str());
	}

	if (str.front() != '/')
	{
		std::stringstream ss;
		ss << "Failed to parse path \"" << str << "\":" << std::endl;
		ss << "A path must start by '/'";
		throw FileError(ss.str());
	}

	m_path.clear();
	std::string name = "";
	bool lastWasPeriod = false;
	bool goUp = false;
	for (auto c : str)
	{
		if (goUp)
		{
			goUp = false;
			if (c != '/')
			{
				std::stringstream ss;
				ss << "Failed to parse path \"" << str << "\":" << std::endl;
				ss << "Unexpected token '" << c << "' after '..', should be '/'";
				throw FileError(ss.str());
			}

			if (m_path.empty())
			{
				std::stringstream ss;
				ss << "Failed to parse path \"" << str << "\":" << std::endl;
				ss << "Unexpected token '..', can't go to parent directory because it would outside of the root";
				throw FileError(ss.str());
			}

			m_path.pop_back();
		}

		switch (c)
		{
			case '/':
				if (name.empty() && m_path.empty())
					continue;
				if (name.empty())
				{
					std::stringstream ss;
					ss << "Failed to parse path \"" << str << "\":" << std::endl;
					ss << "Unexpected token '/', a directory must have a name";
					throw FileError(ss.str());
				}
				m_path.push_back(name + '/');
				name = "";
				break;
			case '.':
				if (lastWasPeriod)
				{
					lastWasPeriod = false;
					goUp = true;
				}
				else lastWasPeriod = true;
				break;
			default:
				if (lastWasPeriod)
				{
					lastWasPeriod = false;
					name += '.';
				}
				name += c;
				break;
		}
	}

	if (!name.empty())
		m_path.push_back(name);
}
