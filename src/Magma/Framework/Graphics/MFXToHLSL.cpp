#include "MFXToHLSL.hpp"

#include <sstream>
#include <regex>

using namespace Magma::Framework::Graphics;

void Compile(const std::string & in, std::string & out)
{
	const int MajorVersion = 1;
	const int MinorVersion = 0;
	const int PatchVersion = 1;

	std::stringstream inss(in);
	std::stringstream outss;

	std::regex versionReg(R"regex(#mfx version (\d+)\.(\d+)\.(\d+))regex");
	std::regex contextReg(R"regex(#context (\w+))regex");
	std::smatch regMatch;

	std::string line;
	while (std::getline(inss, line) && line.empty());
	if (std::regex_search(line, regMatch, versionReg))
	{
		if (MajorVersion != std::stoi(regMatch.str(1)))
			throw std::runtime_error("Failed to compile from MFX to HLSL:\nMajor version unsupported");
		if (MinorVersion < std::stoi(regMatch.str(2)))
			throw std::runtime_error("Failed to compile from MFX to HLSL:\nMinor version unsupported");
	}
	else throw std::runtime_error("Failed to compile from MFX to HLSL:\nVersion header not found");

	bool isHLSL = false;
	while (std::getline(inss, line))
	{
		if (std::regex_search(line, regMatch, contextReg))
		{
			if (regMatch.str(1) == "hlsl")
				isHLSL = true;
			else
				isHLSL = false;
		}
		else if (isHLSL)
		{
			outss << line << std::endl;
		}
	}

	out = outss.str();
}
