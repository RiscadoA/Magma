#include "MFXToGLSL.hpp"

#include <sstream>
#include <regex>

using namespace Magma::Framework::Graphics;

void Compile(const std::string & in, std::string & out, ShaderType type)
{
	const int MajorVersion = 1;
	const int MinorVersion = 0;
	const int PatchVersion = 1;

	std::stringstream inss(in);
	std::stringstream outss;

	std::regex versionReg(R"regex(#mfx version (\d+)\.(\d+)\.(\d+))regex");
	std::regex contextReg(R"regex(#context (\w+))regex");
	std::regex shaderReg(R"regex(#shader (\w+))regex");
	std::smatch regMatch;

	std::string line;
	while (std::getline(inss, line) && line.empty());
	if (std::regex_search(line, regMatch, versionReg))
	{
		if (MajorVersion != std::stoi(regMatch.str(1)))
			throw std::runtime_error("Failed to compile from MFX to GLSL:\nMajor version unsupported");
		if (MinorVersion < std::stoi(regMatch.str(2)))
			throw std::runtime_error("Failed to compile from MFX to GLSL:\nMinor version unsupported");
	}
	else throw std::runtime_error("Failed to compile from MFX to GLSL:\nVersion header not found");

	switch (type)
	{
		case ShaderType::Vertex:
		case ShaderType::Pixel:
		{
			ShaderType typeDetected = ShaderType::Vertex;
			bool isGLSL = false;
			while (std::getline(inss, line))
			{
				if (std::regex_search(line, regMatch, contextReg))
				{
					if (regMatch.str(1) == "glsl")
						isGLSL = true;
					else
						isGLSL = false;
				}
				else if (std::regex_search(line, regMatch, shaderReg))
				{
					if (regMatch.str(1) == "vertex")
						typeDetected = ShaderType::Vertex;
					else if (regMatch.str(1) == "pixel")
						typeDetected = ShaderType::Pixel;
					else
					{
						std::stringstream ss;
						ss << "Failed to compile from MFX to GLSL:" << std::endl;
						ss << "Unsupported shader type \"" << regMatch.str(1) << "\"" << std::endl;
						throw std::runtime_error(ss.str());
					}
				}
				else if (isGLSL && typeDetected == type)
				{
					outss << line << std::endl;
				}
			}
		} break;
		default:
		{
			throw std::runtime_error("Failed to compile from MFX to GLSL:\nUnsupported shader type");
		} break;
	}

	out = outss.str();
}
