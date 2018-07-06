#include "ShaderData.hpp"
#include "../String/Conversion.hpp"

#include <sstream>

using namespace Magma::Framework::Graphics;

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
	m_majorVersion = String::U32FromBE(m_majorVersion); // Convert from big endian to native format

	ss.read((char*)&m_minorVersion, sizeof(unsigned long));
	if (ss.eof())
		throw ShaderError("Failed to load shader data:\nFailed to read minor version:\nStream read EOF (invalid meta data?)");
	m_minorVersion = String::U32FromBE(m_minorVersion); // Convert from big endian to native format

	ss.read((char*)&m_shaderType, sizeof(unsigned long));
	if (ss.eof())
		throw ShaderError("Failed to load shader data:\nFailed to read shader type:\nStream read EOF (invalid meta data?)");
	m_shaderType = (ShaderType)String::U32FromBE((String::U32Char)m_shaderType); // Convert from big endian to native format

	// Get input vars
	size_t inputVarCount = 0;
	ss.read((char*)&inputVarCount, sizeof(unsigned long));
	if (ss.eof())
		throw ShaderError("Failed to load shader data:\nFailed to read input var count:\nStream read EOF (invalid meta data?)");
	inputVarCount = String::U32FromBE(inputVarCount); // Convert from big endian to native format
	m_inputVars.resize(inputVarCount);
	for (size_t i = 0; i < inputVarCount; ++i)
	{
		ss.read((char*)&m_inputVars[i].index, sizeof(unsigned long));
		m_inputVars[i].index = String::U32FromBE(m_inputVars[i].index);
		if (ss.eof())
			throw ShaderError("Failed to load shader data:\nFailed to read input var index:\nStream read EOF (invalid meta data?)");

		unsigned long nameSize = 0;
		ss.read((char*)&nameSize, sizeof(unsigned long));
		nameSize = String::U32FromBE(nameSize);
		m_inputVars[i].name.resize(nameSize);
		ss.read(&m_inputVars[i].name[0], nameSize);
		if (ss.eof())
			throw ShaderError("Failed to load shader data:\nFailed to read input var name:\nStream read EOF (invalid meta data?)");

		ss.read((char*)&m_inputVars[i].type, sizeof(unsigned long));
		m_inputVars[i].type = (ShaderVariableType)String::U32FromBE((String::U32Char)m_inputVars[i].type);
		if (ss.eof())
			throw ShaderError("Failed to load shader data:\nFailed to read input var type:\nStream read EOF (invalid meta data?)");
	}

	// Get output vars
	size_t outputVarCount = 0;
	ss.read((char*)&outputVarCount, sizeof(unsigned long));
	if (ss.eof())
		throw ShaderError("Failed to load shader data:\nFailed to read output var count:\nStream read EOF (invalid meta data?)");
	outputVarCount = String::U32FromBE(outputVarCount); // Convert from big endian to native format
	m_outputVars.resize(outputVarCount);
	for (size_t i = 0; i < outputVarCount; ++i)
	{
		ss.read((char*)&m_outputVars[i].index, sizeof(unsigned long));
		m_outputVars[i].index = String::U32FromBE(m_outputVars[i].index);
		if (ss.eof())
			throw ShaderError("Failed to load shader data:\nFailed to read output var index:\nStream read EOF (invalid meta data?)");

		unsigned long nameSize = 0;
		ss.read((char*)&nameSize, sizeof(unsigned long));
		nameSize = String::U32FromBE(nameSize);
		m_outputVars[i].name.resize(nameSize);
		ss.read(&m_outputVars[i].name[0], nameSize);
		if (ss.eof())
			throw ShaderError("Failed to load shader data:\nFailed to read output var name:\nStream read EOF (invalid meta data?)");
	
		ss.read((char*)&m_outputVars[i].type, sizeof(unsigned long));
		m_outputVars[i].type = (ShaderVariableType)String::U32FromBE((String::U32Char)m_outputVars[i].type);
		if (ss.eof())
			throw ShaderError("Failed to load shader data:\nFailed to read output var type:\nStream read EOF (invalid meta data?)");
	}

	// Get 2D texture vars
	size_t texture2DVarCount = 0;
	ss.read((char*)&texture2DVarCount, sizeof(unsigned long));
	if (ss.eof())
		throw ShaderError("Failed to load shader data:\nFailed to read 2D texture var count:\nStream read EOF (invalid meta data?)");
	texture2DVarCount = String::U32FromBE(texture2DVarCount); // Convert from big endian to native format
	m_texture2DVars.resize(texture2DVarCount);
	for (size_t i = 0; i < texture2DVarCount; ++i)
	{
		ss.read((char*)&m_texture2DVars[i].index, sizeof(unsigned long));
		m_texture2DVars[i].index = String::U32FromBE(m_texture2DVars[i].index);
		if (ss.eof())
			throw ShaderError("Failed to load shader data:\nFailed to read 2D texture var index:\nStream read EOF (invalid meta data?)");

		unsigned long nameSize = 0;
		ss.read((char*)&nameSize, sizeof(unsigned long));
		nameSize = String::U32FromBE(nameSize);
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
	constantBufferCount = String::U32FromBE(constantBufferCount); // Convert from big endian to native format
	m_constantBuffers.resize(constantBufferCount);
	for (size_t i = 0; i < constantBufferCount; ++i)
	{
		ss.read((char*)&m_constantBuffers[i].index, sizeof(unsigned long));
		m_constantBuffers[i].index = String::U32FromBE(m_constantBuffers[i].index);
		if (ss.eof())
			throw ShaderError("Failed to load shader data:\nFailed to read constant buffer index:\nStream read EOF (invalid meta data?)");

		unsigned long nameSize = 0;
		ss.read((char*)&nameSize, sizeof(unsigned long));
		nameSize = String::U32FromBE(nameSize);
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
	constantBufferVarCount = String::U32FromBE(constantBufferVarCount); // Convert from big endian to native format
	m_constantBufferVars.resize(constantBufferVarCount);
	for (size_t i = 0; i < constantBufferVarCount; ++i)
	{
		ss.read((char*)&m_constantBufferVars[i].bufferIndex, sizeof(unsigned long));
		m_constantBufferVars[i].bufferIndex = String::U32FromBE(m_constantBufferVars[i].bufferIndex);
		if (ss.eof())
			throw ShaderError("Failed to load shader data:\nFailed to read constant buffer var buffer index:\nStream read EOF (invalid meta data?)");

		ss.read((char*)&m_constantBufferVars[i].offset, sizeof(unsigned long));
		m_constantBufferVars[i].offset = String::U32FromBE(m_constantBufferVars[i].offset);
		if (ss.eof())
			throw ShaderError("Failed to load shader data:\nFailed to read constant buffer var buffer offset:\nStream read EOF (invalid meta data?)");

		ss.read((char*)&m_constantBufferVars[i].index, sizeof(unsigned long));
		m_constantBufferVars[i].index = String::U32FromBE(m_constantBufferVars[i].index);
		if (ss.eof())
			throw ShaderError("Failed to load shader data:\nFailed to read constant buffer var index:\nStream read EOF (invalid meta data?)");

		unsigned long nameSize = 0;
		ss.read((char*)&nameSize, sizeof(unsigned long));
		nameSize = String::U32FromBE(nameSize);
		m_constantBufferVars[i].name.resize(nameSize);
		ss.read(&m_constantBufferVars[i].name[0], nameSize);
		if (ss.eof())
			throw ShaderError("Failed to load shader data:\nFailed to read constant buffer var name:\nStream read EOF (invalid meta data?)");
		
		ss.read((char*)&m_constantBufferVars[i].type, sizeof(unsigned long));
		m_constantBufferVars[i].type = (ShaderVariableType)String::U32FromBE((String::U32Char)m_constantBufferVars[i].type);
		if (ss.eof())
			throw ShaderError("Failed to load shader data:\nFailed to read constant buffer var type:\nStream read EOF (invalid meta data?)");
	}
}
