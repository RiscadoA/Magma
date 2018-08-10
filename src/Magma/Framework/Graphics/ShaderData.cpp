#include "ShaderData.hpp"
#include "../Memory/Endianness.hpp"

#include <sstream>

using namespace Magma::Framework::Graphics;

Magma::Framework::Graphics::ShaderData::ShaderData(const char * binaryObject, size_t binaryObjectSize)
{
	if (4 > binaryObjectSize)
		throw ShaderError("Faield to create shader data from binary object:\nInvalid size {1}");
	unsigned long bytecodeSize;
	memcpy(&bytecodeSize, binaryObject, sizeof(unsigned long));
	Memory::FromBigEndian4(&bytecodeSize, &m_bytecodeSize);
	if (4 + m_bytecodeSize > binaryObjectSize)
		throw ShaderError("Faield to create shader data from binary object:\nInvalid size {2}");
	m_bytecode = (char*)malloc(m_bytecodeSize);
	memcpy(m_bytecode, binaryObject + 4, m_bytecodeSize);

	if (8 + m_bytecodeSize > binaryObjectSize)
		throw ShaderError("Faield to create shader data from binary object:\nInvalid size {3}");
	unsigned long metaDataSize;
	memcpy(&metaDataSize, binaryObject + 4 + m_bytecodeSize, sizeof(unsigned long));
	Memory::FromBigEndian4(&metaDataSize, &m_metaDataSize);
	if (8 + m_bytecodeSize + m_metaDataSize > binaryObjectSize)
		throw ShaderError("Faield to create shader data from binary object:\nInvalid size {4}");
	m_metaData = (char*)malloc(m_metaDataSize);
	memcpy(m_metaData, binaryObject + 8 + m_bytecodeSize, m_metaDataSize);

	this->Load();
}

Magma::Framework::Graphics::ShaderData::ShaderData(const char * bytecode, size_t bytecodeSize, const char * metaData, size_t metaDataSize)
{
	m_bytecodeSize = bytecodeSize;
	m_bytecode = (char*)malloc(m_bytecodeSize);
	memcpy(m_bytecode, bytecode, m_bytecodeSize);

	m_metaDataSize = metaDataSize;
	m_metaData = (char*)malloc(m_metaDataSize);
	memcpy(m_metaData, metaData, m_metaDataSize);

	this->Load();
}

Magma::Framework::Graphics::ShaderData::~ShaderData()
{
	if (m_bytecode != nullptr)
		free(m_bytecode);
	if (m_bytecode != nullptr)
		free(m_metaData);
}

Magma::Framework::Graphics::ShaderData::ShaderData(ShaderData && rhs)
{
	m_bytecode = rhs.m_bytecode;
	m_bytecodeSize = rhs.m_bytecodeSize;
	rhs.m_bytecode = nullptr;
	rhs.m_bytecodeSize = 0;

	m_metaData = rhs.m_metaData;
	m_metaDataSize = rhs.m_metaDataSize;
	rhs.m_metaData = nullptr;
	rhs.m_metaDataSize = 0;

	m_majorVersion = rhs.m_majorVersion;
	m_minorVersion = rhs.m_minorVersion;
	m_shaderType = rhs.m_shaderType;
	m_inputVars = std::move(rhs.m_inputVars);
	m_outputVars = std::move(rhs.m_outputVars);
	m_texture2DVars = std::move(rhs.m_texture2DVars);
	m_constantBuffers = std::move(rhs.m_constantBuffers);
	m_constantBufferVars = std::move(rhs.m_constantBufferVars);
}

Magma::Framework::Graphics::ShaderData::ShaderData(const ShaderData & rhs)
{
	m_bytecodeSize = rhs.m_bytecodeSize;
	m_bytecode = (char*)malloc(m_bytecodeSize);
	memcpy(m_bytecode, rhs.m_bytecode, m_bytecodeSize);

	m_metaDataSize = rhs.m_metaDataSize;
	m_metaData = (char*)malloc(m_metaDataSize);
	memcpy(m_metaData, rhs.m_metaData, m_metaDataSize);

	m_majorVersion = rhs.m_majorVersion;
	m_minorVersion = rhs.m_minorVersion;
	m_shaderType = rhs.m_shaderType;
	m_inputVars = rhs.m_inputVars;
	m_outputVars = rhs.m_outputVars;
	m_texture2DVars = rhs.m_texture2DVars;
	m_constantBuffers = rhs.m_constantBuffers;
	m_constantBufferVars = rhs.m_constantBufferVars;
}

void Magma::Framework::Graphics::ShaderData::Load()
{
	// Put meta data into stream
	std::stringstream ss(std::ios::in | std::ios::out | std::ios::binary);
	ss.write(m_metaData, m_metaDataSize);
	ss.clear();

	// Get header
	ss.read((char*)&m_majorVersion, sizeof(unsigned long));
	if (ss.eof())
		throw ShaderError("Failed to load shader data:\nFailed to read major version:\nStream read EOF (invalid meta data?)");
	Memory::FromBigEndian4(&m_majorVersion, &m_majorVersion);

	ss.read((char*)&m_minorVersion, sizeof(unsigned long));
	if (ss.eof())
		throw ShaderError("Failed to load shader data:\nFailed to read minor version:\nStream read EOF (invalid meta data?)");
	Memory::FromBigEndian4(&m_minorVersion, &m_minorVersion);

	ss.read((char*)&m_shaderType, sizeof(unsigned long));
	if (ss.eof())
		throw ShaderError("Failed to load shader data:\nFailed to read shader type:\nStream read EOF (invalid meta data?)");
	Memory::FromBigEndian4(&m_shaderType, &m_shaderType);

	// Get input vars
	size_t inputVarCount = 0;
	ss.read((char*)&inputVarCount, sizeof(unsigned long));
	if (ss.eof())
		throw ShaderError("Failed to load shader data:\nFailed to read input var count:\nStream read EOF (invalid meta data?)");
	Memory::FromBigEndian4(&inputVarCount, &inputVarCount);
	m_inputVars.resize(inputVarCount);
	for (size_t i = 0; i < inputVarCount; ++i)
	{
		ss.read((char*)&m_inputVars[i].index, sizeof(unsigned long));
		Memory::FromBigEndian4(&m_inputVars[i].index, &m_inputVars[i].index);
		if (ss.eof())
			throw ShaderError("Failed to load shader data:\nFailed to read input var index:\nStream read EOF (invalid meta data?)");

		unsigned long nameSize = 0;
		ss.read((char*)&nameSize, sizeof(unsigned long));
		Memory::FromBigEndian4(&nameSize, &nameSize);
		m_inputVars[i].name.resize(nameSize);
		ss.read(&m_inputVars[i].name[0], nameSize);
		if (ss.eof())
			throw ShaderError("Failed to load shader data:\nFailed to read input var name:\nStream read EOF (invalid meta data?)");

		ss.read((char*)&m_inputVars[i].type, sizeof(unsigned long));
		Memory::FromBigEndian4(&m_inputVars[i].type, &m_inputVars[i].type);
		if (ss.eof())
			throw ShaderError("Failed to load shader data:\nFailed to read input var type:\nStream read EOF (invalid meta data?)");
	}

	// Get output vars
	size_t outputVarCount = 0;
	ss.read((char*)&outputVarCount, sizeof(unsigned long));
	if (ss.eof())
		throw ShaderError("Failed to load shader data:\nFailed to read output var count:\nStream read EOF (invalid meta data?)");
	Memory::FromBigEndian4(&outputVarCount, &outputVarCount);
	m_outputVars.resize(outputVarCount);
	for (size_t i = 0; i < outputVarCount; ++i)
	{
		ss.read((char*)&m_outputVars[i].index, sizeof(unsigned long));
		Memory::FromBigEndian4(&m_outputVars[i].index, &m_outputVars[i].index);
		if (ss.eof())
			throw ShaderError("Failed to load shader data:\nFailed to read output var index:\nStream read EOF (invalid meta data?)");

		unsigned long nameSize = 0;
		ss.read((char*)&nameSize, sizeof(unsigned long));
		Memory::FromBigEndian4(&nameSize, &nameSize);
		m_outputVars[i].name.resize(nameSize);
		ss.read(&m_outputVars[i].name[0], nameSize);
		if (ss.eof())
			throw ShaderError("Failed to load shader data:\nFailed to read output var name:\nStream read EOF (invalid meta data?)");
	
		ss.read((char*)&m_outputVars[i].type, sizeof(unsigned long));
		Memory::FromBigEndian4(&m_outputVars[i].type, &m_outputVars[i].type);
		if (ss.eof())
			throw ShaderError("Failed to load shader data:\nFailed to read output var type:\nStream read EOF (invalid meta data?)");
	}

	// Get 2D texture vars
	size_t texture2DVarCount = 0;
	ss.read((char*)&texture2DVarCount, sizeof(unsigned long));
	if (ss.eof())
		throw ShaderError("Failed to load shader data:\nFailed to read 2D texture var count:\nStream read EOF (invalid meta data?)");
	Memory::FromBigEndian4(&texture2DVarCount, &texture2DVarCount);
	m_texture2DVars.resize(texture2DVarCount);
	for (size_t i = 0; i < texture2DVarCount; ++i)
	{
		ss.read((char*)&m_texture2DVars[i].index, sizeof(unsigned long));
		Memory::FromBigEndian4(&m_texture2DVars[i].index, &m_texture2DVars[i].index);
		if (ss.eof())
			throw ShaderError("Failed to load shader data:\nFailed to read 2D texture var index:\nStream read EOF (invalid meta data?)");

		unsigned long nameSize = 0;
		ss.read((char*)&nameSize, sizeof(unsigned long));
		Memory::FromBigEndian4(&nameSize, &nameSize);
		m_texture2DVars[i].name.resize(nameSize);
		ss.read(&m_texture2DVars[i].name[0], nameSize);
		if (ss.eof())
			throw ShaderError("Failed to load shader data:\nFailed to read 2D texture var name:\nStream read EOF (invalid meta data?)");
	}

	// Get constant buffers
	size_t constantBufferCount = 0;
	ss.read((char*)&constantBufferCount, sizeof(unsigned long));
	if (ss.eof())
		throw ShaderError("Failed to load shader data:\nFailed to read constant buffer count:\nStream read EOF (invalid meta data?)");
	Memory::FromBigEndian4(&constantBufferCount, &constantBufferCount);
	m_constantBuffers.resize(constantBufferCount);
	for (size_t i = 0; i < constantBufferCount; ++i)
	{
		ss.read((char*)&m_constantBuffers[i].index, sizeof(unsigned long));
		Memory::FromBigEndian4(&m_constantBuffers[i].index, &m_constantBuffers[i].index);;
		if (ss.eof())
			throw ShaderError("Failed to load shader data:\nFailed to read constant buffer index:\nStream read EOF (invalid meta data?)");

		unsigned long nameSize = 0;
		ss.read((char*)&nameSize, sizeof(unsigned long));
		Memory::FromBigEndian4(&nameSize, &nameSize);
		m_constantBuffers[i].name.resize(nameSize);
		ss.read(&m_constantBuffers[i].name[0], nameSize);
		if (ss.eof())
			throw ShaderError("Failed to load shader data:\nFailed to read constant buffer var name:\nStream read EOF (invalid meta data?)");
	}

	// Get constant buffer vars
	size_t constantBufferVarCount = 0;
	ss.read((char*)&constantBufferVarCount, sizeof(unsigned long));
	if (ss.eof())
		throw ShaderError("Failed to load shader data:\nFailed to read constant buffer var count:\nStream read EOF (invalid meta data?)");
	Memory::FromBigEndian4(&constantBufferVarCount, &constantBufferVarCount);
	m_constantBufferVars.resize(constantBufferVarCount);
	for (size_t i = 0; i < constantBufferVarCount; ++i)
	{
		ss.read((char*)&m_constantBufferVars[i].bufferIndex, sizeof(unsigned long));
		Memory::FromBigEndian4(&m_constantBufferVars[i].bufferIndex, &m_constantBufferVars[i].bufferIndex);
		if (ss.eof())
			throw ShaderError("Failed to load shader data:\nFailed to read constant buffer var buffer index:\nStream read EOF (invalid meta data?)");

		ss.read((char*)&m_constantBufferVars[i].offset, sizeof(unsigned long));
		Memory::FromBigEndian4(&m_constantBufferVars[i].offset, &m_constantBufferVars[i].offset);
		if (ss.eof())
			throw ShaderError("Failed to load shader data:\nFailed to read constant buffer var buffer offset:\nStream read EOF (invalid meta data?)");

		ss.read((char*)&m_constantBufferVars[i].index, sizeof(unsigned long));
		Memory::FromBigEndian4(&m_constantBufferVars[i].index, &m_constantBufferVars[i].index);
		if (ss.eof())
			throw ShaderError("Failed to load shader data:\nFailed to read constant buffer var index:\nStream read EOF (invalid meta data?)");

		unsigned long nameSize = 0;
		ss.read((char*)&nameSize, sizeof(unsigned long));
		Memory::FromBigEndian4(&nameSize, &nameSize);
		m_constantBufferVars[i].name.resize(nameSize);
		ss.read(&m_constantBufferVars[i].name[0], nameSize);
		if (ss.eof())
			throw ShaderError("Failed to load shader data:\nFailed to read constant buffer var name:\nStream read EOF (invalid meta data?)");
		
		ss.read((char*)&m_constantBufferVars[i].type, sizeof(unsigned long));
		Memory::FromBigEndian4(&m_constantBufferVars[i].type, &m_constantBufferVars[i].type);
		if (ss.eof())
			throw ShaderError("Failed to load shader data:\nFailed to read constant buffer var type:\nStream read EOF (invalid meta data?)");
	}
}
