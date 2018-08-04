#include "MNIDocument.hpp"
#include "Exception.hpp"

#include <sstream>

Magma::Framework::String::MNIDocument::MNIDocument()
{

}

Magma::Framework::String::MNIDocument::~MNIDocument()
{

}

void Magma::Framework::String::MNIDocument::Read(const std::string & str)
{
	this->Clear();

	std::stringstream ss(str);
	std::string line;
	std::getline(ss, line);
	if (line.back() == '\r')
		line.pop_back();
	if (line != "#!MNI!")
	{
		std::stringstream ss;
		ss << "Failed to read MNI document:" << std::endl;
		ss << "#!MNI! marker not found";
		throw DocumentError(ss.str());
	}

	while (std::getline(ss, line))
	{
		if (line.back() == '\r')
			line.pop_back();
		if (line.empty())
			continue;
		else if (line[0] == '#')
			continue;
		else if (line[0] == '[')
		{
			std::stringstream ss2(line.substr(1));
			m_regions.emplace_back();
			std::getline(ss2, m_regions.back().name, ']');
		}
		else
		{
			if (m_regions.empty())
			{
				std::stringstream ss;
				ss << "Failed to read MNI document:" << std::endl;
				ss << "Failed to read entry:" << std::endl;
				ss << "A region must be declared before declaring any entries";
				throw DocumentError(ss.str());
			}

			MNIEntry entry;

			auto it = line.begin();
			std::string* target = &entry.name;
			while (it != line.end())
			{
				if (*it == '=')
				{
					if (target == &entry.value)
					{
						std::stringstream ss;
						ss << "Failed to read MNI document:" << std::endl;
						ss << "Failed to read entry '" << entry.name << "' on region '" << m_regions.back().name << "':" << std::endl;
						ss << "'=' sign found multiple times on the same line";
						throw DocumentError(ss.str());
					}
					else target = &entry.value;
				}
				else target->push_back(*it);
				++it;
			}

			m_regions.back().entries.push_back(entry);
		}
	}
}

void Magma::Framework::String::MNIDocument::Write(std::string & str) const
{
	std::stringstream ss;

	ss << "#!MNI!" << std::endl;
	ss << "# This MNI document was generated automatically by the Magma::Framework::String::MNIDocument::Write method" << std::endl;
	ss << std::endl;

	for (auto& r : m_regions)
	{
		ss << "[" << r.name << "]" << std::endl;
		for (auto& e : r.entries)
			ss << e.name << "=" << e.value << std::endl;
		ss << std::endl;
	}

	str = ss.str();
}

const Magma::Framework::String::MNIRegion & Magma::Framework::String::MNIDocument::GetRegion(const std::string & name) const
{
	for (auto& r : m_regions)
		if (r.name == name)
			return r;

	std::stringstream ss;
	ss << "Failed to get region from MNI document:" << std::endl;
	ss << "No region found with the name '" << name << "'";
	throw DocumentError(ss.str());
}

const Magma::Framework::String::MNIEntry & Magma::Framework::String::MNIDocument::GetEntry(const std::string & region, const std::string & name) const
{
	for (auto& r : m_regions)
		if (r.name == region)
		{
			for (auto& e : r.entries)
				if (e.name == name)
					return e;

			std::stringstream ss;
			ss << "Failed to get entry from MNI document:" << std::endl;
			ss << "No entry on the region '" << region << "' found with the name '" << name << "'";
			throw DocumentError(ss.str());
		}

	std::stringstream ss;
	ss << "Failed to get entry from MNI document:" << std::endl;
	ss << "No region found with the name '" << region << "'";
	throw DocumentError(ss.str());
}

void Magma::Framework::String::MNIDocument::AddRegion(const std::string & name)
{
	for (auto& r : m_regions)
		if (r.name == name)
		{
			std::stringstream ss;
			ss << "Failed to add region to MNI document:" << std::endl;
			ss << "There is already a region with the name '" << name << "'";
			throw DocumentError(ss.str());
		}

	m_regions.push_back(MNIRegion { name });
}

void Magma::Framework::String::MNIDocument::AddEntry(const std::string & region, const std::string & name, const std::string & value)
{
	for (auto& c : name)
		if (c == '\n' ||
			c == '[' ||
			c == ']')
		{
			std::stringstream ss;
			ss << "Failed to add entry to MNI document:" << std::endl;
			ss << "Invalid entry name:" << std::endl;
			ss << "Entry names cannot contain new lines, '[' nor ']'";
			throw DocumentError(ss.str());
		}

	for (auto& c : value)
		if (c == '\n' ||
			c == '[' ||
			c == ']')
		{
			std::stringstream ss;
			ss << "Failed to add entry to MNI document:" << std::endl;
			ss << "Invalid entry value:" << std::endl;
			ss << "Entry values cannot contain new lines, '[' nor ']'";
			throw DocumentError(ss.str());
		}

	for (auto& r : m_regions)
		if (r.name == region)
		{
			for (auto& e : r.entries)
				if (e.name == name)
				{
					std::stringstream ss;
					ss << "Failed to add entry to MNI document:" << std::endl;
					ss << "There is already an entry in the region '" << region << "' with the name '" << name << "'";
					throw DocumentError(ss.str());
				}

			r.entries.push_back(MNIEntry { name, value });
			return;
		}

	std::stringstream ss;
	ss << "Failed to add entry to MNI document:" << std::endl;
	ss << "No region found with the name '" << region << "'";
	throw DocumentError(ss.str());
}

void Magma::Framework::String::MNIDocument::RemoveRegion(const std::string & name)
{
	for (auto it = m_regions.begin(); it != m_regions.end(); ++it)
		if (it->name == name)
		{
			m_regions.erase(it);
			return;
		}

	std::stringstream ss;
	ss << "Failed to remove region from MNI document:" << std::endl;
	ss << "No region found with the name '" << name << "'";
	throw DocumentError(ss.str());
}

void Magma::Framework::String::MNIDocument::RemoveEntry(const std::string & region, const std::string & name)
{
	for (auto& r : m_regions)
		if (r.name == region)
		{
			for (auto it = r.entries.begin(); it != r.entries.end(); ++it)
				if (it->name == name)
				{
					r.entries.erase(it);
					return;
				}

			std::stringstream ss;
			ss << "Failed to remove entry from MNI document:" << std::endl;
			ss << "No entry on the region '" << region << "' found with the name '" << name << "'";
			throw DocumentError(ss.str());
		}

	std::stringstream ss;
	ss << "Failed to remove entry to MNI document:" << std::endl;
	ss << "No region found with the name '" << region << "'";
	throw DocumentError(ss.str());
}

void Magma::Framework::String::MNIDocument::Clear()
{
	m_regions.clear();
}
